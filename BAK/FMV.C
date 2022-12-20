/************************************************************
 *                                                          *
 *                                                          *
 *   Movie player								                     *
 *                                                          *
 *                                                          *
 ************************************************************/

#define	FY 256


 /* --------------------------------------------------------------------------
 * - Description:
 * - ------------
 * - General purpose movie player.
 * -
 * - Refer to ReadMe.txt for more info.
 * ---------------------------------------------------------------------------
 */ 


#include "machine.h"	// contains PSX, PC, N64

#ifdef PSX
#include "defs_gen.h"
#include "lib_psx.h"	// set to libn64.h , libpc.h or libpsx.h
#include "defs_psx.h"
#include "..\..\global\striker.h"	 // share structure for all formats
#include	"structs.h"
#include	"main.h"
#include	"general.h"
#endif


#include "hardware.h"








void file_preseek();

LONG seek_pos;


typedef struct {
		char		*strName;					// Stream file name. 
		char		*mname;
		short		mode;						// 24-Bit or 16-Bit streaming.
		short		drawBorders;				// != 0 if borders on.
		u_short		scrWidth;					// Screen res width.
		u_short		x;							// X,Y position. 
		u_short		y;
		u_short		width;						// Stream width and height.
		u_short		height;
		u_long		endFrame;					// Last frame No.
		u_long		vlcBufSize;					// Size of each VLC buffer (including header).
		u_short		volume;						// Left and Right ADPCM volume.
} Str_info;


typedef struct {
	volatile short		is24Bit;						// = 1 when 24-bit streaming.
	volatile u_short	scrWidth;						// Screen width resolution.
	volatile u_short	x;								// Str X, Y.
	volatile u_short	y;
	volatile u_short	width;							// Str width and height.
	volatile u_short	height;
	volatile u_long		endFrame;                  		// End frame. 
	volatile short		volume;							// ADPCM Volume.
	volatile u_long		frameCount;						// Frame count.
	RECT				rect[2];
	RECT				slice;							// Slice decoded by DecDCTout().
	volatile short		rewindSwitch;					// = 1 when end of stream.
	volatile short		frameDone;						// = 1 when decoding and drawing done.
	volatile short		rectId;
	volatile short		vlcId;							// VLC buffer id.
	volatile short		imageId;						// Image buffer id.
	u_long				*vlcBuffer[2];					// VLC run-level buffers.
	u_long				*imageBuffer[2];				// Strip image buffers.
	DISPENV				disp[2];						// Display envs.
	volatile u_long	movie_frame;						// Frame count.
	LONG					vlc_size;
} DecEnv;



WORD action( WORD av_no, WORD (*poll)(void) );
short PlayStream( WORD av_no, short (*keyHandler)(void) );
static void StrDecDCToutCB(void);
static void StrInit(Str_info *str);
static void StrEnd(void);
static void StrClearVRAM(void);
static short StrKickCD(CdlLOC *loc);
static u_long *StrGetNextFrame(void);
LONG	StrVLCFrame(u_long *frame);
static void	StrDrawDecodeSync(void);
static short StrFadeOutVol(short *fadeVol);
static long GetDCToutSize(void);
static u_short ScalePPW(u_short n);
static u_short GetDCT_MODE(void);

void setup_movie_screens( void );
void initialise_cd( void );
void initialise_spu( void );






typedef struct
{
	Str_info	*str;
	LONG		control;
//	CdlFILE	cdlfile;

}Av;




// Streaming modes.
#define STR_MODE24				1
#define STR_MODE16				0


// Border modes.
#define STR_BORDERS_OFF			0
#define STR_BORDERS_ON			1


// PlayStream return values.
#define	PLAYSTR_END				1
#define	PLAYSTR_USER			2
#define	PLAYSTR_ERROR			3



#define	WAIT_FOR_IT	WAIT_TIME
#define	MOVIE
#define	MAX_VLC_BUFFER_SIZE	32			vlc buffer size in K
#define	MAX_VLC	(( 1024 * MAX_VLC_BUFFER_SIZE )>>2)


#define FB1_X 0
#define FB1_Y 0
#define FB2_X 0
#define FB2_Y 256
#define FRAME_X 0

CdlFILE cdlfile[64];	//storage for file preseek



Str_info rage_movie =
{
	"\\BRA.XA;1",	// filename
	"hello",
	STR_MODE24,
	STR_BORDERS_ON,						// border ??
  	256,										// x res
	0, 0, 									// x/y offset
	256, 256,								// x/y size ( must match converted av file or BOOM )
	18,										// last frame no
	24000,		  							// vlc buffer size ( in lw )
	127,										// volume

};





Av	available_av[]=
{				

	&rage_movie,0,
	&rage_movie,0,
	&rage_movie,0,
	&rage_movie,0,
	0,

};


/* ---------------------------------------------------------------------------
 * - CONSTANTS
 * ---------------------------------------------------------------------------
 */

// Define for timing and testing functions.

// Define for extra debug info.


// Maximum number of retries.
#define MAX_RETRY				5

// Form1 sector size in bytes.
#define FORM1_SIZE				2048 

// Ring buffer size in sectors.
#define RING_SIZE				64

// Maximum possible VLC buffer size (bytes) = 128K + (Header) = 129K.
#define MAX_VLCBUF_SIZE			(256*1024) + 1024 // was 128

// Slice width in pixels.
#define SLICE_WIDTH				16

// Words/Pixel for 24-bit and 16-bit modes.
#define PPW24					3 / 2
#define PPW16					1    

// DCT modes for 24-bit and 16-bit (STP = 1) streaming.
#define DCT_MODE24				3
#define DCT_MODE16				2

/***************************************************************************/
/*	some macros																					*/
/***************************************************************************/
#define bound(val, n)			((((val) - 1) / (n) + 1) * (n))
#define bound16PPW(val, ppw)	(bound((val), 16 * ppw))
#define bound16(val)			(bound((val), 16))

// Speed of volume fade.
#define FADE_VOL_SPD			8

// RunLevel header size in bytes.
#define RL_HEADER_SIZE			1024

/* ---------------------------------------------------------------------------
 * - DATA TYPE AND STRUCTURE DECLARATIONS
 * ---------------------------------------------------------------------------
 */



/* ---------------------------------------------------------------------------
 * - GLOBAL DEFINITIONS 
 * ---------------------------------------------------------------------------
 */
DecEnv	dec;							// Streaming Env.
LONG		oldHeapbase;  					// Heapbase before PlayStream.
SndVolume oldVolume;						// Old ADPCM volume.

/* ---------------------------------------------------------------------------
 * - PRIVATE FUNCTION PROTOTYPES
 * ---------------------------------------------------------------------------
 */


/* ---------------------------------------------------------------------------
 * - FUNCTION DEFINITIONS
 * ---------------------------------------------------------------------------
 */
				
//DB		movie_buffer[2];
LONG	movie_sp;

WORD	movie_number;
WORD	(*movie_poll)(void);
LONG	movie_stack;

DB *mdb[2];

WORD play_movie( WORD av_no, WORD (*poll)(void) ) 
{


	file_preseek();

	movie_number = av_no;
	movie_poll = poll;
	#if DEBUG
	printf("attempting to play movie %d\n",movie_number);
	#endif

	if ( av_no != -1 )
		action( movie_number, movie_poll );
}



WORD action( WORD av_no, WORD (*poll)(void) ) 
{
//CdlFILE		fp;
u_long		*frame;
short		frameRetry = 0;
short		strQuit = 0;
short		fadeVol = 0;
Str_info 	*str;
LONG		size;
RECT src_rect={0,0,512,256 };
LONG seek_pos;

Av *av = &available_av[ av_no ];

	push(0);	//*** store memory pointer

	mdb[0] = allocate_mem( 0,sizeof(DB ) );
	mdb[1] = allocate_mem( 0,sizeof(DB ) );

	str = av->str;
	dec.vlc_size = 0;

	/************************************************/
	/*	find the movie file									*/
	/************************************************/

#if 0
	if (!CdSearchFile(&fp, str->strName)) 
		{
#ifdef DEBUG
		printf("ERROR: MOVIE -- Could not find stream file '%s'.\n", str->strName);
#endif
		pop(0);
		return PLAYSTR_ERROR;
	}
#endif

#ifdef DEBUG
		printf("MOVIE:  found %s\n", str->strName );
#endif

//	fp = seek_file;

	/************************************************/
	/*	setup movie												*/
	/************************************************/
	StrInit(str);
	//clear_vram( 0,0, 512,511, 0,0,0 );
	DrawSync(0);

/************************************************
 *	seek to movie start							      *
 ************************************************/
	if (!StrKickCD( &cdlfile[av_no].pos )) 
		{
	  #ifdef DEBUG
		printf("ERROR: MOVIE -- Couldn't start the CD!!\n");
	  #endif
		StrEnd();
		pop(0);
		return PLAYSTR_ERROR;
	}

	  #ifdef DEBUG
	   printf("MOVIE:  kicked cd\n" );
	  #endif


	SetDispMask(1);


	while (!(frame = StrGetNextFrame()))  		// If we can't get frame exit!
		{
		if (++frameRetry == MAX_RETRY)
			{
		  #ifdef DEBUG
			printf("ERROR: MOVIE -- Couldn't get first frame!!\n");
		  #endif
			StrEnd();
			pop(0);
			return PLAYSTR_ERROR;
			}
		}
	

	frameRetry = 0;
	StrVLCFrame(frame);


	while (!dec.rewindSwitch && !strQuit)
		{
		DecDCTin(dec.vlcBuffer[dec.vlcId], GetDCT_MODE());
		DecDCTout(dec.imageBuffer[dec.imageId], GetDCToutSize());


		if (!(frame = StrGetNextFrame()))
			{
			if (++frameRetry == MAX_RETRY)
				strQuit = PLAYSTR_ERROR;
			}
		else
			{
			frameRetry = 0;
			}

		StrVLCFrame(frame);


		// Check for exit. 
		if( poll )
			{
			if (!fadeVol && (*poll)())
				fadeVol = dec.volume;

			}

		// Fade the volume down to zero. Then trigger exit.
		if (fadeVol)
			strQuit = StrFadeOutVol(&fadeVol);



		StrDrawDecodeSync();


		DrawSync(0);
		VSync(0);

		PutDispEnv(&dec.disp[dec.rectId]);

		}


	StrEnd();


	if (dec.rewindSwitch)								// Check for end of stream exit.
		strQuit =  PLAYSTR_END;


#ifdef DEBUG
	if (strQuit == PLAYSTR_ERROR) 
		printf("ERROR: MOVIE -- Couldn't get frame from ring buffer!!\n");
#endif


	
//*** Copy screen so that both screen buffers are same ***

	if ( dec.is24Bit )
		src_rect.w = (256*3)/2; //3
	else
		src_rect.w = (256*2)/2;



	DrawSync(0);
	VSync(0);

	PutDispEnv(&dec.disp[1]);


	if ( dec.rectId==1 )
		{
		src_rect.y = 256;
//		MoveImage( &src_rect, 0, 0 );
		}
	else
		{
		src_rect.y = 0;
//		MoveImage( &src_rect, 0, 256 );
		}

	VSync(0);
	DrawSync(0);

	pop(0);
	return strQuit;
}



/* ------------------------------------------------------------------------ */

static void StrDecDCToutCB(void)
{

/* - Type:	CALLBACK
 * -
 * - Usage:	Receives decoded image data from the MDEC and loads it to VRAM.
 */


	// In 24bit mode call StCdInterrupt() manually.
	if (dec.is24Bit) {

		extern StCdIntrFlag;

		if(StCdIntrFlag) {
			StCdInterrupt();
			StCdIntrFlag = 0;
		}
	}

	LoadImage(&dec.slice, dec.imageBuffer[dec.imageId]);
	dec.imageId ^= 1;
	dec.slice.x += dec.slice.w;

	if (dec.slice.x < (dec.rect[dec.rectId].x + dec.rect[dec.rectId].w)) {

		DecDCTout(dec.imageBuffer[dec.imageId], GetDCToutSize());

	} else {

		dec.frameDone = 1;
	}
}

/* ------------------------------------------------------------------------ */


static void StrInit(Str_info *str)
{

short		x, y, w, h;
short		scrWidth;
u_long	*ringBuf;
u_long	vlcBufSize,	imageBufSize;	


//	oldHeapbase = __heapbase;
//	oldHeapbase = (LONG)free_mem_ptr;


	x = str->x;
	y = str->y;	
	w = str->width;
	h = str->height;
	scrWidth = str->scrWidth;

	dec.is24Bit = str->mode; 
	dec.scrWidth = scrWidth;
	dec.x = x;
	dec.y = y;
	dec.width = w;
	dec.height = h;
	dec.endFrame = str->endFrame;
	dec.volume = str->volume;
	dec.frameCount = 0;
	dec.rewindSwitch = 0;
	dec.frameDone = 0;
	dec.rectId = 0;
	dec.vlcId = 0;
	dec.imageId = 0;
	

	// Init the display environments.
	SetDefDispEnv(&dec.disp[0], 0,   FY, scrWidth, FY);
	SetDefDispEnv(&dec.disp[1], 0,   0,       scrWidth, FY);
	setRECT(&dec.disp[0].screen, SCREEN_X, SCREEN_Y, 0, FY);	
	setRECT(&dec.disp[1].screen, SCREEN_X, SCREEN_Y, 0, FY);	
	dec.disp[0].isrgb24 = dec.disp[1].isrgb24 = (str->mode == STR_MODE24);

	dec.disp[0].screen.x = config->screen_xpos;
	dec.disp[0].screen.y = config->screen_ypos;
	dec.disp[1].screen.x = config->screen_xpos;
	dec.disp[1].screen.y = config->screen_ypos;


//	if (str->drawBorders || dec.is24Bit)			// Clear VRAM.
//		StrClearVRAM();


	// Set correct display mode before we begin drawing.
	VSync(0);
	PutDispEnv(&dec.disp[dec.rectId]);


	setRECT(&dec.rect[0], ScalePPW(x), y, ScalePPW(w), h);  
	setRECT(&dec.rect[1], ScalePPW(x), y + FY, ScalePPW(w), h);
	dec.slice = dec.rect[dec.rectId];
	dec.slice.w = ScalePPW(SLICE_WIDTH);






	/************************************************************************/
	/*	allocate image strip buffers														*/
	/************************************************************************/
	if (dec.is24Bit)
		imageBufSize = SLICE_WIDTH * 3 * h;
	else
		imageBufSize = SLICE_WIDTH * 2 * h;

	dec.imageBuffer[0] = (u_long *) allocate_mem(0,imageBufSize);
	dec.imageBuffer[1] = (u_long *) allocate_mem(0,imageBufSize); 
	

	/************************************************************************/
	/*	allocate vlc decode buffers														*/
	/************************************************************************/
	if (str->vlcBufSize)
		vlcBufSize = str->vlcBufSize;
	else
		vlcBufSize = MAX_VLCBUF_SIZE;

	dec.vlcBuffer[0] = allocate_mem( 0,(vlcBufSize<<2) );
	dec.vlcBuffer[1] = allocate_mem( 0,(vlcBufSize<<2) );

	/************************************************************************/
	/*	initialise mdec & allocate ring buffer											*/
	/************************************************************************/
	DecDCTReset(0);
	DecDCTvlcSize(0);
	DecDCToutCallback((void (*)()) StrDecDCToutCB);
	ringBuf = (u_long *) allocate_mem(0, RING_SIZE * FORM1_SIZE );
	StSetRing(ringBuf, RING_SIZE);




	// Start no matter the frame number!
	StSetStream(dec.is24Bit, 0, 0xffffffff, 0, 0);


	// Store old ADPCM volume and set new volume.
	SsGetSerialVol(SS_SERIAL_A, &oldVolume);
	SsSetSerialVol(SS_SERIAL_A, dec.volume, dec.volume);
}

/* ------------------------------------------------------------------------ */

static void StrEnd(void) {
/* - Type:	PRIVATE
 * -
 * - Usage: Clean up.
 */

	
	StUnSetRing();
	CdControlB(CdlPause, 0, 0);
	DecDCToutCallback(0);            
//	__heapbase = oldHeapbase;

//	free_mem_ptr = (LONG *)oldHeapbase;

//	if (dec.is24Bit)			// Clear VRAM to remove any 24bit graphics.
//		StrClearVRAM();


	// Restore previous ADPCM volume. 
	SsSetSerialVol(SS_SERIAL_A, oldVolume.left, oldVolume.right);


}

/* ------------------------------------------------------------------------ */

static void StrClearVRAM(void) {


/* - Type:	PRIVATE
 * -
 * - Usage: Useful before and after streaming in 24bit mode	to ensure
 * - 		24bit data is not displayed in 16bit mode (and vice versa). 
 */


	RECT	clrRect;


	setRECT(&clrRect, 0, 0, ScalePPW(dec.scrWidth), FY);
	ClearImage(&clrRect, 0, 0, 0);
	clrRect.y = FY;
	ClearImage(&clrRect, 0, 0, 0);
}

/* ------------------------------------------------------------------------ */

static short StrKickCD(CdlLOC *loc) {


/* - Type:	PRIVATE
 * -
 * - Param:	loc = (In) location. 
 * -
 * - Ret:	0 on error, 1 otherwise.	
 * -
 * - Usage: Start the CD. 
 */

	
	short	seekRetry = 0,
			readRetry = 0;	


	do {

		while (!CdControl(CdlSeekL, (u_char *)loc, 0)) {
			
			if (++seekRetry == MAX_RETRY)
				return 0;
		}


		seekRetry = 0;

		if (readRetry++ == MAX_RETRY)
			return 0;
				

	} while (!CdRead2(CdlModeStream|CdlModeSpeed|CdlModeRT));


	return 1;
}


/* ------------------------------------------------------------------------ */

static u_long *StrGetNextFrame(void) {


/* - Type:	PRIVATE
 * -
 * - Ret:	Address of next frame or NULL on error.
 * -
 * - Usage:	Gets next frame from the ring buffer. Also checks for the
 * -		end of the stream. If the end has been reached rewindSwitch
 * -		is set. If frame is not obtained from the ring timeout and
 * -		return NULL.
 */


	long		timer = WAIT_FOR_IT;
	u_long		*addr,t;                      
	StHEADER	*sector;                         

    
    // Get the next frame from the ring buffer.
	while(t = StGetNext(&addr, (u_long**) &sector))
		{
			
			if (--timer == 0)
				return NULL;
		}


	dec.frameCount++;				// Update internal frame count.


#ifdef DEBUG
	if (dec.frameCount == 1)
		printf("MOVIE: w = %d, h = %d, sectors = %d\n", 
				sector->width, sector->height, sector->nSectors);

	FntPrint("Frame No. = %d\n", dec.frameCount);
#endif


			
	// End of stream checks.
	if (sector->frameCount > dec.endFrame) {
	
#ifdef DEBUG
		printf("ERROR: MOVIE -- Found sector frame count over > end frame (%d > %d)\n",
				sector->frameCount, dec.endFrame);
#endif
		dec.rewindSwitch = 1;
		return addr;
	}
	

	if (sector->frameCount < dec.frameCount) {
	
#ifdef DEBUG
		printf("ERROR: MOVIE -- Found sector frame count < internal frame count. (%d < %d)\n",
			 sector->frameCount, dec.endFrame);
#endif
		dec.rewindSwitch = 1;
		return addr;
	}


	if (sector->width != dec.width || sector->height != dec.height) {
	
#ifdef DEBUG
		printf("ERROR: MOVIE -- Found bad sized frame (%d x %d) is not (%d x %d).\n",
				sector->width, sector->height, dec.width, dec.height);
#endif
		dec.rewindSwitch = 1;
		return addr;
	}

	
    return addr;                          
}

/* ------------------------------------------------------------------------ */


LONG	StrVLCFrame(u_long *frame)
{
LONG	size;

/* - Type:	PRIVATE
 * -
 * - Param:	frame = (In) Address of frame BS.
 * -
 * - Usage:	VLC decodes the frame's BS. Unlock frame from the ring buffer
 * -		once decoded.
  */


	// If no frame to decode return.
	if (frame == NULL) 
		{
		#ifdef DEBUG
		printf("ERROR: MOVIE -- No frame to VLC decode!!\n");
		#endif
		return 0;
		}

	dec.vlcId ^= 1;

	size = DecDCTvlc(frame, dec.vlcBuffer[dec.vlcId]);

	StFreeRing(frame);
	return size;

}

/* ------------------------------------------------------------------------ */

static void	StrDrawDecodeSync(void) {


/* - Type:	PRIVATE
 * -
 * - Usage:	Wait for current frame's decoding and drawing to be finished.
 */



	long	timer = WAIT_FOR_IT;


	while (!dec.frameDone) {

		if (--timer == 0) {
#if DEBUG
			printf("ERROR: MOVIE -- MDEC decode timeout!!\n");
#endif
			break;
		}
	}

	
	dec.frameDone = 0;

	dec.rectId ^= 1;
	dec.slice.x = dec.rect[dec.rectId].x;
	dec.slice.y = dec.rect[dec.rectId].y;
}


/* ------------------------------------------------------------------------ */

static short StrFadeOutVol(short *fadeVol) {


/* - Type:	PRIVATE
 * -
 * - Param:	fadeVol = (In/Out) Current volume.
 * -
 * - Ret:	1 = Volume completely faded (therefore finish playing the stream).
 * - 		0 = Otherwise.
 * -		
 * -
 * - Usage: Fade out the volume.
 */


	if ((*fadeVol -= FADE_VOL_SPD) < 0)
		*fadeVol = 0;


	SsSetSerialVol(SS_SERIAL_A, *fadeVol, *fadeVol);


	if (!*fadeVol)
		return PLAYSTR_USER;			// Flag user exit.
	else
		return 0;
}

/* ------------------------------------------------------------------------ */


static long GetDCToutSize(void) {


/* - Type:	PRIVATE
 * -
 * - Ret:	DecDCTout size in longs for current playback mode.
 */


	u_short	h = dec.height;

	
	if (dec.is24Bit)
		return (((SLICE_WIDTH * PPW24) * bound16(h)) >> 1);
	else
		return (((SLICE_WIDTH * PPW16) * bound16(h)) >> 1);
}

/* ------------------------------------------------------------------------ */

static u_short ScalePPW(u_short n) {


/* - Type:	PRIVATE
 * -
 * - Param:	n = (In) A number.
 * -
 * - Ret:	n scaled by the correct PPW (words/pixel). This depends
 * -		on the current playback mode.
 */


	if (dec.is24Bit)
		return (n * PPW24);
	else
		return (n * PPW16);
}

/* ------------------------------------------------------------------------ */

static u_short GetDCT_MODE(void) {


/* - Type:	PRIVATE
 * -
 * - Ret:	The current DCT mode.	
 */


	return ((dec.is24Bit) ? DCT_MODE24 : DCT_MODE16);
}

/* ------------------------------------------------------------------------ */


void setup_movie_screens( void )
{

	/*********************************************************************/
	/* initialize environment for double buffer 									*/
	/*********************************************************************/
	SetDefDrawEnv(&mdb[0]->draw, FB1_X, FB1_Y,FRAME_X, FY);
	SetDefDispEnv(&mdb[0]->disp, FB2_X, FB2_Y,FRAME_X, FY);

	SetDefDrawEnv(&mdb[1]->draw, FB2_X, FB2_Y,FRAME_X,FY );
	SetDefDispEnv(&mdb[1]->disp, FB1_X, FB1_Y,FRAME_X,FY);

//	init_prim( &mdb[0], 1 , 1 );
//	init_prim( &mdb[1], 1 , 1 );



}


void initialise_spu( void )
{
   /************************************************************/
	/* setup sound processor												*/
   /************************************************************/

	init_spu();

}


/************************************************************************/
/*	setup cd 																				*/
/************************************************************************/
void initialise_cd( void )
{

//	CdInit( );
//	printf("cd initialised\n");

}



void file_preseek()
{
Av *av=available_av;
WORD cnt=0;
CdlLOC *pos;

	while( av->str )
		{
		if ( CdSearchFile( &cdlfile[cnt], av->str->strName ) == 0 )
			{
			#if DEBUG
			printf("could not preseek file '%s'\n",av->str->strName );
			#endif
			}

//		cdlfile[cnt] = cdlfile; //CdPosToInt( &seek_file.pos );
		pos = &cdlfile[cnt].pos;

//** the movie seek poses should be prestored in a table to speed
//** up the initialisation process
//		printf("seek pos=%d %d %d\n",pos->minute,pos->second,pos->sector );
		cnt++;
		av++;
		}

}


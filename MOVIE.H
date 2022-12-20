


/***************************************************************************/
/*	display bitdepths																			*/
/***************************************************************************/
#define MOVIE_24BIT				1
#define MOVIE_16BIT				0

/***************************************************************************/
/* movie info struct																			*/
/***************************************************************************/
typedef struct
{
	char		*strName;				  	// Stream filename. 
	short		mode;							// 24-Bit or 16-Bit streaming.
	u_short	scrWidth;					// Screen res width.
	u_short	x;								// X,Y position. 
	u_short	y;
	u_short	width;						// Stream width and height.
	u_short	height;
	u_long 	endFrame;					// Last frame No.
	u_long 	vlcBufSize;					// Size of each VLC buffer (including header).
	u_short	volume;						// Left and Right ADPCM volume.
	short		screen_x_offset;			// screen positioni offset for duff tv's
	short		screen_y_offset;			// screen positioni offset for duff tv's
}Movie_info;


typedef struct
{
	volatile short		is24Bit;					// = 1 when 24-bit streaming.
	volatile u_short	scrWidth;				// Screen width resolution.
	volatile u_short	x;							// Str X, Y.
	volatile u_short	y;
	volatile u_short	width;					// Str width and height.
	volatile u_short	height;
	volatile u_long  	endFrame;         // End frame. 
	volatile short		volume;					// ADPCM Volume.
	volatile u_long  	frameCount;			// Frame count.
	RECT					rect[2];
	RECT					slice;						// Slice decoded by DecDCTout().
	volatile short		rewindSwitch;			// = 1 when end of stream.
	volatile short		frameDone;				// = 1 when decoding and drawing done.
	volatile short		rectId;
	volatile short		vlcId;					// VLC buffer id.
	volatile short		imageId;				   // Image buffer id.
	u_long				*vlcBuffer[2];		  	// VLC run-level buffers.
	u_long				*imageBuffer[2];	  	// Strip image buffers.
	DISPENV				disp[2];				  	// Display envs.
	long					vlc_size;
	u_short				*vlc_table;
	u_long				freemem;
	long					decode_time;
	long					longest_decode_time;
}DecEnv;


/************************************************************************/
/*	externs 																					*/
/************************************************************************/
extern	WORD play_movie( Movie_info *, WORD (*keyHandler)(void), ULONG membase );
extern	void set_movie_debug_mode( WORD mode );

/************************************************************************/
/*	exit states																				*/
/************************************************************************/
enum
{
	MOVIE_END = 0,
	MOVIE_USER,
	MOVIE_ERROR
};

#if 0
typedef struct
{
	Movie_info	*str;

}Av;
#endif

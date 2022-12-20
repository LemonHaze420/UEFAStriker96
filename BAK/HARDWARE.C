/********************************************************************
 *																						  *
 * PSX specific functions														  *
 *																						  *
 ********************************************************************/

#include	"incs.h"

#ifdef PSX
#include "..\..\global\striker.def" // defs for PSX striker
#include "..\..\striker\frontend\mod.def"
#include "..\..\striker\src\anim.def"
//#include "..\..\striker\movies\fruit\mod.def"
//#include "..\..\striker\movies\hotdog\mod.def"
#include "..\..\global\s01.h"	// structs used by scene "s01" format
#include	"sod.h"
#include	"scene.h"
#include "datafile.h"
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include "hardware.h"
#include "polydraw.h"	//psx structs
#include	"font.h"
#include "text.h"
#include "plasma.h"
#include	"render.h"
#include "anim.h"	
//#include "test.h"
#include	"mem.h"
#include	"fvars.h"
#include	"pad.h"
#include	"camera.h"
#include	"light.h"
#include "utils.h"
#include	"io.h"
//#include	"sound.h"
//#include "teamsel.h"
#include "options.h"
//#include "..\..\global\image.h"
#endif

/********************************************************************
 *																						  *
 * Prototypes																		  *
 *																						  *
 ********************************************************************/

void draw_bloke();
void clear_vram( UWORD x, UWORD y, UWORD w,UWORD h ,WORD r,WORD g,WORD b);
void draw_callback();
void vsync_callback();
void init_prim( DB *db, UBYTE clear, UBYTE dither );
void setup_system_font();
void set_tbar( WORD x, WORD y, WORD ht, POLY_F4 *prim, UBYTE r, UBYTE g, UBYTE b );


/********************************************************************
 *																						  *
 * Vars																				  *
 *																						  *
 ********************************************************************/

//Prim *spawned_sprite_primitive_list;


WORD frame_update;
volatile WORD frame_counter;		// MUST BE VOLATILE ( for opt lev 3!! )
WORD print_debug_info;
WORD screen_z;
//ULONG oldstack;
WORD enable_display_timer;

DB	*db[2],*cdb;	/* current draw buffer */


void init_hardware()
{

	//show_message("regraph3");
	ResetGraph(3);						/* reset graphic subsystem (0:cold,1:warm) */

	SetGraphDebug(0);					/* set debug mode (0:off, 1:monitor, 2:dump) */

	#if NTSC==YES
		SetVideoMode(MODE_NTSC);
	#else
		SetVideoMode(MODE_PAL);
	#endif


	#if DEBUG
	SetDispMask(1); /* screen on */
	#else
	SetDispMask(1); /* screen orf */
	#endif

	EnterCriticalSection();
	DrawSyncCallback( draw_callback );
 	VSyncCallback( vsync_callback );
	ExitCriticalSection();

	initialise_control_pads( 12, 3 );	// repeat delay and repeat speed

//	clear_vram ( 0,0,1023,511, 67,28,171 );	// clear vram to this RGB
//	clear_vram ( 640,0,1023,511, 0,0,0 );	// clear vram to this RGB



}


/*********************************************************************/
/*	draw/frame finished callbacks													*/
/*********************************************************************/

void draw_callback()
{
	gpu_bar_stop = GetRCnt( RCntCNT1); //GsGetVcount();
//	draw_counter++;
//	FntPrint("dc=%d\n",draw_counter);
}


void vsync_callback()
{
	frame_counter++;
	draw_bloke();	/** Animating bloke **/
	//rand();

}

void clear_vram( UWORD x, UWORD y, UWORD w,UWORD h ,WORD r,WORD g,WORD b)
{
RECT rect;

	setRECT ( &rect, x,y,w,h );
	ClearImage ( &rect, r,g,b );
}							  



void init_display( WORD wd,WORD ht, BYTE bgr, BYTE bgg, BYTE bgb, WORD clear1, WORD clear2, WORD mode24 )
{
WORD cnt;

/********* 
  Clear1 : YES = clear all of vram
  Clear2 : YES = clear screen before draw
***********/


//adjust height for NTSC
	ht = 256; //NTSC?(ht/256)*240:ht;

	config->screen_xres = wd;
	config->screen_yres = ht;

	DrawSync(0);	// hang cpu until GPU has finished drawing

	InitGeom();						 // initialize geometry subsystem 	
	SetGeomOffset( wd/2, ht/2); // set geom origin to centre of screen
	SetGeomOffset( wd/2, 256/2); // set geom origin to centre of screen
	screen_z = SCREEN_Z;
	SetGeomScreen(SCREEN_Z);	 // distance to viewing-screen 		
	GsInitVcount();

	init_timer_bars();		// debug timer bars
	  

	/*********************************************************************/
	/* initialize environment for double buffer 									*/
	/*********************************************************************/

	DrawSync(0);
	SetDefDrawEnv( &db[0]->draw, 0, 0,   wd,ht );
	SetDefDrawEnv( &db[1]->draw, 0, 256, wd,ht );
	SetDefDispEnv( &db[0]->disp, 0, 256, wd,ht );
	SetDefDispEnv( &db[1]->disp, 0, 0,   wd,ht );

//	SetDefDrawEnv( &db[0]->draw, 0, 0,   512,512 );
//	SetDefDrawEnv( &db[1]->draw, 0, 0,   512,512 );
//	SetDefDispEnv( &db[0]->disp, 0, 0,   512,512 );
//	SetDefDispEnv( &db[1]->disp, 0, 0,   512,512 );



	for ( cnt=0;cnt<2;cnt++ )
		{
	// hardware screen xy offs and wd/ht...
		db[cnt]->disp.screen.x = config->screen_xpos; //SCREEN_X;  
		db[cnt]->disp.screen.y = 0; //config->screen_ypos; //SCREEN_Y;  
		db[cnt]->disp.screen.h = FRAME_HT;  
		db[cnt]->disp.screen.w = 256;  
		db[cnt]->disp.isrgb24  = mode24;	// yes = 24bit no=16bit

		init_prim(db[cnt], clear2, YES); //set dispbuff clear? , dither?

		db[cnt]->buffnum	 = cnt;

		setRGB0 ( &db[cnt]->draw, bgr,bgg,bgb );
		}

//	setup_system_font();


	switch ( wd )
		{
		case 256:	camera.x_aspect = 310;	break;
		case 320:	camera.x_aspect = 311;	break;
		case 384:	camera.x_aspect = 320;	break;
		case 512:	camera.x_aspect = 315;	break;
		case 640:	camera.x_aspect = 310;	break;
		}


	VSync(0);
	if ( clear1 )
		{
		clear_vram ( 0,0,wd,511, 0,0,0 );	// clear vram to this RGB
		update_display();

		cdb = (cdb==db[0])? db[1]: db[0]; 
		ClearOTagR( cdb->ot,  OT_SIZE);	/* clear ordering table */

		//swap_display_buffers();

		update_display();

	cdb = (cdb==db[0])? db[1]: db[0]; 
	ClearOTagR( cdb->ot,  OT_SIZE);	/* clear ordering table */

		//swap_display_buffers();
		}


}


void set_defdispdraw()
{
WORD ht,wd;

	wd = config->screen_xres;
	ht = config->screen_yres;
	SetDefDrawEnv( &db[0]->draw, 0, 0,   wd,ht );
	SetDefDrawEnv( &db[1]->draw, 0, 256, wd,ht );
	SetDefDispEnv( &db[0]->disp, 0, 256, wd,ht );
	SetDefDispEnv( &db[1]->disp, 0, 0,   wd,ht );
}


#if DEBUG
void move_scnpos()
{
	if ( joy[4].c & PAD_R1 )
		{
		FntPrint("pppppppp\n");
		if ( joy[4].c & PAD_LT )
			config->screen_xpos--;
		if ( joy[4].c & PAD_RT )
			config->screen_xpos++;
		if ( joy[4].c & PAD_UP )
			config->screen_ypos--;
		if ( joy[4].c & PAD_DN )
			config->screen_ypos++;
		}
}
#endif

void alloc_screen_dbuffs()
{
	db[0] = (DB *)allocate_mem ( 0,sizeof ( DB ));
	db[1] = (DB *)allocate_mem ( 0,sizeof ( DB ));

	db[0]->ot = allocate_mem ( 0,OT_SIZE * 4 );
	db[1]->ot = allocate_mem ( 0,OT_SIZE * 4 );	// each entry is a long...
	swap_screens();
	swap_screens();
}

void init_prim( DB *db, UBYTE clear, UBYTE dither )
{
	/***************************************************/
	/*	set drawing environment parameters					*/
	/***************************************************/
	db->draw.isbg = clear; /*clear*/
	db->draw.dtd = dither;  // Do Dither? Yup or Nope
	db->draw.dfe = 0;

}	


void wait_vblank()
{


	if( frame_counter < frame_update )
		while( frame_counter < frame_update );
	else
		{


		frame_counter = frame_update-1;
		while( frame_counter < frame_update )
			{
			}
		}

	frame_counter = 0;
	
	cpu_bar_start =GsGetVcount();
}



void update_display()
{
//	PutDispEnv(&cdb->disp); /* update display environnment */
//	PutDrawEnv(&cdb->draw); /* update drawing environnment */
//	DrawOTag ( (cdb->ot)+OT_SIZE-1 );	// process gpu ordering table



#if DEBUG
	move_scnpos();
#endif
}



void switch_debug_info()
{
#if DEBUG == YES
	 if ( joy[0].db == PAD_SEL )
		 {
		 print_debug_info++;
		 if ( print_debug_info > 4 )
		  	print_debug_info=0;

		if ( print_debug_info == 1 )
			SetDispMask(0);
		else
			SetDispMask(1);
 		 }
#endif
}



void display_system_font()
{
	if ( (print_debug_info == 0) || (print_debug_info == 3)  )
		FntFlush(-1);

}










void print_error ( WORD error_num, BYTE *text_string, WORD num )
{
	#if DEBUG
	printf("Error %d '%s' %d\n",error_num,text_string,num);
	#endif
}



void enable_display( WORD time )
{

	#if DEBUG
	SetDispMask(1);
	return;
	#endif

	if ( time )
		enable_display_timer = time;	// bg_loop will setdispmask(1) when time=0
	else
		{
		VSync(0);
		SetDispMask(1);	//enable screen immediately
		VSync(0);
		enable_display_timer = -1; //dont enable in bg_loop
		}

}

void disable_display()
{
	#if DEBUG
	SetDispMask(1);
	return;
	#endif

	VSync(0);
	SetDispMask(0);	//switch screen off
	VSync(0);
}













void adjust_hw_screen_pos()
{

	cdb->disp.screen.x = config->screen_xpos;
	cdb->disp.screen.y = config->screen_ypos;

}


void swap_screens()
{
	cdb = (cdb==db[0])? db[1]: db[0]; 
}






/****************/

#if NTSC==NO
#define	YOFFS	0
#else
#define	YOFFS	8
#endif

#define	MAX_ANIMMAN_FRAMES	43
#define	ANIMMAN_TEX_X 		(0 << 0)
#define	ANIMMAN_TEX_Y 		256
#define	ANIMMAN_CLUT_X		0
#define	ANIMMAN_CLUT_Y		509
#define	ANIMMAN_BPP			_8BIT
#define	ANIMMAN_W	 		64
#define	ANIMMAN_H	 		64
#define	ANIMMAN_POS_X		(512-ANIMMAN_W-8)
#define	ANIMMAN_POS_Y		(256-ANIMMAN_H-8-YOFFS)
#define	ANIMMAN_SAVE_X		960-512
#define	ANIMMAN_SAVE_Y		128+256 

WORD animating_man_active = 0;			// 0=no : 1=preserve initial slice : 2=do him
WORD animating_man_frame;



void start_animating_man()
{
	show_loadscn();

	load_scene_materials ( LOADMAN_TEX, NO );


	animating_man_active=YES;
	VSync(0);
	DrawSync(0);
}

void stop_animating_man()
{
WORD y;
WORD ys=5;
	animating_man_active=NO;

	ys=4;
	for(y=0;y<128;y+=ys)
		{
		clear_vram( 0,y, 512, ys, 0,0,0 );
		clear_vram( 0,255-ys-y, 512, ys, 0,0,0 );

		VSync(0);
		}

	clear_vram( 0,0, 512, 512, 0,0,0 );

}

POLY_FT4 genprim;

void draw_bloke()
{
int		x, y, w;
POLY_FT4	*ft4;
RECT rect;

	if( animating_man_active )
		{

/* Preserve initial background behind bloke. */
/*-------------------------------------------*/
		if( animating_man_active == 1 )
			{
			setRECT( &rect, ANIMMAN_POS_X,ANIMMAN_POS_Y,ANIMMAN_W,ANIMMAN_H );
			MoveImage( &rect, ANIMMAN_SAVE_X,ANIMMAN_SAVE_Y );
			VSync(0);
			animating_man_active = 2;
			}

		else
			{
/* Restore backdrop behind bloke. */
/*--------------------------------*/
			setRECT( &rect, ANIMMAN_SAVE_X,ANIMMAN_SAVE_Y,ANIMMAN_W,ANIMMAN_H );
			MoveImage( &rect, ANIMMAN_POS_X,ANIMMAN_POS_Y );

/* Draw next frame of bloke to screen. */
/*-------------------------------------*/
/* Get next frame VRAM coords */
			x = ANIMMAN_TEX_X;
			x += ((animating_man_frame & 0xf) * ANIMMAN_W);
			y = ANIMMAN_TEX_Y;
			y += (animating_man_frame / 16) * ANIMMAN_H;

/* Setup prim */
			ft4 = &genprim;
			SetPolyFT4( ft4 );
			SetSemiTrans( ft4, 0 );
			SetShadeTex( ft4, 1 );
			setRGB0( ft4, 128,128,128 );
			setXYWH( ft4, ANIMMAN_POS_X, ANIMMAN_POS_Y, ANIMMAN_W, ANIMMAN_H );
			setTPage( ft4, ANIMMAN_BPP, 0, x>>(2-ANIMMAN_BPP), y );
			setClut( ft4, ANIMMAN_CLUT_X, ANIMMAN_CLUT_Y );
			x &= ((256 >> (2-ANIMMAN_BPP)) - 1);
			setUVWH( ft4, x, y, ANIMMAN_W, ANIMMAN_H );
			DrawPrim( ft4 );
			}

		animating_man_frame++;
		if ( animating_man_frame> MAX_ANIMMAN_FRAMES )
			animating_man_frame=0;
		}
}

/**************/ 




void show_loadscn()
{
UBYTE *pak_file;
UBYTE *unpak_addr;
RECT rect={ 0,0, 512,256 };


	DrawSync(0);
	VSync(0);
	cdb = db[1];
	PutDispEnv(&cdb->disp); /* update display environnment */

	push(0);
	unpak_addr = allocate_mem( 0, (512*256*2)+256 );

	pak_file = read_datafile_alloc( LOADPIC );	

	unpropak( pak_file, unpak_addr );

	DrawSync(0);
 	LoadImage( &rect, (ULONG *)unpak_addr+5 );

	VSync(0);

//	cdb = db[1];
//	set_defdispdraw();
//	PutDispEnv(&cdb->disp); /* update display environnment */
//	PutDrawEnv(&cdb->draw); /* update display environnment */

	pop(0);



	

}






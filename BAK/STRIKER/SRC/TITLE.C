/****************************************************************************
 *																									 *
 *																									 *
 *           Title screen																	 *
 *																									 *
 *																									 *
 ****************************************************************************/


#include "incs.h"

#ifdef PSX
#include "..\..\global\striker.def" // defs for PSX striker
#include "..\..\global\s01.h"	// structs used by scene "s01" format
#include "..\..\striker\frontend\mod.def"
#include "..\..\striker\src\anim.def"

#include	"sod.h"
#include	"scene.h"
#include "datafile.h"
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include	"shell.h"
#include "hardware.h"
#include "polydraw.h"	//psx structs
#include "text.h"
#include	"font.h"
#include	"pad.h"
#include "fmv.h"
#include "utils.h"
#include "teamedit.h"
#include "options.h"
#include "config.h"
#include "teamsel.h"
#include "render.h"
#include "memcard.h"
#include "cdaudio.h"
#endif


/****************************************************
 *																	 *
 *	 Defines														 *
 *																	 *
 ****************************************************/

//#define DEMO_TIMEOUT ( 2200*TICKS_PER_SEC )
#define DEMO_TIMEOUT 32767

/****************************************************
 *																	 *
 *	 Prototypes													 *
 *																	 *
 ****************************************************/

void title_init();
void title();
void title_exit();
void update_title_list();

void call_team_edit();

void set_title_func( void *addr );

/****************************************************
 *																	 *
 *  Vars															 *
 *																	 *
 ****************************************************/

Rdb_gadget title_rdb_gadget;
Gadget *title_rdb;
Pos tfade;
WORD tfade_dir=0;
WORD title_list, title_item, abort_title;

void draw_rdb_box2();

//void poo();

WORD tp_item_num;

BYTE *title_opt_list0[][8]={
	"New Friendly",
	"Load Friendly setup",
	"New competition",
	"Load competition",
	"Configuration",
	"Records",
	"Training",
	0,

};

BYTE *title_opt_list1[][10]={
	"World cup '98",
	"Euro 2000",
	"Euro 2000 Finals",
	"Territories Cup",
	"Knockout",
	"League",
	"Classic match",
	"Previous",
	0,

};

BYTE *title_opt_list2[][7]={
	"Edit custom teams",
	"Audio",
	"Controllers",
	"Screen position",
	"Save configuration",
	"Previous",
	0,

};

BYTE *title_opt_list3[][4]={
	"New game",
	"Load game",
	"Previous",
	0,
};




BYTE **title_lists[]={ &title_opt_list0[0][0], &title_opt_list1[0][0], &title_opt_list2[0][0], &title_opt_list3[0][0], &title_opt_list1[0][0]  };

//Textstring_gadget title_ts_gads[4];
//Textstring_gadget title_ts_gads2[4];
//Textstring_gadget test_ts_gad;


//Textstring_gadget test_gad;
//Rdb_gadget test_rdb;




void run_title()
{
LONG oldstack;

	abort_title=NO;
	while( abort_title==NO )
		{

		if ( share->title_func == 0 )
			{
			push(0);
			title_init();
			title();

			printf("calling title exit\n");
			title_exit();
			printf("calling title exit done\n");
			pop(0);
			}
		else
			(*share->title_func)();

		}


}



Scene_startup test_sdt_start={
	PAD_SDT,
	-1,
	-1,
	-1,
	0,0,0,0,
	0,
};


#define pl_pad 0

WORD test_model_list[]=
{

	pl_pad,

	-1,

};




void title_init()
{	

	shell=&shell_struct;


	init_process_executor();	/* Call order 0 */
	init_gadgets(60);	/* Call order 1 */

	start_text_manager( TEXTMAN_TSLOTS );	// (2) call before update_frontend
	set_textman_slot( MAIN_TSLOT, 128 ); // (3) for title page


	disable_display();
	enable_display( 0 );

	load_ted_info();	// load ted file and store in ted_block[]

	init_display( 512, 256, 0,0,0, YES, YES, NO );	//screen xy res & bg color


	init_sprites ( TITLE_SAN, 256 ); /* ( 4 ) */
	init_debug_menu();

	joy_curr=-1;	// read current pad pressed into *cjoy


	load_scene_materials ( TITLE_TEX ); /* ( 5 ) */
	setup_system_font();	/* ( 6 ) */


	add_backdrop( TIT_BG );

	title_list=0;
	shell->current_item=0;

	frame_update=1;
	add_process ( update_title_list );

	title_rdb = set_rdb_gadget( 0, &title_rdb_gadget, -600,-600,1200,1200, -1 );

//	draw_rdb_box2();


//	add_process(poo);

	init_fonts();


#if 0
	init_scene_draw( &test_sdt_start );
	test_sdt_start.model_list = test_model_list;	// list of objects to display
	start_scene( &test_sdt_start );
	add_process( render_scene );
#endif

	share->demo_mode = NO;


//	init_plasma();
//	add_plasma( 0,35, 65536, 65536, 40,32,13,8, NO, 2, PLASMA_DEPTH );

//	init_xa_audio();
//	play_xa_audio_track( 0 );


}


POLY_G4 pols[2];
POLY_G4 *pol;

Sprite *poo;

void title()
{
//WORD abort_seq=NO;

WORD demo_timeout;





//	bg_loop_predraw();

#if 0
	poo=spawn_sprite( -1 );
	poo->gouraud = YES;
	poo->x.w.hi = -400;
	poo->y.w.hi = -400;
	poo->w = 400;
	poo->h = 200;
	poo->display=YES;
	poo->depth = 0;
	poo->transp_rate = -1;
	poo->r0 = 255;
	poo->g0 = 0;
	poo->b0 = 0;
	poo->r1 = 255;
	poo->g1 = 0;
	poo->b1 = 0;
	poo->r2 = 0;
	poo->g2 = 128;
	poo->b2 = 255;
	poo->r3 = 0;
	poo->g3 = 128;
	poo->b3 = 255;

#endif	

	demo_timeout = DEMO_TIMEOUT;

	while ( share->title_func == 0 )
		{


#if 0

		pol = &pols[ cdb->buffnum ];
		SetPolyG4( pol );

		setRGB0( pol, 255,0,0 );
		setRGB1( pol, 0,0,255 );
		setRGB2( pol, 255,255,0 );
		setRGB3( pol, 0,255,0 );

		pol->x0 = 30;
		pol->y0 = 30;
		pol->x1 = 400;
		pol->y1 = 30;
		pol->x2 = 30;
		pol->y2 = 200;
		pol->x3 = 400;
		pol->y3 = 200;
		AddPrim ( cdb->ot + NEAREST_DEPTH, pol );
#endif


		if ( cjoy->c )
			demo_timeout = DEMO_TIMEOUT;
		else
			{
			demo_timeout--;
			if ( demo_timeout < 0 )
				{

				set_demo_mode_data();
				}
			}

		if ( joy[0].db & PAD_TRI ) tfade_dir =1;
		if ( joy[0].db & PAD_CIR ) tfade_dir=-1;

		if ( tfade_dir ) 
			{
			tfade.w.hi += tfade_dir*4;
			if ( tfade.w.hi >32 )
				{
				tfade.w.hi = 32;
				tfade_dir = 0;
				}

			if ( tfade.w.hi <=0 )
				{
				tfade.w.hi = 0;
				tfade_dir = 0;
				}

			}

		bg_loop();


//		update_title_list();
		}



	

}





void title_exit()
{
WORD r,g,b,cnt;
ULONG pixel;
WORD x;	

	sfxSpot( SLIDE2, 30 );
//	cd_fade_volume = 1;

	frame_update=1;

#if 0
	pixel=0;
	cnt=0;
	while( pixel<128 )
		{
		cnt=0;

		while(1)
			{
			clear_vram ( 0,pixel+0  ,512,1, 0,0,0 );	// clear vram to this RGB
			clear_vram ( 0,pixel+256,512,1, 0,0,0 );	// clear vram to this RGB

			clear_vram ( 0,255-pixel+0  ,512,1, 0,0,0 );	// clear vram to this RGB
			clear_vram ( 0,255-pixel+256,512,1, 0,0,0 );	// clear vram to this RGB

			pixel++;
			cnt++;
			if ( cnt==6) break;

			}

		swap_screens();
		wait_vblank();
		}

#endif
	frame_update = FRONTEND_UPDATE_RATE;

//	restore_processes();
}




void update_title_list()
{
BYTE **list = title_lists[ title_list ];
WORD total_items=0;
Textman_struct *ts;
SOD *sod;
WORD cnt;
WORD delay;

RECT rdb_sizes[]={ 

	-156, -196/2, 304, 424, 

	-172, -286/2, 344, 520,

	0-(336/2), -160/2, 336, 360,	// config

	-112, -124, 112*2, 208,

};



	title_rdb->rdb_gadget->size.x += (rdb_sizes[ title_list ].x-title_rdb->rdb_gadget->size.x) /3;
	title_rdb->rdb_gadget->size.y += (rdb_sizes[ title_list ].y-title_rdb->rdb_gadget->size.y) /3;
	title_rdb->rdb_gadget->size.w += (rdb_sizes[ title_list ].w-title_rdb->rdb_gadget->size.w) /3;
	title_rdb->rdb_gadget->size.h += (rdb_sizes[ title_list ].h-title_rdb->rdb_gadget->size.h) /3;


	total_items = 0;
	delay=0;
	cnt=0;
	while( *list != 0)
		{
		total_items++;
		delay+=2;
		list++;
		}


		list = title_lists[ title_list ];

//	FntPrint("total items=%d\n",total_items);

	for( cnt=0; cnt< total_items; cnt++ )
		{
		ts = add_text_string ( tslot_poses[MAIN_TSLOT] + cnt , *list, MILFORD_FONT, milford_font_logic, 0, (27*2*cnt)+rdb_sizes[title_list].y, FONT_XC, cnt, 0, 10, 15 );
		ts->delay_until_fade_up = delay;
		delay -= 1;
		list++;
		}


//	modify_current_item( total_items );

	if ( cjoy->db & PAD_DN )
		{

		sfxSpot( HIT1, 30 );

		flash_counter=0;
		if ( shell->current_item < total_items-1 )
			{
			shell->current_item++;
			}
		else
			{
			shell->current_item=0;
			}

		}

	if ( cjoy->db & PAD_UP )
		{
		sfxSpot( HIT1, 30 );
		flash_counter=0;

		if (shell->current_item != 0)
			{
			shell->current_item--;
			}
		else
			{
			shell->current_item = total_items-1;
			}
		}

	if ( cjoy->db & PAD_X )
		{
		if ( title_list == 0 )
			{

			//**** Main menu ****
			if ( shell->current_item == 0 )
				{
				/*** FRIENDLY THREAD ***/


				share->new_game = YES;
				//set_title_func( territory_thread );

				set_title_func( friendly_thread );

				//set_title_func( knockout_thread );
				//set_title_func( league_thread );
				return;
				}

			if ( shell->current_item == 1 )
				{
				/*** FRIENDLY PRESET THREAD ***/
				set_title_func( friendly_preset_thread );
				return;
				}


			if ( shell->current_item == 2 )
				{


				//*** New COMPETITION ***
				share->new_game = YES;
				title_list=1;
				shell->current_item=0;
				sfxSpot( BUTVOC, 30 );
				return;
				}

			if ( shell->current_item == 3 )
				{
				//*** Load COMPETITION ***
				share->new_game = NO;
				title_list=1;
				shell->current_item=0;
				sfxSpot( BUTVOC, 30 );
				return;
				}


			if ( shell->current_item == 4 )
				{
				//*** Chose new game ***
				title_list=2; //** Goto Config ***/
				shell->current_item=0;
				sfxSpot( BUTVOC, 30 );
				return;
				}
			}


		if ( title_list == 1 )
			{
			//**** Competition ****
			if ( shell->current_item == 7 )
				{
				//*** Previous ***
				shell->current_item=0;
				title_list=0;
				sfxSpot( BUTVOC, 30 );
				return;
				}
			else
				{
				sfxSpot( BUTVOC, 30 );

				if ( shell->current_item == 0 )
					{		
					//set_title_func( worldcup_thread );
					}	

				if ( shell->current_item == 1 )
					{		
					set_title_func( euro2000_thread );
					}	

				if ( shell->current_item == 2 )
					{		
					set_title_func( euro_final_thread );
					}	


				if ( shell->current_item == 3 )
					{		
					set_title_func( territory_thread );
					}	

				if ( shell->current_item == 4 )
					{		
					set_title_func( knockout_thread );
					}	

				if ( shell->current_item == 5 )
					{
					set_title_func( league_thread );
					}

				if ( shell->current_item == 6 )
					{
					set_title_func( classic_thread );
					}



				return;
				}
			}

		if ( title_list == 2 )
			{
			//**** Configuration ****

			if ( shell->current_item == 5 )
				{
				//*** Previous ***
				shell->current_item=0;
				title_list=0;
				sfxSpot( BUTVOC, 30 );
				return;
				}


			if ( shell->current_item == 0 )
				{
				//**** Edit teams ****
				set_title_func( team_edit );
				return;
				}

			if ( shell->current_item == 1 )
				{
				//**** Audio config ****
				set_title_func( audio_config );
				return;

				}

			if ( shell->current_item == 2 )
				{
				//**** Pad config ****
				set_title_func( pad_config );
				}


			if ( shell->current_item == 3 )
				{
				//**** Screen positioner ****
				set_title_func( screen_positioner );
				}
			}
		}


//	sod = sod_header->sod_list + 0;
//	sod->rot.vx += 5;
//	sod->rot.vy += 3;
//	sod->rot.vz += 1;

}



WORD ww;
Sprite *zz;

WORD tl_quad_w, tl_quad_h;

Sprite *tl, *tr, *bl, *br, *cen, *lt, *rt, *top, *bot;


WORD x1 = -200;
WORD y1 = -360;
WORD xsize = 200;
WORD ysize = 20*4;

void cack()
{
WORD wd,ht;
Spr_anim_frame_san *fr;	
WORD x2,y2;
WORD ys;

	if ( cjoy->c & PAD_R2 )
		{
		if ( cjoy->c & PAD_LT ) x1-=4;
		if ( cjoy->c & PAD_RT ) x1+=4;
		if ( cjoy->c & PAD_UP ) y1-=4;
		if ( cjoy->c & PAD_DN ) y1+=4;
		}
	else
		{
		if ( cjoy->c & PAD_LT ) xsize-=4;
		if ( cjoy->c & PAD_RT ) xsize+=4;
		if ( cjoy->c & PAD_UP ) ysize-=4;
		if ( cjoy->c & PAD_DN ) ysize+=4;
		}


	x2 = x1 + xsize;
	y2 = y1 + ysize;




//*** tl quad ***
	fr = tl->spr_anim_frame_san+0;
	tl_quad_w = fr->w*2;
	tl_quad_h = fr->h*2;
	tl->x.w.hi = x1;
	tl->y.w.hi = y1;


//** tr quad ***
	fr = tr->spr_anim_frame_san+0;
	tr->x.w.hi = x2 - tl_quad_w;
	tr->y.w.hi = y1;


//** bl quad ***
	fr = bl->spr_anim_frame_san+0;
	bl->x.w.hi = x1;
	bl->y.w.hi = y2 - tl_quad_h;


//** br quad ***
	fr = br->spr_anim_frame_san+0;
	br->x.w.hi = x2 - tl_quad_w;
	br->y.w.hi = y2 - tl_quad_h;


//*** top ***
	fr = top->spr_anim_frame_san + top->frame;
	wd = fr->w*2;
	top->x.w.hi = x1 + tl_quad_w;
	top->y.w.hi = y1;
	top->xscale = ((xsize-(tl_quad_w*2)) * 256)/wd;


//*** bot ***
	fr = bot->spr_anim_frame_san+ bot->frame;
	wd = fr->w*2;
	bot->x.w.hi = x1 + tl_quad_w;
	bot->y.w.hi = y2 - (fr->h*1);
	bot->xscale = ((xsize-(tl_quad_w*2)) * 256)/wd;


//*** lt ***
	fr = lt->spr_anim_frame_san + lt->frame;
	ht = fr->h*4;
	lt->x.w.hi = x1;
	lt->y.w.hi = y1+2 + tl_quad_h;
	lt->yscale = ( (ysize-(tl_quad_h*2)) * 256) / ht;

//*** rt ***
	fr = rt->spr_anim_frame_san + rt->frame;
	ht = fr->h*4;
	rt->x.w.hi = x2 - (fr->w*2);
	rt->y.w.hi = y1 + tl_quad_h;
	rt->yscale = ( (ysize-(tl_quad_h*2)) * 256) / ht;

//*** Centre *****
	fr = cen->spr_anim_frame_san + cen->frame;
	cen->x.w.hi = x1 + (4*4);
	cen->y.w.hi = y1 + (4*4);
	ht = fr->h*4;
	cen->xscale = ( (xsize-20) * (fr->w*2) ) / 256;
	cen->yscale = ( ((ysize-12)/4) * 256 ) / fr->h;



}


void draw_rdb_box2()
{



//*** tl quad ***
	spawn_sprite( BOX );
	tl = sprite;
	tl->frame = 0;
 

//** tr quad ***
	spawn_sprite( BOX );
	tr = sprite;
	tr->frame = 2;



//** bl quad ***
	spawn_sprite( BOX );
	bl = sprite;
	bl->frame = 5;



//** br quad ***
	spawn_sprite( BOX );
	br = sprite;
	br->frame = 7;


//** top centre ***
	spawn_sprite( BOX );
	top = sprite;
	top->frame = 1;
	top->centre_if_scaled = NO;


//** bot centre ***
	spawn_sprite( BOX );
	bot = sprite;
	bot->frame = 6;
	bot->centre_if_scaled = NO;


//** lt ***
	spawn_sprite( BOX );
	lt = sprite;
	lt->frame = 3;
	lt->centre_if_scaled = NO;

//** rt ***
	spawn_sprite( BOX );
	rt = sprite;
	rt->frame = 4;
	rt->centre_if_scaled = NO;

//** centre **
	spawn_sprite( BOX );
	cen = sprite;
 	cen->frame = 8;
	cen->centre_if_scaled=NO;





 	add_process( &cack );
}









void set_title_func( void *addr )
{
	share->title_func = addr;
	share->thread_pos = 0;
	share->game_stat = GAME_INIT;
}


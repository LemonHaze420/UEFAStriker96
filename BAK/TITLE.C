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
#include "skindraw.h"
#include "anim.h"

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

void setup_mascot();
void kill_mascot();
void spawn_ruud();

void title_init();
void title();
void title_exit();
void update_title_list();

void call_team_edit();

void set_title_func( void *addr );
ULONG *title_proc;

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

WORD tp_item_num;

BYTE *title_opt_list0[][8]={
	"New Friendly",
	"Play preset Friendly",
	"New competition",
	"Load competition",
	"Configuration",
	"Records",
	"Training",
	0,

};

BYTE *title_opt_list1[][10]={
	"World cup '98",
	"Euro 2000 Qualifiers",
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
			{
			restore_game_undo_info();
			(*share->title_func)();
			}

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
	shell->mode = TITLE;
	title_proc = add_process ( update_title_list );
	title_rdb = set_rdb_gadget( 0, &title_rdb_gadget, -600,-600,1200,1200, -1 );
	sprite->r0=sprite->g0=sprite->b0=
	sprite->r1=sprite->g1=sprite->b1=
	sprite->r2=sprite->g2=sprite->b2=
	sprite->r3=sprite->g3=sprite->b3=40;
	shell->master_option_active = NO;
	shell->current_item = 0;
	play_xa_audio_track( 6 );

	spawn_ruud();
	
	share->current_group=0;

}


POLY_G4 pols[2];
POLY_G4 *pol;

POLY_FT4 dpr[2];

UWORD test_tp;

//Sprite *poo6;
void title()
{
//WORD abort_seq=NO;

WORD demo_timeout;



	demo_timeout = DEMO_TIMEOUT;

	while ( share->title_func == 0 )
		{

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
WORD old_x, x,y;	
RECT rect, crect;
WORD yo;
WORD spd;

	kill_mascot();

	sfxSpot( SLIDE2, 30 );
//	cd_fade_volume = 1;

	frame_update=1;

	kill_sprite_group( 64 ); /* Ruud piccy sprites */
	kill_rdb_gadget( title_rdb );
	kill_process( title_proc );

	shell->mode = FRONTEND;

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

	-444, -156, 456, 424, 	// main menu

	-444, -172, 456, 468,	// competit

	-444, -124, 456, 360,	// config

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

//	for(cnt=0;cnt<5;cnt++)
//		add_text_string ( tslot_poses[MAIN_TSLOT] + 20+cnt , "oh what a luverly day it is blah de blah de blah de blah", MILFORD_FONT, milford_font_logic, 0,0, FONT_XC, cnt, 0, 10, 15 );

	for( cnt=0; cnt< total_items; cnt++ )
		{
		ts = add_text_string ( tslot_poses[MAIN_TSLOT] + cnt , *list, MILFORD_FONT, milford_font_logic, -(13*8*2), (27*2*cnt)+rdb_sizes[title_list].y, FONT_XC, cnt, 0, 10, 15 );

		if ( (cnt==shell->current_item) && (shell->lt_indicator_sprite->sequence_num==2) )
			{
			ts->text_color.r=220;
			ts->text_color.g=220;
			ts->text_color.b=40;
			}
		else
			{
			ts->text_color.r=
			ts->text_color.g=
			ts->text_color.b=128;
			}
		ts->delay_until_fade_up = delay;
		delay -= 1;
		list++;
		}

//	ts = add_text_string ( tslot_poses[MAIN_TSLOT] + 12 , "ABC DEFGHIJ\nabcdefghijklmnopqrstuvwxyz\n0123456789", FT13_FONT, milford_font_logic, 0, 0, FONT_XC, cnt, 0, 10, 15 );


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
				drop_indicators();
				return;
				}

			if ( shell->current_item == 3 )
				{
				//*** Load COMPETITION ***
				share->new_game = NO;
				title_list=1;
				shell->current_item=0;
				sfxSpot( BUTVOC, 30 );
				drop_indicators();

				return;
				}


			if ( shell->current_item == 4 )
				{
				//*** Chose new game ***
				*config_store = *config;	/** Store configuration to see if it has changed **/
				title_list=2; //** Goto Config ***/
				shell->current_item=0;
				drop_indicators();

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
				drop_indicators();

				sfxSpot( BUTVOC, 30 );
				return;
			  	}
			else
				{
				sfxSpot( BUTVOC, 30 );

				switch( shell->current_item )
					{
					case 0:
						set_title_func( worldcup_thread );
					break;

					case 1:
						set_title_func( euro2000_thread );
					break;

					case 2:
						set_title_func( euro_final_thread );
					break;

					case 3:
						set_title_func( territory_thread );
					break;

					case 4:
						set_title_func( knockout_thread );
					break;

					case 5:
						set_title_func( league_thread );
					break;

					case 6:
						set_title_func( classic_thread );
					break;
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

				set_title_func( check_for_config_save );
//				return;
//				check_for_config_save();

				shell->current_item=0;
				title_list=0;
				drop_indicators();

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

			if ( shell->current_item == 4 )
				{
				//**** Screen positioner ****
				set_title_func( config_io );
				}

			}
		}


//	sod = sod_header->sod_list + 0;
//	sod->rot.vx += 5;
//	sod->rot.vy += 3;
//	sod->rot.vz += 1;

}








void set_title_func( void *addr )
{
	share->title_func = addr;
	share->thread_pos = 0;
	share->game_stat = GAME_INIT;
}






/**** Skinned mascot person ***/

void update_mascot();

void *title_sk_process;
void *title_mascot_process;
Skin_obj sk_mascot_obj;
void draw_skin_mascot()
{
// 	draw_skin_objs( &sk_mascot_obj );
}


void setup_mascot()
{
Skin_header_src *sk_header_src;

	sk_header_src = (Skin_header_src *)read_datafile_alloc( TITLE_PLAYER );
	setup_skinned_object( sk_header_src, &sk_mascot_obj );
	sk_mascot_obj.active=YES;
	sk_mascot_obj.pos.vx = 2400;
	sk_mascot_obj.pos.vy = 1500;
	sk_mascot_obj.pos.vz = 5000;

	title_sk_process = add_process( draw_skin_mascot );
	title_mascot_process = add_process( update_mascot ); 

}

void update_mascot()
{
	sk_mascot_obj.current_anim_frame = timer%257;
}

void kill_mascot()
{
	sk_mascot_obj.active = NO;
	kill_process( title_sk_process );
	kill_process( title_mascot_process );
}

WORD xo=0;
WORD yo=0;
WORD tnt=40;

void zarse( Sprite *sprite )
{
	sprite->xoffs= xo;
	sprite->yoffs= yo;
	sprite->tint.r =
	sprite->tint.g =
	sprite->tint.b = tnt;

	if ( sprite->frame == ((timer/8)&15) )
		{
//		FntPrint("fr=%d\n",sprite->frame);
//		sprite->tint.r=64;
		}
//	else
//		sprite->tint.r=0;
}

void spawn_ruud()
{
WORD cnt;

	for(cnt=0;cnt<14;cnt++)
		{
		spawn_sprite( R );
		sprite->frame = cnt;
		sprite->use_frame_offs = YES;
		sprite->group_num = 64;
		sprite->x.w.hi = (-(256-64-24))+60+70;
		sprite->y.w.hi = ((-512)+20)+60+60;
		sprite->depth = MIDDLE_DEPTH+10;
		sprite->upd_rtn = (void *)zarse;
		sprite->transp_rate = 2;
		}

}

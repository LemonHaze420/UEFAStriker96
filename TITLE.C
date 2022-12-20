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
//#include "fmv.h"
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





BYTE **title_lists[]={ &title_opt_list0[0][0], &title_opt_list1[0][0], &title_opt_list2[0][0], &title_opt_list3[0][0], &title_opt_list1[0][0]  };





void run_title()
{
LONG oldstack;

	abort_title=NO;


	while( abort_title==NO )
		{
		if ( share->title_func == 0 )
			{
			start_title_track();
				
			config_auto_load();

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
			if ( share->game_stat == QUIT_COMPETITION )
				{
				share->title_func=0;
				abort_title=NO;
				}
			else
				{

				process_game_return_data();


				(*share->title_func)();
				}
			}

		}


}





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

	set_shell_bottom_buttons( 0, -1 );
	shell->master_option_active=YES;
//	spawn_ruud();
	

//	reset_competition_data();

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
WORD yo,txt;
BYTE deselect_option[10][10];
WORD x,y;
RECT rdb_sizes[]={ 

	-250, -116, 250*2, 424-52, 	// main menu

	-250, -200+72, 250*2, 468+52,	// competit
			  
	-250, -228+24+88, 250*2, 424,	// config

};




	title_rdb->rdb_gadget->size.x += (rdb_sizes[ title_list ].x-title_rdb->rdb_gadget->size.x) /3;
	title_rdb->rdb_gadget->size.y += (rdb_sizes[ title_list ].y-title_rdb->rdb_gadget->size.y) /3;
	title_rdb->rdb_gadget->size.w += (rdb_sizes[ title_list ].w-title_rdb->rdb_gadget->size.w) /3;
	title_rdb->rdb_gadget->size.h += (rdb_sizes[ title_list ].h-title_rdb->rdb_gadget->size.h) /3;

	if ( title_list != 0 )
		title_rdb->rdb_gadget->size.y-=12;

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

	list += config->language*(total_items+1);

//	FntPrint("total items=%d\n",total_items);

//	for(cnt=0;cnt<5;cnt++)
//		add_text_string ( tslot_poses[MAIN_TSLOT] + 20+cnt , "oh what a luverly day it is blah de blah de blah de blah", MILFORD_FONT, milford_font_logic, 0,0, FONT_XC, cnt, 0, 10, 15,0 );

	if ( title_list != 0 )
		yo=24;
	else
		yo=0;



/** Set options which cant be selected **/
	for(x=0;x<10;x++)
		for(y=0;y<10;y++)
			deselect_option[x][y]=0;

	if ( check_for_a_preset()==NO )
		{
		deselect_option[0][1] = YES;
		}




	if ( title_list != 0 )
		{
		if ( title_list == 1 )
			{
			txt=share->new_game?2:3;
			}
		else
			txt=4;

		cnt=-1;			  
		/** Print load/new game/configure title **/
		add_text_string ( tslot_poses[MAIN_TSLOT] + 16 , title_opt_list0[config->language][txt], TITLE_FONT, milford_font_logic, 0, (27*2*cnt)+rdb_sizes[title_list].y+yo-16, FONT_XC, 16, 0, 10, 15,0 );
		}
	

	for( cnt=0; cnt< total_items; cnt++ )
		{

		ts = add_text_string ( tslot_poses[MAIN_TSLOT] + cnt , *list, MILFORD_FONT, milford_font_logic, 0, (26*2*cnt)+rdb_sizes[title_list].y+yo, FONT_XC, cnt, 0, 10, 15,delay );

		if ( deselect_option[ title_list ][ cnt ] )
			ts->max_brightness = 16;

		delay -= 1;
		list++;
		}

//	ts = add_text_string ( tslot_poses[MAIN_TSLOT] + 12 , "ABC DEFGHIJ\nabcdefghijklmnopqrstuvwxyz\n0123456789", FT13_FONT, milford_font_logic, 0, 0, FONT_XC, cnt, 0, 10, 15,0 );


//	modify_current_item( total_items );

	if ( cjoy->db & PAD_DN )
		{

		sfxSpot( HIT1, 30 );

		flash_counter=0;
		if ( shell->current_item < total_items-1 )
			{
			shell->current_item++;
			if ( deselect_option[ title_list ][ shell->current_item ] )
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

			if ( deselect_option[ title_list ][ shell->current_item ] )
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
				share->game_type = FRIENDLY;
				reset_competition_data();

				set_title_func( friendly_thread );
 
				return;
				}

			if ( shell->current_item == 1 )
				{
				/*** FRIENDLY PRESET THREAD ***/
				share->game_type = FRIENDLY;
				reset_competition_data();
				set_title_func( friendly_preset_thread );

				return;
				}


			if ( shell->current_item == 2 )
				{

				shell->old_item=shell->current_item;

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
				*config_store = *config;	/** Store configuration to see if it has changed **/
				title_list=2; //** Goto Config ***/
				shell->current_item=0;
				drop_indicators();

				sfxSpot( BUTVOC, 30 );
				return;


				}


			if ( shell->current_item == 5 )
				{
				set_title_func( training_thread );
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

				share->game_type = -1;
				switch( shell->current_item )
					{
					case 0:
						share->game_type = WORLDCUP;
						set_title_func( worldcup_thread );
					break;

					case 1:
						share->game_type = EURO2000;
						set_title_func( euro2000_thread );
					break;

					case 2:
						share->game_type = EUROFINAL;
						set_title_func( euro_final_thread );
					break;

					case 3:
						share->game_type = TERRITORY;
						set_title_func( territory_thread );
					break;

					case 4:
						share->game_stat = GAME_INIT;
						share->game_type = KNOCKOUT;
						set_title_func( knockout_thread );
					break;

					case 5:
						share->game_type = LEAGUE;
						set_title_func( league_thread );
					break;

					case 6:
						share->game_type = CLASSIC;
						set_title_func( classic_thread );
					break;
					}

				reset_competition_data();

				title_list=0;
				shell->current_item=0;
				return;
				}
			}

		if ( title_list == 2 )
			{
			//**** Configuration ****

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
				set_title_func( config_restore );
				}

			if ( shell->current_item == 5 )
				{
				//*** Previous ***

				set_title_func( check_for_config_save );

				shell->current_item=0;
				title_list=0;


				return;
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
	comp->thread_pos = 0;
	share->game_stat = GAME_INIT;
}








/****************************************************
 *																	 *
 *	 																 *
 * Territories and classic match select screen		 *
 *	 																 *
 *																	 *
 ****************************************************/


#include "incs.h"

#ifdef PSX
#include "\global\striker.def" // defs for PSX striker
#include "\global\s01.h"
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include	"shell.h"
#include	"pad.h"
#include "anim.def"
#include "font.h"
#include "hardware.h"
#include "datafile.h"
#include "text.h"
#include "cmplogic.h"
#endif


WORD territory_team_cnt[]={ TOTAL_SAMERICA_FLAGS, TOTAL_AMERICAS_FLAGS, TOTAL_ASIA_FLAGS,TOTAL_EUROPE_FLAGS, TOTAL_AFRICA_FLAGS, TOTAL_AUSTRALASIA_FLAGS };
//WORD territory_team_cnt[]={ 2,2,2,2,2,2 };
WORD ter_select_stat_buts[]={  SHELL_NEXT,SHELL_QUIT,0 };
WORD ter_ingame_select_stat_buts[]={  SHELL_NEXT,SHELL_QUIT,0 };
WORD ter_save_select_stat_buts[]={ SHELL_SAVE, SHELL_NEXT,SHELL_QUIT, 0 };

WORD cm_select_stat_buts[]={  SHELL_NEXT,SHELL_QUIT,0 };
WORD cm_ingame_stat_buts[]={ SHELL_SAVE,  SHELL_NEXT,SHELL_QUIT,0 };
WORD cm_ingame_nosel_stat_buts[]={ SHELL_SAVE, SHELL_QUIT,0 };




typedef struct
{
	UBYTE team1, team2;
	UBYTE stadium, duration;
	UBYTE offside, weather;
	UBYTE referee, extra_play;
	UBYTE day, human_team;
}Classic_match_preset;

Classic_match_preset classic_match_preset[ MAX_CLASSIC_MATCHES ]={
	2,10,	// teams
	1,3,	// stadium, duration
	1,0,	// offside, weather
	0,0,	// ref, extra play
	1,0,	// day, human team

	5,6,	// teams
	1,3,	// stadium, duration
	1,0,	// offside, weather
	0,0,	// ref, extra play
	1,0,	// day, human team

	11,45,// teams
	1,3,	// stadium, duration
	1,0,	// offside, weather
	0,0,	// ref, extra play
	1,0,	// day, human team

	21,15,// teams
	1,3,	// stadium, duration
	1,0,	// offside, weather
	0,0,	// ref, extra play
	1,0,	// day, human team

	2,9,	// teams
	1,3,	// stadium, duration
	1,0,	// offside, weather
	0,0,	// ref, extra play
	1,0,	// day, human team

	77,78,// teams
	1,3,	// stadium, duration
	1,0,	// offside, weather
	0,0,	// ref, extra play
	1,0,	// day, human team

	32,29,// teams
	1,3,	// stadium, duration
	1,0,	// offside, weather
	0,0,	// ref, extra play
	1,0,	// day, human team

	55,56,// teams
	1,3,	// stadium, duration
	1,0,	// offside, weather
	0,0,	// ref, extra play
	1,0,	// day, human team

};

/****************************************************
 *																	 *
 * Prototypes													 *
 *																	 *
 ****************************************************/

void set_classic_match_teams();
void set_cm_rdbs();

/****************************************************
 *																	 *
 * Defines														 *
 *																	 *
 ****************************************************/

/****************************************************
 *																	 *
 * Vars															 *
 *																	 *
 ****************************************************/

Rdb_gadget ter_rdbs[10];
Sprite *terr_spr[2];

WORD ter_frames[]={ 1,2,4,5,6,3 };
void init_ter_select()
{
WORD y;

	set_title_lolly( territ_select_lolly_text[config->language] , game_types_text[ config->language ][ share->game_type ] );

	if ( share->game_stat == ROUND_OVER )
		set_shell_bottom_buttons( ter_save_select_stat_buts,SHELL_SAVE );
	else
		if ( share->game_stat == GAME_IN_PROGRESS )
			set_shell_bottom_buttons( ter_ingame_select_stat_buts,-1 );
		else
			set_shell_bottom_buttons( ter_select_stat_buts,-1 );


	if ( share->game_stat == GAME_INIT )
		{
		comp->territory_data.completed = 1;
		comp->territory_data.current_territory = 0;
		}


	for( y=0; y<6; y++ )
		{
		set_rdb_gadget( 0, &ter_rdbs[y], 12, -(69*4)+(22*4*y), 456, 16*4, y );
		share->teams_in_group[ y ] = territory_team_cnt[ y ];
		}

	set_rdb_gadget( 0, &ter_rdbs[6], -464, -276, 448, 508, -2 );

	terr_spr[0] = spawn_sprite( TERRIT );
	sprite->x.w.hi = -430;
	sprite->y.w.hi = -224;
	sprite->transp_rate = 1;

	terr_spr[1] = spawn_sprite( TERRIT );
	sprite->frame=1;
	sprite->x.w.hi = -450;
	sprite->y.w.hi = -230;
	sprite->transp_rate = 1;
	sprite->use_frame_offs = YES;

	share->game_difficulty = 0;


}

void update_ter_select()
{
WORD y, color;
Textman_struct *ts;
WORD col,col2;

	share->current_group = comp->territory_data.current_territory;

	modify_current_item( comp->territory_data.completed );

	if ( shell->master_option_active == NO )
		{

		if ( cjoy->db & (PAD_X|PAD_SQU|PAD_STA) )
			{
			comp->territory_data.current_territory = shell->current_item;
			sfxSpot( BUTVOC, 30 );
			}
		}

	for ( y=0; y<6; y++ )
		{
		ts = add_text_string ( tslot_poses[MAIN_TSLOT]+y, territory_names[ config->language ][y] , MILFORD_FONT, milford_font_logic, 240, -(73*4)+(22*4*y), FONT_XC, y, 0, 64,64,0 );
		if ( y > (comp->territory_data.completed-1) )
			{
			color = 32;
			}
		else
			color = 128;

		ts->text_color.r = ts->text_color.g = ts->text_color.b = color;
		}



	add_text_string ( tslot_poses[MAIN_TSLOT]+6, territory_names[ config->language ][ comp->territory_data.current_territory ] , MILFORD_FONT, milford_font_logic, -236, 144, FONT_XC, -2, 0, 64,64,0 );

	terr_spr[1]->frame = ter_frames[ comp->territory_data.current_territory ];

	col  = ((timer/7)&1)?128:255;
//	col2 = ((timer/7)&1)?32:150;
	terr_spr[1]->tint.r = 0; 
	terr_spr[1]->tint.g = col; 
	terr_spr[1]->tint.b = 0;
}

void kill_ter_select()
{
WORD y;
WORD cnt;
WORD cur_ter;


	cur_ter = comp->territory_data.current_territory;

	for( y=0; y<7; y++ )
		{
		kill_rdb_gadget( ter_rdbs[y].gadget );
		}

	share->teams_in_group[share->current_group] = territory_team_cnt[ cur_ter ];


	kill_sprite( terr_spr[0] );
	kill_sprite( terr_spr[1] );

}

void ter_select()
{
	exec_shell( init_ter_select, update_ter_select, kill_ter_select );
}


















WORD cm_page_mode;
void init_cm_select()
{
WORD y,cnt;

	set_title_lolly( cm_select_lolly_text[config->language] , game_types_text[ config->language ][ share->game_type ] );

	if ( share->game_stat == ROUND_OVER )
		set_shell_bottom_buttons( ter_save_select_stat_buts,-1 );
	else
		//if ( share->classic_data.match_chosen )
		//	set_shell_bottom_buttons( cm_ingame_stat_buts,-1 );
		//else
			set_shell_bottom_buttons( cm_ingame_nosel_stat_buts,-1 );	// cant choose next until a game has been chosen


	if ( share->game_stat == GAME_INIT )
		{
		comp->classic_data.current_match = 0;

		/** Only first 4 matches are available **/

		comp->classic_data.available = 4;

		for( cnt=0;cnt<8;cnt++)
			comp->classic_data.completed[cnt] = NO;

		}


	cm_page_mode = 0;
	set_cm_rdbs();



}


void set_cm_rdbs()
{
WORD y;

	if ( cm_page_mode == 0 )
		{
			set_rdb_gadget( 0, &ter_rdbs[0], -412, -324, 412*2, 600, 0 );

		}
	else
		{
//		set_rdb_gadget( 0, &ter_rdbs[8], -348, -320, 348*2, 592, -2 );
		}
}

void kill_cm_rdbs()
{
WORD y;

	if ( cm_page_mode == 0 )
		{
			kill_rdb_gadget( ter_rdbs[0].gadget );
		}

	if ( cm_page_mode == 1 )
		{
		kill_rdb_gadget( ter_rdbs[8].gadget );
		}


}
WORD cm_req_list[]={ SHELL_YES,SHELL_NO, 0 };


void update_cm_select()
{
WORD y;
Textman_struct *ts;
WORD cnt=0;
WORD flash;
CVECTOR color;

	modify_current_item( comp->classic_data.available );

	if ( shell->master_option_active == NO )
		{
		if ( cjoy->db & (PAD_X|PAD_SQU|PAD_STA) )
			{
			comp->classic_data.current_match = shell->current_item;

			shell->module_abort = YES;
			shell->current_button = SHELL_OK;
			return;
			}
		}

	for ( y=0; y<MAX_CLASSIC_MATCHES; y++ )
		{
		if ( y >= comp->classic_data.available )	
			{
			color.r = color.g = color.b = 32;
			}
		else
			{
			if ( comp->classic_data.completed[ y ] )
				{
				color.r = 192;	// Yellow text for first 4 matches completed
				color.g = 192;
				color.b = 64;
				}
			else
				color.r = color.g = color.b = 128;
			}

		/** Print left team **/

		sprintf( text_buffer, "%s %s %s", (UBYTE *)get_team_name( classic_match_preset[ y ].team1 ) , vs_text[ config->language], (UBYTE *)get_team_name( classic_match_preset[ y ].team2 ) );

		ts = add_text_string ( tslot_poses[MAIN_TSLOT]+cnt, text_buffer , MILFORD_FONT, milford_font_logic, 0, -(336)+(19*4*y), FONT_XC, y, 0, 64,64,0 );
		ts->text_color.r = color.r; 
		ts->text_color.g = color.g;
		ts->text_color.b = color.b;
		cnt++;
		}
}

void kill_cm_select()
{
	kill_cm_rdbs();
}

void cm_select()
{
	while(1)
		{
		exec_shell( init_cm_select, update_cm_select, kill_cm_select );

		if ( shell->current_button == SHELL_OK )
			{
			/** User chose a match to play **/
			shell->current_button = do_request( classic_match_texts[ config->language ][comp->classic_data.current_match], cm_req_list, -10*4, SHELL_YES );
			if ( shell->current_button == SHELL_YES )
				{
				comp->thread_pos++;
				break;
				}
			}
		else
			return;	/* User chose quit or save */
		}

	set_classic_match_teams();

}





void set_classic_match_teams()
{
	if ( classic_match_preset[ comp->classic_data.current_match ].human_team == 0 )
		{
		share->team1_cpu = NO;
		share->team2_cpu = YES;
		}
	else
		{
		share->team2_cpu = NO;
		share->team1_cpu = YES;
		}

	share->team1 = classic_match_preset[ comp->classic_data.current_match ].team1;
	share->team2 = classic_match_preset[ comp->classic_data.current_match ].team2;
}













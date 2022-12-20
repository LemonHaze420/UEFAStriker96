/****************************************************
 *																	 *
 *	 																 *
 * League stats screen										 *
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
#include "teamsel.h"
#endif

BYTE *euro_finalist_teams_lolly_text[]={
	"Finalists",
};

BYTE *eq_group_winner_text[]={
	"Group winners",
};

BYTE *eq_group_runner_text[]={
	"Group runner up",
};

BYTE *euro_game_over_lolly_text[]={
	"Final standings",
};

BYTE *euro_result_lolly_text[]={
	"Results",
};

BYTE *euro_next_match_lolly_text[]={
	"Next match",
};

BYTE *euro_qual_teams_lolly_text[]={
	"Qualifiers",
};

BYTE *worldcup_game_over_lolly_text[]={
	"End of stage 1",
};

BYTE *league_stat_lolly_text[]={
	"League standings",
	"League standings",
	"League standings",
};


BYTE *final_league_stat_lolly_text[]={
	"Final league standings",
	"League standings",
	"League standings",
};



BYTE *territory_stat_lolly_text[6][3]={
	"South American standings",
	"South American standings",
	"South American standings",

	"Americas standings",
	"Americas standings",
	"Americas standings",

	"Asian standings",
	"Asian standings",
	"Asian standings",

	"European standings",
	"European standings",
	"European standings",

	"African standings",
	"African standings",
	"African standings",

	"Australasian standings",
	"Australasian standings",
	"Australasian standings",


};

BYTE *final_territory_stat_lolly_text[6][3]={
	"Final South American standings",
	"Final South American standings",
	"Final South American standings",

	"Final Americas standings",
	"Final Americas standings",
	"Final Americas standings",

	"Final Asian standings",
	"Final Asian standings",
	"Final Asian standings",

	"Final European standings",
	"Final European standings",
	"Final European standings",

	"Final African standings",
	"Final African standings",
	"Final African standings",

	"Final Australasian standings",
	"Final Australasian standings",
	"Final Australasian standings",


};





BYTE *lstat_txt[][8]={
	"Pld", "W", "D", "L", "F", "A", "GD", "Pts",
	"Pld", "W", "D", "L", "F", "A", "GD", "Pts",
};




/****************************************************
 *																	 *
 * Prototypes													 *
 *																	 *
 ****************************************************/

//void set_lstat_teams_list();

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


UWORD league_init_stat_buts[]= { SHELL_PREV,  SHELL_SAVE, SHELL_NEXT, 0 };
UWORD league_stat_buts[]= {  SHELL_SAVE, SHELL_NEXT,SHELL_QUIT, 0 };	// when a game is in progress
//UWORD league_stat_buts_end[]= { SHELL_SAVE, SHELL_QUIT, 0 };	// when a game is over
UWORD league_stat_buts_end[]= { SHELL_NEXT,SHELL_SAVE, SHELL_QUIT, 0 };	// when a game is over

UWORD territory_stat_buts[]= {  SHELL_NEXT,SHELL_QUIT, 0 };
UWORD euro_qual_teams_buts[]= { SHELL_NEXT, 0 };
UWORD euro_final_teams_buts[]= { SHELL_NEXT,SHELL_SAVE, SHELL_QUIT,  0 };
UWORD next_quit_buts[]= { SHELL_NEXT, SHELL_QUIT, 0 };

WORD lstat_yp;
WORD ls_group;

//BYTE *lstat_teams_list;

Rdb_gadget league_stat_rdb[ 3 ];


void reset_league_data()
{
WORD cnt, cnt2, group;

	for( group=0; group<TOTAL_GROUPS; group++ )
		{
		for( cnt=0; cnt< TOTAL_LEAGUE_TEAMS; cnt++ )
			{
			share->league_data[ group ][ cnt ].played 			= 
			share->league_data[ group ][ cnt ].won					= 
			share->league_data[ group ][ cnt ].drawn	 			= 
			share->league_data[ group ][ cnt ].lost	 			= 
			share->league_data[ group ][ cnt ].goals_for			= 
			share->league_data[ group ][ cnt ].goals_against	= 
			share->league_data[ group ][ cnt ].goal_diff			= 
			share->league_data[ group ][ cnt ].points				= 0;
			}
		}

//	share->teams_in_competition = 8;
}


#define LSTAT_YSPACE 68

void init_league_stat_page()
{
WORD cnt,yoffs;
WORD x, gr, team;

	sort_league_stats();


	if ( share->game_stat == GAME_OVER )
		{
		share->current_group = 0;

		/** When GAMEOVER, Set "share->current_group" to point to first group in list that contains a human team **/
		for(gr=0;gr<share->total_groups; gr++ )
			{
			for(x=0;x<share->teams_in_group[ gr ]; x++ )
				{
				team = share->group_teams[ gr ][x];
				if ( share->cpu_teams_index_list[ team ] == NO )
					{
					share->current_group = gr;
					break;
					}
				}
			}
		}


	ls_group = share->current_group;





	//set_lstat_teams_list();	// point lstat_teams_list to list of teams for this group competition

//	yoffs = ((((TOTAL_LEAGUE_TEAMS-share->teams_in_competition) * LSTAT_YSPACE )/2)|3)+1;
//	lstat_yp = -276 + yoffs;

	switch ( share->game_type )
		{
		case TERRITORY:
			set_shell_bottom_buttons( territory_stat_buts,0 );

			if ( share->game_stat == ROUND_OVER )
				{
				set_title_lolly( final_territory_stat_lolly_text[ share->territory_data.current_territory ][config->language], game_types_text[ config->language ][ share->game_type ]  );
				}
			else
				{
				set_title_lolly( territory_stat_lolly_text[ share->territory_data.current_territory ][config->language], game_types_text[ config->language ][ share->game_type ]  );
				}
		break;

		case EURO2000:
			lstat_yp -= 10*4;
			if ( share->game_stat == GAME_OVER )
				{
				set_title_lolly( euro_game_over_lolly_text[config->language], game_types_text[ config->language ][ share->game_type ]  );
				set_shell_bottom_buttons( league_stat_buts_end,0 );
				set_rdb_gadget( 0, &league_stat_rdb[2], -1, -1, -1, -1, 0 );
				}
			else
				{

				if ( share->game_stat == GAME_IN_PROGRESS )
					{
					//set_rdb_gadget( 0, &league_stat_rdb[2], -488, lstat_yp+4+560-(yoffs*2)+32, 972, 64, -1 );
					set_rdb_gadget( 0, &league_stat_rdb[2], -1, -1, -1, -1, -1 );

					sprintf( text_buffer, euro_result_lolly_text[config->language] );
					set_title_lolly( text_buffer , game_types_text[ config->language ][ share->game_type ] );
					set_shell_bottom_buttons( next_quit_buts,0 );
					}
				else
					{
					set_shell_bottom_buttons( league_stat_buts,0 );
					//set_rdb_gadget( 0, &league_stat_rdb[2], -488+200, lstat_yp+4+560-(yoffs*2)+32, 972-400, 64, 0 );
					set_rdb_gadget( 0, &league_stat_rdb[2], -1, -1, -1, -1 , 0 );

					sprintf( text_buffer, euro_next_match_lolly_text[config->language]  );
					set_title_lolly( text_buffer , game_types_text[ config->language ][ share->game_type ] );
					}
				}
		break;
		
		case WORLDCUP:
		case EUROFINAL:
			lstat_yp -= 10*4;
			if ( share->game_stat == GAME_OVER )
				{
				if ( share->game_type == EUROFINAL )
					set_title_lolly( euro_game_over_lolly_text[config->language] , game_types_text[ config->language ][ share->game_type ] );
				else
					set_title_lolly( worldcup_game_over_lolly_text[config->language] , game_types_text[ config->language ][ share->game_type ] );

				set_shell_bottom_buttons( league_stat_buts_end,0 );
				set_rdb_gadget( 0, &league_stat_rdb[2], -1, -1, -1, -1, 0 );
				}
			else
				{
				if ( share->game_stat == GAME_IN_PROGRESS )
					{
					set_rdb_gadget( 0, &league_stat_rdb[2], -1, -1, -1, -1, -1 );
					sprintf( text_buffer, euro_result_lolly_text[config->language] );
					set_title_lolly( text_buffer , game_types_text[ config->language ][ share->game_type ] );
					set_shell_bottom_buttons( next_quit_buts,0 );
					}
				else
					{
					set_shell_bottom_buttons( league_stat_buts,0 );
					set_rdb_gadget( 0, &league_stat_rdb[2], -1, -1, -1, -1 , 0 );

					sprintf( text_buffer, euro_next_match_lolly_text[config->language] );
					set_title_lolly( text_buffer , game_types_text[ config->language ][ share->game_type ] );
					}
				}
		break;

		default:
			/**** Normal League GAME ********/

			if ( share->game_stat == GAME_OVER )
				set_title_lolly( final_league_stat_lolly_text[config->language] , game_types_text[ config->language ][ share->game_type ] );
			else
				set_title_lolly( league_stat_lolly_text[config->language] , game_types_text[ config->language ][ share->game_type ] );

			if ( share->game_stat == GAME_INIT )
				set_shell_bottom_buttons( league_init_stat_buts,0 );
			else
				{
				if ( share->game_stat == GAME_OVER )
					set_shell_bottom_buttons( league_stat_buts_end,0 );
				else
					set_shell_bottom_buttons( league_stat_buts,0 );
				}
		break;
		}



//	set_rdb_gadget( 0, &league_stat_rdb[0], -488, lstat_yp+4, 972, 560-(yoffs*2), -1 );
//	set_rdb_gadget( 0, &league_stat_rdb[1], -112, lstat_yp-76, 596, 64, -1 );

	set_rdb_gadget( 0, &league_stat_rdb[0], -1, -1, -1, -1 ,-1 );
	set_rdb_gadget( 0, &league_stat_rdb[1], -1, -1, -1, -1, -1 );

}


void ls_set_rdbs()
{
WORD yoffs;

	yoffs = ((((TOTAL_LEAGUE_TEAMS-share->teams_in_competition) * LSTAT_YSPACE )/2)|3)+1;
	lstat_yp = -276 + yoffs;

	if ( (share->game_type == EURO2000) || ( share->game_type == EUROFINAL) || ( share->game_type == WORLDCUP)  )
		{
		lstat_yp -= 10*4;
		if ( share->game_stat == GAME_IN_PROGRESS )
		 	set_rdb_box( &league_stat_rdb[2], -488, lstat_yp+4+560-(yoffs*2)+32, 972, 64 );
		else
			set_rdb_box( &league_stat_rdb[2], -488+200, lstat_yp+4+560-(yoffs*2)+32, 972-400, 64 );
		}


 	set_rdb_box( &league_stat_rdb[0], -488, lstat_yp+4, 972, 560-(yoffs*2) );
 	set_rdb_box( &league_stat_rdb[1], -112, lstat_yp-76, 596, 64 );
}


void update_league_stat_page()
{
WORD cnt, index_cnt;
WORD x,y;

WORD xspace[]={ 0, 16, 32, 48, 66, 66+20, 66+40, 66+58 };
Textman_struct *ts;
WORD team_num, team_cnt;

WORD index_y;
WORD text_cnt=0;
/********
 *
 *	pts max   = 32767
 * gs max    = 32767
 * gd max    = 32767
 * alpha max = 1023
 *
 ********/


	team_cnt = share->teams_in_group[ ls_group ]; 
	share->teams_in_competition = team_cnt;
 	ls_set_rdbs();


 	//set_lstat_teams_list();





//*** Print team name in left column ***
	for ( cnt=0; cnt<team_cnt; cnt++ )
		{
  		index_cnt = share->league_data[ ls_group ][ cnt ].win_pos;

		FntPrint("index=%d\n",index_cnt);

		#if DEBUG 
			if ( cjoy->c & PAD_L1 ) index_cnt = cnt;
		#endif

		team_num = share->group_teams[ ls_group ][ index_cnt ];
		team_num = share->group_teams[ ls_group ][ cnt ];

		if ( cnt == 0 )
			{
			share->winning_team = team_num;
			}

		sprintf( text_buffer, "%s\n", get_team_name( team_num ) );
		ts = add_text_string ( tslot_poses[MAIN_TSLOT]+text_cnt, text_buffer , MILFORD_FONT, milford_font_logic, -616, lstat_yp+(index_cnt*LSTAT_YSPACE)-0, FONT_RJUST, -2, 0, 8,0 );

		ts->flash_text = NO;
		if ( (share->game_stat != GAME_OVER) && (share->game_stat != ROUND_OVER ) )
			{
			if ( ( team_num == share->team1) || ( team_num == share->team2 ) )
				ts->flash_text = YES;
			}

		if ( share->cpu_teams_index_list[ team_num ] == NO )
			{
			ts->text_color.r = 180;
			ts->text_color.g = 180;
			ts->text_color.b = 0;
			}
		text_cnt++;
		}

//*** Print 'played won drawn texts ***
	for ( x=0; x<8; x++ )
		{
		add_text_string ( tslot_poses[MAIN_TSLOT]+text_cnt, lstat_txt[config->language][x] , MILFORD_FONT, milford_font_logic, (-15+xspace[x])*4, lstat_yp-92, FONT_XC, -2, 0, 8,0 );
		text_cnt++;
		}


//*** Print Each teams stats ***
	for ( cnt=0; cnt<team_cnt; cnt++ )
		{
  		index_cnt = share->league_data[ ls_group ][ cnt ].win_pos;
		#if DEBUG 
		if ( cjoy->c & PAD_L1 )
			index_cnt = cnt;
		#endif
		for ( x=0; x<8; x++ )
			{
			if ( x == 0 ) sprintf( text_buffer, "%d",share->league_data[ ls_group ][cnt].played );
			if ( x == 1 ) sprintf( text_buffer, "%d",share->league_data[ ls_group ][cnt].won );
			if ( x == 2 ) sprintf( text_buffer, "%d",share->league_data[ ls_group ][cnt].drawn );
			if ( x == 3 ) sprintf( text_buffer, "%d",share->league_data[ ls_group ][cnt].lost );
			if ( x == 4 ) sprintf( text_buffer, "%d",share->league_data[ ls_group ][cnt].goals_for );
			if ( x == 5 ) sprintf( text_buffer, "%d",share->league_data[ ls_group ][cnt].goals_against );
			if ( x == 6 ) sprintf( text_buffer, "%d",share->league_data[ ls_group ][cnt].goal_diff );
			if ( x == 7 ) sprintf( text_buffer, "%d",share->league_data[ ls_group ][cnt].points );

			add_text_string ( tslot_poses[MAIN_TSLOT]+text_cnt, text_buffer , MILFORD_FONT, milford_font_logic, (-15+xspace[x])*4, lstat_yp+(index_cnt*LSTAT_YSPACE)-0, FONT_XC, -2, 0, 8,0 );
			text_cnt++;
			}
		}
	

	if ( (share->game_type == EURO2000) || (share->game_type == EUROFINAL) || (share->game_type == WORLDCUP)  )
		{
		if ( share->game_stat == GAME_IN_PROGRESS ) 
			{
			/*** Show two teams who played ***/

			sprintf(text_buffer,"%s %d - %d %s\n",get_team_name( share->team1 ), share->team1_pts,  share->team2_pts, get_team_name( share->team2 ) );
			add_text_string ( tslot_poses[MAIN_TSLOT]+text_cnt, text_buffer , MILFORD_FONT, milford_font_logic, 0, league_stat_rdb[2].y1-16, FONT_XC, -2, 0, 8,0 );
			text_cnt++;
			}
		else
			{
			/** Modify current group **/
			modify_current_item( 1 );

			sprintf( text_buffer,  group_text[ config->language ], ls_group+1 );
			add_text_string ( tslot_poses[MAIN_TSLOT]+text_cnt, text_buffer , MILFORD_FONT, milford_font_logic, 0, league_stat_rdb[2].y1-16, FONT_XC, 0, 0, 8,0 );
			text_cnt++;

			if ( shell->current_item == 0 )
				modify_word( &ls_group, 0, share->total_groups-1 );
			}
	   } 

}





void kill_league_stat_page()
{
WORD abort;



	kill_shell_bottom_buttons();


	kill_rdb_gadget( league_stat_rdb[0].gadget );
	kill_rdb_gadget( league_stat_rdb[1].gadget );

	if ( (share->game_type == EURO2000) || (share->game_type == EUROFINAL) || (share->game_type == WORLDCUP)   )
		kill_rdb_gadget( league_stat_rdb[2].gadget );
	



}


void league_stat_screen()
{

	exec_shell( init_league_stat_page, update_league_stat_page, kill_league_stat_page );



}





void init_euro2000_qual_results()
{
	if ( share->game_type == EURO2000 )
		{
		set_title_lolly( euro_finalist_teams_lolly_text[config->language], game_types_text[ config->language ][ share->game_type ]  );
		set_shell_bottom_buttons( euro_qual_teams_buts,0 );
		}
	else
		{
		set_title_lolly( euro_finalist_teams_lolly_text[config->language] , game_types_text[ config->language ][ share->game_type ] );
		set_shell_bottom_buttons( euro_final_teams_buts,0 );
		}


	set_rdb_gadget( 0, &league_stat_rdb[0], -208, -340, 416, 612, 0 );

 
}
void update_euro2000_qual_results()
{
BYTE *text;
WORD cnt,team, text_cnt;
Textman_struct *ts;

	text_cnt=0;

	if ( share->game_type == EURO2000 )
		{
		/** Group winner title ***/
		add_text_string ( tslot_poses[MAIN_TSLOT]+text_cnt, eq_group_winner_text[config->language] , MILFORD_FONT, milford_font_logic, 0, -316, FONT_XC, -2, 0, 8,0 );
		text_cnt++;

		/*** List of winners ****/
		for( cnt=0; cnt<9; cnt++ )
			{
			team = share->euro_final_data.teams[ cnt ]; 
			text = get_team_name( team );
			ts = add_text_string ( tslot_poses[MAIN_TSLOT]+text_cnt, text , TINY_FONT, milford_font_logic, 0, -256+(cnt*40), FONT_XC, -2, 0, 8,0 );

			/** Human teams in yellow **/
			if ( share->cpu_teams_index_list[ team ] == NO )
				{
				ts->text_color.r = ts->text_color.g = 180;
				ts->text_color.b = 0;
				}
			text_cnt++;
			}

		/*** Runner up title ***/
		add_text_string ( tslot_poses[MAIN_TSLOT]+text_cnt, eq_group_runner_text[config->language] , MILFORD_FONT, milford_font_logic, 0, 128, FONT_XC, -2, 0, 8,0 );
		text_cnt++;

		/*** Runner up team ***/
		cnt=10;
		team = share->euro_final_data.teams[ 9 ];  
		text = get_team_name( team );
		ts = add_text_string ( tslot_poses[MAIN_TSLOT]+text_cnt, text , TINY_FONT, milford_font_logic, 0, -256+(10*4)+(cnt*40), FONT_XC, -2, 0, 8,0 );
		/** Human teams in yellow **/
		if ( share->cpu_teams_index_list[ team ] == NO )
			{
			ts->text_color.r = ts->text_color.g = 180;
			ts->text_color.b = 0;
			}

		text_cnt++;
		}


	if ( share->game_type == EURO2000_KO )
		{
		for( cnt=0; cnt<16; cnt++ )
			{
			team = share->euro_final_data.teams[ cnt ];
			text = get_team_name( team );
			ts = add_text_string ( tslot_poses[MAIN_TSLOT]+text_cnt, text , TINY_FONT, milford_font_logic, 0, -384+(11*4)+(cnt*36), FONT_XC, -2, 0, 8,0 );
			/** Human teams in yellow **/
			if ( share->cpu_teams_index_list[ team ] == NO )
				{
				ts->text_color.r = ts->text_color.g = 180;
				ts->text_color.b = 0;
				}
			text_cnt++;
			}
		}


}
void kill_euro2000_qual_results()
{
	kill_rdb_gadget( league_stat_rdb[0].gadget );
}



void euro2000_qual_results()
{
WORD human, cnt, team;

/*** Show 9 teams through to final and runner up team ***/

	exec_shell( init_euro2000_qual_results, update_euro2000_qual_results,  kill_euro2000_qual_results );

	human = NO;
/*** Check for existance of a human team through to Euro final ***/
	if ( share->game_type == EURO2000 )
		{

	//* Are any of the finalists human ?
		for( cnt=0; cnt<10; cnt++ )
			{
			team = share->euro_final_data.teams[ cnt ];
			if ( share->cpu_teams_index_list[ team ] == NO )
				human = YES;
			}

	//* Are any of the 8 runner ups human ?
		for( cnt=0; cnt<8; cnt++ )
			{
			team = share->euro_final_data.runner_teams[ cnt ];
			if ( share->cpu_teams_index_list[ cnt ] == NO )
				human = YES;
			}

		}


	if ( share->game_type == EURO2000_KO )
		{
	//* Are any of the finalists human ?
		for( cnt=0; cnt<16; cnt++ )
			{
			team = share->euro_final_data.teams[ cnt ];
			if ( share->cpu_teams_index_list[ team ] == NO )
				human = YES;
			}
		}



	if ( human == NO )
		{
		share->game_stat = GAME_OVER;
		}
	else
		share->game_stat = GAME_IN_PROGRESS;

}

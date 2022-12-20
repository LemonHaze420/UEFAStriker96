/****************************************************
 *																	 *
 *	 Frontend ( Main icon selection )					 *
 *																	 *
 ****************************************************/

#include "incs.h"

#ifdef PSX
#include "..\..\global\striker.def" // defs for PSX striker
#include "..\..\global\s01.h"	// structs used by scene "s01" format
#include "..\..\striker\frontend\mod.def"
#include "..\..\striker\src\anim.def"

#include "options.h"
#include "teamedit.h"
#include "preset.h"
#include	"sod.h"
#include	"scene.h"
//#include	"structs.h"
#include "datafile.h"
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include "hardware.h"
#include "polydraw.h"	//psx structs
#include "text.h"
#include	"font.h"
#include "plasma.h"
#include	"render.h"
#include	"anim.h"
#include	"mem.h"
#include	"fvars.h"
#include	"pad.h"
#include "utils.h"
#include "entname.h"
#include "movie.h"
#include "..\..\global\image.h"
#include "psxmacro.h"
#include "teamsel.h"
#include "manage.h"

#include "title.h"
#include "langsel.h"
#include "shell.h"
#include "hwcard.h"
#include "knockout.h"
#include "territ.h"
#include "cmplogic.h"
#include "cdaudio.h"
#include "teams.h"
#endif


/****************************************************
 *																	 *
 *	 Defines														 *
 *																	 *
 ****************************************************/


/****************************************************
 *																	 *
 *	 Structs														 *
 *																	 *
 ****************************************************/




/****************************************************
 *																	 *
 *	 Prototypes													 *
 *																	 *
 ****************************************************/

void store_game_undo_info();
void restore_game_undo_info();
void init_frontend();
void league_thread();
void knockout_thread();
void classic_thread();
void euro2000_thread();

void thread_save_quit_req();
void thread_game_over_req();
void thread_load_req();
void start_frontend_track();

void training_thread();


/****************************************************
 *																	 *
 *  Vars															 *
 *																	 *
 ****************************************************/


Movie_info rage_logo =
{
	"\\RAGE.STR;1",		// filename
	MOVIE_24BIT,	 	// display pixel depth
	320,				 	// x res
	0, 0, 			 	// x/y offset
	320, 240,		 	// x/y size ( must match converted av file or BOOM )
	305,				 	// last frame no
	18000<<2,		  	 	// vlc buffer size ( << 2 for lw ) about 9000 for 320X240X24
	127,				 	// volume
	0,18,					// scn xy offs
};

Movie_info infogram_logo =
{
	"\\INFOGRAM.STR;1",		// filename
	MOVIE_24BIT,	 	// display pixel depth
	320,				 	// x res
	0, 0, 			 	// x/y offset
	320, 240,		 	// x/y size ( must match converted av file or BOOM )
	88,				 	// last frame no
	18000<<2,		  	 	// vlc buffer size ( << 2 for lw ) about 9000 for 320X240X24
	127,				 	// volume
	0,18,					// scn xy offs
};

Movie_info coolkat =
{
	"\\COOLKAT.STR;1",// filename
	MOVIE_24BIT,	 	// display pixel depth
	320,				 	// x res
	0, 0, 			 	// x/y offset
	320, 240,		 	// x/y size ( must match converted av file or BOOM )
	400,				 	// last frame no
	18000<<2,		 	// vlc buffer size ( << 2 for lw ) about 9000 for 320X240X24
	127,				 	// volume
	0,18,					// scn xy offs
};




BYTE text_buffer[256];
BYTE text_buffer2[256];

WORD thread_quit_req_list[]= { SHELL_YES, SHELL_NO, 0 };
WORD thread_game_over_req_list[]= { SHELL_OK, 0 };
WORD thread_ok_butt[]= { SHELL_OK, 0 };
WORD thread_yes_no_req_list[]= { SHELL_YES, SHELL_NO, 0 };

WORD abort_frontend,abort_game;


void run_frontend()
{
BYTE *addr;
LONG cnt;


	initsound( 127, 127 );
	init_memcard_bootup();


  	init_frontend();
	add_backdrop( BK );
	stop_animating_man();
	run_title();

/** About to exit to stub **/

}




void init_frontend()
{

//	load_scene_materials ( FRONTEND_TEX, NO );

	init_process_executor();

	alloc_screen_dbuffs();

	init_display( 512, 256, 0,0,0, YES, YES, NO );	//screen xy res & bg color

//	start_animating_man();
	load_scene_materials ( FRONTEND_TEX, NO );

	allocate_mem( 0, 240000 );

	init_select_num_teams_bootup();
	init_player_edit_bootup();
	init_team_edit_bootup();
	init_team_select_bootup();
	init_options_bootup();
	init_preset_bootup();



	init_shell();


	shell->mode = FRONTEND;

	*config_store = *config;

}




void league_thread()
{
WORD all_teams_played=NO;
WORD result;


	start_frontend_track();

	abort_frontend=NO;

	push(0);

	share->game_type = LEAGUE;
	share->current_group = 0;
	share->total_groups = 1;

	thread_load_req();


	while(abort_frontend == NO )
		{
		switch( comp->thread_pos )
			{
			case -1:
				abort_frontend=YES;
				abort_title=NO;
				share->title_func = 0; // league is over
			break;

			case 0:
				reset_league_data();
				select_num_teams();

				thread_save_quit_req();

				comp->perform_draw = YES;
				share->team1 = -1;
				share->team2 = -1;

				share->teams_in_group[0] = share->teams_in_competition;

			break;

			case 1:
				team_select();
				init_league_draw();
				thread_save_quit_req();
			break;

			case 2:
				options();

				thread_save_quit_req();
			break;

			case 3:

				if ( comp->perform_draw == YES )
					{

					comp->perform_draw = NO;

					all_teams_played = do_league_draw();

					if ( all_teams_played )
						{
						/** Was league winner human ? **/
						if ( check_human_territory_winner() )
							{
							comp->thread_pos = 9;
							continue;
							}
						else
							share->game_stat = GAME_OVER;
						}
					}

				if ( (share->team1_cpu && share->team2_cpu) && (share->game_stat != GAME_OVER) )
					{
					comp->thread_pos = 7;
					}
				else
					{
					store_game_undo_info();	/* Store round pos etc, in case player abandons this game */
					league_stat_screen();	// show stats
					}

				thread_game_over_req();	// sets abort_frontend
				thread_save_quit_req();
			break;

			case 4:
				pad_assign();
				thread_save_quit_req();
			break;

			case 5:
				team_manager();
				thread_save_quit_req();
			break;

			case 6:
				team_stats();
				thread_save_quit_req();
			break;

			case 7:
				if ( calculate_winner() )
					{
					/** A winner has been chosen **/
					comp->thread_pos = 8;	 /* goto 6 when game has been played */
					comp->perform_draw = YES; 
					}
				else
					{
					/** Don't want to calculate winner, so exit frontend and play game **/
					comp->thread_pos = 8; // goto 6 when game has been played
					comp->perform_draw = YES; 
					}
			break;

			case 8:
				update_game_stats();

				share->game_stat = GAME_IN_PROGRESS;	// now we are into a game
				comp->thread_pos = 3;
				calc_league_stats();
			break;

			case 9:
				share->game_stat = GAME_OVER;
				league_stat_screen();	// show stats
				thread_save_quit_req();
			break;

			case 10:
				do_request( "you came top of the\nleague, hurrah etc\n", thread_ok_butt, -20*4, SHELL_OK );
				abort_frontend=YES;
				share->title_func=0;
				continue;
			break;

			}
		}

	pop(0);

}


void friendly_thread()
{

	start_frontend_track();

	abort_frontend=NO;

	push(0);

	share->teams_in_competition = 2;
	share->game_type = FRIENDLY;
	share->total_groups = 1;
	share->total_human_teams = 2;

	while(abort_frontend == NO )
		{
		switch( comp->thread_pos )
			{
			case 0:
				select_num_teams();

				if ( comp->thread_pos == -1 )
					{
					comp->thread_pos=0;
					shell->current_button = SHELL_QUIT;
					}	   

				thread_save_quit_req();
	  		break;

			case 1:

				team_select();

				thread_save_quit_req();
				if ( abort_frontend == NO )
					{
					share->team1_cpu = share->cpu_teams_index_list[ share->team1 ];
					share->team2_cpu = share->cpu_teams_index_list[ share->team2 ];
					}

//				*config_store = *config;
			break;

			case 2:

				//pad_assign();

				options();

				if ( shell->current_button == SHELL_STORE )
					{
					comp->thread_pos = 5;
					}
				else
					{
					thread_save_quit_req();
					}


			break;

			case 3:

				check_for_config_save();
		

				pad_assign();
				thread_save_quit_req();
			break;

			case 4:

				calculate_winner();
				share->title_func=0;
	  			
			break;

			case 5:
				share->game_type = FRIENDLY_SAVE;
				preset();
				share->game_type = FRIENDLY;

				comp->thread_pos = 2;


			break;


			}


		}



	pop(0);

}


void friendly_preset_thread()
{

	start_frontend_track();


	push(0);

	abort_frontend=NO;
	share->game_type = FRIENDLY;

	share->total_groups = 1;

	while(abort_frontend == NO )
		{
		switch( comp->thread_pos )
			{
			case -1:
			abort_frontend=YES;
			abort_title=NO;
			share->title_func=0;
			break;

			case 0:
				preset();
				thread_save_quit_req();

//				if ( ((share->team1_cpu==NO)&& (share->team2_cpu==NO)) && (pads_connected == 1) )
// 					{
//					do_request( preset_not_enough_pads_req[config->language], thread_ok_butt, -128, SHELL_OK );
//					comp->thread_pos=0;
//					}

			break;

			case 1:
				pad_assign();
				thread_save_quit_req();
			break;


			case 2:

				if ( calculate_winner() )
					{
					}
				else
					{
					abort_title = YES;
					}

				abort_frontend = YES;
				share->title_func = 0;



			break;
			}
		}

//	abort_title=YES;
	pop(0);

}


void knockout_thread()
{
WORD game_over;
WORD result;


	start_frontend_track();


	abort_frontend = NO;

	push(0);

	share->total_groups = 1;


	thread_load_req();

	while( abort_frontend == NO )
		{
		switch ( comp->thread_pos )
			{
			case 0:
				select_num_teams();

				thread_save_quit_req();

				comp->perform_draw = YES;
			break;

			case 1:
				team_select();
				thread_save_quit_req();

				comp->ko_data.page_mode = -1;

				init_knockout_data();
			break;

			case 2:
				options();
				thread_save_quit_req();
				
			
			break;

			case 3:

				if ( comp->perform_draw == YES )
					{
					comp->perform_draw = NO;

					if ( share->game_type == WORLDCUP )
						do_worldcup_knockout_draw();
					else
						do_knockout_draw();

					
					if ( share->game_stat == GAME_OVER )
						{
						thread_game_over_req();		/** Check if no human teams made it **/
						continue;
						}
					}

				if ( (share->team1_cpu && share->team2_cpu) && (share->game_stat != GAME_OVER) && ( share->game_stat != GAME_INIT) )
					{
					comp->thread_pos = 5+2;
					}
				else
					{
					store_game_undo_info();	/* Store round pos etc, in case player abandons this game */

					knockout_stat_page();	// show teams to play

					if ( share->game_type == KNOCKOUT )
						{
						if ( shell->current_button == SHELL_OK )
							{
						/** Knockout is over **/

							abort_frontend = YES;
							share->title_func = 0;
							}
						}
					else
						{
						if ( shell->current_button == SHELL_NEXT )
							{
							if ( share->game_stat == GAME_OVER )
								{
								/** Knockout is over **/
								if ( share->game_type == EURO2000_KO )
									share->title_func = euro2000_thread;
								else
									share->title_func = euro_final_thread;

								abort_frontend = YES;
								comp->thread_pos = 11+2;
								share->game_stat = GAME_IN_PROGRESS;
								share->new_game = YES;
								}
							}
						}

					thread_save_quit_req();
					}	
			break;

			case 4:
				pad_assign();
				thread_save_quit_req();
			break;

			case 5:
				
				team_manager();
				thread_save_quit_req();

			break;

			case 6:
				team_stats();
				thread_save_quit_req();
			break;

			case 7:

				if ( calculate_winner() )
					{
					}
				else
					{
					abort_title = YES;
					abort_frontend = YES;
					}

				comp->thread_pos = 6+2; // go to 6 after game played
				comp->perform_draw = YES; 

			break;

			case 8:
			/** Pc comes here after game played **/

				update_game_stats();

				if ( share->game_stat == QUIT_COMPETITION )
					{
					share->title_func = 0;
					abort_frontend = YES;
					}
				else
					{
					share->game_stat = GAME_IN_PROGRESS;
					comp->ko_data.page_mode = 0; // signal to knockout page that a game has just been played
					comp->thread_pos = 3;
					ko_calc_results();
					comp->perform_draw=YES;

					if ( share->game_stat == GAME_OVER )
						comp->perform_draw = NO;
					}

			break;
			}
		}


	pop(0);
}




void territory_thread()
{
WORD all_teams_played=NO;
WORD result;


	start_frontend_track();



	abort_frontend=NO;

	push(0);

	share->game_type = TERRITORY;
//	share->current_group = 0;
	share->total_groups = 6;

//	share->teams_in_competition = 2;
	share->total_human_teams = 1;


	thread_load_req();


	while(abort_frontend == NO )
		{
		switch( comp->thread_pos )
			{
			case -1:
				abort_frontend=YES;
				abort_title=NO;
				share->title_func = 0; // territory game is over
			break;

			case 0:
				share->total_human_teams = 1;
				ter_select();	//*** Select Territory 
				thread_save_quit_req();
			break;

			case 1:

				team_select();	// select the human team for this territory only

				thread_save_quit_req();


				reset_league_data();
				init_league_draw();

						  



				comp->perform_draw = YES;
				share->team1 = -1;
				share->team2 = -1;
				init_league_draw();
			break;

			case 2:
				options();
				thread_save_quit_req();

			break;

			case 3:

				share->game_stat = GAME_IN_PROGRESS;			// now we are into a game


				if ( comp->perform_draw == YES )
					{
					//store_game_undo_info();	/* Store round pos etc, in case player abandons this game */
					comp->perform_draw = NO;
					all_teams_played = do_league_draw();
					if ( all_teams_played )
						{
						share->game_stat = ROUND_OVER;
						}
					}

				if ( (share->team1_cpu && share->team2_cpu) && ( share->game_stat != ROUND_OVER) )
					{
					comp->thread_pos = 4;
					}
				else
					{
					store_game_undo_info();
					league_stat_screen();	// show final stats
					}


				thread_save_quit_req();

				if ( (share->game_stat == ROUND_OVER) && (comp->territory_data.current_territory == 5 ) )
					{
					comp->thread_pos = 0;	// back territory select
					}

			break;

			case 4:
				if ( share->game_stat == ROUND_OVER )
					{
					if ( (comp->territory_data.completed-1) <= comp->territory_data.current_territory )
						{
						if ( check_human_territory_winner() )
							{
							/** Winning team was HUMAN, so goto next round **/
							comp->territory_data.current_territory++; 	// Auto select next territory
							comp->territory_data.completed = comp->territory_data.current_territory+1; 				// This territory is over, make another available
							comp->thread_pos = 0;
							}
						else
							{
							/** Winning team was CPU, so you are oot of the competition **/

							share->game_stat = GAME_OVER;
							thread_game_over_req();
							}
						}
					else
						comp->thread_pos=0;
					}
				else
					{
					pad_assign();
					thread_save_quit_req();
					}
			break;

			case 5:
				team_manager();
				thread_save_quit_req();
			break;

			case 6:
				team_stats();
				thread_save_quit_req();
			break;

			case 7:
				
				if ( calculate_winner() == YES )
					{
					}
				else
					{
					abort_frontend=YES;
					abort_title=YES;	// play the game...
					//calculate_winner();
					}

				comp->perform_draw = YES; 

				comp->thread_pos = 6+2;

			break;

			case 8:

				update_game_stats();

				if ( share->game_stat == QUIT_COMPETITION )
					{
					/** User chose to exit this competition from within game **/
					abort_frontend=YES;
					share->title_func=0;
					}
				else
					{
					share->game_stat = GAME_IN_PROGRESS;	// now we are into a game
					calc_league_stats();
					comp->thread_pos = 3; // back to stat screen
					}
			break;

			case 9:
				league_stat_screen();
			break;

			}
		}

	pop(0);

}



void classic_thread()
{
WORD cnt,game_over,result;


	start_frontend_track();

	push(0);



	share->game_type = CLASSIC;
	share->total_groups = 1;


	thread_load_req();


	abort_frontend=NO;

	while(abort_frontend == NO )
		{
		switch( comp->thread_pos )
			{
			case -1:
			abort_frontend=YES;
			abort_title=NO;
			break;

			case 0:

				store_game_undo_info();

				cm_select();

				thread_save_quit_req();

				share->game_stat = GAME_IN_PROGRESS;			// now we are into a game

			break;

			case 1:
				pad_assign();
				thread_save_quit_req();
			break;


			case 2:
				if ( calculate_winner() )
					{
					}
				else
					{
					abort_frontend=YES;
					abort_title=YES;
					}

				comp->thread_pos = 3;
			break;

			case 3:
				/** A Game has just been played **/

				game_over = NO;

				if ( share->team1_pts > share->team2_pts )
					{
					/** Team 1 won and was human **/
					if ( share->team1_cpu == YES )
						game_over = YES;
					}
				else
					{
					if ( share->team2_cpu == YES )
						game_over = YES;
					}
		
				if ( game_over == NO )
					{
					comp->classic_data.completed[ comp->classic_data.current_match ] = YES;

					//comp->classic_data.current_match++;

					for(cnt=0;cnt<8;cnt++)
						{
						if ( comp->classic_data.completed[ cnt ] == NO )
							break;
						}
					if ( cnt==8) cnt=7;

					if ( cnt >= 4 )
						{
						comp->classic_data.available = cnt+1;
						}

					}
				else
					{
					//do_request( classic_game_over_text[config->language], territory_game_over_req_list, -20*4, SHELL_OK );
					}
				comp->thread_pos = 0;
			break;
			}
		}

//	abort_title=YES;
	pop(0);

}











void set_demo_mode_data()
{
/********************************
 *
 * Set data for Demo mode game
 *
 ********************************/

	share->demo_mode = YES;
	share->team1 = make_random_number( MAX_COUNTRY_TEAMS-1 );
	share->team2 = make_random_number( MAX_COUNTRY_TEAMS-1 );
	share->team1_cpu = share->team2_cpu = YES;
	while( share->team2 == share->team1 )
		{
		share->team2 = make_random_number( MAX_COUNTRY_TEAMS-1 );
		}

	share->options_presets[ OPT_STADIUM ] = make_random_number( 4 );
	share->options_presets[ OPT_WEATHER ] = make_random_number( 5 );

}









void euro2000_thread() /**** Qualifiers *****/
{
WORD all_teams_played=NO;
WORD result,cnt;
	start_frontend_track();




	abort_frontend=NO;
	push(0);






		share->game_type = EURO2000;

		share->total_groups = 9;



	thread_load_req();



	while(abort_frontend == NO )
		{
		switch( comp->thread_pos )
			{
			case 0:
				reset_league_data();

				select_num_teams();

				thread_save_quit_req();

				comp->perform_draw = YES;
			break;

			case 1:
				team_select();
				thread_save_quit_req();

				init_euro2000_draw();
			break;

			case 2:
				options();

				thread_save_quit_req();

				//set_cpu_teams_list();


			break;

			case 3:

				comp->perform_draw = YES;
				if ( share->game_stat == GAME_IN_PROGRESS )
					{
					if ( (share->team1_cpu == NO) || ( share->team2_cpu == NO )  )
						{
						/** Show previous game results **/
						league_stat_screen();	// show stats
						thread_save_quit_req();
						}
					else
						{
						comp->thread_pos = 4;
						}

					share->current_group++;
					if ( share->current_group == 9 )
						share->current_group = 0;
					}
				else
					comp->thread_pos = 4;
			break;

			case 4:

				if ( comp->perform_draw == YES )
					{
					all_teams_played = do_euro2000_draw();
					comp->perform_draw=NO;
					}
				else
					all_teams_played = NO;



				if ( all_teams_played )
					{
					share->game_stat = GAME_OVER;
					comp->thread_pos = 8+2;
					}
				else
					{
					if ( (share->team1_cpu && share->team2_cpu) && (share->game_stat != GAME_OVER) )
						comp->thread_pos = 6+2;
					else
						{
						share->game_stat = GAME_INIT;
						store_game_undo_info();
						league_stat_screen();	// show stats
						thread_save_quit_req();
						}
					}
			break;

			case 5:
				pad_assign();
				thread_save_quit_req();
			break;

			case 6:
				team_manager();
				thread_save_quit_req();
			break;

			case 7:
				team_stats();
				thread_save_quit_req();
			break;

			case 8:
				if ( calculate_winner() )
					{
					}
				else
					{
					abort_frontend=YES;
					abort_title=YES;
					}

				comp->thread_pos = 7+2;
				comp->perform_draw = YES; 

			break;

			case 9:

				update_game_stats();

				if ( share->game_stat == QUIT_COMPETITION )
					{
					abort_frontend=YES;
					share->title_func=0;
					}
				else
					{
					share->game_stat = GAME_IN_PROGRESS;	// now we are into a game
					comp->thread_pos = 3;
					calc_league_stats();
					}
			break;

			case 10:
				share->game_type = EURO2000;
				league_stat_screen();		// show stats
				thread_save_quit_req();
			break;

			case 11:
				share->game_stat = GAME_IN_PROGRESS;
				share->game_type = EURO2000_CALC;
				replay_euro2000();			// play previously played game with top 4 teams only
				calc_euro2000_2nd_place();	// get 2nd place team and add to euro final list of teams

				share->game_type = EURO2000;
				euro2000_qual_results();	// show 9 qualifiers and runner up

				thread_game_over_req();
				thread_save_quit_req();

			break;

			case 12:
				share->game_type = EURO2000_KO;
				init_knockout_data();

				share->title_func = knockout_thread;
				share->game_stat = GAME_INIT; //_PROGRESS;
				comp->thread_pos = 3;
				comp->perform_draw = YES;

				abort_frontend = YES;
			break;

			case 13:
				share->game_type = EURO2000_KO;
				/** Jump here when knockout finished **/
				add_ko_teams_to_euro_final();
				euro2000_qual_results();	// show all 16 qualifiers 

				thread_save_quit_req();
				thread_game_over_req();


			break;

			case 14:

				reset_league_data();

				share->game_stat = GAME_IN_PROGRESS;

				init_eurofinal_draw();	/* this takes all 16 teams putting into groups game_stat must== GAME_IN_PROGRESS */

			/** Jump to EuroFinal thread now ***/
				share->game_type = EUROFINAL;
				share->title_func = euro_final_thread;
				share->game_stat = GAME_INIT;
				comp->thread_pos = 3;
				share->current_group=0;

				ts_total_teams = 16;	// we have all 16 teams




				abort_frontend=YES;
			break;

			}
		}

	pop(0);

}



void euro_final_thread()
{
WORD all_teams_played=NO;
WORD result,cnt;


	start_frontend_track();


	abort_frontend=NO;
	push(0);
//	init_shell( FRONTEND );
//	play_xa_audio_track( 0 );


//	if ( share->game_stat == GAME_INIT )
//		{
		share->game_type = EUROFINAL;
//		share->current_group = 0;
		share->total_groups = 4;
//		}


	thread_load_req();


	while(abort_frontend == NO )
		{
		switch( comp->thread_pos )
			{
			case 0:
				reset_league_data();

				select_num_teams();

				thread_save_quit_req();

				comp->perform_draw = YES;
			break;

			case 1:
				team_select();
				thread_save_quit_req();

				if ( abort_frontend == NO )
					{
//					init_eurofinal_draw();	/* this generates all 16 teams putting into groups */
					}

			break;

			case 2:
				options();

				thread_save_quit_req();

				//set_cpu_teams_list();


				if ( share->game_stat == GAME_INIT )
					init_eurofinal_draw();	/* this generates all 16 teams putting into groups */

			break;

			case 3:

				//if ( share->game_stat == GAME_INIT )
				//	init_eurofinal_draw();

				comp->perform_draw=YES;

				if ( share->game_stat == GAME_IN_PROGRESS )
					{
					if ( (share->team1_cpu == NO) || ( share->team2_cpu == NO )  )
						{
						league_stat_screen();	// show stats
						thread_save_quit_req();
						}
					else
						{
						comp->thread_pos = 4;
						}

					share->current_group++;
					if ( share->current_group == 4 )
						share->current_group = 0;
					}
				else
					comp->thread_pos = 4;
			break;

			case 4:

				if ( comp->perform_draw == YES )
					{
					all_teams_played = do_euro2000_draw();
					comp->perform_draw=NO;
					}
				else
					all_teams_played = NO;



				if ( all_teams_played )
					{
					share->game_stat = GAME_OVER;
					comp->thread_pos = 8+2;
					}
				else
					{
					if ( (share->team1_cpu && share->team2_cpu) && (share->game_stat != GAME_OVER) )
						comp->thread_pos = 6+2;
					else
						{
						share->game_stat = GAME_INIT;
						store_game_undo_info();
						league_stat_screen();	// show stats
						thread_save_quit_req();
						}
					}
			break;

			case 5:
				pad_assign();
				thread_save_quit_req();
			break;

			case 6:
				team_manager();
				thread_save_quit_req();
			break;

			case 7:
				team_stats();
				thread_save_quit_req();
			break;

			case 8:
				if ( calculate_winner() )
					{
					}
				else
					{
					abort_frontend=YES;
					abort_title=YES;
					}

				comp->thread_pos = 7+2; 
				comp->perform_draw = YES; 

			break;

			case 9:

				update_game_stats();

				if ( share->game_stat == QUIT_COMPETITION )
					{
					abort_frontend=YES;
					share->title_func=0;
					}
				else
					{
					share->game_stat = GAME_IN_PROGRESS;	// now we are into a game
					comp->thread_pos = 3;
					calc_league_stats();
					}
			break;

			case 10:
				share->game_type = EURO2000;
				league_stat_screen();		// show stats
				thread_save_quit_req();
			break;

			case 11:
				share->game_stat = GAME_IN_PROGRESS;

				share->game_type = EUROFINAL_KO;
				init_knockout_data();

				share->title_func = knockout_thread;
				share->game_stat = GAME_INIT; //_PROGRESS;
				comp->thread_pos = 3;
				comp->perform_draw = YES;

				abort_frontend = YES;
			break;

			case 13:
				/** Knockout thread jumps here when knockout finished ( Should now have eurofinal winner) **/

				comp->thread_pos = 12+2;

			break;

			case 14:

				share->title_func = 0;
				abort_frontend = YES;
			break;

			}
		}

	pop(0);

}




void worldcup_thread()
{
WORD all_teams_played=NO;
WORD result,cnt;

	start_frontend_track();



	abort_frontend=NO;
	push(0);
//	init_shell( FRONTEND );
//	play_xa_audio_track( 0 );


	share->game_type = WORLDCUP;
	share->total_groups = 8;

	thread_load_req();


	while(abort_frontend == NO )
		{
		switch( comp->thread_pos )
			{
			case 0:
				reset_league_data();

				select_num_teams();

				thread_save_quit_req();

				comp->perform_draw = YES;
			break;

			case 1:
				team_select();
				thread_save_quit_req();

			break;

			case 2:
				options();

				thread_save_quit_req();

				//set_cpu_teams_list();


				init_worldcup_draw();

			break;

			case 3:

				comp->perform_draw=YES;

				if ( share->game_stat == GAME_IN_PROGRESS )
					{
					if ( (share->team1_cpu == NO) || ( share->team2_cpu == NO )  )
						{
						league_stat_screen();	// show stats
						thread_save_quit_req();
						}
					else
						{
						comp->thread_pos = 4;
						}

//					share->current_group++;
//					if ( share->current_group == 4 )
//						share->current_group = 0;
//					printf("group=%d\n",share->current_group);
					}
				else
					comp->thread_pos = 4;
			break;

			case 4:

				if ( comp->perform_draw == YES )
					{
					all_teams_played = do_worldcup_draw();
					comp->perform_draw=NO;
					}
				else
					all_teams_played = NO;

				if ( all_teams_played )
					{
					share->game_stat = GAME_OVER;
					comp->thread_pos = 8+2;
					}
				else
					{
					if ( (share->team1_cpu && share->team2_cpu) && (share->game_stat != GAME_OVER) )
						comp->thread_pos = 6+2;
					else
						{
						share->game_stat = GAME_INIT;
						store_game_undo_info();
						league_stat_screen();	// show stats
						thread_save_quit_req();
						}
					}
			break;

			case 5:
				pad_assign();
				thread_save_quit_req();
			break;

			case 6:
				team_manager();
				thread_save_quit_req();
			break;

			case 7:
				team_stats();
				thread_save_quit_req();
			break;
			
			case 8:
				if ( calculate_winner() )
					{
					}
				else
					{
					abort_frontend=YES;
					abort_title=YES;
					}

				comp->thread_pos = 7+2;
				comp->perform_draw = YES; 

			break;

			case 9:

				update_game_stats();

				if ( share->game_stat == QUIT_COMPETITION )
					{
					abort_frontend=YES;
					share->title_func=0;
					}
				else
					{
					share->game_stat = GAME_IN_PROGRESS;	// now we are into a game
					comp->thread_pos = 3;
					calc_league_stats();
					}
			break;

			case 10:
				/*** game over ***/
				//share->game_type = EURO2000;
				league_stat_screen();		// show stats
				thread_save_quit_req();
			break;

			case 11:
				share->game_stat = GAME_IN_PROGRESS;

				init_knockout_data();

				share->title_func = knockout_thread;
				share->game_stat = GAME_INIT;
				comp->thread_pos = 3;
				comp->perform_draw = YES;

				abort_frontend = YES;
			break;

			case 13:
				/** Knockout thread jumps here when knockout finished ( Should now have eurofinal winner) **/

				printf("FINAL OVER!!!!!!\n");

			break;

			case 14:
			break;

			}
		}

	pop(0);

}

void training_thread()
{
WORD abort_frontend=NO;

	start_frontend_track();


	share->game_type = TRAINING;


	while( abort_frontend==NO)
		{
		switch( comp->thread_pos )
			{
			case 0:
				training_screen();

				if ( shell->current_button == SHELL_NEXT )
					{
					/** User wants to play the training game **/
					}
				else
					{
					/** Back to title **/
					check_for_config_save();
					share->title_func=0;
					abort_frontend=YES;
					}
			break;

			case 1:

				reset_training_points();

				share->team1_cpu = NO;
				share->team2_cpu = NO;
				share->team1=0;
				share->team2=1;
				if ( calculate_winner() )
					{
					}
				else
					{
					abort_frontend=YES;
					abort_title=YES;
					}
				comp->thread_pos++;
			break;

			case 2:
			/** Game has been played, update hiscores if needed **/

			if ( share->game_stat == QUIT_COMPETITION )
				{
				abort_frontend=YES;
				}
			else
				{
				update_training_points();
				comp->thread_pos=0;
				}
			break;


			}
		}


}

void thread_save_quit_req()
{
BYTE *text;
	if ( shell->current_button == SHELL_SAVE )
		{
		save_game();
		}
	else
		{
		if ( shell->current_button == SHELL_QUIT )
			{
			text = thread_quit_text[ config->language ];

			if ( do_request( text, thread_quit_req_list, 0, SHELL_NO ) == SHELL_YES )
				{
				abort_frontend=YES; 
				share->title_func = 0; // league is over
				}
			}
		}
}




void thread_game_over_req()
{
BYTE *text;

	if ( share->game_stat == GAME_OVER )
		{
		text = thread_game_over_text[ config->language ][ share->game_type ];

		do_request( text, thread_game_over_req_list, -20*4, SHELL_OK );
		abort_frontend=YES; 
		share->title_func = 0; // league is over
		}
}





void thread_load_req()
{
void *funcs[]={
	&friendly_thread,
	&league_thread,
	&knockout_thread,
	&territory_thread,
	&classic_thread,
	&euro2000_thread,
	&euro_final_thread,
	&worldcup_thread,
	&euro2000_thread,
	&euro2000_thread,
	&euro2000_thread,
	&euro2000_thread,
	&euro2000_thread,
	&training_thread,
};

	if ( share->new_game == NO )
		{
		load_game();

		if ( mc->game_loaded == YES )
			{
			share->title_func = funcs[ share->game_type ];
				
			if ( (share->game_type == EURO2000_KO) || (share->game_type == EUROFINAL_KO) )
				{
				share->title_func = knockout_thread;
				abort_frontend = YES;
				}
			store_game_undo_info();
			}
		else
			{
			/**** Back to titlepage ****/
			share->title_func = 0; // league is over
			abort_frontend=YES;
			}
		}


}


void restore_game_undo_info()
{

	if ( share->title_func == 0 )
		{
	/** Nothing to restore cuz we're quitting to title screen **/
		return;
		}

#if 1
/** If game returned quit_comp, go straight to title page, and not back into competition **/
	if ( share->game_stat == QUIT_COMPETITION )
		{
		share->title_func = 0;
		}

/** Game was quit, so copy SHARE_STORE struct to SHARE	**/
	if ( share->game_stat == QUIT_GAME )
		{
		/** Restore previous game settings **/

//		printf("restoring game stats\n");
		*comp = *comp_store;
		share->game_stat = QUIT_GAME;
		}

//	printf("threadppos=%d\n",comp->thread_pos);

#endif 
		

}

void store_game_undo_info()
{
/** Game is about to be played, so copy SHARE_STORE struct to SHARE **/

#if DEBUG
//	printf("Storing game undo info\n");
#endif
	*comp_store = *comp;

//	printf("threadppos=%d\n",comp->thread_pos);
}



void start_frontend_track()
{

	if ( audio_track == TITLE_TRACK )
		{

		fade_off_music();
		}

	play_xa_audio_track( FRONTEND_TRACK );


}

void start_title_track()
{

	if ( audio_track != TITLE_TRACK )
		{
		fade_off_music();


		play_xa_audio_track( TITLE_TRACK );

		}


}


void fade_off_music()
{
	cd_fade_volume=0;
	while( cd_volume!=0 )
		{
		check_audio_track();
		wait_vblank();
		}

	wait_vblank();
	wait_vblank();
	wait_vblank();

}





WORD movie_exit_update()
{
	read_pads();
	if ( cjoy->db & (PAD_X|PAD_SQU|PAD_TRI|PAD_CIR) )
		return 1;

	return 0;		
}

void show_start_movies()
{
	if ( share->init_once_code != INIT_ONCE_PERFORMED_CODE )
		{
		play_movie( &coolkat, movie_exit_update, read_free_addr( 0 ) );
		play_movie( &infogram_logo, movie_exit_update, read_free_addr( 0 ) );
		play_movie( &rage_logo, movie_exit_update, read_free_addr( 0 ) );
		}
}



/****************************************************
 *																	 *
 *	 																 *
 * Competition logic functions							 *
 *	 																 *
 *																	 *
 ****************************************************/

#include "incs.h"

#ifdef PSX
#include "\global\striker.def" // defs for PSX striker
#include "\global\s01.h"
#include "general.h"
#include "text.h"
#include "teamsel.h"
#include "frontend.h"
#include "title.h"
#endif


/****************************************************************************
 *																									 *
 * Prototypes																					 *
 *																									 *
 ****************************************************************************/
void get_euro_teams_to_exclude();
void calc_ko_draws_to_do();
void get_least_played_teams( WORD *w1, WORD *w2 );
void sort_league_stats();
void ko_insert_winner();



/****************************************************************************
 *																									 *
 *																									 *
 * LEAGUE logic functions																	 *
 *																									 *
 *																									 *
 ****************************************************************************/
void init_league_draw()
{
WORD x,y,group;

	for(group=0;group<TOTAL_GROUPS;group++)
		{
		for(y=0;y<TOTAL_LEAGUE_TEAMS;y++)
			{
			for(x=0;x<TOTAL_LEAGUE_TEAMS;x++)
				{
				if( x==y )
					share->league_draw_array[group][y][x]=1;
				else
					share->league_draw_array[group][y][x]=0;
				}
			}
		}
}

WORD do_league_draw()
{
WORD x,y, team1, team2, all_teams_played;
WORD wp1, wp2;

		//printf("Doing league draw  %d teams playing....\n",share->teams_in_competition);

		all_teams_played=YES;
		for(y=0;y<share->teams_in_competition;y++)
			{
			for(x=0;x<share->teams_in_competition;x++)
				{
				if ( share->league_draw_array[share->current_group][y][x] == 0 )
					all_teams_played=NO;
				}
			}

		if ( all_teams_played )
			return all_teams_played;

		while( 1 )
			{
	//		x = make_random_number( share->teams_in_competition-1 );
	//		y = make_random_number( share->teams_in_competition-1 );

			get_least_played_teams( &x, &y );

			while ( share->league_draw_array[share->current_group][y][x] != 0 )
				{
				x = make_random_number( share->teams_in_competition-1 );
				y = make_random_number( share->teams_in_competition-1 );
				}


			if ( share->league_draw_array[share->current_group][y][x] == 0 )
				{
				share->team1_list_pos = x;	// set position within list of teams to play
				share->team2_list_pos = y;
				share->team1 = share->selected_teams_list[ x ];
				share->team2 = share->selected_teams_list[ y ];

				share->team1 = share->group_teams[ share->current_group ][ x ];
				share->team2 = share->group_teams[ share->current_group ][ y ];

				share->team1_cpu = share->cpu_teams_index_list[ share->team1 ];
				share->team2_cpu = share->cpu_teams_index_list[ share->team2 ];

				share->league_draw_array[share->current_group][ share->team2_list_pos ][ share->team1_list_pos ] = 1;

				//printf("(%d)%s will play (%d)%s\n", share->team1,get_team_name( share->team1 ), share->team2,get_team_name( share->team2 ) );

				break;
				}
			}

  	return all_teams_played;

}

void set_league_draw_array()
{
//	share->league_draw_array[share->current_group][ share->team2_list_pos ][ share->team1_list_pos ] = 1;
}

void get_least_played_teams( WORD *w1, WORD *w2 )
{
WORD cnt;
WORD x,y;



	x = y = 200;
	*w1 = x;

	for( cnt=0;cnt< share->teams_in_competition; cnt++ )
		{
		if ( (share->league_data[ share->current_group ][ cnt ].played <= x )  )
			{
			x = share->league_data[ share->current_group ][ cnt ].played;
			*w1 = cnt;
			}
		}

	*w2 = y;
	for( cnt=0;cnt< share->teams_in_competition; cnt++ )
		{
		if ( (share->league_data[ share->current_group ][ cnt ].played <= y) && (cnt!= *w1 ) )
			{
			y = share->league_data[ share->current_group ][ cnt ].played;
			*w2 = cnt;
			}
		}

//	printf("t1=%d t2=%d\n", *w1, *w2 );
}



void sort_league_stats()
{
BYTE *cptr;
WORD *index_list, index_y, group, team_num, x, cnt, teams;
ULONG *l1, *l2;
LONG64 sort_list[ TOTAL_LEAGUE_TEAMS ];
LONG64 al, gs, gd, pt;
WORD base_group;

/**************************************
 *												  *
 * Sort all groups, setting win_pos   *
 * with team's new position			  *
 *												  *
 **************************************/

 	if ( (share->game_type == LEAGUE) || ( share->game_type == EURO2000 ) || (share->game_type == TERRITORY) || ( share->game_type == WORLDCUP ) || ( share->game_type == EURO2000_CALC ) || ( share->game_type == EUROFINAL )  )
		{
		if ( share->game_type == EURO2000_CALC )
			{
			base_group = 9;
			}
		else
			base_group = 0;

		for( group=base_group; group<share->total_groups; group++ )
			{
			//printf("group %d contains teams...\n", group );
			teams = share->teams_in_group[ group ];
			for( cnt=0; cnt<teams; cnt++ )
				{
				team_num = share->group_teams[ group ][ cnt ];
				cptr = (BYTE *)get_team_name( team_num );
				pt = share->league_data[ group ][cnt].points; 
				gd = 500 + share->league_data[ group ][cnt].goal_diff;  // + or - 500
				gs = share->league_data[ group ][cnt].goals_for;   

				al = *cptr;

				//printf("%s gd=%d pts=%d\n",cptr,gd,pt);
				/** Ensure upper and lower case are treated equally **/
				if ( (al >='a') && (al <='z') )
					al -= 'a'-'A';

				al = 127-al;

				sort_list[cnt] = (pt<<40) + ( gd<<25 ) + ( gs<<10 ) + al;

				}

			index_list = bubble_sort64( sort_list, teams, DESCENDING );

			for( x=0; x<teams; x++ )
				{
				share->league_data[ group ][ *index_list ].win_pos = x;
				index_list++;
				}
			}
		}
}



/****************************************************************************
 *																									 *
 *																									 *
 * KNOCKOUT logic functions																 *
 *																									 *
 *																									 *
 ****************************************************************************/

void init_knockout_data()
{
WORD cnt,round, group, x;
WORD list[]={ 3,2,1,0,0 };
WORD flip;
	/*** Initialise Knockout data for first time ***/

	if ( (share->game_type == EURO2000_KO) || ( share->game_type == EUROFINAL_KO) )
		{
		share->teams_in_competition = 8;
		}
	if ( share->game_type == WORLDCUP )
		{
		share->teams_in_competition = 16;
		}

	share->ko_data.current_round = list[share->teams_in_competition/4];
	calc_ko_draws_to_do();

	share->ko_data.winning_list_pos = 0;

	for( round=0;round<5;round++)
		{
		for(cnt=0;cnt<16;cnt++)
			share->ko_data.played[round][cnt] = NO;

		for(cnt=0; cnt<16; cnt++)
			share->ko_data.teams[round][cnt] = -1;
		}


	for(cnt=0;cnt<16;cnt++)
		share->ko_data.teams[ share->ko_data.current_round ][cnt] = share->selected_teams_list[cnt];


	if ( share->game_type == EURO2000_KO )
		{
		/*** Copy 8 runner up teams into knockout data ***/
		for( cnt=0; cnt<8; cnt++ )
			{
			share->ko_data.teams[1][cnt] = share->euro_final_data.runner_teams[ cnt ];	// start in 2nd round
			}
		}

	
	if ( share->game_type == EUROFINAL_KO )
		{
		/*** Copy top 2 winners from each group and place in knockout ***/

		cnt=0;
		for( group=0; group<4; group++ )
			{
			for(x=0;x<4;x++ )
				{
				if ( (share->league_data[ group ][ x ].win_pos == 0) || (share->league_data[ group ][ x ].win_pos == 1) )
					{
					share->ko_data.teams[1][cnt] = share->group_teams[ group ][ x ];
					cnt++;
					}
					
				}
			}
		}

	if ( share->game_type == WORLDCUP )
		{
		/*** Copy top 2 winners from each group and place in knockout ***/
		/** These go group A 1st plays Group B 2nd, Group B 1st plays Group A 2nd, Group C 1st etc **/

		share->worldcup_data.game=0;
		cnt=0;
		for( group=0; group<8; group++ )
			{
			for(x=0;x<4;x++ )
				{
				if ( share->league_data[ group ][ x ].win_pos == 0  )
					{
					share->ko_data.teams[0][cnt+0] = share->group_teams[ group ][ x ];
					cnt+=2;
					}
				}
			}

		cnt=0;
		flip=0;
		for( group=0; group<8; group++ )
			{
			for(x=0;x<4;x++ )
				{
				if ( share->league_data[ group ][ x ].win_pos == 1  )
					{
					flip = 2-flip;
					share->ko_data.teams[0][cnt+1  + flip ] = share->group_teams[ group ][ x ];

					if ( flip == 0 )
						cnt+=4;
					}
				}
			}

		}



}


WORD do_knockout_draw()
{

WORD round;
WORD list[]={3,2,1,0,0};
WORD team;
WORD teams_in_round;
WORD count;
WORD hums,x;


	round = share->ko_data.current_round;
	teams_in_round = 16;
	count = share->ko_data.current_round;
	while(count--)
		{
	  	teams_in_round >>= 1;
		}


															  

	//printf("Doing draw on round %d with %d draws to do and %d teams in this round\n",share->ko_data.current_round, share->ko_data.draws_to_do, teams_in_round );

/*** Choose 1st team to play *****/
	while(1)
		{
		team = make_random_number( teams_in_round-1 );
		if ( share->ko_data.played[ round ][ team ] == NO )
			{
			share->team1_list_pos = team;
			share->team1 = share->ko_data.teams[ share->ko_data.current_round ][ team ];
			share->team1_cpu = share->cpu_teams_index_list[ share->team1 ];

			break;
			}

		}

/*** Choose 2nd team to play *****/
	while(1)
		{
		team = make_random_number( teams_in_round-1 );
		if ( (share->ko_data.played[ round ][ team ] == NO) && ( team != share->team1_list_pos ) )
			{
			share->team2_list_pos = team;
			share->team2 = share->ko_data.teams[ share->ko_data.current_round ][ team ];

			share->team2_cpu = share->cpu_teams_index_list[ share->team2 ];

			break;
			}

		}


	
	if ( share->game_type == EURO2000_KO )
		{
		/** Euro2000 knockout can be played without humans **/
		}
	else
		{
		hums=NO;
		for(x=0;x<teams_in_round;x++)
			{
			team = share->ko_data.teams[ round ][ x ];
			if ( share->cpu_teams_index_list[ team ] == NO )
				hums=YES;
			}

		if ( hums == NO )
			share->game_stat = GAME_OVER;
		}

}

void do_worldcup_knockout_draw()
{

WORD round;
WORD list[]={3,2,1,0,0};
WORD team;
WORD teams_in_round;
WORD count;
WORD hums,x;

	round = share->ko_data.current_round;
	teams_in_round = 16;
	count = share->ko_data.current_round;
	while(count--)
		{
	  	teams_in_round >>= 1;
		}



	//printf("Doing draw on round %d with %d draws to do and %d teams in this round\n",share->ko_data.current_round, share->ko_data.draws_to_do, teams_in_round );




	share->team1_list_pos = (( share->ko_data.total_draws - share->ko_data.draws_to_do ) * 2 ) + 0;
	share->team2_list_pos = (( share->ko_data.total_draws - share->ko_data.draws_to_do ) * 2 ) + 1;

	share->team1 = share->ko_data.teams[ share->ko_data.current_round ][ share->team1_list_pos ];
	share->team2 = share->ko_data.teams[ share->ko_data.current_round ][ share->team2_list_pos ];

	share->team1_cpu = share->cpu_teams_index_list[ share->team1 ];
	share->team2_cpu = share->cpu_teams_index_list[ share->team2 ];


	hums=NO;
	for(x=0;x<teams_in_round;x++)
		{
		team = share->ko_data.teams[ round ][ x ];
		if ( share->cpu_teams_index_list[ team ] == NO )
			hums=YES;
		}

	if ( hums == NO )
		share->game_stat = GAME_OVER;



}

void ko_calc_results()
{
	
	ko_insert_winner();
	share->ko_data.played[ share->ko_data.current_round ][ share->team1_list_pos ] = YES;
	share->ko_data.played[ share->ko_data.current_round ][ share->team2_list_pos ] = YES;

	share->ko_data.draws_to_do--;

	if ( share->ko_data.draws_to_do == 0 )	
		{
		if ( share->game_type == EURO2000_KO )
			share->game_stat = GAME_OVER;
		else
			{
			share->ko_data.current_round++;
			calc_ko_draws_to_do();	// get num of draws for next round

			share->ko_data.winning_list_pos = 0; // reset position at which to add winning teams
			if ( share->ko_data.current_round == 4 )
				{
				share->game_stat = GAME_OVER;
				}
			}
		}
}

void calc_ko_draws_to_do()
{
WORD cnt = share->ko_data.current_round;

	share->ko_data.draws_to_do = 8;

	while( cnt-- )
		{
		share->ko_data.draws_to_do >>= 1;
		}

	share->ko_data.total_draws = share->ko_data.draws_to_do;	// draws_to_do is decremented after every game

}


void ko_insert_winner()
{
WORD list_pos;
WORD winning_team;

	if ( share->team1_pts > share->team2_pts )
		winning_team = share->team1;
	else
		winning_team = share->team2;

	list_pos = share->ko_data.winning_list_pos;
	share->ko_data.teams[ share->ko_data.current_round + 1 ][ list_pos ] = winning_team;
	share->ko_data.winning_list_pos++;
}


void do_winner_simulation()
{
/*** Work out teams points based on their stats ( to be properly implemented ) ***/
	while( 1 )
		{
		share->team1_pts = make_random_number( 7 );
		share->team2_pts = make_random_number( 7 );

		if ( share->team1_pts != share->team2_pts )
			{
			break;	// keep going till both scores are different
			}
		}
}

WORD calculate_winner()
{
/*** If team1 and team2 are both cpu, calculates the winner
     in 'share->winning_team'
***/

/** Returns YES if a winner was calculated ***/


	if ( (share->team1_cpu==YES) && (share->team2_cpu==YES) )
		{
		/*** Both Teams are Cpu, Choose a winner ***/

		do_winner_simulation(); // get random points for two teams

		return YES;
		}
	

	if ( stub_present )
		{
		/** Stub is present, so exit frontend to play the game **/

	  	abort_frontend=YES;
	  	abort_title=YES;

		return NO;
		}
	else	
		{
		/** Stub is not present, so manually choose a winner **/
		do_winner_simulation(); // get random points for two teams
		debug_choose_winner();
	  	abort_frontend=YES;
		return YES;
		}
}




void calc_league_stats()
{
WORD wt_pos;


// calc played, won drawn etc

	//printf("calculating stats for group %d\n",share->current_group );

/** Calc team 1 and 2 played **/
	share->league_data[ share->current_group ][ share->team1_list_pos ].played++;
	share->league_data[ share->current_group ][ share->team2_list_pos ].played++;


/** Calc team 1 and 2 WON, LOST, POINTS **/
	if ( share->team1_pts > share->team2_pts )
		{
		/** Team 1 WON **/
		share->league_data[ share->current_group ][ share->team1_list_pos ].won++;
		share->league_data[ share->current_group ][ share->team2_list_pos ].lost++;
		share->league_data[ share->current_group ][ share->team1_list_pos ].points+=3;
		}
	else
		{
		/** Team 2 WON **/
		if ( share->team1_pts < share->team2_pts )
			{
			share->league_data[ share->current_group ][ share->team2_list_pos ].won++;
			share->league_data[ share->current_group ][ share->team1_list_pos ].lost++;
			share->league_data[ share->current_group ][ share->team2_list_pos ].points+=3;
			}
		else
			{
			/** Team 1 and 2 drawn **/
			if ( share->team1_pts == share->team2_pts )
				{
				share->league_data[ share->current_group ][ share->team1_list_pos ].drawn++;
				share->league_data[ share->current_group ][ share->team2_list_pos ].drawn++;
				share->league_data[ share->current_group ][ share->team1_list_pos ].points+=1;
				share->league_data[ share->current_group ][ share->team2_list_pos ].points+=1;
				}
			}
		}

	
/** Calc team 1 goals for and against **/
	share->league_data[ share->current_group ][ share->team1_list_pos ].goals_for += share->team1_pts;
	share->league_data[ share->current_group ][ share->team1_list_pos ].goals_against += share->team2_pts;

/** Calc team 2 goals for and against **/
	share->league_data[ share->current_group ][ share->team2_list_pos ].goals_for += share->team2_pts;
	share->league_data[ share->current_group ][ share->team2_list_pos ].goals_against += share->team1_pts;

/** Calc goal difference **/
	share->league_data[ share->current_group ][ share->team1_list_pos ].goal_diff = share->league_data[ share->current_group ][ share->team1_list_pos ].goals_for - share->league_data[ share->current_group ][ share->team1_list_pos ].goals_against;
	share->league_data[ share->current_group ][ share->team2_list_pos ].goal_diff = share->league_data[ share->current_group ][ share->team2_list_pos ].goals_for - share->league_data[ share->current_group ][ share->team2_list_pos ].goals_against;


/*** Sort the stats **/

	if ( share->game_type != EURO2000_CALC )
		sort_league_stats();


	if ( share->game_type == EURO2000 )
		{
		/** Store teams who played and their results **/
		share->euro2000_data.result[ share->euro2000_data.games_played ].team1 = share->team1;
		share->euro2000_data.result[ share->euro2000_data.games_played ].team2 = share->team2;
		share->euro2000_data.result[ share->euro2000_data.games_played ].team1_score = share->team1_pts;
		share->euro2000_data.result[ share->euro2000_data.games_played ].team2_score = share->team2_pts;
		share->euro2000_data.games_played++;

//		printf("%s %d-%d %s\n",get_team_name( share->team1 ), share->team1_pts, share->team2_pts, get_team_name( share->team2 ) );
		}
}


void init_eurofinal_draw()
{
WORD x,y, group, rand_team, teams_to_generate, index;

	share->euro2000_data.games_played=0;

	for(group=0;group<TOTAL_GROUPS;group++)
		{
		for(y=0;y<4;y++)
			{
			for(x=0;x<4;x++)
				{
				if( x<=y )
					share->league_draw_array[group][y][x]=1;	// this allows each team to play each team only once.
				else
					share->league_draw_array[group][y][x]=0;
				}
			}
		}

/*** Put all teams into 4 groups ***/


	for(group=0; group<4; group++ )
		for( x=0;x<4;x++)
			share->group_teams[ group ][x]=-1;


/** groups 1,2 must contain Neth, Belg **/
	share->group_teams[1][0] = Netherlands;
	share->group_teams[3][0] = Belgium;


	for( group=0; group<4; group++ )
		{
		share->euro2000_data.group_over[ group ] = NO;
		share->teams_in_group[group] = 4;
		}



	if ( share->game_stat != GAME_IN_PROGRESS )	// only generate remaining teams for a NEW EUROFINAL, not one started automatically after a euro qualifier
		{
		/** Take selected_teams_list, as outputted by teamsel.c and create remaining teams **/

		/** Add Belgium if not already picked **/
		for(x=0;x<ts_total_teams;x++)
			{
			if ( share->selected_teams_list[ x ] == Belgium )
				{
				share->selected_teams_list[x]=-1;
				x=-1;
				break;
				}
			}

		if ( x != -1 )
			{
			/** If Belgium does not exist, create it **/
			for(y=0;y<16;y++)
				{
				if ( share->cpu_teams_index_list[ share->selected_teams_list[ y ] ] )	/** Replace a non human team with Belgium **/
					{
					share->selected_teams_list[y]=-1; //Belgium;
					break;
					}
				}
			}

		/** Add Netherlands if not already picked **/
		for(x=0;x<ts_total_teams;x++)
			{
			if ( share->selected_teams_list[ x ] == Netherlands )
				{
				share->selected_teams_list[x]=-1;
				x=-1;
				break;
				}
			}

		if ( x != -1 )
			{
			for(y=0;y<16;y++)
				{
				if ( share->cpu_teams_index_list[ share->selected_teams_list[ y ] ] )	/** Replace a non human team with NETHER **/
					{
					share->selected_teams_list[y]=-1; //Netherlands;
					break;
					}
				}
			}			
	
		/** Create remaining teams **/

		teams_to_generate = 16-ts_total_teams;

		while( teams_to_generate )
			{
			rand_team = make_random_number( share->teams_selection_size-1 );
			rand_team = *(ts_source_team_list+rand_team);

			if ( check_ts_team_exists( rand_team ) == NO )
				{
				add_team_to_ts_list( rand_team );
				teams_to_generate--;
				}
			}

		}
	else
		{
		for( x=0; x<16; x++ )
			{
			share->selected_teams_list[ x ] = share->euro_final_data.teams[ x ];
			}
		}
		

		teams_to_generate = 14;	// 2 have already be added to group_teams

	/** Group 3 should have Germany, if they were selected. We won the war you know, but the Germans are better off..mutter.. **/
		for(y=0;y<16;y++)
			{
			if ( share->selected_teams_list[y] == Germany )
				{
				share->selected_teams_list[y]=-1;
				share->group_teams[2][0] = Germany;
//				teams_to_generate--;
				}
			}


/* Remove any previously added teams, so only the remainder can be put into group_teams[][] */
	for(y=0;y<16;y++)
		{
//		if ( (share->selected_teams_list[y]==Belgium) || (share->selected_teams_list[y]==Netherlands) || (share->selected_teams_list[y]==Germany) )
//		if ( share->selected_teams_list[y] == Germany )
//			share->selected_teams_list[y]=-1;
		}


/* Randomly choose a team from selected_teams_list and and to group_teams */
	for( group=0; group< 4; group++ )
		{
		for( x=0; x<4; x++ )
			{
			if ( share->group_teams[ group ][ x ] == -1 )
				{
				while( 1 )
					{
					index = make_random_number( 16-1 );	/* Get num between 0-15 */
					rand_team = share->selected_teams_list[ index ];
					if ( rand_team != -1 )
						{
						share->group_teams[ group ][ x ] = rand_team;
						share->selected_teams_list[ index ] = -1;
						break;
						}
					}
				}
			}
		}


		
}




void init_euro2000_draw()
{
WORD x,y, group;


	share->euro2000_data.games_played=0;

	for(group=0;group<TOTAL_GROUPS;group++)
		{
		for(y=0;y<MAX_TEAMS_IN_EURO2000_GROUP;y++)
			{
			for(x=0;x<MAX_TEAMS_IN_EURO2000_GROUP;x++)
				{
				if( x==y )
					share->league_draw_array[group][y][x]=1;
				else
					share->league_draw_array[group][y][x]=0;
				}
			}
		}

	for( group=0; group< TOTAL_GROUPS; group++ )
		{
		share->euro2000_data.group_over[ group ] = NO;
		for( x=0; x< MAX_TEAMS_IN_EURO2000_GROUP; x++ )
			{
			if ( euro2000_teams[group][x] == -1 )
				share->teams_in_group[group] = 5;
			else
				share->teams_in_group[group] = 6;
			}
		}



}

WORD drawcnt=0;
WORD do_euro2000_draw()
{
WORD x,y, team1, team2, all_teams_played , wp1, wp2 ,team_cnt , cnt,group;



		if ( share->euro2000_data.group_over[ share->current_group ] == YES )
			{
			while ( share->euro2000_data.group_over[ share->current_group ] == YES )
				{
				share->current_group++;
				if ( share->current_group == share->total_groups )
					share->current_group = 0;

				for( cnt=0;cnt<share->total_groups;cnt++)
					{
					if ( share->euro2000_data.group_over[ cnt ] == NO )
						{
						share->current_group = cnt;
						break;
						}
					}
				if ( cnt == share->total_groups )
					{
					/** Checked all groups and they've all been completed **/
					return YES;
					}
				}
			}

		team_cnt = share->teams_in_group [ share->current_group ];
		share->teams_in_competition = team_cnt;

		all_teams_played=YES;
		for(y=0;y<team_cnt;y++)
			for(x=0;x<team_cnt;x++)
				if ( share->league_draw_array[share->current_group][y][x] == 0 )
					all_teams_played=NO;

		if ( all_teams_played )
			share->euro2000_data.group_over[ share->current_group ] = YES;


		if ( share->game_type != EUROFINAL )
			{
			for(y=0;y<4;y++)
				share->selected_teams_list[y] = share->euro_final_data.teams[ (share->current_group * 4) + y ];
			}
		else	
			{
			for(y=0;y<team_cnt;y++)
				share->selected_teams_list[y] = euro2000_teams[ share->current_group ][y];
			}



/** Don't do a draw on this group if a game in this group have been played **/
		//printf("draw=%d  for group %d\n",drawcnt, share->current_group);
		drawcnt++;

		while( 1 )
			{
	//		x = make_random_number( share->teams_in_competition-1 );
	//		y = make_random_number( share->teams_in_competition-1 );

			get_least_played_teams( &x, &y );

			while ( share->league_draw_array[share->current_group][y][x] != 0 )
				{
				x = make_random_number( team_cnt-1 );
				y = make_random_number( team_cnt-1 );
				}


			if ( share->league_draw_array[share->current_group][y][x] == 0 )
				{
				share->team1_list_pos = x;	// set position within list of teams to play
				share->team2_list_pos = y;
				share->team1 = share->group_teams[ share->current_group ][ x ];
				share->team2 = share->group_teams[ share->current_group ][ y ];

				share->team1_cpu = share->cpu_teams_index_list[ share->team1 ];
				share->team2_cpu = share->cpu_teams_index_list[ share->team2 ];


//				printf("(%d)%s will play (%d)%s\n", share->team1,get_team_name( share->team1 ), share->team2,get_team_name( share->team2 ) );

				share->league_draw_array[share->current_group][y][x] = 1;
				break;
				}
			}


	team_cnt = share->teams_in_group [ share->current_group ];
	all_teams_played=YES;
	for(y=0;y<team_cnt;y++)
		for(x=0;x<team_cnt;x++)
			if ( share->league_draw_array[share->current_group][y][x] == 0 )
				all_teams_played=NO;

	if ( all_teams_played )
		share->euro2000_data.group_over[ share->current_group ] = YES;



  	return NO;

}




WORD euro_teams_to_exclude[2];
WORD euro_2nd_place_team[ TOTAL_GROUPS ];
WORD eq_runner_teams[9];

WORD get_team_list_pos( WORD team_to_find, WORD team_num )
{
WORD team, group;

/*
 pass - team_to_find ( team 0-82 )
 pass - team =0 , team1
      - team =1 , team2
 returns YES if successful
         else NO
 sets team1_list_pos , or team2_list_pos
 and share->current_group ( group that team belongs to )
*/

	for( group=0;group<share->total_groups; group++ )
		{
		for( team=0; team< share->teams_in_group[ group ]; team++ )
			{
			if ( share->group_teams[ group ][ team ] == team_to_find )
				{
				if ( team_num == 0 )
					share->team1_list_pos = team;
				else
					share->team2_list_pos = team;
				share->current_group = group;
				return YES;
				}
			}
		}
	printf("Not found...error\n");
	return NO;
}

void calc_euro_teams_to_exclude()
{
WORD teams,team,group,cnt;

	group = share->current_group;
	teams = share->teams_in_group[ group ];

	if ( teams==5 )
		{
		//*** Exclude 4th entry
		euro_teams_to_exclude[0] = -1;

		for(cnt=0; cnt< teams; cnt++ )
			{
			if ( share->league_data[ group ][ cnt ].win_pos == 4 )
				euro_teams_to_exclude[ 1 ] = share->group_teams[group][cnt];
			}
		}
	else
		{
		/** Teams = 6 **/

		for(cnt=0; cnt< teams; cnt++ )
			{
			if ( share->league_data[ group ][ cnt ].win_pos == 4 )
				euro_teams_to_exclude[ 0 ] = share->group_teams[group][cnt];
			if ( share->league_data[ group ][ cnt ].win_pos == 5 )
				euro_teams_to_exclude[ 1 ] = share->group_teams[group][cnt];
			}
		}
}




void replay_euro2000()
{
WORD cnt;
WORD game;
WORD team,group,teams;




/*** Before replay, Store 9 1st place winning teams and 9 2nd place teams ***/
	for( group=0; group<9; group++ )
		{
		teams = share->teams_in_group[ group ];

		for( cnt=0;cnt<teams;cnt++)
			{
			if ( share->league_data[ group ][ cnt ].win_pos == 0 )
				{
			/** Get 1st place team from this group **/
				share->euro_final_data.teams[ group ] = share->group_teams[ group ][ cnt ];
				}

			if ( share->league_data[ group ][ cnt ].win_pos == 1 )
				{
			/** Get 2nd place team from this group **/
				eq_runner_teams[ group ] = share->group_teams[ group ][ cnt ];
				}
			}
		}

	share->game_type = EURO2000_CALC;
	reset_league_data();



/*********************************************************
 * Play whole Euro2000 qualifier again, with only the    *
 * Top four teams in each group                          *
 *********************************************************/
	for( game=0; game<TOTAL_EURO_GAMES; game++ )
		{
		share->team1 	  = share->euro2000_data.result[ game ].team1; 
		share->team1_pts = share->euro2000_data.result[ game ].team1_score;
		share->team2     = share->euro2000_data.result[ game ].team2; 
		share->team2_pts = share->euro2000_data.result[ game ].team2_score;

		get_team_list_pos( share->team1, 0 );	// output to team1_list_pos
		get_team_list_pos( share->team2, 1 );	// output to team2_list_pos


		calc_euro_teams_to_exclude();	// get teams who came bottom of their group

		if ( (euro_teams_to_exclude[0] == share->team1) || (euro_teams_to_exclude[1] == share->team1) || (euro_teams_to_exclude[0] == share->team2) || (euro_teams_to_exclude[1] == share->team2) )
			{
			/* Dont play this team in game **/
			//printf("not playing %s and %s in group %d\n",get_team_name( share->team1 ), get_team_name( share->team2 ), share->current_group );
			}
		else
			calc_league_stats();
		}
}

void calc_euro2000_2nd_place()
{
WORD cnt,teams;
WORD game;
WORD team,group;
WORD final_cnt=0;
WORD ko_cnt=0;

/**************************************************************************
 * Put all NEWLY SORTED 2nd place teams into a new list and pick best one *
 **************************************************************************/

	for( group=0; group<9; group++ )
		{
		teams = share->teams_in_group[ group ];
		for( team=0; team<teams; team++)
			{
			if ( share->group_teams[ group ][ team ] == eq_runner_teams[ group ] )
				{
				/** Put all data for this 2nd place team into SPECIAL group 9 ( not normally used ) ***/
				share->league_data[ 9 ][ final_cnt ]  = share->league_data[ group ][ team ];
				share->group_teams[ 9 ][ final_cnt ]  = share->group_teams[ group ][ team ];
				final_cnt++;
				}
			}
		}



/************************************************
 *	 Group 9 now contains 9 2nd place teams 	   *
 *  So, sort them and find best 2ns place team  *
 ************************************************/

	share->current_group = 9;				/*** Use group 9 for sort ***/
	share->total_groups = 10;
	share->teams_in_group[ 9 ] = 9;		/*** there are 9 2nd place teams ***/
	sort_league_stats();						// sort these 9 teams



/*******************************************
 *  Sort done, find best 2nd place team    *
 *******************************************/

	for( team=0; team<9; team++ )
		{
		if ( share->league_data[9][team].win_pos == 0 )
			{
			share->euro_final_data.teams[ 9 ] = share->group_teams[ 9 ][ team ];
//			printf("runner up team=%s\n",get_team_name( share->euro_final_data.teams[ 9 ] ));
		
			}
		else
			{
			/*** All other 2nd place teams go into a KNOCKOUT ***/
			share->euro_final_data.runner_teams[ ko_cnt ] = share->group_teams[9][ team ];
			ko_cnt++;
			}
		}
}


void add_ko_teams_to_euro_final()
{
WORD cnt;
/** Copy knockout teams from 3rd round to euro final teams **/
	for( cnt=0; cnt<4; cnt++ )
		{
		share->euro_final_data.teams[ 10+cnt ] = share->ko_data.teams[ 2 ][ cnt ];
		}

/** Add Belgium and Netherlands, cuz they're shite speaking Cunts ***/

	share->euro_final_data.teams[ 14 ] = Belgium;	
	share->euro_final_data.teams[ 15 ] = Netherlands;	

}





WORD check_human_territory_winner()
{
WORD cnt,team;
	for( cnt=0; cnt< share->teams_in_group[ share->current_group ]; cnt++ )
		{
		if ( share->league_data[ share->current_group][ cnt ].win_pos == 0 )
			{
			team = share->group_teams[ share->current_group][ cnt ];
			if ( share->cpu_teams_index_list[ team ] == 0 )
				{
				// team at top of this league winning was human
	 			return YES;
				}
			}
		}
 	return NO;
}




void init_worldcup_draw()
{
WORD x, gr;

	share->worldcup_data.game=0;

	for( gr=0;gr<8; gr++ )
		{
		for (x=0;x<4;x++ )
			share->group_teams[gr][x] = worldcup_group_teams[gr][x];
		share->teams_in_group[gr]=4;
		}
}

WORD do_worldcup_draw()
{
WORD gr,x;

	if ( share->worldcup_data.game == 48 )
		return YES;


	share->team1 = worldcup_fixlist[ share->worldcup_data.game ][0];
	share->team2 = worldcup_fixlist[ share->worldcup_data.game ][1];
	share->team1_cpu = share->cpu_teams_index_list[ share->team1 ];
	share->team2_cpu = share->cpu_teams_index_list[ share->team2 ];



/** Get list pos of each team playing and group **/

	for( gr=0; gr<8; gr++ )
		{
		for(x=0;x<4;x++ )
			{
			if ( worldcup_group_teams[ gr ][ x ] == share->team1 )
				{
				share->team1_list_pos = x;
				share->current_group = gr;
				//printf("gr=%d\n",gr);
				}

			if ( worldcup_group_teams[ gr ][ x ] == share->team2 )
				share->team2_list_pos = x;
			}
		}

//	printf("doing wc draw of teams %s and %s\n",get_team_name(share->team1), get_team_name(share->team2) );

	share->worldcup_data.game++;

	return NO;


}




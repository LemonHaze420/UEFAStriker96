/****************************************************
 *																	 *
 *	 																 *
 * Knockout stats screen									 *
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

BYTE *knock_next_match_lolly_text[]={
	"Next match",
};

BYTE *winner_lolly_text[]={
	"Winner",
};

BYTE *winners_lolly_text[]={
	"Winners",
};





/****************************************************
 *																	 *
 * Prototypes													 *
 *																	 *
 ****************************************************/

void set_ko_clip_sprites();
void kill_ko_clip_sprites();

void show_knockout_page();
void knockout_logic();

/****************************************************
 *																	 *
 * Defines														 *
 *																	 *
 ****************************************************/

#define Ko_boxwd (110*4)
#define Ko_boxsp (84*4)

/****************************************************
 *																	 *
 * Vars															 *
 *																	 *
 ****************************************************/

UWORD knockout_stat_shell_buttons[]={ SHELL_PREV, SHELL_NEXT, 0 };
UWORD euro2000_qual_shell_buttons[]={ SHELL_SAVE,  SHELL_NEXT, SHELL_QUIT, 0 };


UWORD knockout_over_shell_buttons[]={ SHELL_OK, 0 };
UWORD knockout_quit_next_shell_buttons[]={ SHELL_NEXT, SHELL_QUIT, 0 };
UWORD knockout_end_of_round_shell_buttons[]={ SHELL_SAVE, SHELL_NEXT, SHELL_QUIT, 0 };



WORD ko_windx,ko_windx_adjust,ko_windx_base;
Sprite *ko_clip_sp[2];
Sprite *ko_arrows[2];
RECT ko_clip_rect={ 50-4,0,512-(40*2)+4,285  };
Rdb_gadget ko_stat_rdb[5];

void init_knockout_stat_page()
{
WORD cnt;
WORD list[]={3,2,1,0,0};


	if ( share->game_type == KNOCKOUT )
		{
		if ( share->game_stat == GAME_OVER ) //game_over )
			{
			set_shell_bottom_buttons( knockout_over_shell_buttons, 0 );	
			set_title_lolly( winner_lolly_text[config->language] , game_types_text[ config->language ][ share->game_type ] );
			}
		else
			{
			set_title_lolly( knock_next_match_lolly_text[config->language] , game_types_text[ config->language ][ share->game_type ] );

			if ( share->game_stat != GAME_IN_PROGRESS )
				set_shell_bottom_buttons( knockout_stat_shell_buttons ,0 );	
			else
				{
				//if ( share->ko_data.draws_to_do  )
				//	set_shell_bottom_buttons( knockout_quit_next_shell_buttons,0 );	
				//else
					set_shell_bottom_buttons( knockout_end_of_round_shell_buttons,0 );	
				}
			}
		}
	else
		{
		if ( share->game_stat == GAME_OVER ) //game_over )
			{
			if ( (share->game_type == EURO2000) || (share->game_type == EURO2000_KO )  )
				set_title_lolly( winners_lolly_text[config->language] , game_types_text[ config->language ][ share->game_type ] );	
			else
				set_title_lolly( winner_lolly_text[config->language] , game_types_text[ config->language ][ share->game_type ] );	

			}
		else
			set_title_lolly( knock_next_match_lolly_text[config->language] , game_types_text[ config->language ][ share->game_type ] );

		set_shell_bottom_buttons( euro2000_qual_shell_buttons,0 );														 
		}



	ko_windx_adjust = ko_windx = 0;

	set_ko_clip_sprites();

	ko_arrows[0]=spawn_sprite( SLIDER );
	sprite->frame = 2;
	sprite->transp_rate = 1;
	sprite->x.w.hi = -235*2;

	ko_arrows[1]=spawn_sprite( SLIDER );
	sprite->frame = 3;
	sprite->transp_rate = 1;
	sprite->x.w.hi = 222*2;


	set_rdb_gadget( 0, &ko_stat_rdb[0], -308, -380+(0*10*4), 	Ko_boxwd-64+40, 668,-1 );
	set_rdb_gadget( 0, &ko_stat_rdb[1], -308, -380+(4*10*4), 	Ko_boxwd-64+40, 88*4,-1 );
	set_rdb_gadget( 0, &ko_stat_rdb[2], -308, -380+(6*10*4), 	Ko_boxwd-64+40, 49*4,-1 );
	set_rdb_gadget( 0, &ko_stat_rdb[3], -308, -380+(7*10*4), 	Ko_boxwd-64+40, 28*4,-1 );
	set_rdb_gadget( 0, &ko_stat_rdb[4], -308, -380+(7*10*4)+20, Ko_boxwd-64+40, 72,-1 );
	ko_stat_rdb[0].depth = PLASMA_DEPTH-4;
	ko_stat_rdb[1].depth = PLASMA_DEPTH-4;
	ko_stat_rdb[2].depth = PLASMA_DEPTH-4;
	ko_stat_rdb[3].depth = PLASMA_DEPTH-4;
	ko_stat_rdb[4].depth = PLASMA_DEPTH-4;




	ko_windx_base = list[share->teams_in_competition/4];

	ko_windx_adjust = share->ko_data.current_round;
	if ( ko_windx_adjust > 3 )
		ko_windx_adjust = 3;

	ko_windx = (ko_windx_adjust * Ko_boxwd) - (Ko_boxwd/2);



}

void update_knockout_stat_page()
{
//	FntPrint("t1=%s t2=%s\n",get_team_name( share->team1 ), get_team_name( share->team2 ) );
	show_knockout_page();


}
							 
void kill_knockout_stat_page()
{
WORD cnt;


	kill_ko_clip_sprites();

	for(cnt=0;cnt<5;cnt++)
		{
		kill_rdb_gadget( ko_stat_rdb[cnt].gadget );
		}

	kill_sprite( ko_arrows[0] );
	kill_sprite( ko_arrows[1] );
	kill_shell_bottom_buttons();


}








void show_knockout_page()
{
BYTE *txt;
Textman_struct *ts;
WORD ko_windx_start, ko_windx_dest,team_num,ylines,x,y,cnt,cnt2,yoffs;
WORD times[]={ 40,20, 20, 50 };
static WORD round_over;						
CVECTOR color;
WORD both_teams_cpu;


//	FntPrint("cr=%d winpos=%d drawsleft=%d\n",share->ko_data.current_round, share->ko_data.winning_list_pos, share->ko_data.draws_to_do );


	if ( (share->game_type == KNOCKOUT) || (share->game_type == EUROFINAL_KO) || (share->game_type == WORLDCUP)  )
		{
		if ( ko_windx_base != 3 )
			modify_current_item( 1 );
		}

	ko_windx_start = -(Ko_boxwd/2);
	ko_windx_dest = ko_windx_start + ( ko_windx_adjust * Ko_boxwd);

	ko_windx = ko_windx + ( (ko_windx_dest-ko_windx) /4);

//	if ( ko_windx  >1097 )
//		ko_windx = 1097;

	ko_stat_rdb[0].size.x = -(512+64+28) - ko_windx - 40;
	ko_stat_rdb[1].size.x = ko_stat_rdb[0].size.x+((Ko_boxwd)*1);
	ko_stat_rdb[2].size.x = ko_stat_rdb[0].size.x+((Ko_boxwd)*2);
	ko_stat_rdb[3].size.x = ko_stat_rdb[0].size.x+((Ko_boxwd)*3);
	ko_stat_rdb[4].size.x = ko_stat_rdb[0].size.x+((Ko_boxwd)*4);



	cnt = cnt2= yoffs = 0;

	ylines = 16;
	for( x=0; x<5; x++ )
		{
		for( y=0; y<ylines; y++ )
			{

			if ( x >= ko_windx_base )
				{
				team_num = share->ko_data.teams[ x ][ y ];
				if ( team_num != -1 )
					{
					sprintf( text_buffer, "%s (%s)", get_team_name( team_num ),humcpu_txt[config->language][ share->cpu_teams_index_list[team_num]?1:0 ] );
					}
				else
					{
					sprintf( text_buffer, "- - - - - - - - - - -" );
					}


				ts=add_text_string ( tslot_poses[MAIN_TSLOT]+cnt2, text_buffer, TINY_FONT, milford_font_logic, (-102*4)+(x*Ko_boxwd)-ko_windx-28, ((-94+(y*10))*4)+yoffs, FONT_XC, -2, 0, 0,0 );

//				printf("kowindx=%d x=%d\n",ko_windx,x  );

				sprite = ts->tail_sprite;

#if 0
				if ( (share->cpu_teams_index_list[ team_num ] == NO) && ( team_num != -1 ) )
					{
					/*** Yellow text for Human teams ***/
					color.r = 220;
					color.g = 220;
					color.b = 32;
					}
				else
					{
#endif
					color.r = 
					color.g = 
					color.b = 128;
//					}

				if ( share->ko_data.played[ x ][ y ] )
					{
					color.r >>= 2;
					color.g >>= 2;
					color.b >>= 2;
					}		 
			


				if ( (x == share->ko_data.current_round) && ( share->game_stat != GAME_OVER ) )
					{
					if ( (share->team1 == share->ko_data.teams[ x ][ y ] ) || (share->team2 == share->ko_data.teams[ x ][ y ] )  )
						{
						if ( (timer/8)&1)
							{
							color.r >>= 1;
							color.g >>= 1;
							color.b >>= 1;
							}
						}
					}

				ts->depth = PLASMA_DEPTH-5;

				ts->text_color.r = color.r;
				ts->text_color.g = color.g;
				ts->text_color.b = color.b;
				}
			cnt2++;
			cnt++;
			}

		
		ylines >>=1;
		yoffs += ylines*10*2;

		if ( ylines == 0 )
			break;
		}



	ko_arrows[0]->display = NO;
	ko_arrows[1]->display = NO;

	if ( shell->master_option_active == NO )
		{
		if ( cjoy->db & PAD_LT )
			{
			if ( ko_windx_adjust > ko_windx_base )
				ko_windx_adjust--;
			//else
			//	ko_windx_adjust = ko_windx_base;
			}
		else
			{
			if ( cjoy->db & PAD_RT )
				{
				if ( ko_windx_adjust < 3 )
					ko_windx_adjust++;
				}
			}

		if ( ko_windx_base != 3 )
			{
	 		ko_arrows[0]->display = (timer/8)&1;
 			ko_arrows[1]->display = (timer/8)&1;
			}
		}



//	if ( (ko_windx_adjust+ko_windx_base) > 3 )
//		ko_windx_adjust--;


}

void set_ko_clip_sprites()
{
	ko_clip_sp[0] = spawn_clip_sprite( &ko_clip_rect, PLASMA_DEPTH-2 );
	ko_clip_sp[1] = spawn_null_clip_sprite( PLASMA_DEPTH-7 );
}



void kill_ko_clip_sprites()
{
	kill_sprite ( ko_clip_sp[0] );	
	kill_sprite ( ko_clip_sp[1] );	
}

void knockout_stat_page()
{
		exec_shell( init_knockout_stat_page, update_knockout_stat_page, kill_knockout_stat_page );
}



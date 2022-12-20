/****************************************************
 *																	 *
 *	 																 *
 * Configuration routines for title						 *
 *	 																 *
 *																	 *
 ****************************************************/

#include "incs.h"

#include "\global\s01.h"
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include "font.h"
#include "shell.h"
#include	"pad.h"
#include "anim.def"
#include "hardware.h"
#include "render.h"
#include "datafile.h"
#include "text.h"
#include "mem.h"
#include "cmplogic.h"
/******************************
 *										*
 * defines 							*
 *										*
 ******************************/

#define TM_STAT_BAR_WD 384
#define TM_TEXT_HT (13*4)


/******************************
 *                            *
 * Structs 							*
 *										*
 ******************************/



/******************************
 *										*
 * Prototypes						*
 *										*
 ******************************/
void check_for_red_card_player();

void show_tm_player_list();
void tm_control();
void show_tm_formation();
void show_tm_player_stats();
void draw_tm_stat_bar( WORD color );
void update_team_manager();
BYTE find_player_from_shirt_num( WORD team, BYTE shirt_num );


/******************************
 *                            *
 *     Vars							*
 *										*
 ******************************/

UWORD man_ok_butt[]={  SHELL_OK,0 };
Database_team *db_teams = (Database_team *)&ability_file_addr; //[MAX_NON_USER_TEAMS+MAX_CLUB_TEAMS];
WORD red_card_in_lineup;

UWORD tm_buttons[]={ SHELL_PREV, SHELL_NEXT, SHELL_QUIT };
WORD tm_windy, tm_windy_dest;
RECT tm_clip_rect={ 0, 40, 512, 157 };
Sprite *tmf_spr[11];
WORD manager_team_num, tm_mode;
WORD tm_mode0_item, tm_mode1_item;
WORD tm_current_player, tm_swap_player;

WORD manage_ok_butt[]={ SHELL_OK, 0 };

BYTE *tm_forms_nums[][5]={ 

	"3-5-2",		
	"3-5-1-1", 	
	"3-1-4-2", 	
	"1-2-5-2", 	
	"1-2-5-1-1",

	"4-4-2", 	
	"4-2-2-2", 	
	"4-1-3-2", 	
	"4-3-1-2", 	
	"arse",  	

	"4-5-1", 	
	"4-2-3-1", 	
	"arse", 		
	"arse", 		
	"arse", 		

	"4-3-3", 	
	"4-2-1-3",	
	"4-1-2-3", 	
	"arse", 		
	"arse", 		

	"5-3-2", 	
	"5-3-1-1", 	
	"1-4-3-2",	
	"arse", 		
	"arse",		

	"2-5-3",		
	"2-4-1-3",	
	"2-5-2-1",	
	"arse",		
	"arse",		

	"4-2-4",		
	"4-1-1-4",	
	"arse",		
	"arse",		
	"arse",		
};


BYTE tm_forms_game_nums[][5]={

0,1,2,3,4,

5,6,7,8,0,

9,10,0,0,0,

11,12,13,0,0,

14,15,16,0,0,

17,18,19,0,0,

20,21,0,0,0,

};

WORD human_team_pos;

Sprite *tm_stat_sprs[ TM_PLAYER_STATS ];
Rdb_gadget tm_rdb[4];
Rdb_gadget tm2_rdb[4];

WORD tm_forms[][10][2]={

405-361, 152-129,
462-361, 174-129,
434-361, 162-129,
510-361, 160-129,
495-361, 184-129,
445-361, 140-129,
475-361, 149-129,
535-361, 195-129,
444-361, 193-129,
429-361, 210-129,

478-361, 193-129,
408-361, 203-129,
389-361, 162-129,
434-361, 179-129,
444-361, 214-129,
438-361, 140-129,
463-361, 167-129,
534-361, 210-129,
523-361, 163-129,
481-361, 152-129,

381-361, 157-129,
422-361, 173-129,
500-361, 194-129,
441-361, 148-129,
462-361, 183-129,
442-361, 214-129,
539-361, 205-129,
405-361, 205-129,
482-361, 143-129,
509-361, 166-129,

460-361, 174-129,
443-361, 192-129,
425-361, 213-129,
423-361, 163-129,
499-361, 185-129,
448-361, 144-129,
384-361, 151-129,
544-361, 196-129,
503-361, 158-129,
480-361, 138-129,

385-361, 158-129,
421-361, 174-129,
546-361, 205-129,
498-361, 195-129,
456-361, 184-129,
412-361, 209-129,
439-361, 146-129,
479-361, 158-129,
445-361, 218-129,
514-361, 168-129,

454-361, 218-129,
530-361, 205-129,
413-361, 205-129,
409-361, 138-129,
539-361, 167-129,
495-361, 156-129,
450-361, 146-129,
482-361, 190-129,
439-361, 179-129,
388-361, 160-129,

483-361, 182-129,
468-361, 200-129,
455-361, 225-129,
420-361, 185-129,
401-361, 202-129,
456-361, 152-129,
439-361, 168-129,
548-361, 174-129,
500-361, 164-129,
408-361, 137-129,

406-361, 135-129,
504-361, 162-129,
552-361, 175-129,
445-361, 145-129,
463-361, 167-129,
393-361, 170-129,
443-361, 188-129,
504-361, 205-129,
405-361, 201-129,
457-361, 218-129,

409-361, 129-129,
453-361, 141-129,
498-361, 152-129,
553-361, 165-129,
415-361, 158-129,
465-361, 172-129,
516-361, 187-129,
440-361, 190-129,
401-361, 200-129,
461-361, 223-129,

413-361, 132-129,
456-361, 146-129,
500-361, 191-129,
499-361, 158-129,
550-361, 171-129,
465-361, 181-129,
428-361, 169-129,
387-361, 154-129,
541-361, 204-129,
429-361, 210-129,

412-361, 132-129,
462-361, 147-129,
508-361, 158-129,
555-361, 169-129,
444-361, 161-129,
481-361, 173-129,
403-361, 169-129,
455-361, 190-129,
510-361, 209-129,
428-361, 208-129,

411-361, 131-129,
458-361, 145-129,
505-361, 159-129,
554-361, 171-129,
416-361, 166-129,
463-361, 180-129,
513-361, 196-129,
378-361, 189-129,
431-361, 207-129,
477-361, 225-129,

408-361, 132-129,
458-361, 147-129,
507-361, 160-129,
554-361, 172-129,
430-361, 159-129,
481-361, 175-129,
445-361, 189-129,
380-361, 192-129,
427-361, 210-129,
491-361, 229-129,

405-361, 131-129,
450-361, 145-129,
503-361, 161-129,
558-361, 174-129,
461-361, 172-129,
429-361, 180-129,
475-361, 196-129,
370-361, 190-129,
435-361, 212-129,
491-361, 230-129,

403-361, 135-129,
438-361, 144-129,
474-361, 153-129,
515-361, 163-129,
553-361, 172-129,
411-361, 170-129,
455-361, 182-129,
499-361, 193-129,
415-361, 206-129,
457-361, 218-129,

464-361, 173-129,
423-361, 159-129,
407-361, 133-129,
447-361, 195-129,
513-361, 160-129,
555-361, 171-129,
444-361, 145-129,
429-361, 214-129,
507-361, 186-129,
480-361, 153-129,

403-361, 136-129,
451-361, 153-129,
494-361, 166-129,
417-361, 168-129,
456-361, 218-129,
477-361, 141-129,
403-361, 201-129,
556-361, 182-129,
460-361, 184-129,
502-361, 194-129,

384-361, 195-129,
493-361, 151-129,
459-361, 143-129,
388-361, 151-129,
426-361, 164-129,
464-361, 175-129,
501-361, 186-129,
545-361, 196-129,
432-361, 212-129,
477-361, 225-129,

467-361, 143-129,
508-361, 154-129,
406-361, 148-129,
445-361, 159-129,
488-361, 173-129,
539-361, 187-129,
459-361, 187-129,
393-361, 195-129,
433-361, 210-129,
479-361, 227-129,

457-361, 144-129,
435-361, 213-129,
495-361, 156-129,
402-361, 148-129,
434-361, 161-129,
472-361, 171-129,
507-361, 180-129,
549-361, 192-129,
431-361, 185-129,
462-361, 196-129,

409-361, 136-129,
459-361, 149-129,
500-361, 160-129,
552-361, 171-129,
440-361, 172-129,
477-361, 183-129,
372-361, 184-129,
422-361, 200-129,
458-361, 211-129,
518-361, 230-129,

411-361, 131-129,
453-361, 143-129,
497-361, 157-129,
549-361, 171-129,
466-361, 172-129,
446-361, 192-129,
361-361, 184-129,
411-361, 202-129,
458-361, 218-129,
522-361, 239-129,


};

WORD variant_cnt[]={ 5, 4, 2, 3, 3, 3, 2 };


void init_team_manager()
{
WORD cnt, player;


	tm_windy=tm_windy_dest = 0; //21*TM_TEXT_HT;

	set_title_lolly( team_manager_lolly_text[config->language], (BYTE *)get_team_name( manager_team_num ) );

	set_shell_bottom_buttons( tm_buttons,-1 );

	spawn_clip_sprite ( &tm_clip_rect, MIDDLE_DEPTH-9 );
	sprite->group_num = 888;
	spawn_null_clip_sprite ( MIDDLE_DEPTH-12 );
	sprite->group_num = 888;

	for( cnt=0;cnt<10;cnt++)
		{
		tmf_spr[cnt] = spawn_sprite( SPIN );
		sprite->group_num = 888;
		sprite->use_frame_offs = YES;
		}

	set_pad_repeat_speed( 12, 1 );
	tm_mode = 0;

	tm_mode0_item = 0; //21;
	tm_mode1_item = 23;
	tm_current_player=0;




/**** Stat bars ****/

	for(cnt=0;cnt<TM_PLAYER_STATS;cnt++)
		{
		tm_stat_sprs[cnt] = spawn_sprite( -1 );
		sprite->group_num = 888;
		sprite->gouraud = YES;

		sprite->r0 = 0;
		sprite->g0 = 90;
		sprite->b0 = 0;

		sprite->r1 = 70;
		sprite->g1 = 190;
		sprite->b1 = 0;

		sprite->r2 = sprite->r0;
		sprite->g2 = sprite->g0;
		sprite->b2 = sprite->b0;

		sprite->r3 = sprite->r1;
		sprite->g3 = sprite->g1;
		sprite->b3 = sprite->b1;

		sprite->untextured_frame.w = TM_STAT_BAR_WD;
		sprite->untextured_frame.h = 7*4;

		sprite->x.w.hi = 96-28-24+12-6;
		sprite->y.w.hi = 36+12+12+( cnt*(9*4) );

		sprite->depth = MIDDLE_DEPTH + 10;

		sprite->transp_rate=1;
		}



	for(cnt=0;cnt<7;cnt++)
		{
		spawn_sprite( PITCH );
		sprite->group_num = 888;
		sprite->use_frame_offs=YES;
		sprite->y.w.hi = -(192+100) + 40;
		sprite->x.w.hi = 38;
		sprite->frame = cnt;
		sprite->transp_rate = 1;
		sprite->depth = FURTHEST_DEPTH-12;
		}


	set_rdb_gadget( 0, &tm_rdb[0], -476,-348, 472, 640-(6*4), -1 );
	set_rdb_gadget( 0, &tm_rdb[1], 40,52,404,220, -1 );	// box around stats
	set_rdb_gadget( 0, &tm_rdb[2], 40,0,404,36, -1 );	// box around strategy
	set_rdb_gadget( 0, &tm_rdb[3], 40,-348, 404, 84, -1 );	// box around formation and variant




	human_team_pos = set_human_team_pos( manager_team_num );


	for(cnt=0;cnt<8;cnt++)
		update_team_manager();



}

void update_team_manager()
{

	tm_control();
	show_tm_player_stats();
	show_tm_player_list();
	show_tm_formation();

}



void kill_team_manager()
{
WORD cnt;

	kill_sprite_group( 888 );
	restore_repeat_speed();

	for(cnt=0;cnt<4;cnt++)
		kill_rdb_gadget( tm_rdb[cnt].gadget );
}


void team_manager()
{
WORD old_thread_pos=comp->thread_pos;
WORD cnt;
WORD tp_store;


	if ( share->team1_cpu == NO )
		{
		manager_team_num = share->team1;

		tp_store = comp->thread_pos;

		while(1)
			{
			exec_shell( init_team_manager, update_team_manager, kill_team_manager );
			if ( red_card_in_lineup && (shell->current_button == SHELL_NEXT) )
				{
				do_request( red_card_player_text[config->language], manage_ok_butt, 20*4, SHELL_OK );
				//comp->thread_pos = old_thread_pos;
				}
			else
				{

				//comp->thread_pos = tp_store;
				break;
				}
			}

			
		share->human_team1_pos = human_team_pos;

		if ( shell->current_button == SHELL_NEXT )
			comp->thread_pos = tp_store;
		else
			return;
		}

	

	if ( share->team2_cpu == NO )
		{
		manager_team_num = share->team2;


		while(1)
			{
			exec_shell( init_team_manager, update_team_manager, kill_team_manager );

			if ( red_card_in_lineup && (shell->current_button == SHELL_NEXT) )
				{
				do_request( red_card_player_text[ config->language ], manage_ok_butt, 20*4, SHELL_OK );


				comp->thread_pos = old_thread_pos;
				}
			else
				break;
			}
		share->human_team2_pos = human_team_pos;
		}
	else
		{
		comp->thread_pos++;
		}

//	if ( (share->team1_cpu == YES ) && (share->team2_cpu == YES) )



}





void show_tm_player_list()
{
WORD y;
WORD dy;
WORD scn_y;
Textman_struct *ts;
Database_player *player;
BYTE gk_text_buffer[8];
WORD index;
WORD yp=0;
WORD tslot;
WORD pos;

	if ( ((tm_mode == 0)||(tm_mode == 2)) && (shell->master_option_active==NO) )
		{
  		tm_windy = (shell->current_item* TM_TEXT_HT);
		tm_current_player = shell->current_item;
		}

//	if ( shell->master_option_active == NO )
		tm_windy_dest += (tm_windy - tm_windy_dest)/4;


	tslot=0;	
	for( y=0; y<22; y++)
		{
		index = share->order[ human_team_pos ][ y ];
		player = &db_teams[ manager_team_num ].player[ index ];

		if ( player->position == 0 )
			sprintf( gk_text_buffer, " - %s ",gk_text[config->language] );
		else
			strcpy( gk_text_buffer, " " );

		sprintf( text_buffer, "%s%s(%d)",player->name, gk_text_buffer, player->shirt_num );

		if ( y<11 )
			yp = -28;
		else
			yp = 44;

		/** Print "Starting line-up" **/
		if ( y == 0 )
			{
			ts=add_text_string ( tslot_poses[MAIN_TSLOT]+tslot, starting_text[ config->language], MILFORD_FONT, milford_font_logic, -426, yp + (y*TM_TEXT_HT)-tm_windy_dest-(32*4)-(14*4), FONT_NORMAL, -2, 0, 10,0,y );
			ts->depth = MIDDLE_DEPTH-10;

			set_ts_title_color( ts );	/** Make this text stand out **/

			tslot++;
			}


		/** Print "Substitutions" **/
		if ( y == 11 )
			{
			ts=add_text_string ( tslot_poses[MAIN_TSLOT]+tslot, subs_text[ config->language], MILFORD_FONT, milford_font_logic, -426, yp + (y*TM_TEXT_HT)-tm_windy_dest-(32*4)-(14*4), FONT_NORMAL, -2, 0, 10,0,y );
			ts->depth = MIDDLE_DEPTH-10;  
			set_ts_title_color( ts );	/** Make this text stand out **/
			tslot++;
			}

		/** Print player name **/
		ts=add_text_string ( tslot_poses[MAIN_TSLOT]+tslot, text_buffer, MILFORD_FONT, milford_font_logic, -426, yp + (y*TM_TEXT_HT)-tm_windy_dest-(32*4), FONT_NORMAL, y, 0, 10,0,shell->master_option_active?y:0 );
		tslot++;


		if ( (tm_mode == 2) && (tm_swap_player == y) && (shell->master_option_active==NO) )
			{
			ts->text_color.r = 120;
			ts->text_color.g = 120;
			ts->text_color.b = 30;
			}
		else
			{
			pos = set_human_team_pos( manager_team_num );
			if ( share->red_cards_awarded[ pos ][ player->shirt_num ] )
				{
				ts->text_color.r = 255;
				ts->text_color.g = 0;
				ts->text_color.b = 0;
				}
			else
				{
				ts->text_color.r = 
				ts->text_color.g = 
				ts->text_color.b = 128;
				}
			}


		ts->depth = MIDDLE_DEPTH-10;  

		}



}

void show_tm_formation()
{
WORD spr,cnt,fspd;
LONG x2,y2;
Textman_struct *ts;
WORD f,v;

/** Calc the single formation num 0-22 from formation and variant **/

	f = share->formation[human_team_pos];
	v = share->formation_variant[ human_team_pos ][ f ];

	FntPrint("f=%d v=%d gf=%d\n",f,v,share->game_formation[ human_team_pos ] );
	share->game_formation[ human_team_pos ] = tm_forms_game_nums[ f ][ v ];



	for( cnt=0; cnt<10; cnt++ )
		{
		x2 = tm_forms[ share->game_formation[ human_team_pos ] ][cnt][0];
		y2 = tm_forms[ share->game_formation[ human_team_pos ] ][cnt][1]; 

		x2 = 88 + ((x2*(174*2)) / 256);
		y2 = -272 + ((y2*(59*4)) / 144);

		sprite = tmf_spr[cnt];

		sprite->x.w.hi = x2; //+= (x2-sprite->x.w.hi)/4;
		sprite->y.w.hi = y2; //+= (y2-sprite->y.w.hi)/4;

		sprite->depth = NEAREST_DEPTH+200-(((400+sprite->y.w.hi)/8));

		}

	f=share->formation[ human_team_pos ];
	v=share->formation_variant[ human_team_pos ][ f ];

/*** Formation text ***/
	fspd=shell->master_option_active?10:0;
	sprintf( text_buffer, "%s : %s", tm_form_text[ config->language ], tm_forms_nums[f][0] );
	ts=add_text_string ( tslot_poses[MAIN_TSLOT]+24, text_buffer, TINY_FONT, milford_font_logic, 244, -360, FONT_XC, 22, 0, fspd,0,0 );

/*** Variant text ***/
	sprintf( text_buffer, "%s : %s", tm_variant_text[ config->language ], tm_forms_nums[f][v] );
	ts=add_text_string ( tslot_poses[MAIN_TSLOT]+25, text_buffer, TINY_FONT, milford_font_logic, 244, -374+54, FONT_XC, 23, 0, fspd,0,0 );



/*** Strategy text ***/
	add_text_string ( tslot_poses[MAIN_TSLOT]+26, tm_strat_text[ config->language ][ share->strategy[ human_team_pos ] ], TINY_FONT, milford_font_logic, 244, -12, FONT_XC, 24, 0, fspd,0,0 );


/*** Spin current player ***/
	
	for(cnt=0;cnt<10;cnt++)
		{
		if ( cnt == (db_teams[ manager_team_num ].player[tm_current_player].position-1) )
			tmf_spr[cnt]->frame = (flash_counter/2)%12;
		else
			tmf_spr[cnt]->frame = 10;
		}




}



void show_tm_player_stats()
{
WORD cnt,wd;
Database_player *player;

	player = &db_teams[ manager_team_num ].player[ tm_current_player ];

	for(cnt=0;cnt<TM_PLAYER_STATS;cnt++)	
		{
		switch ( cnt )
			{
			case 0: 
				wd = player->speed;
			break;

			case 1:
				wd = player->strength;
			break;

			case 2:
				wd = player->passing;
			break;

			case 3:
				wd = player->tackling;
			break;

			case 4:
				wd = player->shooting;
			break;

			case 5:
				wd = player->ball_control;
			break;
			}

		wd = (wd * TM_STAT_BAR_WD)/256;
		tm_stat_sprs[cnt]->untextured_frame.w += (wd-tm_stat_sprs[cnt]->untextured_frame.w)/2;
		}


	for( cnt=0; cnt< TM_PLAYER_STATS; cnt++ )
		{
		add_text_string ( tslot_poses[MAIN_TSLOT]+27+cnt, tm_player_stats[config->language][cnt], TINY_FONT, milford_font_logic, 72-24+16-6, 28+(9*4*cnt)+12, FONT_NORMAL, -2, 0, 10,0,cnt );
		}
}



void tm_control()
{
WORD pl_store;
Database_team *team;
BYTE *order1, *order2;
BYTE footballer_type1, footballer_type2;
WORD f;

	team = &db_teams[ manager_team_num ];

	if ( shell->current_item == 22 )
		{
		modify_byte( &share->formation[ human_team_pos ], 0, 6 );
		}

	if ( shell->current_item == 23 )
		{
		f=share->formation[ human_team_pos ];
		modify_byte( &share->formation_variant[ human_team_pos ][ f ], 0, variant_cnt[ f ]-1 );
		}


	if ( shell->current_item == 24 )
		{
		modify_byte( &share->strategy[ human_team_pos ], 0, 1 );
		}


	if ( (tm_mode == 0) || (tm_mode==2) )
		{
		if ( cjoy->db & PAD_X )
			{
			if ( tm_mode == 0 )
				{
				tm_swap_player = tm_current_player;
				tm_mode = 2; /** Swap mode **/
				}
			else
				{
				if (tm_mode==2) 
					{
					/** swap tm_current_player and tm_swap_player **/
				
					order1 = &share->order[ human_team_pos ][ tm_swap_player ];
					order2 = &share->order[ human_team_pos ][ tm_current_player ];

					footballer_type1 = db_teams[ manager_team_num ].player[ *order1 ].position;
					footballer_type2 = db_teams[ manager_team_num ].player[ *order2 ].position;

					 if ( (footballer_type1 == 0) && (footballer_type2 == 0) || (footballer_type1 != 0) && (footballer_type2 != 0) )
						{
						/*** Ensure goalies and fielders aren't being swapped with eachother ***/
						pl_store = *order2;
						*order2 = *order1;
						*order1 = pl_store;

						sfxSpot( GOALNET3,40);

						}
					else
						{
						sfxSpot( WHOOF,40 );

						}

					tm_mode=0;

					}
				}

			}

		if ( ( cjoy->db & PAD_UP ) && (shell->master_option_active) )
			{
			set_current_item( tm_mode0_item );
			shell->master_option_active=NO;
			}
		else
			{
			if ( shell->master_option_active==NO)
				tm_mode0_item = shell->current_item;

			modify_current_item_r(22); /** Use pad repeat on up/down **/

			if ( (cjoy->db & PAD_RT) && (shell->master_option_active==NO) )
				{
				set_current_item( tm_mode1_item );
				tm_mode = 1;
				}
			}
		}
	else
		{
		if ( tm_mode == 1 )
			{
			if ( shell->master_option_active )
				{
				if (  cjoy->db & PAD_UP  )
					{
					set_current_item( tm_mode1_item );
					shell->master_option_active = NO;
					}
				}
			else
				{
				if ( cjoy->db & PAD_LT )	
					{
					tm_mode1_item = shell->current_item;
					set_current_item( tm_mode0_item );
					tm_mode = 0;
					}
				else
					{
					if ( cjoy->db & PAD_DN )
						{
						if ( shell->current_item == 24 )
							{
							goto_shell_buttons( SHELL_NEXT );
							}
						else
							{
							if ( shell->current_item == 22 )
								{
								set_current_item( 23 );
								tm_mode1_item=23;
								}
							else
								{
								set_current_item( 24 );
								tm_mode1_item=24;
								}
							}
						}
					else
						{
						if ( cjoy->db & PAD_UP )
							{
							if ( shell->current_item == 23 )
								{
								tm_mode1_item=22;
								set_current_item( 22 );
								}
							else
								{
								if ( shell->current_item == 24 )
									{
									tm_mode1_item=23;
									set_current_item( 23 );
									}
								}
							}
						}
					}
				}
			}
		else
			{
			/*** Swap mode ***/
			}
		}


	check_for_red_card_player();

}






WORD teamstat_buttons[]={ SHELL_PREV, SHELL_NEXT, SHELL_QUIT,0 };

void init_team_stats()
{
WORD team, cnt;

	set_title_lolly( team_stat_lolly_text[config->language], game_types_text[config->language][share->game_type]  );
	set_shell_bottom_buttons( teamstat_buttons,-1 );

	set_rdb_gadget( 0, &tm2_rdb[0], -460, -304, 920, 72, -1 );
	set_rdb_gadget( 0, &tm2_rdb[1], -460, -212, 456, 480, -1 );
	set_rdb_gadget( 0, &tm2_rdb[2], 4, -212, 456, 480, -1 );


	set_top_team_scorers();

}

void update_team_stats()
{
WORD yp = -324;
WORD cnt;
Textman_struct *ts;
BYTE pl;
BYTE shirt_num;

/*** Show 2 teams who are playing ***/

/** Print vs text **/
	ts=add_text_string ( tslot_poses[MAIN_TSLOT]+0, vs_text[ config->language], MILFORD_FONT, milford_font_logic, 0, yp, FONT_XC, 0, 0, 10,0,0 );
	set_ts_title_color( ts );
/** Print team1 **/
	ts=add_text_string ( tslot_poses[MAIN_TSLOT]+1, (UBYTE *)get_team_name( share->team1 ), MILFORD_FONT, milford_font_logic, -248, yp, FONT_XC, 0, 0, 10,0,0 );
	set_ts_title_color( ts );
/** Print team2 **/
	ts=add_text_string ( tslot_poses[MAIN_TSLOT]+2, (UBYTE *)get_team_name( share->team2 ), MILFORD_FONT, milford_font_logic, 248, yp, FONT_XC, 0, 0, 10,0,0 );
	set_ts_title_color( ts );

	yp += 28*4;
	
/** goals for **/
	sprintf( text_buffer,"%s : %d", goals_for_text[ config->language ] , share->goals_for[ share->team1 ] );
	add_text_string ( tslot_poses[MAIN_TSLOT]+3, text_buffer, MILFORD_FONT, milford_font_logic, -248, yp, FONT_XC, 0, 0, 10,0,2 );

	sprintf( text_buffer,"%s : %d", goals_for_text[ config->language ] , share->goals_for[ share->team2 ] );
	add_text_string ( tslot_poses[MAIN_TSLOT]+4, text_buffer, MILFORD_FONT, milford_font_logic, 248, yp, FONT_XC, 0, 0, 10,0,2 );

	yp += 14*4;

/** goals against **/
	sprintf( text_buffer,"%s : %d", goals_against_text[ config->language ] , share->goals_against[ share->team1 ] );
	add_text_string ( tslot_poses[MAIN_TSLOT]+5, text_buffer, MILFORD_FONT, milford_font_logic, -248, yp, FONT_XC, 0, 0, 10,0,4 );

	sprintf( text_buffer,"%s : %d", goals_against_text[ config->language ] , share->goals_against[ share->team2 ] );
	add_text_string ( tslot_poses[MAIN_TSLOT]+6, text_buffer, MILFORD_FONT, milford_font_logic, 248, yp, FONT_XC, 0, 0, 10,0,4 );

	yp += 24*4;

/** Top scorers text **/
	ts=add_text_string ( tslot_poses[MAIN_TSLOT]+7, top_scorers_text[ config->language], MILFORD_FONT, milford_font_logic, -248, yp, FONT_XC, 0, 0, 10,0,6 );
	set_ts_title_color( ts );
	ts=add_text_string ( tslot_poses[MAIN_TSLOT]+8, top_scorers_text[ config->language], MILFORD_FONT, milford_font_logic, 248, yp, FONT_XC, 0, 0, 10,0,6 );
	set_ts_title_color( ts );
	
	yp += 18*4;

/** Top scorers' names **/

	for(cnt=0;cnt<3;cnt++)
		{
	/** Left team **/
		sprintf( text_buffer, "%d.", cnt+1 );

		shirt_num = top_team_scorers[ share->team1 ][cnt];

		if ( shirt_num == -1 )
			{
			strcat( text_buffer, "------" );
//			strcat( text_buffer, none_text[ config->language ]  );
			}
		else
			{
			pl = find_player_from_shirt_num( share->team1, shirt_num );
			strcat( text_buffer, get_database_player_name( share->team1, pl ) );
			}
		add_text_string ( tslot_poses[MAIN_TSLOT]+9+cnt,  text_buffer, MILFORD_FONT, milford_font_logic, -248, yp, FONT_XC, 0, 0, 10,0,8+cnt );

	/** Right team **/
		sprintf( text_buffer, "%d.", cnt+1 );

		shirt_num = top_team_scorers[ share->team2 ][cnt];

		if ( shirt_num == -1 )
			{
			strcat( text_buffer, "------" );
			//strcat( text_buffer, none_text[ config->language ] );
			}
		else
			{
			pl = find_player_from_shirt_num( share->team2, shirt_num );
			strcat( text_buffer, get_database_player_name( share->team2, pl ) );
			}
		 
		add_text_string ( tslot_poses[MAIN_TSLOT]+12+cnt, text_buffer, MILFORD_FONT, milford_font_logic,  248, yp, FONT_XC, 0, 0, 10,0,8+cnt );
		yp += 14*4;		
		}

}

BYTE find_player_from_shirt_num( WORD team, BYTE shirt_num )
{
WORD cnt;

/*** Pass shirt num to find this players position within database ***/

	for(cnt=0;cnt<22;cnt++)
		{
		if ( db_teams[ team ].player[ cnt ].shirt_num == shirt_num )
			return cnt;
		}

	sprintf( text_buffer, "Shirt %d not on this team!!!", shirt_num );
	debug_print_mess( text_buffer, 0 );			
	return 0;
}

void kill_team_stats()
{
}

void team_stats()
{
WORD cnt;

	if ( (share->team1_cpu==YES) && (share->team2_cpu==YES) )
		comp->thread_pos++;
	else
		exec_shell( init_team_stats, update_team_stats, kill_team_stats );


	for(cnt=0;cnt<3;cnt++)
		{
		kill_rdb_gadget( tm2_rdb[cnt].gadget);
		}
}





void check_for_red_card_player()
{
WORD cnt,shirt_num,index;
WORD pos;

	red_card_in_lineup=NO;


	pos = set_human_team_pos( manager_team_num );

	for( cnt=0;cnt<11;cnt++)
		{
		index = share->order[ pos ][ cnt ];
		shirt_num = db_teams[ manager_team_num ].player[ index ].shirt_num;
		if ( share->red_cards_awarded[ pos ][ shirt_num ] )
			{
			 red_card_in_lineup=YES;
			}
		}



}





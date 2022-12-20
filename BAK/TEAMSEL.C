/****************************************************
 *																	 *
 *	 																 *
 * Team select													 *
 *	 																 *
 *																	 *
 ****************************************************/


#include "incs.h"

#ifdef PSX
#include "\global\s01.h"
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include	"shell.h"
#include	"pad.h"
#include "anim.def"
#include "font.h"
#include "hardware.h"
#include "render.h"
#include "datafile.h"
#include "skindraw.h"
#include "sod.h"
#include	"scene.h"
#include "anim.h"
#include "polydraw.h"
#include "text.h"
#include "territ.h"
#include "..\..\striker\frontend\mod.def"
#endif


/****************************************************
 *																	 *
 * Prototypes													 *
 *																	 *
 ****************************************************/
void calc_teams_to_generate();
void create_cpu_teams_index_list();
void calc_ts_team_num();

WORD check_ts_team_exists( WORD team );
void init_ts_teams_list();
void add_team_to_ts_list( WORD team );
void remove_team_from_ts_list( WORD team );

void display_ts_pick_list();
void adjust_teamsel_cursor();
void draw_flags();
void kill_team_select();
BYTE *get_team_name( WORD team );
void ts_create_remaining_teams();


/****************************************************
 *																	 *
 * Defines														 *
 *																	 *
 ****************************************************/

#define FLAG_X_SIZE (60*2)
#define FLAG_Y_SIZE (22*4)	//Y space between flags 

#define FLAG_X_CNT 4
#define FLAG_Y_CNT 7	// visible amount of flags on screen

#define FLAG_WINDX_CNT 4	// total flags in x
#define FLAGS_X (-220*2)
#define FLAGS_Y (-65*4)

#define FLAG_WINDX_SIZE ( FLAG_WINDX_CNT * FLAG_X_SIZE )
#define TS_SCROLL_YS (10*4) // yspace between lines on rt scroll

/****************************************************
 *																	 *
 * Vars															 *
 *																	 *
 ****************************************************/


BYTE team_ratings[]={
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	16,
	17,
	18,
	19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	32,
	33,
	34,
	35,
	36,
	37,
	38,
	39,
	40,
	41,
	42,
	43,
	44,
	45,
	46,
	47,
	48,
	49,
	50,
	51,
	52,
	53,
	54,
	55,
	56,
	57,
	58,
	59,
	60,
	61,
	62,
	63,
	64,
	65,
	66,
	67,
	68,
	69,
	70,
	71,
	72,
	73,
	74,
	75,
	76,
	77,
	78,
	79,
	80,
	81,
	82,
	83,
	84,
	85,
	86,
};






BYTE ts_teams_index_list[ TOTAL_FLAGS ];



UBYTE *ts_source_team_list;

WORD ts_teams_to_choose;	// no. of teams user is allowed to choose
WORD ts_total_teams;			// total amount of teams currently selected
WORD ts_total_human_teams; // total amount of human teams currently selected
Rdb_gadget ts_rdbs[2];
UWORD teamsel_shell_buttons[]={  SHELL_PREV, SHELL_NEXT, SHELL_QUIT,0 };
UWORD snt_shell_buttons[]={  SHELL_NEXT, SHELL_QUIT,0 };
WORD cpu_teams_to_generate, human_teams_to_generate;

Sprite *border_flag;
Sprite *ts_arrow_up, *ts_arrow_dn;

WORD ts_team_num, ts_last_team_edited;
BYTE *ts_team_name_ptr;

Sprite *ts_sprs[ FLAG_X_CNT * FLAG_Y_CNT ];
Sprite *ts_drop_sprs[ FLAG_X_CNT * FLAG_Y_CNT ];

WORD ts_ydir;

WORD ts_windx, ts_old_windx;
WORD ts_windy, ts_old_windy;

WORD ts_group;


WORD ts_cx = 0;
WORD ts_cy = 0;
WORD ts_dest_cx=0;
WORD ts_dest_cy=0;

WORD ts_scroll_y, ts_scroll_window, ts_scroll_dy;
WORD ts_visible_spr_ycnt;
WORD ts_flag_windy_size, ts_flag_windy_cnt;

void init_team_select_bootup()
{
WORD cnt;

	for ( cnt=0; cnt< TOTAL_FLAGS; cnt++ )
		{
		//ts_pick_teams_list[ cnt ] = -1;
		//ts_pick_teams_type[ cnt ] = 0;
		}

}

RECT flag_clip_rect={ 0, (FLAGS_Y/4)+128, 512,  ((FLAG_Y_SIZE/4)*6)-0 };
RECT scroll_clip_rect={ 0, 57, 512,  123 };

BYTE ts_textbuffer[256];






void init_team_select()
{
BYTE text[128];
WORD cnt,x, y, team_num;

BYTE *name_ptr;
WORD ycnt,mod,lines,teams;

	ts_cx=0;
	ts_cy = 0;
	ts_dest_cx=0;
	ts_dest_cy=0;
	ts_old_windy = ts_windy = 0;

  	ts_team_num=0;




/*** Calculate no. of rows of flags from no. of teams selection ***/
	teams = share->teams_selection_size;
	mod = teams % FLAG_X_CNT;
	lines = (teams- mod) / FLAG_X_CNT;

	if ( mod )
		lines++;
	ts_flag_windy_cnt = lines;
	ts_flag_windy_size = lines * FLAG_Y_SIZE;



	if ( FLAG_Y_CNT > ts_flag_windy_cnt )
		ts_visible_spr_ycnt = ts_flag_windy_cnt-0;
	else
		ts_visible_spr_ycnt = FLAG_Y_CNT;




	ts_scroll_y = 0;

	//teams=2;
	ts_scroll_window = NO;	// start in Flag select window

	//ts_last_team_edited=-1;




	ts_group = 1690;


	border_flag = spawn_sprite( FLGBORD );
	sprite->group_num = ts_group;
	sprite->depth = NEAREST_DEPTH;
	sprite->display=YES;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 0;

	ts_arrow_up = spawn_sprite( SLIDER );
	sprite->group_num = ts_group;
	sprite->frame = 2;
	sprite->x.w.hi = 260;
	sprite->y.w.hi = -81*4;
	ts_arrow_dn = spawn_sprite( SLIDER );
	sprite->frame = 3;
	sprite->x.w.hi = 260;
	sprite->y.w.hi = 55*4;
	sprite->group_num = ts_group;


	ts_windx = ts_windy = 0;
	ts_old_windx=-1;



	/*** Clip box for flags ***/
	spawn_clip_sprite ( &flag_clip_rect, MIDDLE_DEPTH-9 );
	sprite->group_num = ts_group;
	spawn_null_clip_sprite ( MIDDLE_DEPTH-12 );
	sprite->group_num = ts_group;

	/*** Clip box for scroll text ***/
	spawn_clip_sprite ( &scroll_clip_rect, MIDDLE_DEPTH-5 );
	sprite->group_num = ts_group;
	spawn_null_clip_sprite ( MIDDLE_DEPTH-7 );
	sprite->group_num = ts_group;




	for ( cnt=0;cnt<FLAG_X_CNT * ts_visible_spr_ycnt; cnt++ )
		{
		/*** Spawn flag ***/
		ts_sprs[cnt] = spawn_sprite ( TES_AA+cnt );
		sprite->display=YES;
		sprite->group_num = ts_group;

		/*** Spawn drop shadow ***/
		ts_drop_sprs[cnt] = spawn_sprite ( TS_DROP );
		sprite->display=YES;
		sprite->depth = MIDDLE_DEPTH - 10;
		sprite->group_num = ts_group;
		sprite->frame=0;
		sprite->transp_rate = 2;
		sprite->tint.r = sprite->tint.g = sprite->tint.b = 50;
		}


	if ( share->game_type == TERRITORY )
		set_title_lolly( def_team_single_lolly_text[config->language], game_types_text[config->language][share->game_type]  );
	else
		set_title_lolly( def_team_lolly_text[config->language],  game_types_text[config->language][share->game_type]   );


  	set_rdb_gadget( 0, &ts_rdbs[0], 64-16,   -336, 416+32, 596 , -1 );
	set_rdb_gadget( 0, &ts_rdbs[1], -448, -336, 460, 60 , -1 );


	set_shell_bottom_buttons( teamsel_shell_buttons,0 );	


	set_pad_repeat_speed( 7, 0 );


	adjust_teamsel_cursor();
	draw_flags();


	shell->disable_button[ SHELL_NEXT ] = YES;



}



void update_team_select()
{

	if ( shell->master_option_active )
		modify_current_item(1);



	ts_create_remaining_teams();

//	FntPrint("scroll window on=%d\n",ts_scroll_window?"yes":"no" );

	if ( shell->master_option_active == NO )
		adjust_teamsel_cursor();
	else
		{
		border_flag->display=NO;
		ts_arrow_up->display = 
		ts_arrow_dn->display = 0;

		}


	draw_flags();

	display_ts_pick_list();





}

void kill_team_select()
{
	kill_sprite_group( ts_group );
	kill_rdb_gadget( ts_rdbs[0].gadget );
	kill_rdb_gadget( ts_rdbs[1].gadget );
	  
   


	shell->disable_button[ SHELL_NEXT ] = NO;

	//create_cpu_teams_index_list();

}



void ts_create_remaining_teams()
{
WORD gen_cnt;
WORD rand_team;
WORD start,team_to_add;
WORD teams_chosen;
WORD diff_limit_lo, diff_limit_hi;

	diff_limit_lo = 0;
	diff_limit_hi = 20;

	calc_teams_to_generate();
	gen_cnt = human_teams_to_generate + cpu_teams_to_generate;


	if ( gen_cnt )
		{
		shell->disable_button[ SHELL_NEXT ] = YES;
		}
	else
		{
		shell->disable_button[ SHELL_NEXT ] = NO;
		}


	if ( (share->game_type == LEAGUE) || (share->game_type == FRIENDLY) || (share->game_type == KNOCKOUT) || (share->game_type == EURO2000 ) || (share->game_type == EUROFINAL ) || (share->game_type == TERRITORY ) || (share->game_type == WORLDCUP ) )
		{

		if ( (share->game_type == TERRITORY) || (share->game_type == EUROFINAL) )
			{
			diff_limit_lo = 0;
			diff_limit_hi = MAX_NON_USER_TEAMS-1;
			}
		else
			{
			if ( share->game_difficulty == 0 )
				{
				diff_limit_lo = 0;
				diff_limit_hi = 20;
				}
			if ( share->game_difficulty == 1 )
				{
				diff_limit_lo = 21;
				diff_limit_hi = 40;
				}
			if ( share->game_difficulty == 2 )
				{
				diff_limit_lo = 40;
				diff_limit_hi = MAX_NON_USER_TEAMS-1;
				}
			}




		if ( shell->current_button == SHELL_NEXT )
			{
			shell->current_button = -1;

			if ( gen_cnt > 0 )
				{
				while( gen_cnt )
					{
					/** When chosing a team at random, ensure it 
					   matches the user's previously chosen difficulty level **/
					

					/** At the moment, choose any team from list **/
					rand_team = make_random_number( share->teams_selection_size-1 );
					rand_team = *(ts_source_team_list+rand_team);

					if ( (team_ratings[ rand_team ] >= diff_limit_lo ) && ( team_ratings[ rand_team ] <= diff_limit_hi ) )
						{
						if ( check_ts_team_exists( rand_team ) == NO )
							{
							add_team_to_ts_list( rand_team );
							ts_last_team_edited = rand_team;
							gen_cnt--;
							}
						}
					}
				}
			}
		}
}














void draw_flags()
{
WORD cnt,x,y;
WORD base;
Sprite *sprite2;
WORD flag,flag_index;
//WORD spr_type_index;
WORD type;


	base = ( ts_windy / FLAG_Y_SIZE) * FLAG_X_CNT;
	cnt=0;
	for ( x=0;x<FLAG_X_CNT;x++ )
		{
		for ( y=0; y<ts_visible_spr_ycnt; y++ )
			{
			sprite = ts_sprs[cnt];
			sprite2 = ts_drop_sprs[cnt];

			flag_index = base + (y*FLAG_X_CNT) + x;
			type = (*(ts_source_team_list+flag_index));
			
//			if ( type == 255 )
			if ( flag_index >= share->teams_selection_size )
				{
			 	/** No flag at this point in table **/
				sprite->display = NO;
				sprite2->display = NO;
				cnt++;
				}
			else
				{
				type += TES_AA;
				set_sprite ( sprite, type );
				sprite->depth = MIDDLE_DEPTH - 11;

				sprite->x.w.hi = (FLAGS_X + ( x * FLAG_X_SIZE ) - ts_windx);
				sprite->y.w.hi = (FLAGS_Y + ( y * FLAG_Y_SIZE ) - (ts_windy%FLAG_Y_SIZE) );

				sprite2->x.w.hi = sprite->x.w.hi - 16;
				sprite2->y.w.hi = sprite->y.w.hi + 16;
				sprite2->display= YES;

				flag = ts_source_team_list[ flag_index ];

				if ( ts_teams_index_list[ flag ] )
					{
					sprite->tint.r = sprite->tint.g = sprite->tint.b = 55;
					}
				else
					sprite->tint.r = sprite->tint.g = sprite->tint.b = 128;


				cnt++;
				}
			}
		}
}







void adjust_teamsel_cursor()
{
WORD dest;
WORD xpos, ypos;
WORD ybot;
WORD cnt, ylines;
WORD ts_team_num_index;
WORD clicks;
	ts_ydir = (ts_cy - (ts_windy+200) )/2;
	ts_windy += ts_ydir;




	ylines = ts_visible_spr_ycnt-1;
	if ( ts_flag_windy_cnt < 7 )
		ybot=0;
	else
		ybot = ts_flag_windy_size - (ylines*FLAG_Y_SIZE);

	if ( ybot < 0 ) ybot=0; //** added19

	if ( ts_windy < 0 )
		{
		ts_windy=0;
		}
	else
		{
		if ( ts_windy > ybot )
			{
			ts_windy= ybot;
			}
		}


	
	if ( ts_old_windy != ts_windy )
		{
		ts_old_windy = ts_windy;
		sfxSliderNoise( 1 );
		}
	else
		sfxSliderNoise( 0 );



//*** Set cursor position ***


	if ( ts_scroll_window == 0 )
		{
		if ( cjoy->db & PAD_X )
			{
				add_team_to_ts_list( ts_team_num );

//			if ( ts_total_teams > ts_teams_to_choose )
//				remove_team_from_ts_list( ts_last_team_edited );

 // 			ts_last_team_edited = ts_team_num;

			}


	/**** Scroll cursor on flag select ****/
		if ( ( cjoy->r & PAD_DN )  )
			{

			ts_team_num = (((ts_cy+FLAG_Y_SIZE)/FLAG_Y_SIZE) * FLAG_WINDX_CNT) + ( ts_cx/FLAG_X_SIZE);
			if ( ts_team_num >= share->teams_selection_size  )
				{
				goto_shell_buttons();
				return;
				}


			if ( ts_cy < (ts_flag_windy_size-FLAG_Y_SIZE)) 
				{
				if ( ts_cy == ts_dest_cy )
					{
					ts_dest_cy += FLAG_Y_SIZE;
					}
				}
			else
				{
				goto_shell_buttons();
				}
			}


		if ( cjoy->r & PAD_UP )
			{
			if ( ts_cy == ts_dest_cy )
				{


				if ( ts_dest_cy != 0 )
					ts_dest_cy -= FLAG_Y_SIZE;
				else
					ts_dest_cy = 0;
				}
			}




		if ( cjoy->c & PAD_RT )
			{
			if ( (ts_cx == ts_dest_cx)  )
				{
				ts_team_num = ((ts_cy/FLAG_Y_SIZE) * FLAG_WINDX_CNT) + ( (ts_cx+FLAG_X_SIZE)/FLAG_X_SIZE);
				if ( ts_team_num >= share->teams_selection_size  )
					{
					if ( ts_total_teams >= 12 )
						{
						ts_scroll_window = 1; // onto scroll select page
						return;
						}
					else
						{
						goto_shell_buttons();
						return;
						}
					}

				if ( ts_cx ==  (FLAG_WINDX_SIZE-FLAG_X_SIZE) )
					{
					if ( ts_total_teams >= 12 )
						ts_scroll_window = 1; // onto scroll select page
					}
				else
					ts_dest_cx += FLAG_X_SIZE;

				}
			}

		if ( cjoy->c & PAD_LT )
			{
			if ( ts_cx == ts_dest_cx )
				{
				if ( ts_dest_cx != 0 )
					ts_dest_cx -= FLAG_X_SIZE;
				else
					ts_dest_cx = 0;
				}
			}
		}		  
	else
		{
		//*** Control team names in right scoll window ****

		if ( cjoy->c & PAD_UP )
			{
			if ( ts_scroll_dy == 0 )
				ts_scroll_dy = -8;
			if ( ts_scroll_dy > -24 ) 
				ts_scroll_dy -= 2; 
			}
		else
			{
			if ( cjoy->c & PAD_DN )
				{
				if ( ts_scroll_dy == 0 )
					ts_scroll_dy = 8;

				if ( ts_scroll_dy < 24 ) 
				ts_scroll_dy += 2;
				}
			else
				ts_scroll_dy = 0;
			}

		if ( cjoy->c & PAD_LT )
			{
			ts_scroll_window = NO; // Back to flag selec page
			}


		ts_scroll_y += ts_scroll_dy;

		//** Set yscroll top limit **
		if ( ts_scroll_y < 0 ) ts_scroll_y = 0;


		//** Set yscroll bottom limit **
		ybot = (ts_total_teams-11) * TS_SCROLL_YS;
		if ( ybot < 0 ) 
			ybot = 0;

		if ( ts_scroll_y >= ybot )
			ts_scroll_y = ybot;



		
		}




//	clicks=0;

	if ( ts_cy < ts_dest_cy )
		{
		ts_cy += FLAG_Y_SIZE/4; //22; moddedx
//		clicks=1;
		}

	if ( ts_cy > ts_dest_cy )
		{

		ts_cy -= FLAG_Y_SIZE/4; //22; moddedx
//		clicks=1;
		}
	
//	sfxSliderNoise( ts_scroll_window );




/*** Set cursor X pos ***/
	if ( ts_cx < ts_dest_cx )
		{
		ts_cx += 20;
		if ( ts_cx > ts_dest_cx ) ts_cx = ts_dest_cx;
		}
		

	if ( ts_cx > ts_dest_cx )
		{
		ts_cx -= 20;
		if ( ts_cx < ts_dest_cx ) ts_cx = ts_dest_cx;
		}






	border_flag->x.w.hi = ts_cx + FLAGS_X - 4;
	border_flag->y.w.hi = ts_cy - ts_windy + FLAGS_Y - 4;
	border_flag->tint.r = border_flag->tint.g = border_flag->tint.b = ((timer/8)&1)?10:160;
//	border_flag->display = 1-ts_scroll_window;

	if ( ts_scroll_window == YES )
		{
		border_flag->display = NO;
		}
	else
		{
		border_flag->display = YES;
		}



	ts_arrow_up->display = 
	ts_arrow_dn->display = 0;

	if ( (ts_scroll_window == NO) )
		{
		if ( ts_total_teams > 11 )
			{
			ts_arrow_up->display = YES;
			ts_arrow_dn->display = YES;
			}
		}
	else

		{

		if ((timer/8)&1)
			{
			ts_arrow_up->display = 
			ts_arrow_dn->display = YES;
			}
		}
		


//	ypos = ts_cy - ts_windy + FLAGS_Y;
//	dest = (border_flag->y.w.hi - ypos)  / 1;
//	border_flag->y.w.hi = FLAGS_Y + ( ts_cy * FLAG_Y_SIZE );


	//FntPrint("windy=%d\n",ts_windy);

}





BYTE *get_team_name( WORD team )
{
//*** Pass team num 0-80+
//** returns ptr to team name
BYTE *txt;

	//FntPrint("get teamnum=%d\n",team);

//	team = team_index_lists[ config->language ][ team ];

	if ( team >= MAX_NON_USER_TEAMS )
		{
		// team name is user-defined...
		txt = config->team_detail[team-MAX_NON_USER_TEAMS].name;
		//FntPrint("txt=%s\n",txt);
		return ( txt );
		}
	else
		{
		//team name is pre-defined
		txt = team_names[config->language][team];
		//FntPrint("txt=%s\n",txt);
		return txt;
		}
}


void calc_ts_team_num()
{
	ts_team_num = ((ts_cy/FLAG_Y_SIZE) * FLAG_WINDX_CNT) + ( ts_cx/FLAG_X_SIZE);
	ts_team_num = *(ts_source_team_list + ts_team_num);
}

void display_ts_pick_list()
{
Textman_struct *ts;
WORD team_num, cnt;
BYTE *team_name_ptr;
BYTE *type;
WORD tint,y;
WORD humcpu;

	//FntPrint("teamnum=%d\n",ts_team_num );

	y=0;
	for ( cnt=0; cnt<share->teams_selection_size; cnt++ )
		{			  
		team_num = ts_source_team_list[ cnt ]; //share->selected_teams_list[ cnt ];
		humcpu = ts_teams_index_list[ team_num ];
			  
		if ( humcpu != 0  )
			{
			team_name_ptr = get_team_name( team_num );

			/*** Print 'teamname' ***/


			sprintf( text_buffer, "%s (%s)",  team_name_ptr, humcpu_txt[config->language][ humcpu-1 ] );
			ts = add_text_string ( tslot_poses[MAIN_TSLOT]+y , text_buffer, TINY_FONT, milford_font_logic, 270*1, -(76*4)+(y*TS_SCROLL_YS)-ts_scroll_y, FONT_XC, -2, 0, 0,0 );

			if ( (ts_team_num == team_num ) && ( ts_scroll_window == NO ) && ( shell->master_option_active == NO) )
				ts->flash_text = YES;
			else
				ts->flash_text = NO;

			ts->depth = MIDDLE_DEPTH-6;  
			y++;
			}
		}




/*** Show current team selected for Flag ***/
	calc_ts_team_num();




//	team_num = team_index_lists[ config->language ][ ts_team_num ];
	
	ts_team_name_ptr = get_team_name( ts_team_num );

//	ts_team_num = team_index_lists[ config->language ][ ts_team_num ];

	add_text_string ( tslot_poses[MAIN_TSLOT]+48 , ts_team_name_ptr , MILFORD_FONT, milford_font_logic, -228*1, -(89*4), FONT_XC, -2, 0, 0,0 );


//*** Point text window on right to current team being hi-lighted on left ***
	if ( ts_scroll_window == NO )
		{

		cnt=0;
//		FntPrint("team=%d\n",ts_team_num);
		for( y=0; y<TOTAL_FLAGS; y++ )
			{
			if ( ts_teams_index_list[y] )
				{
				if ( y == ts_team_num )
					{

//					FntPrint("cnt=%d y=%d\n",cnt,y );

					if ( (ts_total_teams > 12) && (cnt>6) )
						{
						ts_scroll_y = (cnt-6) * TS_SCROLL_YS;
						break;
						}
					else
						ts_scroll_y = 0;
					}
				cnt++;
				}
			} 
		}


}




Rdb_gadget snt_rdb[3];



WORD snt_yp = -45*4;
WORD snt_ys = 28*4;
WORD snt_team_cnt;

void init_select_num_teams_bootup()
{
	snt_team_cnt=0;
}

void init_select_num_teams_once()
{	
	share->game_difficulty = 0;
}


void init_select_num_teams()
{
WORD cnt, val;

	if ( share->game_type != TERRITORY )
		init_ts_teams_list();	// erase all selected teams in TEAMSELECT


	set_shell_bottom_buttons( snt_shell_buttons,0 );	

	for( cnt=0; cnt<3; cnt++ )
		{
//		if ( (share->game_type == TERRITORY) || (share->game_type == EURO2000) || (share->game_type == EUROFINAL) || (share->game_type == WORLDCUP)  || (share->game_type == FRIENDLY)  )
//			{
//			if ( cnt == 1 )
//				val = 0;
//			else
//				val = -2;
//			}
//		else
//			val = cnt;

		set_rdb_gadget( 0, &snt_rdb[cnt], -250, snt_yp + ( snt_ys*cnt)+8, 250*2,58, cnt );
		}


//	if ( shell->called_before == NO )
//		{
		set_title_lolly( select_num_teams_lolly_text[config->language] , game_types_text[config->language][share->game_type]  );

		share->total_human_teams = 1;

		switch (share->game_type)
			{
			case FRIENDLY:
				share->teams_in_competition = 2;
			break;

			case LEAGUE:
				share->teams_in_competition = 8;
			break;

			case KNOCKOUT:
				share->teams_in_competition = 8;
				snt_team_cnt = 1;	// index into snt_knockout_list[1]
			break;

			case EURO2000:
				share->teams_in_competition = TOTAL_EURO2000_FLAGS;
			break;

			case EUROFINAL:
				share->teams_in_competition = 16; //TOTAL_EURO2000_FLAGS;
			break;

			case WORLDCUP:
				share->teams_in_competition = 32;
			break;
			}


//		}



}

void update_select_num_teams()
{
WORD snt_knockout_list[]={ 4,8,16 };
WORD cnt;
Textman_struct *ts;
WORD min,max;

	min=0;
	max=8;

	if ( (share->game_type == TERRITORY) || (share->game_type == EURO2000) || ( share->game_type == EUROFINAL) || ( share->game_type == WORLDCUP) || ( share->game_type == FRIENDLY) )
		{
		modify_current_item ( 1 );
		if ( shell->current_item == 0 )
			shell->current_item = 1;
		}
	else			
		modify_current_item ( 3 );

	if ( shell->current_item == 0 )
		{
		switch( share->game_type )
			{
			case LEAGUE:
				min=2;
				max=8;
			break;

			case KNOCKOUT:
				min = 0;
				max = 2;
			break;

			case TERRITORY:
				min = 1;
				max = share->teams_in_competition;
			break;

			case EURO2000:
			case EUROFINAL:
			case WORLDCUP:
				min = 1;
				max = 8;
			break;
			}

		if ( share->game_type == KNOCKOUT )
			{
			modify_word( &snt_team_cnt , min,max );
			share->teams_in_competition = snt_knockout_list[ snt_team_cnt ];
			}
		else
			{
			modify_byte( &share->teams_in_competition , min,max );
			}


		}

	if ( shell->current_item == 1 )
		{
		min = 0;
		
		/** Ensure player cant select two human teams in two pads aren't connected **/
		if ( (share->total_human_teams > 1) && (pads_connected < 2) )
			max = 1;
		else
			{
			max = share->teams_in_competition;
			if ( max > 8 )
				max=8;	/** No more than 8 human teams **/
			}

		modify_byte( &share->total_human_teams , min,max );
		}

	if ( shell->current_item == 2 )
		modify_byte( &share->game_difficulty , 0,2 );


	if ( share->total_human_teams > share->teams_in_competition )
		share->total_human_teams = share->teams_in_competition;




/** Print total teams x ***/
	cnt=0;
	sprintf( text_buffer, "%s : %d",snt_totteams_txt[config->language],share->teams_in_competition );
	ts=add_text_string ( tslot_poses[MAIN_TSLOT]+cnt , text_buffer, MILFORD_FONT, milford_font_logic, 0, snt_yp+(cnt*snt_ys)-20, FONT_XC, 0, 0, 32,0 );
	if ( (share->game_type == TERRITORY) || (share->game_type == EURO2000) || (share->game_type == EUROFINAL) || (share->game_type == WORLDCUP) || (share->game_type == FRIENDLY )   )
		{
		ts->text_color.r = ts->text_color.g = ts->text_color.b = 32;
		}

/** Print human teams x ***/
	cnt=1;
	sprintf( text_buffer, "%s : %d",snt_humteams_txt[config->language],share->total_human_teams );
	add_text_string ( tslot_poses[MAIN_TSLOT]+cnt , text_buffer, MILFORD_FONT, milford_font_logic, 0, snt_yp+(cnt*snt_ys)-20, FONT_XC, 1, 0, 0,0 );

/** Print difficulty ***/
	cnt=2;
	sprintf( text_buffer, "%s : %s",snt_diff_txt2[config->language], snt_diff_txt1[config->language][share->game_difficulty] );
	ts=add_text_string ( tslot_poses[MAIN_TSLOT]+cnt ,text_buffer, MILFORD_FONT, milford_font_logic, 0, snt_yp+(cnt*snt_ys)-20, FONT_XC, 2, 0, 0,0 );
	if ( (share->game_type == TERRITORY) || (share->game_type == EURO2000) || (share->game_type == EUROFINAL) || (share->game_type == WORLDCUP) || (share->game_type == FRIENDLY )    )
		{
		ts->text_color.r = ts->text_color.g = ts->text_color.b = 32;
		}



}

void kill_select_num_teams()
{
WORD cnt;


	for( cnt=0;cnt<3;cnt++)
		kill_rdb_gadget( snt_rdb[cnt].gadget );



}






void init_ts_teams_list()
{
WORD cnt;
	for ( cnt=0; cnt<MAX_TEAMS_IN_COMPETITION+1; cnt++ )
		{
		share->selected_teams_list[cnt] = -1;
		}

	ts_total_teams=0;			
	ts_total_human_teams=0;

	calc_teams_to_generate();


	for( cnt=0; cnt< TOTAL_FLAGS; cnt++ )
		{
		ts_teams_index_list[cnt] = 0;
		}

}


void add_team_to_ts_list( WORD team_num )
{
WORD cnt;
WORD mode;
WORD team_exists;


	calc_teams_to_generate();	/* Dont enable this team if all required teams have been enabled */

	

/** calc sets human_teams_to_generate, cpu_teams_to_generate **/


	team_exists = check_ts_team_exists( team_num );

	mode=0;

	switch( ts_teams_index_list[ team_num ] )
		{
		case 0:
			if ( human_teams_to_generate )
				{
				mode = 1;
				}
			else
				{
				if ( cpu_teams_to_generate )
					{
					mode = 2;
					}
				}
		break;

		case 1:
			if ( cpu_teams_to_generate )
				{
				mode = 2;
				}
			else
				{
				if ( human_teams_to_generate )
					mode = 1;
				}
		break;

		case 2:
			mode=0;
		break;
		}

	ts_teams_index_list[ team_num ] = mode;
	


	ts_total_human_teams=0;
	ts_total_teams=0;
	for( cnt=0; cnt< TOTAL_FLAGS; cnt++ )
		{
		if ( ts_teams_index_list[ cnt ] == 1 )
			ts_total_human_teams++;

		if ( ts_teams_index_list[ cnt ] != 0 )
			ts_total_teams++;

		}
}




WORD check_ts_team_exists( WORD team )
{
WORD cnt;

	if ( ts_teams_index_list[ team ] == 0 )
		return NO;
	else
		return YES;

}


void create_cpu_teams_index_list()
{
WORD cnt,team,y;


/** Make selected_teams_list **/
	y=0;
	for( cnt=0; cnt< TOTAL_FLAGS; cnt++ )
		{
		team = ts_source_team_list[cnt];

		if ( ts_teams_index_list[cnt] )
			{
			share->selected_teams_list[y] = cnt;
			y++;
			}
		}


/*** Create cpu teams index list ****/
	for( cnt=0; cnt<TOTAL_FLAGS; cnt++ )
		{
		share->cpu_teams_index_list[ cnt ] = YES;
		}

	for ( cnt=0; cnt< TOTAL_FLAGS; cnt++ )
		{
		if ( ts_teams_index_list[ cnt ] == 1 )
			{
//			team = ts_source_team_list[ cnt ];
			share->cpu_teams_index_list[ cnt ] = NO;
			}
		}

        
}






void set_group_teams()
{
WORD x, group, rand_team;

	/** For friendly, set 1st two teams to play ***/


	switch( share->game_type )
		{
		case FRIENDLY:
			share->team1 = share->selected_teams_list[0];
			share->team2 = share->selected_teams_list[1];
		break;
#if 0

		case LEAGUE:
			share->team1 = share->selected_teams_list[0];
			share->team2 = share->selected_teams_list[1];
			share->team1_cpu = share->cpu_teams_index_list[ share->team1 ];
			share->team2_cpu = share->cpu_teams_index_list[ share->team2 ];

			for( x=0; x< TOTAL_LEAGUE_TEAMS;x++ )
				share->group_teams[ 0 ][x] = euro2000_teams[ group ][x];

		break;
#endif
		case EURO2000:
			for( group=0; group<share->total_groups; group++ )
				{
				for( x=0; x< TOTAL_LEAGUE_TEAMS;x++ )
					share->group_teams[ group ][x] = euro2000_teams[ group ][x];
				}
		break;

		case TERRITORY:
			for( group=0; group<share->total_groups; group++ )
				for( x=0; x<6; x++ )
					share->group_teams[ group ][x] = territory_teams_list[ group ][x];
		break;

		case EUROFINAL:
		case WORLDCUP:
			/** Group teams are set in init_eurofinal_draw **/
		break;

		case LEAGUE:
		default:
			for( x=0; x< TOTAL_LEAGUE_TEAMS;x++ )
				share->group_teams[ 0 ][x] = share->selected_teams_list[x];
		break;
		}
}



void team_select()
{

	if ( share->game_type == FRIENDLY )
		{
		ts_source_team_list = &ts_all_teams_list[ config->language ][0];
		share->teams_selection_size = TOTAL_FLAGS;
		init_ts_teams_list();	/* erase all selected teams */
		ts_teams_to_choose = share->teams_in_competition;
		}

	if ( share->game_type == LEAGUE )
		{
		ts_source_team_list = &ts_all_teams_list[ config->language ][0];
		share->teams_selection_size = TOTAL_FLAGS;	/* can choose ANY team to play */
		ts_teams_to_choose = share->teams_in_competition;
		}

	if ( share->game_type == KNOCKOUT )
		{
		ts_source_team_list = &ts_all_teams_list[ config->language ][0];
		share->teams_selection_size = TOTAL_FLAGS;	// can choose ANY team to play
		ts_teams_to_choose = share->teams_in_competition;
		}

	if ( (share->game_type == EURO2000)  )
		{
		ts_source_team_list = &ts_euro2000_teams_list[ config->language ][0];
		share->teams_selection_size = TOTAL_EURO2000_FLAGS;	// 
		ts_teams_to_choose = share->total_human_teams;
		}

	if ( (share->game_type == EUROFINAL)  )
		{
		ts_source_team_list = &ts_eurofinal_teams_list[ config->language ][0];
		share->teams_selection_size = TOTAL_EUROFINAL_FLAGS;	// 
		ts_teams_to_choose = share->total_human_teams;
		}

	if ( (share->game_type == WORLDCUP)  )
		{
		ts_source_team_list = &worldcup_group_teams[0][0];
		share->teams_selection_size = 32;
		ts_teams_to_choose = share->total_human_teams;
		}



	if ( share->game_type == TERRITORY )
		{
		ts_source_team_list = &territory_teams_list[ share->current_group ][ 0 ];
		share->teams_selection_size = territory_team_cnt[ share->current_group ];
		share->teams_in_competition = territory_team_cnt[ share->current_group ];
		ts_teams_to_choose = share->total_human_teams;
		init_ts_teams_list();	// erase all selected teams
		}




 
/********************
 *						  *
 * Call Team_Select *
 *						  *
 ********************/

	exec_shell( init_team_select, update_team_select, kill_team_select );
	create_cpu_teams_index_list();
	set_group_teams();	/** Set group_teams with relevant teams for each group ALSO friendly, leagues **/

}

void select_num_teams()
{
	exec_shell( init_select_num_teams, update_select_num_teams, kill_select_num_teams );
}



void calc_teams_to_generate()
{

//		if ( share->game_type == EUROFINAL )
//			{
//			human_teams_to_generate = share->total_human_teams - ts_total_teams;
//			cpu_teams_to_generate = 0;
//			}
//		else
//			{
			if ( (share->game_type == EURO2000) || (share->game_type == TERRITORY )  || (share->game_type == WORLDCUP ) )
				{
				/** For euro2000 only generate remaining HUMAN teams , others are all fixed... **/
				human_teams_to_generate = share->total_human_teams - ts_total_teams;
				cpu_teams_to_generate = 0;
				}
			else
				{
				human_teams_to_generate = share->total_human_teams - ts_total_human_teams;
				cpu_teams_to_generate = share->teams_in_competition - ts_total_teams - human_teams_to_generate;
				}
//			}
}


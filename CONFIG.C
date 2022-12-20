/****************************************************
 *																	 *
 *	 																 *
 * Configuration routines for title						 *
 *	 																 *
 *																	 *
 ****************************************************/

#include "incs.h"

#ifdef PSX
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
#include "skindraw.h"
#include "sod.h"
#include	"scene.h"
#include "anim.h"
#include "..\..\striker\frontend\mod.def"
#include "text.h"
#include "teamsel.h"
#include "hwcard.h"
#endif

void set_audio_global_vars();
WORD compare_config_store();
void auto_set_human_player_side();
WORD set_pa_pad_yp( WORD ybase, WORD box_ht, WORD num, WORD total );

UWORD training_buttons[]={  SHELL_NEXT, SHELL_QUIT,0 };
UWORD pad_assign_buttons[]={  SHELL_PREV, SHELL_NEXT, SHELL_QUIT,0 };
UWORD audio_config_buttons[]={ SHELL_OK, SHELL_CANCEL,0 };
UWORD pad_config_buttons[]={ SHELL_OK, SHELL_CANCEL,0 };
UWORD scnpos_config_buttons[]={ SHELL_OK, SHELL_CANCEL,0 };
UWORD save_config_buttons[]={ SHELL_OK, SHELL_CANCEL,0 };
UWORD conf_yn_butt[]={ SHELL_YES,SHELL_NO,0 };
UWORD conf_ok_butt[]={ SHELL_OK,0 };
WORD pc_button_poses[][3]={
	216, -52, FONT_NORMAL,
	208, 40, FONT_NORMAL,
	-528, 100, FONT_RJUST, // shoot
	-600, 16, FONT_RJUST,    // low action
	136, -244, FONT_NORMAL,
	208, -128, FONT_NORMAL, // control
	-704, -332, FONT_RJUST,
	-756, -248, FONT_RJUST,
};
WORD pc_button_layouts[][8]={
	0,1,2,3,4,5,6,7,
	5,4,3,2,1,0,7,6,
	3,1,2,4,5,6,7,0,
	7,6,5,4,3,2,1,0,
};











Slider_gadget ac_slider[4];
Rdb_gadget ac_rdb[2];
Rdb_gadget scn_pos_info_rdb;
void draw_pc_lines();
			  
WORD ac_master_volume_store, ac_music_volume_store, ac_com_volume_store, ac_sfx_volume_store, ac_sound_mode_store;

WORD ac_y=-50*4;
WORD ac_x=-240;
WORD ac_ys=25;
WORD old_sfx_volume;

void init_audio_config()
{
WORD cnt,x,y,ys;

	share->game_type = CONFIG;
	set_title_lolly( audio_config_lolly_text[config->language], game_types_text[config->language][share->game_type]   );

	set_shell_bottom_buttons( audio_config_buttons,-1 );

	cnt=0;

	x = ac_x;
	y = ac_y;
	ys = ac_ys;

	set_rdb_gadget( 0, &ac_rdb[0], -440, -236, 920, 400, cnt );
	set_rdb_gadget( 0, &ac_rdb[1], -1, -1, -1, -1, 1 ); 

	set_slider_gadget( x+(65*4), y+(ys*4*cnt), &ac_slider[cnt], 0, 256, &config->master_volume, cnt );
	ac_slider[cnt].base_frame=39+(6*cnt);

	cnt++;
	set_slider_gadget( x+(65*4), y+(ys*4*cnt), &ac_slider[cnt], 0, 256, &config->music_volume, cnt );
	ac_slider[cnt].base_frame=39+(6*cnt);

	cnt++;
	set_slider_gadget( x+(65*4), y+(ys*4*cnt), &ac_slider[cnt], 0, 256, &config->com_volume, cnt );
	ac_slider[cnt].base_frame=39+(6*cnt);

	cnt++;
	set_slider_gadget( x+(65*4), y+(ys*4*cnt), &ac_slider[cnt], 0, 256, &config->sfx_volume, cnt );
	ac_slider[cnt].base_frame=39+(6*cnt);



	ac_master_volume_store = config->master_volume;
	ac_music_volume_store = config->music_volume;
	ac_com_volume_store = config->com_volume;
	ac_sfx_volume_store = config->sfx_volume;
	ac_sound_mode_store = config->sound_mode;

	old_sfx_volume = config->sfx_volume;

}


WORD pad_rel;

void update_audio_config()
{
WORD cnt,y;
WORD w,h;
WORD oldy2;
Textman_struct *ts;
	modify_current_item( 5 );


	for(cnt=0; cnt<4;cnt++)
		add_text_string ( tslot_poses[MAIN_TSLOT]+cnt, ac_tgad_text[config->language][cnt] , MILFORD_FONT, milford_font_logic, -540, ac_y+(ac_ys*4*cnt)-(9*4), FONT_RJUST, -2, 0, 0,0,0 );

/** Print sound mode **/
	ts = add_text_string ( tslot_poses[MAIN_TSLOT]+4 , audio_word2[config->language], MILFORD_FONT, milford_font_logic, -540, ac_y+(ac_ys*4*cnt)-(9*4), FONT_RJUST, -2, 0, 0, 0,0 );
	
	oldy2=ts->y1;
	set_rdb_size_from_tm( &ac_rdb[1], ts );

	ac_rdb[1].size.x = ts->x1-(6*4);
	w = ts->x2 - ts->x1;
/** Print stereo, inverse etc **/
	ts = add_text_string ( tslot_poses[MAIN_TSLOT]+5 , audio_word1[config->language][config->sound_mode], MILFORD_FONT, milford_font_logic, -4, ac_y+(ac_ys*4*cnt)-(9*4), FONT_NORMAL, 4, 0, 0, 0,0 );
	w += ts->x2 - ts->x1;

	ac_rdb[1].size.w = w + (24*4);
	ts->y1 = oldy2;

	if ( shell->current_item == 4 )
		modify_byte( &config->sound_mode, 0,2 );

	
	if ( ((config->sfx_volume) != (old_sfx_volume))  )
		{
		old_sfx_volume = config->sfx_volume;

		pad_rel = NO;

		}


	if ( ((cjoy->c & (PAD_LT|PAD_RT))==0) && ( pad_rel == NO ) )
		{
		pad_rel = YES;
		sfxSpot( BUTVOC, 255 ); //(255*config->sfx_volume)/255 );
		//FntPrint("poo......\n");
		}

	set_audio_global_vars();

}

void kill_audio_config()
{
WORD cnt;



	kill_rdb_gadget( ac_rdb[0].gadget );
	kill_rdb_gadget( ac_rdb[1].gadget );

	for(cnt=0;cnt<4;cnt++)
		{
		kill_slider_gadget( ac_slider[cnt].gadget );
		}

	if ( ( get_shell_type() == SHELL_CANCEL) )
		{
		//user pressed cancel...
		config->master_volume = ac_master_volume_store;
		config->music_volume = ac_music_volume_store;
		config->com_volume = ac_com_volume_store;
		config->sfx_volume = ac_sfx_volume_store;
		config->sound_mode = ac_sound_mode_store;
		}

	share->title_func=0;


}


void audio_config()
{
//	push(0);
//	init_shell( FRONTEND );
	exec_shell( init_audio_config, update_audio_config, kill_audio_config );
//	pop(0);
}







WORD scn_pos_mode;
Sprite *sp_spr[4];

WORD sp_x = 0;
WORD sp_y = 20*4;

WORD screen_xpos_store, screen_ypos_store;

void sp_icon_upd( Sprite *sprite )
{

	sprite->frame = sprite->temp+0;

	if ( shell->current_item == sprite->group_num )
		{
		if ( (cjoy->c & (PAD_X|PAD_SQU)) )
			{
			sprite->frame = sprite->temp+2;
			}
		else
			{
			if ( ((flash_counter/8)&1)==0)
				sprite->frame = sprite->temp+1;
			}
		}
}

void init_screen_pos()
{


	share->game_type = CONFIG;

	set_title_lolly( screen_pos_lolly_text[config->language],game_types_text[config->language][share->game_type]  );
	set_shell_bottom_buttons( scnpos_config_buttons,-1 );
	set_pad_repeat_speed( 7, 0 );
	scn_pos_mode=0;

	screen_xpos_store = config->screen_xpos;
	screen_ypos_store = config->screen_ypos;


	set_rdb_gadget( 0, &scn_pos_info_rdb, -1, -1, -1, -1 , -1 );

	sp_spr[0]=spawn_sprite( POS );
	sprite->temp = 6;
	sprite->group_num = 0;
	sprite->upd_rtn = ( void * )sp_icon_upd;
	sprite->use_frame_offs = YES;

	sp_spr[1]=spawn_sprite( POS );
	sprite->temp = 9;
	sprite->group_num = 1;
	sprite->upd_rtn = ( void * )sp_icon_upd;
	sprite->use_frame_offs = YES;

	sp_spr[2]=spawn_sprite( POS );
	sprite->temp = 0;
	sprite->group_num = 2;
	sprite->upd_rtn = ( void * )sp_icon_upd;
	sprite->use_frame_offs = YES;

	sp_spr[3]=spawn_sprite( POS );
	sprite->temp = 3;
	sprite->group_num = 3;
	sprite->upd_rtn = ( void * )sp_icon_upd;
	sprite->use_frame_offs = YES;



}


void update_screen_pos()
{
WORD cnt,col,display;
Textman_struct *ts;
Spr_anim_frame_san *fr;
WORD wd,ht,y2;

	fr = get_sprite_frame_addr( ICON );
	fr += 30;

	wd = fr->real_w*1;
	ht = fr->real_h*2;

	ts=add_text_string ( tslot_poses[MAIN_TSLOT]+0 , sp_info_text[ config->language ], MILFORD_FONT, milford_font_logic, 0, -(16*4), FONT_XC+FONT_YC, -1, 0, 0, 0,0 );
	
	y2= ts->y2-8;

	sp_spr[0]->x.w.hi = -wd;
	sp_spr[1]->x.w.hi = -wd;
	sp_spr[2]->x.w.hi = ts->x1 - 80 - wd;
	sp_spr[3]->x.w.hi = ts->x2 + 60 - wd;

	sp_spr[0]->y.w.hi = ts->y1 - 100 - ht;
	sp_spr[1]->y.w.hi = y2 + 80 - ht;
	sp_spr[2]->y.w.hi = -18*4; 
	sp_spr[3]->y.w.hi = -18*4; 

	set_rdb_box( &scn_pos_info_rdb , ts->x1 - 120, ts->y1 - 160, (ts->x2-ts->x1)+240,(y2-ts->y1)+320 );


	if ( shell->master_option_active )
		{
		modify_current_item( 1 );
		if ( shell->master_option_active == NO )
			shell->current_item = 1;
		}
	else
		{
		if ( cjoy->db & PAD_UP )
			shell->current_item=0;

		if ( cjoy->db & PAD_DN )
			{
			if ( shell->current_item == 1 )
				goto_shell_buttons(-1);
			else
				shell->current_item=1;
			}

		if ( cjoy->db & PAD_LT )
			shell->current_item=2;

		if ( cjoy->db & PAD_RT )
			shell->current_item=3;


		if ( cjoy->r & (PAD_X|PAD_SQU) )
			{
			if ( shell->current_item == 0 )
				{
				if ( config->screen_ypos > 1 )
					config->screen_ypos--;
				else
					config->screen_ypos=1;
				}

			if ( shell->current_item == 1 )
				{
				if ( config->screen_ypos < 30 )
					config->screen_ypos++;
				else
					config->screen_ypos=30;
				}

			if ( shell->current_item == 2 )
				{
				if ( config->screen_xpos > -20 )
					config->screen_xpos--;
				else
					config->screen_xpos = -20;
				}

			if ( shell->current_item == 3 )
				{
				if ( config->screen_xpos < 20 )
					config->screen_xpos++;
				else
					config->screen_xpos=20;
				}
			}	
		}



}

void kill_screen_pos()
{
WORD cnt;

	restore_repeat_speed();	//**** Back to normal speed ****
//	kill_rdb_gadget( scn_pos_rdb.gadget );
	kill_rdb_gadget( scn_pos_info_rdb.gadget );

	for(cnt=0;cnt<4;cnt++)
		kill_sprite( sp_spr[cnt] );

	if ( ( get_shell_type() == SHELL_CANCEL) )
		{
		config->screen_xpos = screen_xpos_store;
		config->screen_ypos = screen_ypos_store;
		}

	share->title_func=0;

}


void screen_positioner()
{
	push(0);
//	init_shell( FRONTEND );
	exec_shell( init_screen_pos, update_screen_pos, kill_screen_pos );
	pop(0);
}





/***************************************************
 *
 * Pad configuration
 *
 ***************************************************/


Rdb_gadget pc_rdb;
WORD pc_x, pc_y;

void init_pad_config_once()
{
WORD cnt;
WORD just;

	for(cnt=0;cnt<8;cnt++)
		config->pad_config[cnt]=0; // all pad configs default to ZERO
}

BYTE pc_val;

void init_pad_config()
{
WORD cnt,x,y,ys;

	share->game_type = CONFIG;

	set_title_lolly( pad_config_lolly_txt[config->language],game_types_text[config->language][share->game_type]  );
	set_shell_bottom_buttons( pad_config_buttons,-1 );

	cnt=0;

	pc_x = 0*2;
	pc_y = 45*4;

	set_rdb_gadget( 0, &pc_rdb, pc_x-(380/2),pc_y+16,380, 16*4, 0 );

	joy_curr = joy_curr_output;	//** Lock pad to last pad read

	pc_val = config->pad_config[ joy_curr ];

	for(cnt=0;cnt<12;cnt++)
		{
		spawn_sprite( PAD );
		sprite->use_frame_offs = YES;
		sprite->frame = cnt;
		sprite->depth = MIDDLE_DEPTH+1;
		sprite->group_num = 16384;
		sprite->x.w.hi = -190;
		sprite->y.w.hi = -100*2;
		}

#if 0
	for(y=0;y<8;y++)
		{
		spawn_sprite( PCONLIN );
		sprite->frame = y;
		sprite->use_frame_offs = YES;
		sprite->x.w.hi = -512;
		sprite->y.w.hi = -512;
		sprite->transp_rate = 2;
		sprite->depth = MIDDLE_DEPTH;
		sprite->tint.r = sprite->tint.g = sprite->tint.b = 32;
		sprite->group_num = 16384; 
		}
#endif

	

}

void update_pad_config()
{

WORD cnt;
Textman_struct *ts;
WORD num;
WORD text_group, text_num;
BYTE *text;

	modify_current_item( 1 );

	cnt = 0;
	

	num = pc_val+1; // goes 1,2,3,4

	//FntPrint("num=%d\n",num);
	sprintf( text_buffer, "%s %d", pad_config_words[config->language], num );
	ts = add_text_string ( tslot_poses[MAIN_TSLOT]+0 , text_buffer, MILFORD_FONT, milford_font_logic, pc_x, pc_y-12, FONT_XC, 0, 0, 0, 0,0 );


	if ( shell->current_item == 0 )
		modify_byte( &pc_val, 0,3 );


//*** Display pad button details ***
	
	for( cnt=0;cnt<8;cnt++)
		{		
		text_group = pc_val;

		text_num = pc_button_layouts[ text_group ][cnt];

		text = pc_button_text[ config->language ][ text_num ];

		ts=add_text_string ( tslot_poses[MAIN_TSLOT]+1+cnt , text, TINY_FONT, milford_font_logic, pc_button_poses[cnt][0], pc_button_poses[cnt][1], pc_button_poses[cnt][2], -1, 0, 0, 0,0 );
//		ts->max_brightness = 90;
		}


	draw_pc_lines();

}



void kill_pad_config()
{
	if ( shell->current_button == SHELL_OK )
		config->pad_config[ joy_curr ] = pc_val;

	joy_curr = -1; // read current pad back on
	kill_rdb_gadget( pc_rdb.gadget );
	kill_sprite_group( 16384 );
	share->title_func=0;
}


void pad_config()
{
	push(0);
	exec_shell( init_pad_config, update_pad_config, kill_pad_config );
	pop(0);
}


void draw_pc_lines()
{
WORD cnt;
WORD l[][4]={ 

  -192,  -280,  -116,  -200,		// push
  -240,  -200,  -172,  -172, 		// l1
	124,  -188,    104,  -148,		// r2
	-12,   136,    32,    80,		// X
	 76,   -20,   204,   -16,		// change player
	108,    56,   200,    76,		// pass
	116,   -88,   196,   -92,		// control
	-80,    52,     4,    24,		// lo action

};
	init_lines();

	for( cnt=0;cnt<8;cnt++)
		draw_line( l[cnt][0], l[cnt][1], l[cnt][2], l[cnt][3], 255,255,255 ); 
}




















/***************************************************
 *
 * Pad assign
 *
 ***************************************************/

void update_pad_assign();

Rdb_gadget pa_rdb[4];
void update_pa_pad_poses();
Sprite *pa_sprs[8];
Sprite *pa_orient_spr[8];
Sprite *pa_button_spr[8];

CVECTOR pa_tints[]={
	
	130,40,40,0,	// red
	32,130,32,0,  	// green	
	130,40,130,0,	// pur
	40,40,140,0,	// blue

	128,128,32,0, 	// yellow
	130,90,30,0,	// orange
	40,140,160,0,	// cyan
	160,160,160,0,	// white


};

void init_pad_assign_once()
{
WORD cnt;

	share->team1 = 0;
	share->team2 = 0;

	for( cnt=0; cnt<8; cnt++ )
		share->human_player_orient[cnt]=0;
}

Sprite *pc_lt_icon, *pc_rt_icon;

void init_pad_assign()
{
WORD cnt,x,y,ys;
Gadget *gad;

//	share->team1 = 56;
//	share->team2 = 16;

	if ( (share->game_type == FRIENDLY) && (shell->called_before==NO) )
		{
		/*** Both teams to play are HUMAN by default ***/
//		share->team1_cpu = NO;
//		share->team2_cpu = NO;
		}

	if ( shell->called_before == NO )
		{
		set_title_lolly( pad_assign_lolly_txt[config->language],game_types_text[config->language][share->game_type]  );
		for( y=0; y<8; y++ )
			share->human_player_side[y] = 0;
		}

	set_shell_bottom_buttons( pad_assign_buttons,-1 );

	cnt=0;

	//*** Top box ***
	set_rdb_gadget( 0, &pa_rdb[0], -460,-344, 460*2, 68, -1 );

	//** Middle box **
	set_rdb_gadget( 0, &pa_rdb[1], -112,-256, 112*2, 532, -1 );

	//** Left box **
	gad=set_rdb_gadget( 0, &pa_rdb[2], -460,-256, 340, 532, -1 );
	if ( share->team1_cpu == YES )
		gad->rdb_gadget->flash_tint_default = 32;

	//** Right box **
	gad=set_rdb_gadget( 0, &pa_rdb[3], 120,-256, 340, 532, -1 );
	if ( share->team2_cpu == YES )
		gad->rdb_gadget->flash_tint_default = 32;



	for(y=0;y<8;y++)
		{
		pa_sprs[y] = spawn_sprite( TEAMPAD );
		sprite->depth = NEAREST_DEPTH;
		sprite->tint = pa_tints[y];
		sprite->dest_tint = pa_tints[y];
		sprite->group_num = 10;

		pa_orient_spr[y] = spawn_sprite( TEAMPAD );
		sprite->depth = NEAREST_DEPTH-1;
		sprite->group_num = 10;
		sprite->frame = 5;
		sprite->display = NO;

	 	pa_button_spr[y] = spawn_sprite( TEAMPAD );
		sprite->depth = NEAREST_DEPTH-1;
		sprite->group_num = 10;
		sprite->frame = 4;
		sprite->display = NO;
		sprite->transp_rate=1;
		sprite->use_frame_offs = YES;
		}

	joy_curr = joy_curr_output;	//** Lock pad to last pad read


	pc_lt_icon = spawn_sprite( TEAMPAD );
	sprite->x.w.hi = (-208*2)+48;
	sprite->y.w.hi = -20*4;
	sprite->frame = 3;
	sprite->group_num = 10;

	pc_rt_icon = spawn_sprite( TEAMPAD );
	sprite->x.w.hi = (80*2)+48;
	sprite->y.w.hi = -20*4;
	sprite->frame = 3;
	sprite->group_num = 10;



/**** Turn off playstion if Home/away team is player controlled ****/

	pc_lt_icon->display = share->team1_cpu;
	pc_rt_icon->display = share->team2_cpu;


	update_pad_assign();

	for(y=0;y<30;y++)
		update_pa_pad_poses();


	auto_set_human_player_side();

}

BYTE *pa_vs_txt="vs";

void update_pad_assign()
{
BYTE *team_txt;
WORD cnt;
Textman_struct *ts;

	modify_current_item( 1 );



//*** Print left team name ***
	team_txt = get_team_name( share->team1 );
	ts=add_text_string ( tslot_poses[MAIN_TSLOT]+0 , team_txt, MILFORD_FONT, milford_font_logic, -250, -364, FONT_XC, -1, 0, 10, 0,0 );
	set_ts_title_color( ts );

//*** Print right team name ***
	team_txt = get_team_name( share->team2 );
	ts=add_text_string ( tslot_poses[MAIN_TSLOT]+1 , team_txt, MILFORD_FONT, milford_font_logic, 250, -364, FONT_XC, -1, 0, 10, 0,4 );
	set_ts_title_color( ts );


//*** Print 'vs' in middle ***
	ts=add_text_string ( tslot_poses[MAIN_TSLOT]+2 , pa_vs_txt, MILFORD_FONT, milford_font_logic, 0, -364, FONT_XC, -1, 0, 10, 0,2 );
	set_ts_title_color( ts );


	update_pa_pad_poses();


/** Cant proceed from this screen if a side has not been picked **/
	shell->disable_button[ SHELL_NEXT ] = YES;
	for(cnt=0;cnt<8;cnt++)
		{
		if ( share->human_player_side[cnt] != 0 )
			{
			shell->disable_button[ SHELL_NEXT ] = NO;
			}
		}


}



void kill_pad_assign()
{
WORD cnt;
WORD lt_side_human, rt_side_human;

	joy_curr = -1; // read current pad back on

	for(cnt=0;cnt<4;cnt++)
		kill_rdb_gadget( pa_rdb[cnt].gadget );

	kill_sprite_group( 10 );


//	share->title_func=0;


/****************************************************
 *																	 *
 * Check for absence of human players on both teams *
 *																	 *
 ****************************************************/

 	if ( shell->current_button == SHELL_NEXT )
		{
		//*** Decide which team in to be cpu or human ***
 		lt_side_human = rt_side_human = NO;
		for( cnt=0; cnt<8; cnt++ )
			{
			if ( share->human_player_side[cnt] == -1 )
				lt_side_human = YES;

			if ( share->human_player_side[cnt] == 1 )
				rt_side_human = YES;
			}



		if ( lt_side_human == NO )
			{
			/** There are no humans on the left team, therefore  make it CPU */
			share->team1_cpu = YES;
			}

		if ( rt_side_human == NO )
			{
			/** There are no humans on the right team, therefore  make it CPU **/
			share->team2_cpu = YES;
			}
		}
	else
		{
		//share->team1_cpu = share->team2_cpu = NO;
		}

	shell->disable_button[ SHELL_NEXT ] = NO;

}




void pad_assign()
{
	if ( share->team1_cpu && share->team2_cpu )
		{
		comp->thread_pos++;
		}
	else
		exec_shell( init_pad_assign, update_pad_assign, kill_pad_assign );
}






void update_pa_pad_poses()
{
WORD cnt,y,xoffs,ctrl,xo;
WORD lt_limit, rt_limit, x;
WORD lefts, rights, centres;
WORD left_cnt=0;
WORD right_cnt=0;
WORD centre_cnt=0;
WORD xp , yp, diff;
WORD pad_size;

	centres = pads_connected;
	if ( pads_connected < 4 )
		{
		for ( cnt=0; cnt<8; cnt++ )
			{
			sprite = pa_sprs[cnt];

			if ( pads_connected_list[cnt] == YES )
				{
				sprite->display = YES;
				}
			else
				{
				sprite->display=NO;
				share->human_player_side[cnt]=0;
				}
			}
		}
	else
		{
		if ( pads_connected < 6 )
			{
			for ( cnt=0; cnt<8; cnt++ )
				{
				sprite = pa_sprs[cnt];

				if ( pads_connected_list[cnt] == YES )
					{
					sprite->display = YES;
					}
				else
					sprite->display=NO;
				}

			}
		else
			{
			for ( cnt=0; cnt<8; cnt++ )
				{
				sprite = pa_sprs[cnt];

				if ( pads_connected_list[cnt] == YES )
					{
					sprite->display = YES;
					}
				else
					sprite->display=NO;
				}
			}
		}


/** Count left and right sides **/
	lefts=rights=0;
	for(cnt=0;cnt<8;cnt++)
		{
		if ( pads_connected_list[cnt] )
			{
			if ( share->human_player_side[cnt] == -1 )
				lefts++;
			else
				{
				if ( share->human_player_side[cnt] == 1 )
					rights++;
				}
			}
		}

	centres -= (lefts+rights);

	//FntPrint("lefts=%d rights=%d centres=%d\n",lefts, rights, centres );



	for ( cnt=0;cnt<8;cnt++)
		{
		pa_orient_spr[ cnt ]->display = NO;
		pa_button_spr[ cnt ]->display = NO;
		if ( pads_connected_list[cnt] )
			{
			sprite = pa_sprs[cnt];


			if ( (cnt == joy_curr) )
				ctrl = 1 - shell->master_option_active;
			else
				ctrl=YES;
			
			if ( share->team1_cpu == YES )
				lt_limit = 0;
			else
				lt_limit = -1;

			if ( share->team2_cpu == YES )
				rt_limit = 0;
			else
				rt_limit = 1;

			if ( share->game_type == FRIENDLY )
				{

				lt_limit=YES;
				rt_limit=YES;
				for(x=0;x<8;x++)
					{
					if ( share->human_player_side[x]==-1)
						lt_limit = NO;
					if ( share->human_player_side[x]==1)
						rt_limit = NO;
					}

				pc_lt_icon->display=lt_limit;
				pc_rt_icon->display=rt_limit;
				share->team1_cpu = lt_limit;
				share->team2_cpu = rt_limit;

				lt_limit = -1;
				rt_limit = 1;

				}


			if ( ctrl )
				{	
				if ( joy[cnt].r & PAD_LT )
					{

					if ( share->human_player_side[cnt] > lt_limit) 
						{
						if ( (share->human_player_side[cnt]==0) && (lefts==4) )		/** Only 4 humans on each side **/
							{
							}
						else
							share->human_player_side[cnt]--;
						}
					}

				if ( joy[cnt].r & PAD_RT )
					{
					if ( share->human_player_side[cnt] != rt_limit )	
						{
						if ( (share->human_player_side[cnt] == 0) && (rights==4) )	/** Only 4 humans on each side **/
							{
							}
						else
							share->human_player_side[cnt]++;
						}
					}
				}


			sprite = pa_sprs[ cnt ];

	/** Move pad sprites to left or right **/
			if ( share->human_player_side[cnt] == 0 )
				{
				sprite->temp = 0;
				yp = set_pa_pad_yp( -256, 532, centre_cnt, centres );

				if ( centres >= 6 )
					sprite->frame = 0;
				else
					{
					if ( centres <= 3 )
						sprite->frame = 2;
					else
						sprite->frame = 1;
					}

				centre_cnt++;

				xp = -70;

				/** Its in the centre **/
				if ( centres < 4 )
					xo = -8;
				else
					if ( centres >= 6 )
						xo = 12;
					else
						xo = 0;

				}

			if ( share->human_player_side[cnt] == 1 )
				{

				if ( rights == 8 )
					sprite->frame = 0;
				else
					{
					if ( rights < 4 )
						sprite->frame = 2;
					else
						sprite->frame = 1;
					}

				yp = set_pa_pad_yp( -256, 532, right_cnt, rights );
				right_cnt++;

				xp = 212;
				/** Its on the right **/
				if ( rights < 4 )
					xo = 0;
				else
					xo = 16;
				

				}

			if ( share->human_player_side[cnt] == -1 )
				{

				if ( lefts == 8 )
					sprite->frame = 0;
				else
					{
					if ( lefts < 4 )
						sprite->frame = 2;
					else
						sprite->frame = 1;
					}

				yp = set_pa_pad_yp( -256, 532, left_cnt, lefts );
				left_cnt++;


					xp = -370;

				/** Its on the left **/
				if ( lefts < 4 )
					xo = 0;
				else
					xo = 16;
				

				}



				





	/** Adjust pad sprite yposes when they reach either side **/

			//FntPrint("xp=%d\n",xp);

			if ( yp != 32000 )
				sprite->y.w.hi += ((yp - sprite->y.w.hi ))/4;
			else
				sprite->display = 0;

			diff = xp-sprite->x.w.hi;

			if ( (xp/16) == (sprite->x.w.hi/16) )
				sprite->x.w.hi = xp;
			else
				sprite->x.w.hi += diff/4;

			diff /= 4;

			sprite->xoffs = xo;


			pad_size = sprite->frame;

		/** Pad sprite is on either side,
		    so show Orientation sprite **/

			if ( (diff == 0) && (share->human_player_side[cnt] != 0) && ( sprite->display ) )
				{
				sprite = pa_orient_spr[ cnt ];
				sprite->display = YES;
				sprite->x.w.hi = pa_sprs[cnt]->x.w.hi - (20*4);
				sprite->y.w.hi = pa_sprs[cnt]->y.w.hi - (4*4);
				sprite->frame = share->human_player_orient[cnt]?4:5;
				if ( pad_size == 1 )
					{
					sprite->frame += 2;
					/** Adjust for medium Pad **/
					}

		   /** Show Button sprite **/
				sprite = pa_button_spr[ cnt ];
				sprite->display = YES;
				sprite->x.w.hi = pa_sprs[cnt]->x.w.hi;
				sprite->y.w.hi = pa_sprs[cnt]->y.w.hi;
				sprite->xoffs = pa_sprs[cnt]->xoffs;
				sprite->frame = 8;

				sprite->display = (flash_counter/8)&1;

				if ( pad_size == 1 )
					{
					/** Adjust for medium Pad **/
					sprite->frame = 9;
					}

				}
			if ( (joy[cnt].db & (PAD_X|PAD_SQU|PAD_CIR)) && (diff==0) && ((joy_curr_output==cnt)&&(shell->master_option_active==NO)) )
				share->human_player_orient[cnt] = 1-share->human_player_orient[cnt];


			}
		}



	

//*** Flash pad belonging to controlling player ***

#if 1

	sprite = pa_sprs[ joy_curr ];
	sprite->tint = sprite->dest_tint;

	if ( (shell->current_item == 0) )
		{

		if (  ((flash_counter/8)&1)  )
			{
			sprite->tint.r = sprite->dest_tint.r/2;
			sprite->tint.g = sprite->dest_tint.g/2;
			sprite->tint.b = sprite->dest_tint.b/2;
			}
		}

/** Disable flashy button sprite on controlling pad if cursor is on shell buttons **/
	if ( shell->current_item!=0 )
		{
		sprite = pa_button_spr[ joy_curr ];
		sprite->display=NO;
		}



#endif

}


void auto_set_human_player_side()
{
WORD x, cnt;

	for(x=0;x<8;x++)
		share->human_player_side[x]=0;

	x=joy_curr_output;

	if ( (share->team1_cpu == YES) && (share->team2_cpu == NO ) )
		{
		/** Home team is cpu **/
//		for(x=0;x<8;x++)
//			{
			share->human_player_side[x] = 1; /* Humans on away team */
//			}
		return;
		}

	if ( (share->team1_cpu == NO) && (share->team2_cpu == YES ) )
		{
		/** Away team is cpu **/
//		for(x=0;x<8;x++)
//			{
			share->human_player_side[x] = -1; /* Humans on home team */
//			}
		return;
		}

//	if ( (share->team1_cpu == NO) && (share->team2_cpu == NO ) )
//		{
//		/** Neither team is cpu, so half of teams should be home, half should be away **/
//		cnt = pads_connected/2;
//		for(x=0;x<8;x++)
//			{
//			if ( cnt > 0 )
//				share->human_player_side[x] = 1; 
//			else
//				share->human_player_side[x] = -1;
//			cnt--;
//			}
//		}


}


WORD set_pa_pad_yp( WORD ybase, WORD box_ht, WORD num, WORD total )
{
WORD zonesize;
WORD zonespace;
WORD hts[]={ 0, 108, 108, 108, 96, 96, 64, 64, 64 };
WORD yp, spr_ht;

	if ( total == 0 )
		{
		return 32000;
		}

	spr_ht = hts[ total ];


	zonespace = (box_ht - (spr_ht*total))/ (total+1);



	yp = zonespace*(num+1)+(spr_ht*num);	/* yp at top of zone */
	yp += ybase;
	
	return yp;

}

















void init_config_io()
{
WORD cnt,x,y,ys;

	share->game_type = CONFIG;
	set_title_lolly( save_config_lolly_text[config->language],game_types_text[config->language][share->game_type]  );
	set_shell_bottom_buttons( save_config_buttons,-1 );

	save_game();
}


void update_config_io()
{
	modify_current_item( 1 );
}

void kill_config_io()
{

	share->title_func=0;
}


void config_io()
{
	share->game_type = CONFIG;
	save_game();

	if ( mc->game_saved )
		{
		share->config_loaded=YES;
		}

	share->game_type = FRIENDLY;
  	share->title_func=0;


}

Rdb_gadget conf_rdb;

void config_auto_load()
{
Textman_struct *ts;
WORD cnt;

	if ( share->config_loaded == NO )
		{
//		if ( compare_config_store()==NO )
//			{
			shell->mode = TITLE;
			shell->master_option_active = NO;
			shell->current_item = 0;
			shell->requester_box_active=YES;
			set_title_lolly( restore_config_lolly_text[config->language],game_types_text[config->language][share->game_type]   );

			set_rdb_gadget( 0, &conf_rdb, 0, 0,0,0,  -1 );

			if ( check_load_config() == YES )
				{
				for(cnt=0;cnt<2;cnt++)
					{
					ts=add_text_string ( tslot_poses[REQUEST_TSLOT]+0, autoload_conf_file_txt[ config->language ] , MILFORD_FONT, milford_font_logic, 0, 0, FONT_XC+FONT_YC, -2, 0, 0,0,0 );
					set_rdb_size_from_tm( &conf_rdb, ts );
					bg_loop();
					//shell_control();
					}
				do_mc_load_config();
				}

			shell->requester_box_active=NO;
			kill_rdb_gadget( conf_rdb.gadget );
//			}
		}
}



void check_for_config_save()
{

WORD diff;
WORD cnt;


	diff = compare_config_store();	/** Returns yes if config != config_store **/


	if ( diff )
		{
		for(cnt=0;cnt<20;cnt++)
			{
			update_memcard();
			wait_vblank();
			}

		if ( mc->card_present[0] || mc->card_present[1] )
			{
			do_request( conf_save_req[ config->language ], conf_yn_butt, -20*4, SHELL_YES );

			if ( shell->current_button == SHELL_YES )
				{
				config_io();
				share->config_loaded=YES;
				*config_store = *config;
				}
			else
				{
				*config_store = *config;
				share->config_loaded=YES;
				}
			}

		}

	share->title_func=0;	/* title not to call this function again **/

}



void config_restore()
{
	share->game_type = CONFIG;

	set_title_lolly( restore_config_lolly_text[config->language],game_types_text[config->language][share->game_type]   );

	//**** Screen positioner ****
	if ( do_request( restore_config_txt[ config->language], conf_yn_butt, -20*4, SHELL_NO ) == SHELL_YES )
		{
		share->init_once_code = 0;
		init_once();
		do_request( config_restored[ config->language], conf_ok_butt, -20*4, SHELL_OK );

		}
//	else
//		do_request( cancelled_txt[ config->language], conf_ok_butt, -20*4, SHELL_OK );

	share->title_func=0;
	share->config_loaded=YES;

}

WORD compare_config_store()
{
WORD diff=NO;
WORD len;
BYTE *addr1 = (BYTE *)config;
BYTE *addr2 = (BYTE *)config_store;


	len = sizeof( Config );

	while(len--)
		{
		if ( *addr1 != *addr2 )
			{
		/*** Need to do a save, first check that a memory card exists ***/

			diff=YES;
			break;
			}
		addr1++;
		addr2++;
		}

	return diff;
}




void init_training_data_once()
{
WORD cnt;
	for(cnt=0;cnt<TRAINING_MODES;cnt++)
		{
		config->training_data.points[cnt] = -1;
		comp->training_data.points[cnt] = -1;
		}
}




void update_training_screen();


WORD tr_xo,tr_x1,tr_x2,tr_yp;
Rdb_gadget tr_rdb;
void init_training_screen()
{
	share->game_type = TRAINING;
	set_title_lolly( training_lolly_text[config->language],game_types_text[config->language][share->game_type]  );
	set_shell_bottom_buttons( training_buttons,-1 );
	tr_xo=0;
	tr_x1=1024;
	tr_x2=-1024;
	tr_yp = -60*4;


	update_training_screen();

	tr_xo = (( (512+tr_x1) - (512-tr_x2) )/2) + (32*2);
	set_rdb_gadget( 0, &tr_rdb, tr_x1-28-tr_xo, -256+(8*4), tr_x2-tr_x1+(70*2)+56, 452-(15*4), 0 );


	update_training_screen();
	update_training_screen();
	update_training_screen();


}

void update_training_screen()
{
WORD y;
Textman_struct *ts;
WORD pts;
WORD x;

	for( y=0; y<TRAINING_MODES; y++ )	
		{
		ts=add_text_string ( tslot_poses[MAIN_TSLOT]+y, training_types[y][config->language], MILFORD_FONT, milford_font_logic, -512 - tr_xo, tr_yp+(y*60), FONT_RJUST, -2, 0, 0,0,0 );

		//FntPrint("x1=%d x2=%d\n",ts->x1, ts->x2);

		if ( ts )
			{
			if ( ts->x1 < tr_x1 )
				tr_x1 = ts->x1;

			}

		if ( ts )
			{
			if ( ts->x2 > tr_x2 )
				tr_x2 = ts->x2;
			}



		pts = config->training_data.points[y];
		if ( pts == -1 )
  			sprintf( text_buffer, "-- / --" );
		else
			sprintf(text_buffer, "%d / 10",pts );

		ts=add_text_string ( tslot_poses[MAIN_TSLOT]+y+TRAINING_MODES, text_buffer, MILFORD_FONT, milford_font_logic, tr_x2+32, tr_yp+(y*60), FONT_NORMAL, -2, 0, 0,0,0 );


		}




}

void kill_training_screen()
{
	kill_rdb_gadget ( tr_rdb.gadget );
}



void training_screen()
{
	exec_shell( init_training_screen, update_training_screen, kill_training_screen );
}

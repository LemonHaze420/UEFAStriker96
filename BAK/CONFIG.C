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
#include "shell.h"
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
#include "..\..\striker\frontend\mod.def"
#include "text.h"
#include "teamsel.h"
#include "hwcard.h"
#endif

void set_audio_global_vars();
void auto_set_human_player_side();

UWORD pad_assign_buttons[]={  SHELL_PREV, SHELL_NEXT, SHELL_QUIT,0 };
UWORD audio_config_buttons[]={ SHELL_OK, SHELL_CANCEL,0 };
UWORD pad_config_buttons[]={ SHELL_OK, SHELL_CANCEL,0 };
UWORD scnpos_config_buttons[]={ SHELL_OK, SHELL_CANCEL,0 };
UWORD save_config_buttons[]={ SHELL_OK, SHELL_CANCEL,0 };
UWORD conf_yn_butt[]={ SHELL_YES,SHELL_NO,0 };

WORD pc_button_poses[][2]={
	248,-192,
	260,-24,
	260, 76-12,
	240,-(96+12),
	136,-(352+12),
	188,-(268+12),
	-704, -(352+12),
	-756, -(268+12),
};
WORD pc_button_layouts[][8]={
	0,1,2,3,4,5,6,7,
	5,4,3,2,1,0,7,6,
	3,1,2,4,5,6,7,0,
	7,6,5,4,3,2,1,0,
};

BYTE *conf_save_req[MAX_LANGS]={
	"Configuration has changed\nDo you wish to save to a Memory Card?",
};

BYTE *pc_button_text[][8]={
	"High action",
	"Pass",
	"Shoot",
	"Low action",
	"Sprint",
	"Control",
	"Push",
	"Change player",


					  


};

BYTE *pad_config_words[]={
	"Config",
	"Config",
	"Config",
	"Config",
	"Config",
};
BYTE *sp_info_text[]={
	"Press X to\nposition screen",
	"Press X to\nposition screen",
	"Press X to\nposition screen",
	"Press X to\nposition screen",
	"Press X to\nposition screen",
};


BYTE *audio_words[][3]={
	"sound mode : mono\n",
	"sound mode : stereo\n",
	"sound mode : inverse stereo\n",

	"sound mode : mono\n",
	"sound mode : stereo\n",
	"sound mode : inverse stereo\n",

	"sound mode : mono\n",
	"sound mode : stereo\n",
	"sound mode : inverse stereo\n",

};

BYTE *ac_tgad_text[][4]={
	"Master volume",
	"Music volume",
	"Commentary volume",
	"Sfx volume",

	"Master volume\n",
	"Music volume\n",
	"commentary volume\n",
	"sfx volume\n",

};

BYTE *screen_pos_shell_opts[]={
	"ok",
	"cancel",
	0,

	"fok",
	"fcancel",
	0,
};

BYTE *pad_config_lolly_txt[]={
	"Configure controller",
	"Configure controller",
	"Configure controller",
	"Configure controller",
};


BYTE *pad_assign_lolly_txt[]={
	"Assign controller",
	"Assign controller",
	"Assign controller",
};













Slider_gadget ac_slider[4];
Rdb_gadget ac_rdb[2];
Rdb_gadget scn_pos_info_rdb;
			  
WORD ac_master_volume_store, ac_music_volume_store, ac_com_volume_store, ac_sfx_volume_store, ac_sound_mode_store;

WORD ac_y=-50*4;
WORD ac_x=-240;
WORD ac_ys=25;
WORD old_sfx_volume;

void init_audio_config()
{
WORD cnt,x,y,ys;

	set_title_lolly( 0, audio_config_lolly_text[config->language]  );
	set_shell_bottom_buttons( audio_config_buttons,0 );

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


void update_audio_config()
{
WORD cnt,y;

Textman_struct *ts;
	modify_current_item( 5 );



	for(cnt=0; cnt<4;cnt++)
		add_text_string ( tslot_poses[MAIN_TSLOT]+cnt, ac_tgad_text[config->language][cnt] , MILFORD_FONT, milford_font_logic, -540, ac_y+(ac_ys*4*cnt)-(9*4), FONT_RJUST, -2, 0, 0,0 );

/** Print sound mode **/
	ts = add_text_string ( tslot_poses[MAIN_TSLOT]+4 , audio_words[config->language][config->sound_mode], MILFORD_FONT, milford_font_logic, 0, ac_y+(ac_ys*4*cnt)-(9*4), FONT_XC, 4, 0, 0, 0 );


	if ( shell->current_item == 4 )
		modify_byte( &config->sound_mode, 0,2 );

	if ( ts )
		{
		set_rdb_size_from_tm( &ac_rdb[1], ts );
		}

	
	if ( (config->sfx_volume/4) != (old_sfx_volume/4) )
		{
		old_sfx_volume = config->sfx_volume;
		sfxSpot( CLICK2, (255*config->sfx_volume)/255 );

		}



	set_audio_global_vars();

}

void kill_audio_config()
{
WORD cnt;


	kill_shell_bottom_buttons();

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
	push(0);
//	init_shell( FRONTEND );
	exec_shell( init_audio_config, update_audio_config, kill_audio_config );
	pop(0);
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

	set_title_lolly( screen_pos_lolly_text[config->language],0 );
	set_shell_bottom_buttons( scnpos_config_buttons,0 );
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

	ts=add_text_string ( tslot_poses[MAIN_TSLOT]+0 , sp_info_text[ config->language ], MILFORD_FONT, milford_font_logic, 0, -(16*4), FONT_XC+FONT_YC, -1, 0, 0, 0 );
	
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
				goto_shell_buttons();
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

	kill_shell_bottom_buttons();
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

	set_title_lolly( pad_config_lolly_txt[config->language],0 );
	set_shell_bottom_buttons( pad_config_buttons,0 );

	cnt=0;

	pc_x = 0*2;
	pc_y = 45*4;

	set_rdb_gadget( 0, &pc_rdb, pc_x-(380/2),pc_y+16,380, 16*4, 0 );

	joy_curr = joy_curr_output;	//** Lock pad to last pad read

	pc_val = config->pad_config[ joy_curr ];

	spawn_sprite( CONFPAD );
	sprite->x.w.hi = -220;
	sprite->y.w.hi = -50*4;
	sprite->depth = MIDDLE_DEPTH+1;
	sprite->group_num = 16384;

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

	

}

void update_pad_config()
{

WORD cnt;
Textman_struct *ts;
WORD num;
WORD text_group, text_num;
BYTE *text;
WORD just;

	modify_current_item( 1 );

	cnt = 0;
	

	num = pc_val+1; // goes 1,2,3,4

	FntPrint("num=%d\n",num);
	sprintf( text_buffer, "%s %d", pad_config_words[config->language], num );
	ts = add_text_string ( tslot_poses[MAIN_TSLOT]+0 , text_buffer, MILFORD_FONT, milford_font_logic, pc_x, pc_y, FONT_XC, 0, 0, 0, 0 );


	if ( shell->current_item == 0 )
		modify_byte( &pc_val, 0,3 );


//*** Display pad button details ***
	
	for( cnt=0;cnt<8;cnt++)
		{		
		text_group = pc_val;

		text_num = pc_button_layouts[ text_group ][cnt];

		text = pc_button_text[ config->language ][ text_num ];

		if ( cnt<6 )
			{
			just = FONT_NORMAL;
			}
		else
			just = FONT_RJUST;

		ts=add_text_string ( tslot_poses[MAIN_TSLOT]+1+cnt , text, MILFORD_FONT, milford_font_logic, pc_button_poses[cnt][0], pc_button_poses[cnt][1], just, -1, 0, 0, 0 );
//		ts->max_brightness = 90;
		}



}



void kill_pad_config()
{
	if ( shell->current_button == SHELL_OK )
		config->pad_config[ joy_curr ] = pc_val;

	joy_curr = -1; // read current pad back on
	kill_rdb_gadget( pc_rdb.gadget );
	kill_shell_bottom_buttons();
	kill_sprite_group( 16384 );
	share->title_func=0;
}


void pad_config()
{
	push(0);
	exec_shell( init_pad_config, update_pad_config, kill_pad_config );
	pop(0);
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

CVECTOR pa_tints[]={
	160,160,160,0,	// white
	128,128,32,0, 	// yellow
	40,140,160,0,	// cyan
	32,130,32,0,  	// green	
	130,90,30,0,	// orange
	130,40,130,0,	// pur
	130,40,40,0,	// red
	40,40,140,0,	// blue
};

void init_pad_assign_once()
{
	share->team1 = 0;
	share->team2 = 0;
}


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

	set_shell_bottom_buttons( pad_assign_buttons,0 );

	cnt=0;

	//*** Top box ***
	set_rdb_gadget( 0, &pa_rdb[0], -460,-344, 460*2, 64, -1 );

	//** Middle box **
	set_rdb_gadget( 0, &pa_rdb[1], -112,-256, 112*2, 536, -1 );

	//** Left box **
	gad=set_rdb_gadget( 0, &pa_rdb[2], -460,-256, 340, 536, -1 );
	if ( share->team1_cpu == YES )
		gad->rdb_gadget->flash_tint_default = 32;

	//** Right box **
	gad=set_rdb_gadget( 0, &pa_rdb[3], 128,-256, 340, 536, -1 );
	if ( share->team2_cpu == YES )
		gad->rdb_gadget->flash_tint_default = 32;



	for(y=0;y<8;y++)
		{
		pa_sprs[y] = spawn_sprite( TEAMPAD );
		sprite->depth = NEAREST_DEPTH;
		sprite->tint = pa_tints[y];
		sprite->dest_tint = pa_tints[y];
		sprite->group_num = 10;
		}

	joy_curr = joy_curr_output;	//** Lock pad to last pad read


/**** Display playstion if Away team is cpu controlled ****/
	if ( share->team2_cpu )
		{
		spawn_sprite( TEAMPAD );
		sprite->x.w.hi = 88*2;
		sprite->y.w.hi = -20*4;
		sprite->frame = 3;
		sprite->group_num = 10;
		}

/**** Display playstion if Home team is cpu controlled ****/
	if ( share->team1_cpu )
		{
		spawn_sprite( TEAMPAD );
		sprite->x.w.hi = -208*2;
		sprite->y.w.hi = -20*4;
		sprite->frame = 3;
		sprite->group_num = 10;
		}

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

	modify_current_item( 1 );



//*** Print left team name ***
	team_txt = get_team_name( share->team1 );
	add_text_string ( tslot_poses[MAIN_TSLOT]+0 , team_txt, MILFORD_FONT, milford_font_logic, -512-48, -364, FONT_RJUST, -1, 0, 0, 0 );


//*** Print right team name ***
	team_txt = get_team_name( share->team2 );
	add_text_string ( tslot_poses[MAIN_TSLOT]+1 , team_txt, MILFORD_FONT, milford_font_logic, 0+48, -364, FONT_NORMAL, -1, 0, 0, 0 );


//*** Print 'vs' in middle ***
	add_text_string ( tslot_poses[MAIN_TSLOT]+2 , pa_vs_txt, MILFORD_FONT, milford_font_logic, 0, -364, FONT_XC, -1, 0, 0, 0 );


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

	kill_shell_bottom_buttons();
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

	shell->disable_button[ SHELL_NEXT ] = NO;

}




void pad_assign()
{
	if ( share->team1_cpu && share->team2_cpu )
		{
		share->thread_pos++;
		}
	else
		exec_shell( init_pad_assign, update_pad_assign, kill_pad_assign );
}





void update_pa_pad_poses()
{
WORD cnt,y,ys,xoffs,ctrl;
WORD lt_limit, rt_limit;


	if ( pads_connected < 4 )
		{
		y= (-10 - ( (pads_connected-1)*20)) * 4; 
		for ( cnt=0; cnt<8; cnt++ )
			{
			xoffs = 75*4;
			sprite = pa_sprs[cnt];
			sprite->frame = 2;
			sprite->y.w.hi = y;
			sprite->x.w.hi = -82;

			if ( pads_connected_list[cnt] == YES )
				{
				sprite->display = YES;
				y += 40*4;
				}
			else
				sprite->display=NO;
			}
		}
	else
		{
		if ( pads_connected < 6 )
			{
			xoffs = 75*4;
			y= (-43 - ( (pads_connected-4)*15)) * 4; 
			for ( cnt=0; cnt<8; cnt++ )
				{
				sprite = pa_sprs[cnt];
				sprite->frame = 1;
				sprite->y.w.hi = y;
				sprite->x.w.hi = -70;

				if ( pads_connected_list[cnt] == YES )
					{
					sprite->display = YES;
					y += 25*4;
					}
				else
					sprite->display=NO;
				}

			}
		else
			{
			xoffs = 75*4;
			ys = 16+((8-pads_connected)*2);
			y= (-61*4 ) + ( (8-pads_connected)*8); 
			for ( cnt=0; cnt<8; cnt++ )
				{
				sprite = pa_sprs[cnt];
				sprite->frame = 0;
				sprite->x.w.hi = -56;
				sprite->y.w.hi = y;

				if ( pads_connected_list[cnt] == YES )
					{
					sprite->display = YES;
					y += ys*4;
					}
				else
					sprite->display=NO;
				}
			}
		}




	for ( cnt=0;cnt<8;cnt++)
		{
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


			if ( ctrl )
				{	
				if ( joy[cnt].r & PAD_LT )
					{

					if ( share->human_player_side[cnt] > lt_limit )
						share->human_player_side[cnt]--;
					}

				if ( joy[cnt].r & PAD_RT )
					{
					if ( share->human_player_side[cnt] != rt_limit )
						share->human_player_side[cnt]++;
					}
				}




			if ( share->human_player_side[cnt] == 0 )
				sprite->temp = 0; //xoffs = 0;

			if ( share->human_player_side[cnt] == 1 )
				sprite->temp = xoffs;

			if ( share->human_player_side[cnt] == -1 )
				sprite->temp = -xoffs;


			sprite->xoffs = sprite->xoffs + ((sprite->temp - sprite->xoffs ))/2;

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

#endif

}


void auto_set_human_player_side()
{
WORD x, cnt;

	if ( (share->team1_cpu == YES) && (share->team2_cpu == NO ) )
		{
		/** Home team is cpu **/
		for(x=0;x<8;x++)
			{
			share->human_player_side[x] = 1; /* Humans on away team */
			}
		return;
		}

	if ( (share->team1_cpu == NO) && (share->team2_cpu == YES ) )
		{
		/** Away team is cpu **/
		for(x=0;x<8;x++)
			{
			share->human_player_side[x] = -1; /* Humans on home team */
			}
		return;
		}

	if ( (share->team1_cpu == NO) && (share->team2_cpu == NO ) )
		{
		/** Neither team is cpu, so half of teams should be home, half should be away **/
		cnt = pads_connected/2;
		for(x=0;x<8;x++)
			{
			if ( cnt > 0 )
				share->human_player_side[x] = 1; 
			else
				share->human_player_side[x] = -1;
			cnt--;
			}
		}
}




















void init_config_io()
{
WORD cnt,x,y,ys;

	set_title_lolly( save_config_lolly_text[config->language],0 );
	set_shell_bottom_buttons( save_config_buttons,0 );

}


void update_config_io()
{
	modify_current_item( 1 );
}

void kill_config_io()
{

	kill_shell_bottom_buttons();
	share->title_func=0;
}


void config_io()
{
	share->game_type = CONFIG;
	push(0);
//	init_shell( FRONTEND );
//	exec_shell( init_config_io, update_config_io, kill_config_io );
	save_game();


	share->title_func=0;
	share->game_type = FRIENDLY;

	pop(0);
}


void config_auto_load()
{
	if ( share->config_loaded == NO )
		{
		share->game_type = CONFIG;
		load_config();
		}
}



void check_for_config_save()
{
BYTE *addr1 = (BYTE *)config;
BYTE *addr2 = (BYTE *)config_store;
WORD len = sizeof( Config );
WORD cnt;

WORD diff=NO;

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


	if ( diff )
		{
		for(cnt=0;cnt<20;cnt++)
			{
			update_memcard();
			wait_vblank(); //bg_loop();
			}

		if ( mc->card_present[0] || mc->card_present[1] )
			{
			do_request( conf_save_req[ config->language ], conf_yn_butt, -20*4, SHELL_YES );

			if ( shell->current_button == SHELL_YES )
				config_io();
			}

		}

	share->title_func=0;	/* title not to call this function again **/

}



/****************************************************
 *																	 *
 *	 Options 													 *
 *																	 *
 ****************************************************/


#include "incs.h"

#ifdef PSX

#include "..\..\global\striker.def" // defs for PSX striker
#include "datafile.h"
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include	"font.h"
#include "shell.h"
#include "hardware.h"
#include "text.h"
#include	"pad.h"
#include "anim.def"
#endif
/****************************************************
 *																	 *
 *	 Defines														 *
 *																	 *
 ****************************************************/


#define KO_EXTRAPLAY_OPTS 3
#define FRIENDLY_EXTRAPLAY_OPTS 6
#define OTHER_EXTRAPLAY_OPTS 1


/****************************************************
 *																	 *
 *  Prototypes													 *
 *																	 *
 ****************************************************/
void kill_options();


/****************************************************
 *																	 *
 *  Vars															 *
 *																	 *
 ****************************************************/

UWORD options_shell_buttons[]={  SHELL_PREV, SHELL_NEXT,SHELL_QUIT, 0 };
UWORD friendly_options_shell_buttons[]={  SHELL_STORE, SHELL_PREV, SHELL_NEXT,SHELL_QUIT, 0 };
//Sprite *opt_sprite;
WORD opt_type, old_opt_type[TOTAL_OPTIONS];
Sprite *opt_spr[TOTAL_OPTIONS];


Rdb_gadget opt_rdb;


BYTE **opt_button_content_lists[]={ opt_button_list1, opt_button_list2, opt_button_list3, opt_button_list4, opt_button_list5,opt_button_list6,opt_button_list7,opt_button_list8, 0 };

WORD opt_cnt[]={ 4,2,2,5,6,8,2,3 };

WORD opt_iconlist1[]={ WEA,0, WEA,1, WEA,2, WEA,3  };
WORD opt_iconlist2[]={ OFS,0, OFS,1 };
WORD opt_iconlist3[]={ DAY, 1, DAY,0 };															  
WORD opt_iconlist4[]={ DUR,0, DUR,1, DUR,2, DUR,3, DUR,4 };
WORD opt_iconlist5[]={ EPL,0, EPL,1,EPL,2, EPL,3,  EPL,4, EPL,5  };
WORD opt_iconlist6[]={ STD,0, STD,5, STD,4,  STD,3, STD,1, STD,2, STD,7, STD,6 };
WORD opt_iconlist7[]={ CAM,0, CAM,1,CAM,2, CAM,3,  CAM,4 };
WORD opt_iconlist8[]={ CAM,0, CAM,1,CAM,2 };

WORD opt_iconlist5_knockout[]={ EPL,2, EPL,4,EPL,5 };
WORD opt_iconlist5_other[]={ EPL,2 };
WORD opt_iconlist5_friendly[]={ EPL,1, EPL,2, EPL,3, EPL,4, EPL,5, EPL, 0 };

WORD *opt_iconlists[] = { (void *)opt_iconlist1, (void *)opt_iconlist2, (void *)opt_iconlist3, (void *)opt_iconlist4, (void *)opt_iconlist5, (void *)opt_iconlist6, (void *)opt_iconlist7, (void *)opt_iconlist8 };


void init_options_bootup()
{
}

void init_options_once()
{
	share->options_presets[OPT_WEATHER]     = 0;
	share->options_presets[OPT_OFFSIDES]    = 0;
	share->options_presets[OPT_DAYNIGHT]    = 0;
	share->options_presets[OPT_DURATION]    = 0;
	share->options_presets[OPT_EXTRAPLAY]   = 0;
	share->options_presets[OPT_STADIUM]     = 0;
	share->options_presets[OPT_CAMERA_TYPE] = 0;
	share->options_presets[OPT_CAMERA_DIST] = 0;

}


void init_options()
{
WORD cnt;
WORD y,yp;


//	cnt=0;
//	for(y=0;y<TOTAL_OPTIONS;y++ )
//		{
//		set_rdb_gadget( 0, &opt_rdb[cnt], -300, -(22*4)+(y*17*4)-(9*4)-(16*4), 300*2,58, cnt );
//		cnt++;
//		}


	set_rdb_gadget( 0, &opt_rdb, -400, -344, 400*2,612, -1 );



//	if ( shell->called_before == NO )
//		{
		set_title_lolly( opt_lolly_text[config->language], game_types_text[config->language][share->game_type] );
		
		
		for(y=0;y<TOTAL_OPTIONS;y++)
			{
			opt_spr[y] = 0;
			old_opt_type[y] = -1;
			}
//		}


	if ( share->game_type != FRIENDLY )
		set_shell_bottom_buttons( options_shell_buttons,-1 );	
	else
		set_shell_bottom_buttons( friendly_options_shell_buttons,-1 );	



	for(y=0;y<8;y++)
		{


		opt_spr[y]=spawn_sprite( *(opt_iconlists[ y ] ));

		yp = -344+(160*(y/2));
		if ( y & 1 )
			{
			sprite->x.w.hi = 400-(72*2);
			sprite->y.w.hi = yp;
			}
		else
			{
			sprite->x.w.hi = -400;
			sprite->y.w.hi = yp;
			}
		}


	if ( share->game_type == KNOCKOUT )
		{
		if ( share->options_presets[OPT_EXTRAPLAY] >= KO_EXTRAPLAY_OPTS )
			share->options_presets[OPT_EXTRAPLAY] = 0;
		opt_iconlists[4] = opt_iconlist5_knockout;
		}
	else
		{
		if ( share->game_type == FRIENDLY )
			{
			opt_iconlists[4] = opt_iconlist5_friendly;
			if ( share->options_presets[OPT_EXTRAPLAY] >= FRIENDLY_EXTRAPLAY_OPTS )
				share->options_presets[OPT_EXTRAPLAY] = 0;

			}
		else
			{
			opt_iconlists[4] = opt_iconlist5_other;
			share->options_presets[OPT_EXTRAPLAY] = 0;
			}									 		
		}

	


}

void update_options()
{
BYTE *text;
WORD item;
WORD index;
WORD cnt, fade_delay;
WORD y,yp;
Textman_struct *ts;
WORD text_num;

	modify_current_item_array( 0, 2,4 );

	if ( shell->master_option_active == NO )
		{
		if ( shell->current_item == 4 )
			{
			if ( share->game_type == KNOCKOUT )
				modify_byte( &share->options_presets[ OPT_EXTRAPLAY ],0, KO_EXTRAPLAY_OPTS-1 );
			else
				{
				if ( share->game_type != FRIENDLY )
					share->options_presets[ OPT_EXTRAPLAY ] = 0;
				else
					modify_byte( &share->options_presets[ OPT_EXTRAPLAY ],0, FRIENDLY_EXTRAPLAY_OPTS-1 );
				}
			}
		else
			modify_byte( &share->options_presets[ shell->current_item ],0, opt_cnt[ shell->current_item]-1 );

		}

	for(y=0;y<TOTAL_OPTIONS;y++)
		{
		//** Display contents of item selected **

		opt_spr[y]->frame = *(opt_iconlists[ y ] + (share->options_presets[ y ]*2)+1   );
		}

	for(cnt=0;cnt<TOTAL_OPTIONS;cnt++)
		{

		text_num = opt_spr[cnt]->frame;

		index = text_num; //share->options_presets[ cnt ] + (config->language* (opt_cnt[ cnt] +1 )) ; 
		text = *((opt_button_content_lists[ cnt ])+index);

		if ( shell->master_option_active )
			fade_delay = cnt;
		else
			fade_delay = 0;

		yp = -328+(160*(cnt/2));
		if ( cnt&1 )
			{

			ts=add_text_string ( tslot_poses[MAIN_TSLOT]+cnt+TOTAL_OPTIONS , opt_txts[config->language][cnt] , MILFORD_FONT, milford_font_logic, -256, yp-44, FONT_RJUST, -2, 0, 10,0,fade_delay );
			set_ts_title_color( ts );
			add_text_string ( tslot_poses[MAIN_TSLOT]+cnt , text , MILFORD_FONT, milford_font_logic, -256, yp, FONT_RJUST, cnt, 0, 10,0,fade_delay );
			}
		else
			{
			ts=add_text_string ( tslot_poses[MAIN_TSLOT]+cnt+TOTAL_OPTIONS , opt_txts[config->language][cnt] , MILFORD_FONT, milford_font_logic, -244, yp-44, FONT_NORMAL, -2, 0, 10,0,fade_delay );
			set_ts_title_color( ts );
			add_text_string ( tslot_poses[MAIN_TSLOT]+cnt , text , MILFORD_FONT, milford_font_logic, -244, yp, FONT_NORMAL, cnt, 0, 10,0, fade_delay );
			}

		}
}

void kill_options()
{
WORD cnt;


// Store wheel positions for options in share struct
// and kill the options gadgets

	for ( cnt=0;cnt<TOTAL_OPTIONS;cnt++ )
		{
		kill_sprite( opt_spr[cnt] );
		}

	kill_rdb_gadget( opt_rdb.gadget );


}


void options()
{
 	exec_shell( init_options, update_options, kill_options );
}




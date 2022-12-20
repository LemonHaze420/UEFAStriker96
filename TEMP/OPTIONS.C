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
#include "shell.h"
#include "hardware.h"
#include "text.h"
#include	"font.h"
#include	"pad.h"
#include "anim.def"
#endif
/****************************************************
 *																	 *
 *	 Defines														 *
 *																	 *
 ****************************************************/





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


#if 0

BYTE *opt_button_names[][7]= { 

	"Weather",
	"Offsides",
	"Night / Day",
	"Duration",
	"Extra play",
	"Stadium",
	"Referee",

	"Weather\n",
	"Offsides\n",
	"Wind strength\n",
	"Duration\n",
	"Extra play\n",
	"Stadium\n",
	"Referee",

	"gWeather\n",
	"gOffsides\n",
	"gWind strength\n",
	"gDuration\n",
	"gExtra play\n",
	"gStadium\n",
	"Referee",

};
#endif


BYTE *opt_txts[MAX_LANGS][TOTAL_OPTIONS]={
	"Weather",
	"Offsides",
	"Lighting",
	"Duration",
	"Extra time",
	"Stadium",
	"Referee",
};


BYTE *opt_button_list4[]={

	"2 mins",
	"5 mins",
	"10 mins",
	"20 mins",
	"45 mins",
	0,

	"g2 mins",
	"g5 mins",
	"g10 mins",
	"g20 mins",
	"g45 mins",
	0,
};	  


BYTE *opt_button_list6[]={
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	0,

	"fStadium A",
	"fStadium B",
	"fStadium C",
	"fStadium D",
	"fStadium E",
	0,


};

BYTE *opt_button_list5[]={
	"Extra time",
	"Extra time\nand Pens",
	"Golden goal\nand pens",
	"None",
	"Pens and\nExtra time",
	"Pens and\nGolden Goal",
	0,

	"gExtra time",
	"gExtra time and Penalties",
	"gGolden goal and penalties",
	"gNone",
	"gPenalties and Extra time",
	"gPenalties and Golden Goal",
	0,
};	  

BYTE *opt_button_list2[]={
	"Off",
	"On",
	0,

	"gOffside off",
	"gOffside on",
	0,
};	  

BYTE *opt_button_list1[]={
	"Sun",
	"Rain",
	"Snow",
	"Random",
	0,

	"gDry",
	"gHot",
	"gSnow",
	"gWet",
	0,
};	  

BYTE *opt_button_list3[]={
	"Day",
	"Night",
	0,

	"gLow",
	"gMedium",
	"gHigh",
	"gOff",
	0,
};	  

BYTE *opt_button_list7[]={
	"Ref 1",
	"Ref 2",
	"Ref 3",
	"Ref 4",
	0,

	"gLow",
	"gMedium",
	"gHigh",
	"gOff",
	0,
};	  


//Icon_gadget opt_icon_gad;
Rdb_gadget opt_rdb;




BYTE *options_shell_opts[]={
	"ok",
	"save",
	"prev",
	"cancel",
	0,

};



BYTE **opt_button_content_lists[]={ opt_button_list1, opt_button_list2, opt_button_list3, opt_button_list4, opt_button_list5,opt_button_list6,opt_button_list7, 0 };

WORD opt_cnt[]={ 4,2,2,5,6,8,4 };

WORD opt_iconlist1[]={ WEA,0, WEA,1, WEA,2, WEA,3  };
WORD opt_iconlist2[]={ OFS,0, OFS,1 };
WORD opt_iconlist3[]={ DAY, 0, DAY,1 };															  
WORD opt_iconlist4[]={ DUR,0, DUR,1, DUR,2, DUR,3, DUR,4 };
WORD opt_iconlist5[]={ EPL,0, EPL,1,EPL,2, EPL,3,  EPL,4, EPL,5  };
WORD opt_iconlist6[]={ STAD_A,0, STAD_B,0,STAD_C,0, STAD_D,0,  STAD_E,0, STAD_F,0, STAD_G,0, STAD_H,0 };
WORD opt_iconlist7[]={ REF,0, REF,0,REF,0, REF,0,  REF,0 };

WORD *opt_iconlists[] = { (void *)opt_iconlist1, (void *)opt_iconlist2, (void *)opt_iconlist3, (void *)opt_iconlist4, (void *)opt_iconlist5, (void *)opt_iconlist6,(void *)opt_iconlist7   };


void init_options_bootup()
{
}

void init_options_once()
{
	share->options_presets[0]=1;
	share->options_presets[1]=0;
	share->options_presets[2]=0;
	share->options_presets[3]=0;
	share->options_presets[4]=0;
	share->options_presets[5]=0;
	share->options_presets[6]=0;

}


void init_options()
{
WORD cnt;
WORD y;


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
		set_shell_bottom_buttons( options_shell_buttons,0 );	
	else
		set_shell_bottom_buttons( friendly_options_shell_buttons,0 );	



	for(y=0;y<4;y++)
		{
		opt_spr[y]=spawn_sprite( STAD_A );
		sprite->x.w.hi = -400;
		sprite->y.w.hi = -344+(160*y);
		}

	for(y=0;y<3;y++)
		{
		opt_spr[y+4]=spawn_sprite( STAD_A );
		sprite->x.w.hi = 400-(72*2);
		sprite->y.w.hi = -344+(160*y)+(34*2);
		}




	


}

void update_options()
{
BYTE *text;
WORD item;
WORD index;
WORD cnt;
WORD y;
Textman_struct *ts;

	modify_current_item( TOTAL_OPTIONS );

	if ( shell->master_option_active == NO )
		modify_byte( &share->options_presets[ shell->current_item ],0, opt_cnt[ shell->current_item ]-1 );

	for(y=0;y<TOTAL_OPTIONS;y++)
		{
		//** Display contents of item selected **

		opt_type  = *(opt_iconlists[ y ] + (share->options_presets[ y ]*2)+0   );

		if ( old_opt_type[y] != opt_type )
			{

			if ( opt_spr[y] )
				{
				kill_sprite( opt_spr[y] );
				}

			old_opt_type[y] = opt_type;
			opt_spr[y] = spawn_sprite( opt_type );

			if ( y & 1 )
				{
				sprite->x.w.hi = 400-(72*2);
				sprite->y.w.hi = -344+(80*y)+(34*0);
				}
			else
				{
				sprite->x.w.hi = -400;
				sprite->y.w.hi = -344+(80*y);
				}
			}

		opt_spr[y]->frame = *(opt_iconlists[ y ] + (share->options_presets[ y ]*2)+1   );
		}

	for(cnt=0;cnt<TOTAL_OPTIONS;cnt++)
		{

		index = share->options_presets[ cnt ] + (config->language* (opt_cnt[ cnt] +1 )) ; 
		text = *((opt_button_content_lists[ cnt ])+index);

		if ( cnt&1 )
			{
			ts=add_text_string ( tslot_poses[MAIN_TSLOT]+cnt+TOTAL_OPTIONS , opt_txts[config->language][cnt] , MILFORD_FONT, milford_font_logic, -256, -260+(80*cnt)-(34*2)-44, FONT_RJUST, -2, 0, 0,0 );
			ts->text_color.r=255;
			ts->text_color.g=255;
			ts->text_color.b=0;
			add_text_string ( tslot_poses[MAIN_TSLOT]+cnt , text , MILFORD_FONT, milford_font_logic, -256, -260+(80*cnt)-(34*2)-0, FONT_RJUST, cnt, 0, 0,0 );
			}
		else
			{
			ts=add_text_string ( tslot_poses[MAIN_TSLOT]+cnt+TOTAL_OPTIONS , opt_txts[config->language][cnt] , MILFORD_FONT, milford_font_logic, -244, -328+(80*cnt)-44, FONT_NORMAL, -2, 0, 0,0 );
			ts->text_color.r=255;
			ts->text_color.g=255;
			ts->text_color.b=0;
			add_text_string ( tslot_poses[MAIN_TSLOT]+cnt , text , MILFORD_FONT, milford_font_logic, -244, -328+(80*cnt)-0, FONT_NORMAL, cnt, 0, 0,0 );
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
	kill_shell_bottom_buttons();


}


void options()
{
 	exec_shell( init_options, update_options, kill_options );
}




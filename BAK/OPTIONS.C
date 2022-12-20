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
Sprite *opt_sprite;
WORD opt_type, old_opt_type;

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
	"Stadium A",
	"Stadium B",
	"Stadium C",
	"Stadium D",
	"Stadium E",
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
	"Extra time and Penalties",
	"Golden goal and penalties",
	"None",
	"Penalties and Extra time",
	"Penalties and Golden Goal",
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
	"Offside off",
	"Offside on",
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
Rdb_gadget opt_rdb [ 7 ];
Rdb_gadget opt_rdb2;
Button_gadget opt_button[7];



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


BYTE *options_shell_opts[]={
	"ok",
	"save",
	"prev",
	"cancel",
	0,

};



BYTE **opt_button_content_lists[]={ opt_button_list1, opt_button_list2, opt_button_list3, opt_button_list4, opt_button_list5,opt_button_list6,opt_button_list7, 0 };


WORD opt_iconlist1[]={ WEA,0, WEA,1, WEA,2, WEA,3  };
WORD opt_iconlist2[]={ OFS,0, OFS,1 };
WORD opt_iconlist3[]={ WIN, 1, WIN,2, WIN, 0, WIN, 3 };
WORD opt_iconlist4[]={ DUR,0, DUR,1, DUR,2, DUR,3, DUR,4 };
WORD opt_iconlist5[]={ EPL,0, EPL,1,EPL,2, EPL,3,  EPL,4, EPL,5  };
WORD opt_iconlist6[]={ STAD_A,0, STAD_B,0,STAD_C,0, STAD_D,0,  STAD_E,0 };
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


	cnt=0;
	for(y=0;y<TOTAL_OPTIONS;y++ )
		{
		set_button_gadget( &opt_button[cnt], 0, -(51*4)+(y*17*4)+60-(16*4), opt_button_names[config->language][cnt] ,opt_button_content_lists[ cnt ],share->options_presets[cnt], MILFORD_FONT, FONT_XC, tslot_poses[MAIN_TSLOT]+cnt, cnt,-1 );
		set_rdb_gadget( opt_button[cnt].gadget, &opt_rdb[cnt], -300, -(22*4)+(y*17*4)-(9*4)-(16*4), 300*2,58, cnt );
		cnt++;
		}


// set big icon
//	set_icon_gadget( opt_button[ 4 ].gadget, -300+8+512, -334+60-(16*4)+512, &opt_icon_gad, opt_iconlists[ 4 ] );

	


// box next to big icon
	set_rdb_gadget( 0, &opt_rdb2, -300, -208-(140/2)-(16*4), 300*2,140, -1 );



	if ( shell->called_before == NO )
		{
		set_title_lolly( opt_lolly_text[config->language], 0 );
		opt_sprite = 0;
		old_opt_type = -1;
		}


	set_shell_bottom_buttons( options_shell_buttons );	
	


}

void update_options()
{
Button_gadget *but;
BYTE *text;
WORD item;
WORD index;
WORD frame;

	if ( shell->master_option_active == NO )
		{
		item = shell->current_item;


		if ( item >= 0 )
			{
//			opt_icon_gad.type_list = opt_iconlists[ item ];
//			opt_icon_gad.gadget->parent = opt_button[ item ].gadget;
			but = &opt_button[ item ];

		//** Display contents of item selected **

	//		FntPrint ( "t=%d\n",but->total_words );

			index = but->gadget->selected_num + (config->language* (but->total_words+1)); 
			text = *(but->text_list + index);

			//FntPrint("text = %s   index=%d\n",text,index );

			add_text_string ( tslot_poses[MAIN_TSLOT]+TOTAL_OPTIONS , text , MILFORD_FONT, milford_font_logic, -128, -310+60-(16*4), FONT_NORMAL, -2, 0, 32,32 );

			}


		opt_type  = *(opt_iconlists[ shell->current_item ] + (opt_button[ shell->current_item ].gadget->selected_num*2)+0   );
		if ( old_opt_type != opt_type )
			{

			if ( opt_sprite )
				{
				kill_sprite( opt_sprite );
				}

			FntPrint("frame=%d\n",frame);
			old_opt_type = opt_type;
			opt_sprite = spawn_sprite( opt_type );
			opt_sprite->x.w.hi = -300+8;
			opt_sprite->y.w.hi = -334+60-(16*4);
			}

		opt_sprite->frame = *(opt_iconlists[ shell->current_item ] + (opt_button[ shell->current_item ].gadget->selected_num*2)+1   );
		modify_current_item ( TOTAL_OPTIONS );
		}
	else
		{

		if ( opt_sprite )
			{
			kill_sprite( opt_sprite );
			opt_sprite = 0;
			old_opt_type = -1;
			}
		if ( cjoy->db & PAD_UP )
			{
			sfxSpot( HIT1, 30 );
			shell->current_item = TOTAL_OPTIONS-1;
			shell->master_option_active = NO;
			}
		}


//	FntPrint("type = %d\nframe=%d\n",type ,frame );

}

void kill_options()
{
WORD cnt;


// Store wheel positions for options in share struct
// and kill the options gadgets

	for ( cnt=0;cnt<TOTAL_OPTIONS;cnt++ )
		{
		share->options_presets[ cnt ] = opt_button[cnt].gadget->selected_num;
		kill_gadget( opt_button[ cnt ].gadget );
		kill_rdb_gadget( opt_rdb[ cnt ].gadget );
		}

	kill_rdb_gadget( opt_rdb2.gadget );
//	kill_gadget( opt_icon_gad.gadget );
	kill_shell_bottom_buttons();


}







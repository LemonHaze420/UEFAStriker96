/****************************************************
 *																	 *
 *	 																 *
 * Stadium select ( from icon wheel )					 *
 *	 																 *
 *																	 *
 ****************************************************/

#include "incs.h"

#ifdef PSX
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include	"font.h"
#include	"pad.h"
#include "anim.def"
#include "shell.h"
#include "options.h"
#include "teamsel.h"
#include "memcard.h"
#include "hwcard.h"
#include "config.h"
#endif


BYTE *preset_save_lolly_text[]={
	"Store Friendly settings",
};

BYTE *preset_choose_lolly_text[]={
	"Choose a preset Friendly",
};

BYTE *empty_text[]={
	"Empty",
};

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

/****************************************************
 *																	 *
 *  Vars															 *
 *																	 *
 ****************************************************/

BYTE *preset_buffer;

BYTE *preset_button_name[]={
	"Preset",
};

//Button_gadget preset_button;
Rdb_gadget preset_rdb[2];
BYTE *textline;
WORD preset_y_gap_store;
UWORD preset_buts[]= {  SHELL_NEXT, SHELL_QUIT, 0 };
UWORD preset_save_buts[]= { SHELL_YES, SHELL_QUIT, 0 };
WORD pr_current_preset;

WORD pr_yes_no_req_list[]= { SHELL_YES, SHELL_NO, 0 };


void init_preset_once()
{
WORD cnt;

	for ( cnt=0;cnt<TOTAL_PRESETS;cnt++ )
		{
		config->preset_data[ cnt ].init = NO; 
		}
}


void init_preset_bootup()
{
	pr_current_preset=0;
}

void init_preset()
{
WORD cnt,y;


	push(0);
	preset_buffer = allocate_mem( 0,2048 );
	textline = allocate_mem( 0,256 );

//*** Adjust y gap size ****
	preset_y_gap_store = resident_fonts[MILFORD_FONT]->font_data->y_gap_size;
	resident_fonts[MILFORD_FONT]->font_data->y_gap_size = 28*2;



	set_rdb_gadget( 0, &preset_rdb[0], -472, -332, 472*2, 504, -1 );
	set_rdb_gadget( 0, &preset_rdb[1], -160, 216-16, 324, 60, 0 );



	if ( share->game_type == FRIENDLY_SAVE )
		{
		set_shell_bottom_buttons( preset_save_buts,0 );
		set_title_lolly( preset_save_lolly_text[config->language], game_types_text[ config->language ][ share->game_type ]  );
		}
	else
		{
		set_shell_bottom_buttons( preset_buts,0 );

		set_title_lolly( preset_choose_lolly_text[config->language], game_types_text[ config->language ][ share->game_type ]  );


		/** Point preset to 1st initialised preset **/

		pr_current_preset=0;

		for(cnt=0;cnt<8;cnt++)
			{
			if ( config->preset_data[ cnt ].init == YES )
				{
				pr_current_preset = cnt;
				break;
				}
			}

		}

	

}

void update_preset()
{
WORD cnt;
BYTE *text, *text1, *text2;
WORD option;
WORD y;
BYTE *ptr;

	modify_current_item ( 1 );
	if ( shell->master_option_active == NO )
		{
		modify_word( &pr_current_preset , 0,7 );
		
		}

	//** Print 1st preset team **

	if ( config->preset_data[ pr_current_preset ].init == NO )
		{
		shell->disable_button[ SHELL_NEXT ] = YES;	// can't exit this screen if user choose an empty slot
		}
	else
		shell->disable_button[ SHELL_NEXT ] = NO;



	cnt=0;

	if ( config->preset_data[ pr_current_preset ].init )
		{
		text1 = get_team_name( config->preset_data[ pr_current_preset ].teams[ 0 ]);
		text2 = get_team_name( config->preset_data[ pr_current_preset ].teams[ 1 ]);
		sprintf ( preset_buffer, "%s vs %s\n",text1,text2 );
		}
	else
		strcpy( preset_buffer, empty_text[ config->language ] );

//*** Print team names ***
	add_text_string ( tslot_poses[MAIN_TSLOT]+TOTAL_PRESETS+cnt , preset_buffer , MILFORD_FONT, milford_font_logic, 0, -336, FONT_XC, -2, 0, 8,32 );

	preset_buffer[0]=0;

	cnt=2;


	for ( y=0; y< TOTAL_OPTIONS; y++ )
		{
		if ( config->preset_data[ pr_current_preset ].init )	
			{
			option = config->preset_data[ pr_current_preset ].option_preset[ y ]; 
			text1 = opt_txts[config->language][y];
			text2 = *((opt_button_content_lists[y])+option);

			/** Copy option description, ignoring Newline \n **/
			ptr = text_buffer;
			while( *text2 )
				{
				*ptr = *text2;
				if ( *ptr == '\n' )
					*ptr = ' ';
				ptr++;
				text2++;		
				}
			*ptr=0;

			sprintf( textline, "%s : %s\n",text1, text_buffer );
			}
		else
			strcpy( textline, "------------\n" );

		strcat ( preset_buffer, textline );
		}

	/*** Print big box of preset data ***/

	add_text_string ( tslot_poses[MAIN_TSLOT]+TOTAL_PRESETS+cnt , preset_buffer , MILFORD_FONT, milford_font_logic, 0, -260, FONT_XC, -2, 0, 0,0 );

/*** print 'preset x ***/
	sprintf( text_buffer, "%s %d", preset_button_name[config->language], pr_current_preset+1 );
	add_text_string ( tslot_poses[MAIN_TSLOT]+3 , text_buffer , MILFORD_FONT, milford_font_logic, 0, 200-24, FONT_XC, 0, 0, 0,0 );


}




void kill_preset()
{
Preset_data *pd;
WORD cnt;


	shell->disable_button[ SHELL_NEXT ] = NO;

	pop(0);

	resident_fonts[MILFORD_FONT]->font_data->y_gap_size  = preset_y_gap_store;

	kill_rdb_gadget( preset_rdb[0].gadget );
	kill_rdb_gadget( preset_rdb[1].gadget );



//	pd = &config->preset_data[ pr_current_preset ];


//	share->team1 = pd->teams[0];
//	share->team2 = pd->teams[1];


	if ( (share->game_type == FRIENDLY_SAVE ) && (shell->current_button == SHELL_YES ) )
		{

		sprintf(text_buffer, preset_save_request[config->language],pr_current_preset+1 );
		if ( do_request( text_buffer, pr_yes_no_req_list, -20*4, SHELL_YES ) == SHELL_YES )
			{
			/** Store current friendly options in config file **/
			for( cnt=0; cnt<TOTAL_OPTIONS; cnt++ )
				{
				config->preset_data[ pr_current_preset ].option_preset[ cnt ]=share->options_presets[ cnt ]; 
				config->preset_data[ pr_current_preset ].init = YES;
				config->preset_data[ pr_current_preset ].teams[0] = share->team1;
				config->preset_data[ pr_current_preset ].teams[1] = share->team2;
				config->team1_cpu = share->team1_cpu;
				config->team2_cpu = share->team2_cpu;
				}
			}

		}

	if ( (share->game_type == FRIENDLY) && (shell->current_button == SHELL_NEXT) )
		{
		for( cnt=0; cnt<TOTAL_OPTIONS; cnt++ )
			share->options_presets[ cnt ] = config->preset_data[ pr_current_preset ].option_preset[ cnt ];
		share->team1 = config->preset_data[ pr_current_preset ].teams[0];
		share->team2 = config->preset_data[ pr_current_preset ].teams[1];
		share->team1_cpu = config->team1_cpu;
		share->team2_cpu = config->team2_cpu;
		}

	kill_shell_bottom_buttons();




}



void preset()
{
 	exec_shell( init_preset, update_preset, kill_preset );

}

/****************************************************
 *																	 *
 *																	 *
 *	 Memory card access										 *
 *  Using hwcard.c											 *
 *																	 *
 *																	 *
 ****************************************************/
#include "incs.h"

#ifdef PSX
#include "..\..\global\striker.def" // defs for PSX striker
#include "..\..\global\s01.h"	// structs used by scene "s01" format
#include "..\..\striker\frontend\mod.def"
#include "..\..\striker\src\anim.def"

#include	"sod.h"
#include	"scene.h"
#include "datafile.h"
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include	"shell.h"
#include "hardware.h"
#include "polydraw.h"	//psx structs
#include "text.h"
#include	"font.h"
#include	"pad.h"
#include "fmv.h"
#include "utils.h"
#include "teamedit.h"
#include "options.h"
#include "config.h"
#include "teamsel.h"
#include "render.h"
#include "mem.h"
#include "hwcard.h"
#include "preset.h"
#endif

//*** These relate directly to share->game_type ****


BYTE *memory_card_appendix_names[ TOTAL_GAME_TYPES ]={
	"FRIEND",		// Friendly games are never saved
	"LEAGUE",
	"KNOCK",	// *** MAX 6 CHARS!!!! ****
	"TERRIT",
	"CLASS",
	"EUROQ",
	"EUROF",
	"WORLD",
	"EUROQ",
	"EUROQ",
	"EUROF",
	"CONFIG",
	"CONFIG",

};


UWORD mc_load_shell_buttons[]={ SHELL_OK, 0 };
UWORD mc_save_shell_buttons[]={ SHELL_SAVE, SHELL_OK, 0 };
WORD mc_yes_no_butts[]= { SHELL_YES, SHELL_NO, 0 };
WORD mc_yes_no_cancel_butts[]= { SHELL_YES, SHELL_NO, SHELL_CANCEL, 0 };
WORD mc_ok_butt[]= { SHELL_OK, 0 };
WORD mc_cancel_butt[]= { SHELL_CANCEL, 0 };

/****************************************************
 *																	 *
 *	 Defines														 *
 *																	 *
 ****************************************************/


/****************************************************
 *																	 *
 *	 Prototypes													 *
 *																	 *
 ****************************************************/

BYTE *get_card_port_text();
void show_slot_num_text();
void restore_loaded_game_data();
void do_mc_load_config();
ULONG make_mc_checksum( BYTE *src, UWORD len );
void store_saved_game_data();

void init_load_game();
void update_load_game();
void kill_load_game();
void load_game();
WORD check_mcard_load();
void do_mc_load();

void init_save_game();
void kill_save_game();
void save_game();
void update_save_game_config();
void update_save_game();
WORD check_mcard_save();
void do_mc_save();

void do_mc_delete();

void overwrite_game();


void show_card_files( WORD show_all_striker_games, WORD load );
void show_mc_info_text();

void mc_init_func( WORD command );
void mc_exit_func( WORD command );

/****************************************************
 *																	 *
 *	 Structs														 *
 *																	 *
 ****************************************************/

/****************************************************
 *																	 *
 *	 Vars															 *
 *																	 *
 ****************************************************/

POLY_FT4 mc_polys[2][15];
//Mc_control card_struct;
WORD cursor_y;
WORD store[2];
Rdb_gadget mc_rdb[6];
BYTE mc_info_buffer[96];
UBYTE mc_file_nums[16];
UWORD memcard_ok_butt[]= { SHELL_OK, 0 };
void *mc_process;










void init_save_game()
{
WORD cnt;

	if ( share->game_type == CONFIG )
		set_title_lolly( save_config_lolly_text[config->language] , game_types_text[ config->language ][ share->game_type ] );
	else
		set_title_lolly( save_game_lolly_text[config->language] , game_types_text[ config->language ][ share->game_type ] );

	set_shell_bottom_buttons( memcard_ok_butt,-1 );


	mc_info_buffer[0]=0;

	mc->perform_dir_read=YES;
	mc->filesize = 8192*1;
	mc->icon_x = 512;
	mc->icon_y = 256;
	mc->clut_x = 512;
	mc->clut_y = 256+16;
	mc->tim_icon = &card_icon_tim;
	mc->header_bytes_to_read = 128; // Don't need to read icon from header

	strcpy( mc->save_name, "BESLES-00256" );
	strcpy( mc->game_save_name, "RGS-" );
	strcpy( mc->save_appendix,  memory_card_appendix_names[ share->game_type ] );


	cursor_y = 0;

	mc->init_func = mc_init_func;
	mc->exit_func = mc_exit_func;

	for(cnt=0;cnt<20;cnt++)
		bg_loop();




	set_rdb_gadget( 0, &mc_rdb[0], -152, 512, 152*2, 532 ,  -1 );

	set_rdb_gadget( 0, &mc_rdb[1], -1, 512, -1, -1, 0 );
	set_rdb_gadget( 0, &mc_rdb[2], -1, 512, -1, -1, 1 );
	set_rdb_gadget( 0, &mc_rdb[3], -1, 512, -1, -1, 2 );
	set_rdb_gadget( 0, &mc_rdb[4], -1, 512, -1, -1, -1 );	// box behind info box
	set_rdb_gadget( 0, &mc_rdb[5], -1, 512, -1, -1, -1 );

 	store[0] = shell->current_item;
	store[1] = shell->master_option_active;


	shell->master_option_active = YES;
	shell->current_item=SHELL_ITEM_BASE;

	mc_process = add_process( show_mc_info_text );


	store_saved_game_data();


	for(cnt=0;cnt<20;cnt++)
		{
		update_memcard();
		bg_loop();
		}

	mc->mode=0;
}




void update_save_game()
{
Textman_struct *ts;


	update_memcard();

	if ( share->game_type == CONFIG )
		{
		mc->card_present[1]=NO;
		mc->card_busy[1]=NO;
		mc->selected_card = 0;
		}

	if ( (mc->card_present[0]==0) && (mc->card_present[1]==0 ) && (mc->card_busy[0]==NO && mc->card_busy[1]==NO) )
		{
		if ( share->game_type == CONFIG )
			{
//			text_buffer = get_card_port_text();
			sprintf( mc_info_buffer, mc_no_lt_card_present_text[ config->language ], get_card_port_text() );
			}
		else
			strcpy( mc_info_buffer, mc_no_cards_present_text[ config->language ] );
		mc_rdb[0].size.y = mc_rdb[1].size.y = mc_rdb[2].size.y = mc_rdb[3].size.y = 512;

		//goto_shell_buttons();
		}
	else
		{
		mc_info_buffer[0]=0;

		if ( mc->card_busy[0] || mc->card_busy[1]  )
			strcpy( mc_info_buffer, mc_please_wait_text[ config->language ] );

		if ( (share->game_type != CONFIG) ||( (share->game_type==CONFIG)&&(mc->total_specific_game_files[0])==0 ) )
			show_card_files( YES, NO );

		if ( mc->command == Mc_ready )
			do_mc_save();	
		}


}




void do_mc_save()
{
WORD result,total_text_lines;
WORD abort=NO;
WORD filenum,cnt, len;
BYTE *src;
WORD free_slots;
WORD app_len;
BYTE newname[64];
WORD newfile;
Textman_struct *ts;
WORD cards_present;

	mc_rdb[1].size.y = mc_rdb[2].size.y = mc_rdb[3].size.y = 512;

	if ( shell->master_option_active )
		{
		mc->mode=0;
		}

	if ( mc->card_present[ mc->selected_card ] )
		{
		result = check_mcard_save( mc->selected_card );

		if ( result == Mc_cancel )
			{
			shell->module_abort = YES;
			shell->current_button = SHELL_OK;

			do_request( mc_save_cancelled_text[ config->language ], mc_ok_butt, -128, SHELL_OK );

			return;
			}
		else
			{
			if ( result == Mc_busy )
				return;
			else
				{
				if ( result == Mc_format )
					{
					/** Format has occurred, restart save module **/
					shell->module_abort=YES;
					shell->current_button = SHELL_NO;
					}
				}
			}
		}

//	mc->filesize = 8192;

//	if ( (share->game_type != CONFIG) ) //|| ((share->game_type==CONFIG)&&(mc->total_specific_game_files[0]==0)) )
//		show_slot_num_text(); //was here

	total_text_lines = mc->total_game_files[ mc->selected_card ];
	free_slots = (15 - mc->total_blocks[ mc->selected_card ])?1:0;

	if ( free_slots )
		total_text_lines++;	/* Include 'create new game' text */

	if ( (mc->card_present[0]==0) && (mc->card_present[1]==0 ) && (mc->card_busy[0]==NO) && (mc->card_busy[1]==NO) )
		cards_present = NO;
	else 
		cards_present = YES;

	if ( cards_present == NO )
		{
		strcpy( mc_info_buffer, mc_no_cards_present_text[ config->language ] );

		shell->module_abort = YES;
		shell->current_button = SHELL_NO;
		goto_shell_buttons(-1);
		}
	else
		{
		mc_info_buffer[0]=0;
		//show_slot_num_text();

		if ( mc->mode == 0 )
			{
			modify_current_item( 2 );	
			if ( shell->current_item == 0 )
				{
				shell->current_item = 512 + (total_text_lines-1);
				mc->mode++;
				}

			if ( shell->current_item == 1 )
				{
				if ( modify_word( &mc->selected_card, 0, 1 ) )
					{
					if ( mc->card_present[ mc->selected_card ] == NO )
						{
						mc->selected_card = 1-mc->selected_card;
						}
					}
				}
			}
		else
			{
			if ( ((cjoy->db & PAD_DN ) && ( shell->current_item == (512+total_text_lines))) || ( mc->card_present[ mc->selected_card ]==NO) || (mc->new_card[ 0 ] && mc->card_present[0]) || (mc->new_card[1] && mc->card_present[1]) )
				{
				mc->mode=0;
				shell->current_item = 1;
				}
			else
				{
				if ( cjoy->db & PAD_UP )	
					{
//					if ( shell->current_item == 1 )
//						shell->current_item = 512+total_text_lines-1;
					}
				modify_word_ud( &shell->current_item, 511, 512+(total_text_lines-1) );
				if ( shell->current_item == 511 )
					{
					shell->current_item = 1;
					mc->mode = 0;
					}
				}
			}


		if ( ((cjoy->db & (PAD_X|PAD_SQU ))&&(mc->mode)) || ((share->game_type == CONFIG)&&(mc->card_present[0])&&(mc->total_specific_game_files[0]) )  )
			{
			sfxSpot( BUTVOC, 30 );

			if ( (((shell->current_item-512) >= mc->total_game_files[ mc->selected_card])) || ((share->game_type == CONFIG)&&(mc->total_specific_game_files[0]) ) )
				{
				/***** User has selected a free slot *****/
				/** So do a SAVE **/

				if ( share->game_type == CONFIG )
					{
				/* Only ever one config file on a card */
					*mc->save_digit_buff=0;
					}
				else
					set_savename_digits();

				sprintf( mc->filename,"%s%s%s", mc->save_name, mc->save_appendix, mc->save_digit_buff );

				if ( share->game_type == CONFIG )
					{
						bg_loop();
					bg_loop();
					bg_loop();
					result=do_request( save_config_prompt[ config->language ], mc_yes_no_butts, -128, SHELL_NO );
					if ( result == SHELL_YES )
						mc_save_file();
					else
						{
						do_request( mc_save_cancelled_text[ config->language ], mc_ok_butt, -128, SHELL_OK );
						//goto_shell_buttons();
						shell->module_abort = YES;
						shell->current_button = SHELL_NO;
						mc->game_saved=YES;
			
						return;
						}
					}
				else
					mc_save_file();

				mc_rdb[0].size.y = 512;
				mc_rdb[5].size.y = 512;

				if ( mc->result == McErrNone )
					{
					mc->game_saved = YES;
					len = strlen( mc->save_name );
					sprintf( text_buffer, mc_save_ok_text[config->language], &mc->filename[len] );
					mc_rdb[1].size.y = 512;
					do_request( text_buffer, mc_ok_butt, -128, SHELL_OK );
					}
				else
					{
					mc_rdb[1].size.y = 512;
					do_request( mc_write_error_text[ config->language ], mc_ok_butt, -128, SHELL_OK );
					}
				}
			else
				{
				/***** User has chosen to overwrite an existing slot *****/

				/** Set name of file to overwrite **/
				filenum = mc_file_nums[ shell->current_item-512 ];	// get file number from ALL files
				strcpy( mc->filename, &mc->directory[ mc->selected_card ][ filenum ].name[0] );
				len = strlen( mc->save_name );

				/*** Check that file we are saving is of type that is being overwritten ***/
				app_len = strlen( mc->save_appendix );
				if ( strncmp( &mc->filename[len], mc->save_appendix, app_len ) == 0 )
					{
					/** Yes they are both same ***/
					sprintf( text_buffer,  mc_are_you_sure_overwrite_text[config->language], &mc->filename[ len ] );
					mc_rdb[0].size.y = 512;
					mc_rdb[5].size.y = 512;
					mc_rdb[1].size.y = 512;
					result = do_request( text_buffer, mc_yes_no_butts, -128, SHELL_YES );
					newfile=NO;
					}
				else
					{
					/*** They are not the same ****/

					newfile=YES;

					set_savename_digits();

					if ( share->game_type == CONFIG )
						*mc->save_digit_buff=0;	/** No numerical appendix **/

					sprintf( newname,"%s%s%s", mc->save_name, mc->save_appendix, mc->save_digit_buff );

					printf("new name '%s'\n",newname);

					mc_rdb[0].size.y = 512;
					mc_rdb[1].size.y = 512;
					mc_rdb[5].size.y = 512;
					sprintf( text_buffer,  mc_are_you_sure_overwrite_with_text[config->language], &mc->filename[ len ], &newname[ len ] );
					result = do_request( text_buffer, mc_yes_no_butts, -128, SHELL_NO );

					if ( result == SHELL_YES )
						{
						/*** User chose yes, so delete old file and save newname ***/

						//printf("deleting '%s'\n",mc->filename );
						mc_delete_file();

						if ( mc->result != McErrNone )
							{
							do_request( mc_overwrite_error_text[config->language], mc_ok_butt, -128, SHELL_OK );
							result = SHELL_CANCEL;
							}
						else
							{
	 						strcpy( mc->filename , newname );
							result = SHELL_YES;
							}
						}			
					
					shell->module_abort = YES;
					shell->current_button = SHELL_NO;

					}


				mc_rdb[0].size.y = 512;
				mc_rdb[5].size.y = 512;
				mc_rdb[1].size.y = 512;

				if ( result == SHELL_YES )
					{

					abort = YES;
					strcpy( mc_info_buffer, mc_overwriting_text[ config->language ] );
					for(cnt=0;cnt<10;cnt++)
						bg_loop();
					mc_save_file();
					mc_info_buffer[0]=0;

					if ( mc->result == McErrNone )
						{
						len = strlen( mc->save_name );

						if ( newfile == NO )
							sprintf( text_buffer, mc_overwrite_ok_text[config->language], &mc->filename[len] );
						else
							sprintf( text_buffer, mc_save_ok_text[config->language], &mc->filename[len] );

	
	
	
						do_request( text_buffer, mc_ok_butt, -128, SHELL_OK );

						mc->game_saved=YES;
						}
					else
						{



						do_request( mc_overwrite_error_text[config->language], mc_ok_butt, -128, SHELL_OK );
						}


						goto_shell_buttons(-1); 
					shell->module_abort=YES;
					shell->current_button = SHELL_NO;
					mc->mode=0;
					}
				else
					{
					if ( result == SHELL_NO )
						{
						do_request( mc_overwrite_cancelled_text[ config->language ]  , mc_ok_butt, -128, SHELL_OK );
						}
					}
				
				}
			shell->module_abort = YES;
			shell->current_button = SHELL_NO;


			}

		if ( mc_rdb[0].size.y != 512 )
			show_slot_num_text();
		}
}













void init_load_game()
{
WORD cnt;
	mc_info_buffer[0]=0;

	set_shell_bottom_buttons( mc_load_shell_buttons,-1 );
	set_title_lolly( load_game_lolly_text[config->language] , game_types_text[ config->language ][ share->game_type ] );

	for(cnt=0;cnt<20;cnt++)
		bg_loop();

	mc->filesize = 8192*1;

	strcpy( mc->save_appendix,  memory_card_appendix_names[ share->game_type ] );

	mc->init_func = mc_init_func;
	mc->exit_func = mc_exit_func;


	set_rdb_gadget( 0, &mc_rdb[0], -152, 512, 152*2, 428+32,  -1 );
	set_rdb_gadget( 0, &mc_rdb[1], -1, 512, -1, -1, 0 );
	set_rdb_gadget( 0, &mc_rdb[4], -1, 512, -1, -1, -1 );	// box behind info box

//	set_shell_bottom_buttons( mc_load_shell_buttons,0 );

	mc_process = add_process( show_mc_info_text );

	for(cnt=0;cnt<20;cnt++)
		{
		update_memcard();
		bg_loop();
		}

	//goto_shell_buttons();
	mc->mode=0;
}

void update_load_game()
{
Textman_struct *ts;

	update_memcard();


	if ( (mc->card_present[0]==0) && (mc->card_present[1]==0 ) && (mc->card_busy[0]==NO) && (mc->card_busy[1]==NO) )
		{
		strcpy( mc_info_buffer, mc_no_cards_present_text[ config->language ] );
		mc_rdb[0].size.y = mc_rdb[1].size.y = mc_rdb[2].size.y = mc_rdb[3].size.y = 512;
		}
	else
		{
		mc_info_buffer[0]=0;

		if ( (mc->card_busy[0])  || (mc->card_busy[1] ) )
			strcpy( mc_info_buffer, mc_please_wait_text[ config->language ] );
		else
			{
			show_card_files( NO, YES );

			if ( mc->command == Mc_ready )
		 		do_mc_load();
			}
		}
}








void do_mc_load()
{
WORD result,total_text_lines;
WORD abort=NO;
WORD filenum,cnt, len;
BYTE *src;
WORD cards_present;

	mc_rdb[1].size.y = mc_rdb[2].size.y = mc_rdb[3].size.y = 512;

	if ( shell->master_option_active )
		{
		mc->mode=0;
		}

	result = check_mcard_load();


	if ( mc->card_present[ mc->selected_card ] )
		{
		if ( result == Mc_cancel )
			{
			shell->module_abort = YES;
			shell->current_button = SHELL_OK;

			do_request( mc_load_cancelled_text[ config->language ], mc_ok_butt, -128, SHELL_OK );
			return;
			}
		}

	mc->filesize = 8192;

	show_slot_num_text();


	total_text_lines = mc->total_specific_game_files[ mc->selected_card ];

	mc_info_buffer[0]=0;

	if ( mc->mode == 0 )
		{
		modify_current_item( 2 );	

		if ( shell->current_item == 0 )
			{
			shell->current_item = 512 + (total_text_lines-1);
			mc->mode++;
			}

		if ( shell->current_item == 1 )
			{
			modify_word( &mc->selected_card, 0, 1 );
			}

		if ( (mc->card_present[ mc->selected_card ] == NO) || ( mc->total_specific_game_files[ mc->selected_card]==0 ) )
			{
			mc->selected_card = 1-mc->selected_card;
			}

		}
	else
		{
		if ( ((cjoy->db & PAD_DN ) && ( shell->current_item == (512+total_text_lines))) || ( mc->card_present[ mc->selected_card ]==NO) || (mc->new_card[ 0 ] && mc->card_present[0]) || (mc->new_card[1] && mc->card_present[1]) )
			{
			mc->mode=0;
			shell->current_item = 1;
			}
		else
			{
			modify_word_ud( &shell->current_item, 511, 512+(total_text_lines-1) );
			if ( shell->current_item == 511 )
				{
				shell->current_item = 1;
				mc->mode = 0;
				}
			}
		}


	if ( (cjoy->db & (PAD_X|PAD_SQU )) && (mc->mode) )
		{
		mc_rdb[0].size.y = 512;
		mc_rdb[5].size.y = 512;
		mc_rdb[1].size.y = 512;

		sfxSpot( BUTVOC, 30 );

		/** Set name of file to load **/
		filenum = mc_file_nums[ shell->current_item-512 ];	// get file number from ALL files
		strcpy( mc->filename, &mc->directory[ mc->selected_card ][ filenum ].name[0] );


		strcpy( mc_info_buffer, mc_loading_text[ config->language ] );

		mc_load_file();
		mc_info_buffer[0]=0;

		mc_rdb[0].size.y = 512;
		mc_rdb[1].size.y = 512;
		mc_rdb[4].size.y = 512;

		if ( mc->result == McErrNone )
			{
			restore_loaded_game_data();
			len = strlen( mc->save_name );
			sprintf( text_buffer, mc_load_ok_text[config->language], &mc->filename[len] );
			do_request( text_buffer, mc_ok_butt, -128, SHELL_OK );
			shell->current_button = SHELL_OK;
			shell->module_abort=YES;
			}
		else
			{
			do_request( mc_load_error_text[config->language], mc_ok_butt, -128, SHELL_OK );
			shell->module_abort=YES;
			shell->current_button = SHELL_NO;
			}
		}
}






void load_game()
{
	mc->game_loaded=NO;

	while( 1 )
		{
		exec_shell( init_load_game, update_load_game, kill_load_game );
		if ( shell->current_button == SHELL_OK )
			{
			break;
			}
		}
}
 
WORD check_mcard_load()
{

/** Check selected memory card prior to doing a Load ***/

/** Returns Mc_ok, if 'card' is valid for Load  ***/
/** Returns Mc_cancel if invalid **/
/** Returns Mc_busy if card active invalid **/

WORD ret = Mc_ok;


	if ( (mc->card_present[0] || mc->card_present[1]) )
		{
		if ( (mc->total_specific_game_files[0]==0) && (mc->total_specific_game_files[1]==0) )
			{
			/** No files on either card **/
			sprintf( text_buffer, mc_no_specific_game_files_text[ config->language ], mc->save_appendix );
			mc_rdb[0].size.y = 512;
			mc_rdb[5].size.y = 512;
			do_request( text_buffer , mc_ok_butt, -128, SHELL_OK );
			ret = Mc_cancel;
			}
		}
	else
		{
		ret = Mc_cancel;
		}

	return ret;
}

WORD check_mcard_save()
{
/** Check selected memory card prior to doing a Save ***/

/** Returns Mc_ok, if 'card' is valid for Save , Load or Overwrite ***/
/** Returns Mc_cancel if User cancelled operation **/
/** Returns Mc_busy if card active **/

WORD result;
WORD ret = Mc_cancel;


	if ( (mc->card_present[ mc->selected_card ] == YES) && (mc->card_busy[ mc->selected_card ] == NO ) )
		{
		if ( mc->card_formatted[ mc->selected_card ] == NO )
			{
			//*** Card not formatted, does user want to format it ? ***

			sprintf( text_buffer, mc_unformat_text[config->language], get_card_port_text );
			mc_rdb[0].size.y = 512;
			mc_rdb[5].size.y = 512;
			result = do_request( text_buffer, mc_yes_no_butts, -128, SHELL_NO );
			if ( result == SHELL_YES )
				{
				REFORM:	
	
				mc_format_card();
				//mc->command = Mc_format;
				//strcpy( mc_info_buffer, mc_format_text[ config->language ] );


				if ( mc->result != McErrNone )
					{
					mc_rdb[0].size.y = 512;
					mc_rdb[5].size.y = 512;
					result = do_request( mc_reformat_text[config->language], mc_yes_no_butts, -128, SHELL_NO );
					if ( result == SHELL_YES )
						{
						goto REFORM;
						}
					else
						{
						ret = Mc_cancel; /** Format cancelled **/
						}
					}
				else
					{
					mc_rdb[0].size.y = 512;
					mc_rdb[5].size.y = 512;
					do_request( mc_format_done_text[config->language], mc_ok_butt, -128, SHELL_OK );
					ret = Mc_format;
					}
				}
			else
				{
				mc_info_buffer[0]=0;
				ret = Mc_cancel;
				}
			}
		else
			{

			if ( mc->total_blocks[ mc->selected_card ] == 15 )
				{

				// card is full

				if (mc->total_game_files[mc->selected_card])
					{
					ret = Mc_ok;
					}
				else
					{
					/** No game files on this card **/
					mc_rdb[0].size.y = 512;
					mc_rdb[5].size.y = 512;
					do_request( mc_no_slots_to_overwrite_text[ config->language ]  , mc_ok_butt, -128, SHELL_OK );
					ret = Mc_cancel;
					}
				}
			else
				{
				ret = Mc_ok;
				}
			}
		}
	else
		{
		if ( mc->card_busy[ mc->selected_card ] )
			ret = Mc_busy;
		else
			ret = Mc_cancel;
		}

	return ret;
}



void mc_init_func( WORD command )
{
WORD cnt;

	if ( command == Mc_save_file )
		{
		sprintf( mc_info_buffer, mc_saving_text[ config->language ], mc->selected_card+1 );

		for(cnt=0;cnt<10;cnt++)
			bg_loop();

		}

}

void mc_exit_func( WORD command )
{
	if ( command == Mc_save_file )
		{
		mc_info_buffer[0] = 0;
		}
}


void show_card_files( WORD show_all_striker_games, WORD load )
{
WORD cnt=0;
BYTE *text;
WORD len;
BYTE *dir_name;

BYTE full_name[48];
WORD yp=-1;
WORD game_name_len;
WORD free_slots;

	mc_rdb[0].size.y = 512;

//	FntPrint("total files %d, striker files %d\n",mc->total_files[ mc->selected_card ], mc->total_game_files[ mc->selected_card ] );

	strcpy( full_name, mc->save_name );

	if ( show_all_striker_games == NO )
		strcat ( full_name, mc->save_appendix );	// only view striker games of specific type eg League games

	
	game_name_len = strlen ( mc->game_save_name );

	len = strlen( full_name );

	free_slots = 15 - mc->total_blocks[ mc->selected_card ];

	yp=0;
	if ( mc->card_present[ mc->selected_card ] && mc->directory_loaded[ mc->selected_card ] )
		{
		for( cnt=0; cnt< mc->total_files[ mc->selected_card ]; cnt++ )
			{
			dir_name = &mc->directory[ mc->selected_card ][cnt].name[0];

			if ( strncmp ( dir_name, full_name, len ) == 0 )
				{
				/** Only show Striker files of this game type eg LEAGUE files **/
				
				text = &mc->ascii_text_name[ mc->selected_card ][cnt][ 0 ];
				mc_rdb[0].size.y = -208-32-32-32-16-16-4;

				add_text_string ( tslot_poses[MAIN_TSLOT]+32+cnt, text , TINY_FONT, milford_font_logic, 0, -(90*4)+(36*yp), FONT_XC, yp+512, 0, 0,0 );
				mc_file_nums[yp] = cnt;
				yp++;
				}

			}

		if ( load == NO )
			{
			/*** Add 'Create new game' message ****/

			if ( (share->game_type != CONFIG) || ((share->game_type == CONFIG) && ( mc->total_specific_game_files[ mc->selected_card ] == 0 )) )
				{
				if ( free_slots )
					{
					mc_rdb[0].size.y = -208-32-32-32-16-16-4;
					text = mc_free_slot_text[ config->language ];
					add_text_string ( tslot_poses[MAIN_TSLOT]+32+cnt, text , TINY_FONT, milford_font_logic, 0, -(90*4)+(36*yp), FONT_XC, yp+512, 0, 0,0 );
					}
				}
			}
		}
	
	cnt++;

}


void show_mc_info_text()
{
Textman_struct *ts;
WORD card;

	if ( mc_info_buffer[0] )
		{
		ts = add_text_string ( tslot_poses[MAIN_TSLOT]+30, mc_info_buffer , MILFORD_FONT, milford_font_logic, 0, 0, FONT_YC+FONT_XC, -2, 0, 0,0 );
		set_rdb_size_from_tm( &mc_rdb[4], ts );
		}
	else
		{
		mc_rdb[0].size.y = 512;
		mc_rdb[1].size.y = 512;
		mc_rdb[2].size.y = 512;
		mc_rdb[3].size.y = 512;
		mc_rdb[4].size.y = 512;
		mc_rdb[5].size.y = 512;
		}

}




void kill_load_game()
{


	kill_rdb_gadget( mc_rdb[0].gadget );
	kill_rdb_gadget( mc_rdb[1].gadget );
//	kill_rdb_gadget( mc_rdb[2].gadget );
	kill_rdb_gadget( mc_rdb[4].gadget );

	kill_process( mc_process );



}

void kill_save_game()
{
WORD cnt;


	for( cnt=0; cnt<5; cnt++ )
		kill_rdb_gadget( mc_rdb[cnt].gadget );

	if ( share->game_type == CONFIG )
		kill_rdb_gadget( mc_rdb[5].gadget );

	kill_process( mc_process );

}




void save_game()
{
WORD cnt;

	mc->game_saved=NO;
	while(1)
		{
		exec_shell( init_save_game, update_save_game, kill_save_game );
		
		if ( shell->current_button == SHELL_OK )
			break;
		if ( (share->game_type == CONFIG) && (mc->game_saved==YES) )
			{
			*config_store = *config;
			break;
			}
		}
}






void restore_loaded_game_data()
{
/*** Chosen file has been loaded to 8k load_file_buffer ****/

Share *share_src;
Config *config_src;
BYTE *src;
WORD x,y,cnt,group;
Share *sh;
ULONG checksum;

	mc->game_loaded=YES;
	src = (BYTE *)&mc->load_file_buffer[0];
	src += 512;

	if ( share->game_type == CONFIG )
		{
		config_src = (Config *)src;
		*config = *config_src;
		*config_store = *config;
		}
	else
		{
		share_src = (Share *)src;

		checksum = share_src->checksum;
		share_src->checksum=0;
		printf("loaded block checksum=%d\n",make_mc_checksum( src, sizeof( Share )) );

		printf("block contents checksum=%d\n",checksum );


		*share = *share_src;
		}
	share->config_loaded=YES;

}



void store_saved_game_data()
{
/*** Chosen file has been loaded to 8k load_file_buffer ****/

Share *share_src;
Config *config_src;
BYTE *src;
WORD cnt,x,y;
WORD len;
ULONG checksum;
Preset_data *pd;

	src = (BYTE *)&mc->save_file_buffer[0];
	src += 512;

	if ( share->game_type == CONFIG )
		{
//		printf("storing config struct (%d bytes)\n",sizeof( Config ) );

		config_src = (Config *)src;

	/** Save everything TEMPORARILY as long as config struct does not exceed 8192 bytes ***/

		*config_src = *config;
		}
	else
		{
		printf("storing share struct (%d bytes)\n",sizeof( Share ) );
	
		share_src = (Share *)src;

	/** Save everything TEMPORARILY as long as share struct does not exceed 8192 bytes ***/

		*share_src = *share;

		share->checksum=0;
		checksum = make_mc_checksum( src, sizeof( Share ) );
		share->checksum = checksum;

		printf("check save is %d\n",share->checksum );
		}
}

ULONG make_mc_checksum( BYTE *src, UWORD len )
{
ULONG checksum=0;

//	printf("len=%d\n",len );

	while( len-- )
		{
		checksum += *src;
		src++;
		}
	return checksum;
}	















WORD check_load_config()
{
WORD cnt;

	mc_info_buffer[0]=0;

	mc->filesize = 8192*1;

	share->game_type = CONFIG;
	strcpy( mc->save_appendix,  memory_card_appendix_names[ share->game_type ] );

	share->config_loaded=NO;
	mc->game_loaded=NO;
	mc->selected_card=0;

	mc->card_busy[0]=YES;	

	for(cnt=0;cnt<80;cnt++)
		{
		wait_vblank();
		update_memcard();

		if ( (mc->command == Mc_ready) && (mc->card_busy[0]==NO) )
			{
			if ( ( mc->total_specific_game_files[0] ) && (mc->card_present[0] ) )
				{
				printf("card found with files after %d checks\n",cnt);
				return YES;
				}
			if ( (mc->card_present[0]==NO) )
				{
				printf("no card found after %d checks\n",cnt);
				return NO;
				}
			}

		}

	return NO;
}






void do_mc_load_config()
{
WORD result,file_cnt;
WORD abort=NO;
WORD filenum,cnt, len;
BYTE *src;
WORD card;

	mc->selected_card=0;
	result = check_mcard_load();
	if ( result != Mc_ok )
		{
		return;
		}

	mc->filesize = 8192;

	file_cnt = mc->total_specific_game_files[ mc->selected_card ] + 0;


	card = mc->selected_card;
	while( abort == NO)
		{
		update_memcard();

		if ( mc->card_present[ card ]==NO )
			{
			abort = YES;
			}

		
	/** Set name of file to load **/

		strcpy( mc->filename, mc->save_name );
		strcat ( mc->filename, memory_card_appendix_names[ CONFIG ] );
		abort = YES;

		mc_load_file();

		if ( mc->result == McErrNone )
			{
			restore_loaded_game_data();
			}
		}
}


void show_slot_num_text()
{
Textman_struct *ts;
BYTE *only_txt;

	if ( share->game_type == CONFIG )
		{
		sprintf( text_buffer, "%s %s %s\n",card_num_text[ config->language ], get_card_port_text(), only_text[config->language] );
		}
	else
		sprintf( text_buffer, "%s %s\n",card_num_text[ config->language ], get_card_port_text() );

	/**** Slot num *****/
	ts = add_text_string ( tslot_poses[MAIN_TSLOT]+64, text_buffer , MILFORD_FONT, milford_font_logic, 0, 232-48, FONT_XC, 1, 0, 0,0 );
	set_rdb_size_from_tm( &mc_rdb[1], ts );
}


BYTE gcp_text[8];
BYTE *get_card_port_text()
{

	if ( mc->selected_card == 0 )
		strcpy( gcp_text, "1" );
	else
		strcpy( gcp_text, "2" );
		
	if ( multitaps_connected_list[0] && (mc->selected_card==0) )
		strcat( gcp_text, "A" );
	
	if ( multitaps_connected_list[1] && (mc->selected_card==1) )
		strcat( gcp_text, "B" );


	return gcp_text;
}


/************************************************************************/
/*																							  	*/
/*																							  	*/
/*	Module: main.c																		  	*/
/*																							 	*/
/*																							 	*/
/************************************************************************/


#include "incs.h"

#include "\global\striker.def" // defs for PSX striker
#include "\global\s01.h"	// structs used by scene "s01" format
#include	"sod.h"
#include "hardware.h"
#include "utils.h"
#include "options.h"
#include "teamedit.h"
#include "preset.h"
#include "mem.h"
#include "langsel.h"
#include "teamsel.h"
#include "pad.h"
#include "league.h"
#include "hwcard.h"
#include "sprite.h"
#include "gadget.h"
#include "shell.h"
#include "cdaudio.h"
#include "movie.h"
#include "config.h"

/************************************************************************ 
 *																							  	* 
 *	prototypes																			  	* 
 *																							  	* 
 ************************************************************************/

void main_go();
void init_once();
void init_main();
void exit_to_stub();
void init_game_loop();

void setup_game_share_data();

/************************************************************************ 
 *																							  	* 
 *	vars																					  	* 
 *																							  	* 
 ************************************************************************/

Share *share;
Comp *comp, *comp_store;
Config *config, *config_store; 
Config config_store_struct;
ULONG timer;
LONG oldstack;
WORD flash_counter,stub_present;
ULONG original_stack;



int main ( void )
{
/** Put no vars in this function! **/

/*** Set up structure shared by front-end, stub and game ***/



	#if DEBUG
	printf("\n\n\n");
	printf("ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป\n"),
	printf("บ                                                                                บ\n"),
	printf("บ   FRONTEND                                                                     บ\n"),
	printf("บ   UEFA Striker                                                                 บ\n"),
	printf("บ   initialisation                                                               บ\n"),
	printf("บ                                                                                บ\n"),
	printf("ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ\n\n\n"),
	#endif


	share 		 = (Share *)SHARE_DATA;
	comp		    = (Comp *)COMP_DATA;
	comp_store   = (Comp *)COMP_DATA_STORE;
	config 		 = (Config *)CONFIG_DATA;
	config_store = (Config *)&config_store_struct;

	if ( share->stub_code != STUB_CODE )
		{
		SetSp ( 0x807fff00 );	/** set stack to 8mbyte if stub not present **/

		#if DEBUG
		printf("Stack is set from Stub\n");
		#endif
		stub_present = NO;
		}
	else
		{
		#if DEBUG
		printf("Stack is set from Frontend\n");   /** Stack will be at 2mbyte **/
		#endif
		stub_present = YES;
		}

	stub_present = YES;	// for hula and play testers only


#if DEBUG
	printf("share %d (max %d)\n",sizeof( Share ), SHARE_DATA_SIZE );
	printf("config %d (max %d)\n",sizeof( Config ), CONFIG_DATA_SIZE );
	printf("comp %d (max %d)\n",sizeof( Comp ), COMP_DATA_SIZE );


	printf("Max available heap size=%d\n", GetSp() - get_heap_start(0) );
#endif

							
	if ( stub_present==NO )
		{
		main_go();
		}
	else
		{
		//** PC only comes to here if stubloader is present **
		main_go();
		exit_to_stub();
		}



}





void main_go()
{

	#if DEBUG
	printf("debug mode %s   Master %s  CDload %s   Real PSX %s   Exe stack=%x  Exe addr=%x\n\n\n",
	DEBUG?"yes":"no" , MASTER?"yes":"no",CD_LOAD?"yes":"no",REAL_PLAYSTATION?"yes":"no",  GetSp(),EXE_AREA );

	#endif

	init_spu();
	CdInit();
	init_xa_audio();
	init_once();
	init_main();
	run_frontend();

}


void bg_loop()
{
WORD cnt;

#if DEBUG
	init_lines();	// reset line draw
#endif
	read_pads();

	timer++;		
	timer&=0x7fff;	// variable that changes every frame...

	adjust_hw_screen_pos();

	swap_screens();
	ClearOTagR( cdb->ot,  OT_SIZE);	/* clear ordering table */



//*** All processing here ***

	oldstack = SetSp( 0x1f800400 );	//Stack in cache
	sfxProcessVoices();
	execute_processes();
	check_audio_track();
	SetSp ( oldstack );					// Stack out of cache





#if DEBUG == YES
	draw_cpu_bar();		//debug check cpu speed
#endif


	wait_vblank();



#if DEBUG
	draw_gpu_bar();	//display graphic processor timing bar
#endif

  	gpu_bar_start = GetRCnt( RCntCNT1 ); //GsGetVcount();



	PutDispEnv(&cdb->disp); /* update display environnment */

//	compact_otr( cdb->ot , OT_SIZE );


	DrawOTagEnv( cdb->ot+OT_SIZE-1, &cdb->draw );




#if DEBUG

	if ( (print_debug_info == 0) || (print_debug_info == 3)  )		// display system font to screen
		FntFlush(-1);

	if ( print_debug_info == 2 )
		show_border_area();
#endif

#if REAL_PLAYSTATION == NO
	pollhost(); 
#endif

	single_step_pause();			// pause game loop and step frame by frame



#if DEBUG
	switch_debug_info();			//switch screen debug info on/off
	move_scnpos();
	peek();	  		
	save_screen_grab ( screen_xres,screen_yres,3 );
	show_cpugpu_time();


	if ( mc )
		{
//		FntPrint("A:new %c, in %c, fr %c, dl %c, bk %d, tf %d, tgf %d busy %c\n",mc->new_card[0]?'Y':'N', mc->card_present[0]?'Y':'N', mc->card_formatted[0]?'Y':'N', mc->directory_loaded[0]?'Y':'N',mc->total_blocks[0],mc->total_files[0],mc->total_game_files[0],mc->card_busy[0]?'Y':'N' );
//		FntPrint("B:new %c, in %c, fr %c, dl %c, bk %d, tf %d, tgf %d busy %c\n",mc->new_card[1]?'Y':'N', mc->card_present[1]?'Y':'N', mc->card_formatted[1]?'Y':'N', mc->directory_loaded[1]?'Y':'N',mc->total_blocks[1],mc->total_files[1],mc->total_game_files[1],mc->card_busy[1]?'Y':'N' );
		FntPrint("ci=%d\n",shell->current_item );
		}

//	FntPrint("ram free = %d\n",0x801fff00-(read_free_addr(0)) );
#endif

	flash_counter++;

	make_random_number(10);

}







void init_game_loop()
{
	timer=0;
	frame_update=FRONTEND_UPDATE_RATE;
	random_number.l = 12345678;
//	start_scene_struct = 0;

#if DEBUG == YES
	print_debug_info=0;
#endif
}


void init_main()
{
//*** this function called every time frontend is started up ***


	init_memory_alloc();


	ResetCallback();

	init_hardware();


	//*** init movie player ***
	set_movie_debug_mode( 1 );					// to see any debug messages

	open_datafile();	// open up large data file to read

#if DEBUG == NO
	show_start_movies();
#endif

	init_game_loop();

	share->init_once_code = INIT_ONCE_PERFORMED_CODE;

}


void init_once()
{

	if ( share->init_once_code != INIT_ONCE_PERFORMED_CODE )
		{

	//*** this function called ONCE when frontend is started up ***

		share->game_type = FRIENDLY;
	
		share->config_loaded = NO;
		share->club_comp = NO;
		config->screen_xpos = 9; 
		config->screen_ypos = NTSC?1:22;
		config->store_performed=NO;

		config->master_volume = 244;
		config->music_volume  = 244;
		config->com_volume    = 244;
		config->sfx_volume 	 = 244;
		config->sound_mode 	= FR_STEREO;

		config->language = ENG;

		share->game_stat = GAME_INIT;
		share->title_func = (void *)0;
	  	share->demo_mode = NO;
		init_pad_assign_once();
		init_select_num_teams_once();
		init_preset_once();
		init_options_once();
		init_player_edit_once();
		init_team_edit_once();
		init_pad_config_once();
		init_training_data_once();

		*config_store = *config;
		}


#if DEBUG
//	printf("end of ram=%x\n",&end_of_ram );
#endif
}




void exit_to_stub()
{
BYTE *humtxt[]={ "HUMAN","CPU" };
WORD x;


	fade_off_music();


//	init_memory_alloc();
//	show_loadscn( YES ); // yes=load it 


	setup_game_share_data();


#if DEBUG
	printf("EXITTING TO STUB..........\n\n");
	printf("Demo mode is %s\n",share->demo_mode?"On":"Off" );
	printf("team 1 num = %d  name='%s'  (%s)\n",share->team1, share->team1_name_string, humtxt[share->team1_cpu] );
	printf("team 2 num = %d  name='%s'  (%s)\n",share->team2, share->team2_name_string, humtxt[share->team2_cpu] );
	printf("stadium    = %d\n",share->options_presets[ OPT_STADIUM ]);
	printf("weather    = %d\n",share->options_presets[ OPT_WEATHER ]);
	printf("title func=%d\n",share->title_func);
#endif

//	debug_print_mess( "closing datafile", 60 );

	close_datafile();	// close large data file

	VSync(0);

//	debug_print_mess( "resetgraph3", 60 );
	ResetGraph(3);		// ResetGraph(0) is also available.
	DrawSync(0);		// [post-processing]


//	debug_print_mess( "mc stop", 60 );

	MemCardStop();


//	debug_print_mess( "stopping rcounters", 60 );

	StopRCnt(RCntCNT0);
	StopRCnt(RCntCNT1);
	StopRCnt(RCntCNT2);
	StopRCnt(RCntCNT3);
	VSync(0);
	StopTAP( );
	VSync(0);
	StopCallback( );
	VSync(0);



//	close_card();

}

















void setup_game_share_data()
{
WORD x;


/*** Make strings for team names ***/
	sprintf( share->team1_name_string, get_team_name( share->team1 ) );
	sprintf( share->team2_name_string, get_team_name( share->team2 ) );

/*** Set flags for multitaps ***/
	share->multitaps_connected_list[0] = multitaps_connected_list[0];
	share->multitaps_connected_list[1] = multitaps_connected_list[1];

/*** Zero human player side array if pads have been removed ***/
	for(x=0;x<8;x++)
		{
		if ( pads_connected_list[x]==0)
			{
			share->human_player_side[x]=0;
			}
		}


	for(x=0;x<4;x++)
		share->human_player_side[x]=-1;
	for(x=4;x<8;x++)
		share->human_player_side[x]=1;


}












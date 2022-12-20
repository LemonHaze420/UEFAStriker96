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


/************************************************************************/
/*																							  	*/
/*	prototypes																			  	*/
/*																							  	*/
/************************************************************************/

void init_once();
void main_go();
void init_game_loop();
void init_main();
void exit_to_stub();

/************************************************************************/
/*																							  	*/
/*	vars																					  	*/
/*																							  	*/
/************************************************************************/

WORD zzcnt1, zzcnt2;

Share *share, *share_store;
Config *config, *config_store; //, *original_config;
//Config original_config_struct;
Config config_store_struct;
ULONG timer;
LONG oldstack;
WORD flash_counter,yp,stub_present;

int main ( void )
{
//** Put no vars in this function! **

//*** Set up structure shared by front-end, stub and game ***

	#if DEBUG
	printf("\n\n\n\n\n");
	printf("********************************************\n");
	printf("********************************************\n");
	printf("**                                        **\n");
	printf("**                                        **\n");
	printf("**              FRONTEND                  **\n");
	printf("**                                        **\n");
	printf("**                                        **\n");
	printf("********************************************\n");
	printf("********************************************\n\n\n\n\n");
	#endif


	share 		 = (Share *)SHARE_DATA;
	share_store  = (Share *)SHARE_STORE_DATA;
	config 		 = (Config *)CONFIG_DATA;
	config_store = (Config *)&config_store_struct;

	//original_config = &original_config_struct;



	if ( share->stub_code != STUB_CODE )
		{
		SetSp ( EXE_STACK );	// set stack if stub not present
		#if DEBUG
		printf("******* Stack SET , Stub is absent *********\n");
		#endif
		stub_present = NO;
		}
	else
		{
		#if DEBUG
		printf("******* Stack NOT set , Stub is present *********\n");
		#endif
		stub_present = YES;
		}


	stub_present = NO;	// for hula only


#if DEBUG
	printf("stub code=%x once_init_code=%d\n",share->stub_code,share->init_once_code );


	if ( (sizeof ( Share )) > SHARE_DATA_SIZE )
		{
		printf("\n\n\n\n****** ERROR!!! SHARE_DATA struct (%d) exceeds memory allocated for it ( %d )\n",sizeof(Share), SHARE_DATA_SIZE ); 
		}
	else
		printf("share struct is %d bytes long with %d bytes allocated\n",sizeof( Share ), SHARE_DATA_SIZE );

	if ( (sizeof ( Config )) > CONFIG_DATA_SIZE )
		{
		printf("\n\n\n\n****** ERROR!!! CONFIG_DATA struct (%d) exceeds memory allocated for it ( %d )\n",sizeof(Config), CONFIG_DATA_SIZE ); 
		}
	else
		printf("config struct is %d bytes long with %d bytes allocated\n",sizeof( Config ), CONFIG_DATA_SIZE );
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
	printf("debug mode %s\nmaster %s\ncdload %s\nreal playstation %s\nexe stack=%x\nexe addr=%x\n\n\n",
	DEBUG?"yes":"no" , MASTER?"yes":"no",CD_LOAD?"yes":"no",REAL_PLAYSTATION?"yes":"no",  EXE_STACK,EXE_AREA );

	printf("share struct size=%d bytes\n",sizeof(Share ) );
	#endif

	initialise_spu();
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

	if( joy[0].db & PAD_CIR )
		{
		for(cnt=0;cnt<78;cnt++)
			share->cpu_teams_index_list[cnt]=0;
		}



	wait_vblank();



	#if DEBUG
		draw_gpu_bar();	//display graphic processor timing bar
	#endif

  	gpu_bar_start = GetRCnt( RCntCNT1 ); //GsGetVcount();



	PutDispEnv(&cdb->disp); /* update display environnment */

	compact_otr( cdb->ot , OT_SIZE );


	DrawOTagEnv( cdb->ot+OT_SIZE-1, &cdb->draw );




#if DEBUG

	if ( (print_debug_info == 0) || (print_debug_info == 3)  )		// display system font to screen
		FntFlush(-1);

	if ( print_debug_info == 0 )
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
	save_screen_grab ( config->screen_xres,config->screen_yres,3 );
	show_cpugpu_time();

	if ( mc )
		{
//		FntPrint("A:new %c, in %c, fr %c, dl %c, bk %d, tf %d, tgf %d busy %c\n",mc->new_card[0]?'Y':'N', mc->card_present[0]?'Y':'N', mc->card_formatted[0]?'Y':'N', mc->directory_loaded[0]?'Y':'N',mc->total_blocks[0],mc->total_files[0],mc->total_game_files[0],mc->card_busy[0]?'Y':'N' );
//		FntPrint("B:new %c, in %c, fr %c, dl %c, bk %d, tf %d, tgf %d busy %c\n",mc->new_card[1]?'Y':'N', mc->card_present[1]?'Y':'N', mc->card_formatted[1]?'Y':'N', mc->directory_loaded[1]?'Y':'N',mc->total_blocks[1],mc->total_files[1],mc->total_game_files[1],mc->card_busy[1]?'Y':'N' );
		FntPrint("ci=%d\n",shell->current_item );
		}

#endif

	flash_counter++;


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
	open_datafile();	// open up large data file to read


	init_game_loop();

//*** init movie player ***
//	play_movie( -1 , 0 );	// just init movie player




}


void init_once()
{

	if ( share->init_once_code != INIT_ONCE_PERFORMED_CODE )
		{
		share->init_once_code = INIT_ONCE_PERFORMED_CODE;

	//*** this function called ONCE when frontend is started up ***

		printf("************************************\n");
		printf("************************************\n");
		printf("**                                **\n");
		printf("**    Once only initialisation    **\n");
		printf("**                                **\n");
		printf("************************************\n");
		printf("************************************\n\n");

		share->game_type = FRIENDLY;
		share->config_loaded = NO;
		config->screen_xpos = 9; 
		config->screen_ypos = NTSC?1:22;
		config->store_performed=NO;

		config->master_volume = 244;
		config->music_volume  = 244;
		config->com_volume    = 244;
		config->sfx_volume 	 = 244;
		config->sound_mode 	= FR_STEREO;


		share->game_stat = GAME_INIT;
		share->title_func = (void *)0;

		#if DEBUG
		//	init_display( DEFAULT_XRES, DEFAULT_YRES, 20,20,40, NO );	//screen xy res & bg color
		#endif


		init_select_num_teams_once();
		init_langsel_once();
		init_preset_once();
		init_options_once();
		init_player_edit_once();
		init_team_edit_once();
		init_pad_config_once();

		*config_store = *config;
		}


	#if DEBUG
	printf("end of ram=%x\n",&end_of_ram );
	#endif
}




void exit_to_stub()
{
BYTE *humtxt[]={ "HUMAN","CPU" };

	sprintf( share->team1_name_string, get_team_name( share->team1 ) );
	sprintf( share->team2_name_string, get_team_name( share->team2 ) );

	#if DEBUG
	printf("EXITTING TO STUB..........\n\n");
	printf("Demo mode is %s\n",share->demo_mode?"On":"Off" );
	printf("team 1 num = %d  name='%s'  (%s)\n",share->team1, share->team1_name_string, humtxt[share->team1_cpu] );
	printf("team 2 num = %d  name='%s'  (%s)\n",share->team2, share->team2_name_string, humtxt[share->team2_cpu] );
	printf("stadium    = %d\n",share->options_presets[ OPT_STADIUM ]);
	printf("weather    = %d\n",share->options_presets[ OPT_WEATHER ]);
	#endif

	close_datafile();	// close large data file

	VSync(0);

	ResetGraph(3);		// ResetGraph(0) is also available.
	DrawSync(0);		// [post-processing]

	MemCardStop();

	StopRCnt(RCntCNT0);
	StopRCnt(RCntCNT1);
	StopRCnt(RCntCNT2);
	StopRCnt(RCntCNT3);
	VSync(0);
	StopTAP( );
	StopCallback( );

//	close_card();

}






























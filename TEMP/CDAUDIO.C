/************************************************************************/
/*																							  	*/
/*	Module: Cd audio control														  	*/
/*																							  	*/
/*	Author: Rob			  																  	*/
/*																							  	*/
/*	Status:	 																			  	*/
/*																							 	*/
/************************************************************************/

/************************************************************************/
/*																							  	*/
/*	include default headers															  	*/
/*																							  	*/
/************************************************************************/
#include	"incs.h"
//#include	"include\main.h"
//#include	"include\effects.h"

/************************************************************************/
/*																							  	*/
/*	game includes																		  	*/
/*																							  	*/
/************************************************************************/
/************************************************************************/
/*																							  	*/
/*	prototypes																			  	*/
/*																							  	*/
/************************************************************************/
void set_audio_global_vars();
void install_cd_volume( void );
void set_cd_audio( WORD control );
void check_audio_track( void );
WORD init_xa_audio( void );
WORD cdplay( void );
void set_audio_channel( WORD channel );
WORD play_xa_audio_track( WORD track );
WORD cd_ff( LONG many );
void cdseek( LONG pos );
WORD seekdone( void );
void wait_cdseek( void );
WORD audio_track_ready( void );
void ready_audio_track( WORD track );
void save_audio_track_pos( void );
void inter_play( void );
void do_cd_fade( void );
void set_cd_volume( void );
void modify_cd_volume( WORD volume );


/************************************************************************/
/*																							  	*/
/*	defs																					  	*/
/*																							  	*/
/************************************************************************/
#define CDAUDIO

/******************************************************************/
/*	cd audio defs																	*/
/******************************************************************/



#define	CD_POLL_TIME			30
#define	AUDIO_CLICK_RETRY		20
#define	CD_FADE_SPEED			4

#define	CD_EVENT_READY			1
#define	NEED_NEXT_CD_EVENT	1


#define	WAIT_FADE							1
#define	WAIT_SEEK							2
#define	SECONDARY_AUDIO_TRACK_READY	3
#define	RESTORE_LAST_TRACK				4
#define	RESTORE_LAST_CHANNEL				5


/************************************************************************/
/*																							  	*/
/*	vars																					  	*/
/*																							  	*/
/************************************************************************/
WORD	cd_volume, cd_volume_offset, cd_stereo, cd_fade_volume,cd_volume_setting;
WORD	cd_base_volume;
LONG	audio_track_start, audio_track_end, audio_track_pos;
LONG	save_pos,save_pos_start,save_pos_end;
LONG	audio_bank_1_start,audio_bank_2_start;
LONG	audio_track_end_reload;
LONG	cd_param,cd_event_control;
WORD	audio_check_poll;
WORD	audio_click;
WORD	audio_control,next_track,current_audio_channel,save_channel;
WORD	cd_playing = FALSE;
CdlLOC	g_loc;

WORD cd_initialised = YES;

WORD	audio_track;

/************************************************************************/
/*																							  	*/
/*	constant data																		  	*/
/*																							  	*/
/************************************************************************/
Audio_track	available_audio_tracks[] =
{
	0,0,0,999999,250,				// hollowslide
	0,1,0,999999,250,				// kramer east
	0,2,0,999999,250,				// annex
	0,3,0,999999,250,				// bandpass
	0,4,0,999999,250,				// calex
	0,5,0,999999,250,						// escape
	0,6,0,999999,250,						// jungle racer
	0,7,0,999999,250,						// sequester

};

/************************************************************************/
/*	setup cd audio																			*/
/************************************************************************/
WORD init_xa_audio( void )
{
	#ifdef CDAUDIO
CdlFILE	fp;
WORD	c;


	/*********************************************************************/
	/* find cd audio bank 1	 															*/
	/*********************************************************************/
//	if (CdSearchFile( &fp, "\\DBZ\\AUDIO\\CD1.XA;1" ) == 0)
	if (CdSearchFile( &fp, "\\CDAUDIO.XA;1" ) == 0)
		{
		printf("  cant find audio bank 1\n");
		return FALSE;
		}
	
	audio_bank_1_start = CdPosToInt(&fp.pos);

	CdPause();
	cd_volume = 0;

	#endif

	return YES;
}

/************************************************************************/
/*	setup cd audio, mono/stereo etc													*/
/* control- MONO								mono output								*/
/* 			STEREO ( OR LEFT_RIGHT )	normal stereo							*/
/* 			RIGHT_LEFT						inverse stereo							*/
/* 			0									off										*/
/************************************************************************/
void set_stereo_mode( WORD stereo )
{
	cd_stereo = stereo;

}

/************************************************************************/
/*	plays an interleaved xa audio track												*/
/*																								*/
/*																								*/
/************************************************************************/
WORD play_xa_audio_track( WORD track )
{
CdlFILE	fp;
Audio_track	*at;
UBYTE	param[4];
WORD	stat;

	set_audio_global_vars();


	if( cd_initialised == TRUE )
		{
	
		at = &available_audio_tracks[ track ];

		audio_track = track;

		#ifdef DT
		printf("  start audio track %d",track);
		#endif
		if( at -> audio_bank == 0 )
			audio_track_start = audio_bank_1_start + at -> start;
		else
			audio_track_start = audio_bank_2_start + at -> start;

		audio_track_end   = audio_track_start + at -> end;
		audio_track_pos   = audio_track_start;


		/*********************************************************************/
		/*	set soundtrack volume adjust													*/
		/*********************************************************************/
//		cd_volume = at -> volume;
//		cd_fade_volume = at -> volume;
//		cd_volume_setting = at -> volume;

//		set_cd_volume( cd_volume );


		/*********************************************************************/
		/* set cd mode & speed																*/
		/*********************************************************************/
		param[0] = CdlModeSpeed|CdlModeRT|CdlModeSF;
		CdControlB(CdlSetmode, param, 0);

		set_audio_channel( at -> channel );
		cdseek( audio_track_start );
		wait_cdseek();
		cdplay();

		if( cd_playing == FALSE )
			{
			cd_playing = TRUE;
			}

		stat = TRUE;
		}
	else
		{
		printf("request for audio track when cd hasn't been initialised!!\n");
		stat = FALSE;
		}
	return stat;

}

WORD stop_xa_audio( void )
{
	CdPause();
	cd_playing = FALSE;
	check_audio_track();
}

/************************************************************************/
/*	save current audio track position												*/
/************************************************************************/


/************************************************************************/
/*	set cd read position																	*/
/************************************************************************/
void cdseek( LONG pos )
{

	CdIntToPos( pos, &g_loc);
	CdControlF( CdlSeekP, (UBYTE *)&g_loc );

}

/************************************************************************/
/*	wait for cd seek																		*/
/************************************************************************/
void wait_cdseek( void )
{
	while( seekdone() == FALSE )
		{
		VSync(0);
		}

	return;
}

/************************************************************************/
/*	return last seek status																*/
/************************************************************************/
WORD seekdone( void )
{
UBYTE		result[8];

	if( CdSync(1, result) == CdlComplete )
		{
		return TRUE;
		}

	return FALSE;
}

/************************************************************************/
/*	set audio channel 																	*/
/************************************************************************/
void set_audio_channel( WORD channel )
{
CdlFILTER	filter;
UBYTE			result[8];

	filter.file = 1;
	filter.chan = channel;
	CdControlB(CdlSetfilter, (UBYTE *)&filter, result );
	current_audio_channel = channel;

}

/************************************************************************/
/*	play audio track 																		*/
/************************************************************************/
WORD cdplay( void )
{
	CdControlF( CdlReadS, 0);
}

/************************************************************************/
/*	fast foreward cd																		*/
/************************************************************************/
WORD cd_ff( LONG many )
{
CdlLOC	loc;
	
	audio_track_pos += many;
	CdIntToPos( audio_track_pos, &loc);
	if (CdControl( CdlReadS, (UBYTE *)&loc, 0) != 1)
		return( FALSE );

	return YES;
}


void modify_cd_volume( WORD volume )
{
CdlATV	vol;

	if( cd_stereo & LEFT_RIGHT )
		{
		vol.val0 = (UBYTE)volume;
		vol.val2 = (UBYTE)volume;
		vol.val1 =
		vol.val3 = 0;
		}
	if( cd_stereo & RIGHT_LEFT )
		{
		vol.val1 = (UBYTE)volume;
		vol.val3 = (UBYTE)volume;
		vol.val0 =
		vol.val2 = 0;

		}

	CdMix( &vol );

}

void set_cd_volume( void )
{
CdlATV	vol;
WORD		volume;

	volume = config -> cd_volume;	

	if( volume > 255 )
		volume = 255;

	cd_volume = 
	cd_fade_volume = volume;


	if( cd_stereo & LEFT_RIGHT )
		{
		vol.val0 = (UBYTE)volume;
		vol.val2 = (UBYTE)volume;
		vol.val1 =
		vol.val3 = 0;
		}
	if( cd_stereo & RIGHT_LEFT )
		{
		vol.val1 = (UBYTE)volume;
		vol.val3 = (UBYTE)volume;
		vol.val0 =
		vol.val2 = 0;

		}

	CdMix( &vol );
}


WORD	bob_audio 	= 0;
WORD	bob_ff 	= 0;
LONG	bob_audio_pos	= 0;
UBYTE		g_param[4];

/************************************************************************/
/*	check audio track for loop															*/
/************************************************************************/
void check_audio_track( void )
{
UBYTE		result[8];
LONG		cnt, ret;


	#ifndef CDAUDIO
		return;
	#endif

//	FntPrint("check audio track\n");

	if( bob_ff < 0 )
		{
		bob_ff = 0;
		cd_ff( -1000 );

		}

	if( audio_check_poll != -2 )
		{

		if( audio_check_poll == -1 )
			{
			if( seekdone() == TRUE )
				{
				cdplay();
				audio_check_poll = 0;
				}

			}

		else
			{
			if( audio_check_poll++ > CD_POLL_TIME )
				audio_check_poll = 0;

			if( audio_check_poll == ( CD_POLL_TIME / 2 ) )
				{
				CdControlF(CdlGetlocP, g_param );
				}
			else
			if( audio_check_poll == CD_POLL_TIME )
				{
				ret = CdSync(1, result);
				if( ret == CdlDiskError )
					{

					if( audio_click++ == AUDIO_CLICK_RETRY )
						{
						/*********************************************************/
						/* disk read failed / could be cack bit or top opened. 	*/
						/*	forward it a bit anyway											*/
						/*********************************************************/
						cd_ff( 1000 );
						audio_click = 0;
						}

					}
				else
				if( ret == CdlComplete )
					{
					if( CdLastCom() == CdlGetlocP &&
			    		(cnt = CdPosToInt((CdlLOC *)&result[5])) > 0)
						{
						audio_track_pos = cnt;

						if( audio_track_pos >= audio_track_end )
							{
							/************************************************/
							/*	end of track											*/
							/************************************************/
							if( audio_control == RESTORE_LAST_CHANNEL )
								{
								set_audio_channel( save_channel );
								cd_fade_volume = 0;
								cd_volume = 1;
								audio_control = RESTORE_LAST_TRACK;

								}
							else
							if( audio_control == RESTORE_LAST_TRACK )
								{
								audio_control = 0;
								audio_track_start = save_pos_start;
								audio_track_end = save_pos_end;
								cdseek( save_pos );
								cd_fade_volume = cd_volume_setting;
								cd_volume = 0;

								audio_check_poll = -1;
								}
							else
								{
								cdseek( audio_track_start );
								audio_check_poll = -1;
								}

							}

						}

					}
				}
			}

		}

	do_cd_fade();

}

void do_cd_fade( void )
{

	if( cd_fade_volume != cd_volume  )
		{
		if( cd_fade_volume < cd_volume )
			{
			cd_volume -= CD_FADE_SPEED;
 			if( cd_volume < cd_fade_volume )
				cd_volume = cd_fade_volume;

			}

		if( cd_fade_volume > cd_volume )
			{
			cd_volume += CD_FADE_SPEED;
 			if( cd_volume > cd_fade_volume )
				cd_volume = cd_fade_volume;

			}

		modify_cd_volume( cd_volume );
		}

}


void set_audio_global_vars()
{

	config->cd_volume = (config->music_volume * config->master_volume ) / 255;

	if ( config->sound_mode == 0 )
		set_stereo_mode( MONO );
	if ( config->sound_mode == 1 )
		set_stereo_mode( STEREO );

	if ( config->sound_mode == 2 )
		set_stereo_mode( RIGHT_LEFT );	// inverse stereo

	set_cd_volume();

}




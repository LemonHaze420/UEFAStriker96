extern		WORD bob_ff;
extern	void set_cd_volume( void );
extern	void install_cd_volume( void );
extern	void set_stereo_mode( WORD stereo );
extern	WORD	audio_track;
extern	WORD stop_xa_audio( void );
extern	WORD init_xa_audio();
extern	WORD play_xa_audio_track( WORD track );
extern	void check_audio_track( void );
extern	void set_cd_audio( WORD control );
extern	void set_audio_channel( WORD channel );
extern	WORD audio_track_ready( void );
extern	void ready_secondary_audio_track( WORD track, WORD fade_volume );
extern	void start_secondary_audio_track_asafp( void );
extern	void set_audio_global_vars();

extern	LONG	audio_track_start, audio_track_end, audio_track_pos, bob_audio_pos;
extern	WORD	cd_volume, cd_stereo, cd_fade_volume;
extern	Audio_track	available_audio_tracks[];
extern	LONG	audio_bank_1_start,audio_bank_2_start;
extern	WORD	cd_playing;





#define TITLE_TRACK		0
#define FRONTEND_TRACK	3

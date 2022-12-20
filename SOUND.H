// STRIKER sound header file
// rrRRRrrRRRrrrrRage against the machine


#include "sfx.def"
#include "sfxfront.def"

#define		AUDIODEV			0	// 0=no edit stuff, 1=edit stuff included		

#define		MAXVOICES		24
#define		MAXSFX			120
#define		MAXSCRIPTS		5
#define		MAXECHOS			8
#define		MAXSPEECHQ		6
#define		UNITY				48
#define		MALLOCMAX		100
#define		LOADSPACE		(1024*250) // "watch this space" too big? too small? see a doctor? he'll tell you it's genetic ? tough 
#define		REVERBRAM		44512
#define		SPURAMSIZ		(512*1024)
#define		SPUMALSIZ		(SPU_MALLOC_RECSIZ*(MALLOCMAX+1))
#define		MAXSPURAM		(SPURAMSIZ-SPUMALSIZ-REVERBRAM)

//#define	MONO					( LEFT_RIGHT | RIGHT_LEFT )			
//#define	STEREO				LEFT_RIGHT			

//#define		STEREO			0
//#define		MONO				1

// sfxTrigModifiers
#define		FIXVOLUME		0
#define		DELTAVOLUME		1
#define		FIXPITCH			2
#define		DELTAPITCH		3
#define		FIXPARTIAL		4

typedef struct
{
	short int		sampleHandle;		//	0 .. n
	short int		volume;				//	0 - 127
	short int		pan;					// pan position  0 - 64 inclusive 32 centre
	short int		panMode;				// pan mode
	short	int		timeLength;			// loop n frames, 0 loop forever, -1 one shot
	short int		reverb;				// 0 on 1 off
	short int		ar;					// attack rate
	short int		dr;					// decay
	short int		sr;					// sustain
	short int		sl; 					// sustain level
	short int		rr;					//	release
	short int		apt;					// attack pitch time
	short	int		app;					// attack pitch target level
	short	int		dpt;					// decay pitch time
	short	int		dpp;					// decay pitch target level
	short	int		scope;				// scope of output
	short	int		note;					// base pitch offset MidiNote
	char	name[12];						// name
	char	partialName[12];				// partial name
}	SFXPROGRAM;

typedef struct
{
	int	voice;			// platform specific voice id ( i.e. PSX SPU_XX )
	int	active;			// -1 off, 0 on key off, 1 on key on
	int	startTime;		// world clock start
	int	timeLength;		// the time to play for
	int	sfx;				//	handle of sound effect being played
	int	volume;			// current volume
	int	pan;				// current pan
	int	pitchEnv;		// stage of envelope
	int	mute;				// 0 On 1 Muted
	int	note;				// base pitch offset MidiNote assigned to voice
}	VOICETYPE;

typedef struct
{
	int			active;		// is this one active
	int			startTime;	// time this one start playing
	short int	*scriptP;	// pointer to script data
}	SCRIPTTYPE;

typedef struct
{
	int	sfxH;				// handle of sfx to use
	int	active;			// is this one active
	int	lastTime;		// time the last one was triggered
	int	delayTime;		// time until the next one
	int	deltaDelayTime;// the change in delay time with each echo
	int	volume;			// the current volume
	int	deltaVolume;	// the change in volume between each echo
	int	pitch;			// the current pitch
	int	deltaPitch; 	// the change in pitch between each echo
}	ECHOTYPE;

typedef struct
{
	int	addr;				// address in SPU ram
	int	size;				// size in bytes
	char	name[12];		// name
}	PARTIALTYPE;

#if	AUDIODEV
extern int				sfxEdit;
#endif
extern int 				worldClock;
extern int				speakerMode;
extern int				sfxNextAmbi;
extern short int		sfxAnimPlayer[];

int			sfxTrigOn				( int sfxH, int numMod, ... );
void			sfxTrigOff				( int	V );
void			sfxEchoOn				( int sfxH, int delayTime, int deltaDelayTime, int deltaVolume, int deltaPitch );
int			sfxScriptOn				( short int* script );
void			sfxScriptOff			( int );
void			sfxMod					( int	V, int modType, int modValue );
void			sfxProcessVoices		( void );
void			sfxInitVoices			( void );
void			sfxFadeAllSoundOff	( void );
void			initsound				( int masVol, int musVol );
void			sfxRestart				( void );
void			sfxShutUp				( void );
void			sfxSetMusicVolume		( int );
void			sfxSetMasterVolume	( int );
void			sfxSetSfxVolume		( int );
void			spu_init( void );

// RUDDY STRIKER STUFF
void			sfxSpot					( int, int );
void			sfxSliderNoise			( int sfxSlider );

#if	AUDIODEV
void			sfxEditor	();
void			sfxEditor1	( int );
#endif


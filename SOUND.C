//	STRIKER 98 Ruddy Gullit																	|					
//	sound.c
//	Rage against the machine

#define SOUND_FILE 1

#include "incs.h"
#include "datafile.h"
#include "io.h"

#if	AUDIODEV

char	panModes[5][4]=
{
	"OFF",
	"L>n",
	"R>n",
	"L>e",
	"R>e"
};

char	sfxScopeNames[3][4]=
{
	"IN ",	// in the arena
	"TV ",	// on tv in the game
 	"OUT"		// menu / options / paused / out of the game
};
#endif

short int	sfxAnimPlayer[]=
{
		22*2, BALL2, 20,
	29*2, TOES1, 4,
	29*2, HEEL1, 4,
		35*2, BALL2, 25,
	42*2, TOES2, 4,
		51*2, BALL2, 30,
	54*2, HEEL1, 4,
		69*2, KICK2, 30,
	77*2, TOES2, 4,
		85*2, BALL2, 42,	
	//92*2,		HEEL1, 8,
	100*2,	TOES1,  4,
		102*2, TAKVOK3, 10,
	103*2,	HEEL2, 8,
		104*2, KICK3, 48,
	108*2,	BREATH2,	4,
	114*2,	HEEL1, 6,
	118*2,	HEEL1, 2,
	118*2,	TOES1, 6,
	126*2,	HEEL1, 6,
	126*2,	TOES2, 6,
		136*2, KICK2, 22,
	137*2,	BREATH1, 4,
		155*2, BALL2, 30,
	159*2,	BREATH2, 2,
	163*2,	HEEL1, 8, 
		171*2, KICK2, 18,
	177*2,	HEEL2, 7,
		184*2, BALL2, 35,
	192*2,	TOES1, 4,
	192*2,	HEEL2, 4, 
	203*2,	BREATH2, 1,	
		203*2, KICK2, 20,
	212*2,	HEEL2, 6,
		221*2, KICK3, 20,
	236*2,	HEEL1, 6,
		239*2, TRAPBALL, 26,
	252*2,	BREATH1, 2,
	-1
};

SFXPROGRAM		Sound_Effect	[MAXSFX];
SCRIPTTYPE		scripts			[MAXSCRIPTS];
VOICETYPE		voices			[MAXVOICES];
ECHOTYPE			echos				[MAXECHOS];
PARTIALTYPE		partials			[MALLOCMAX];
char 				spu_malloc_rec	[SPU_MALLOC_RECSIZ*(MALLOCMAX+1)];
int				masterVol;
int				musicVol=0;
int				sfxVol;
int				frameCounter=0;	// frames
int				worldClock=0;		// 1/50 of a second
short int		sample_count;
int				numberOfSfx;
int				currentScope;
int				numPartials;
int				iAmTalking=0;
int				stopTalkingTime;
int				chainSawVoice=-1;
int				chainSawStartTime;
int				speechQ=-1;
int				speechQueue		[MAXSPEECHQ];
int				numberOfVoicesFree;
int				speakerMode=STEREO;
int				sfxNextAmbi;

// RUDDY STRIKER STUFF

#if	AUDIODEV
int				sfxTest=0;
int				sfxEditBank=0;
int				sfxEdit=0;
int				sx;
int				sy;

BYTE *load_buffer;

void	PS( int x, int y, char* textout )
{
	//print_string( x, y, textout, FNT_WHITE1, SMALL_FONT, TRANSOFF, INGAME_MENU_ICON_DEPTH );
	sx=x;
	sy=y;
}

void	SpecialP( int x, int y, char* textout )
{
int	i;
int	len;
int	centre;
int	clen;
char	buff[2];

int	anim;

	clen=strlen( textout );
	len=clen*10;
	centre=x+(len/2);

	anim=(worldClock%60)/3;

	if( anim>10 ) anim=20-anim;
	
	anim+=4;
	
	for( i=0; i<(clen/2); ++i )
		{
		buff[0]=*(textout+((clen/2)-i));
		buff[1]=0;
		PS( centre-(i*anim), y, buff );
		buff[0]=*(textout+i+(clen/2));
		buff[1]=0;
		PS( centre+(i*anim), y, buff );
		}
  
//double pow(double, double),exp(double);


}

void	PN( int x, int y, int num )
{
char	buff[10];

	sprintf( buff, "%d", num );
	PS( x, y, buff );
	sx=x;
	sy=y;
}


void	S( char* textout )
{
	sx=16;
	sy+=10;
	PS( sx, sy, textout );
}


void	N( int num )
{
	sx=80;
	PN( sx, sy, num );
}
#endif

void	sfxInitReverb( void )
{
long 				rev_mode;
SpuReverbAttr	r_attr;
   
	
	rev_mode = SPU_REV_MODE_OFF;
	r_attr.mask        = (SPU_REV_MODE |
								 SPU_REV_DEPTHL | SPU_REV_DEPTHR);
	r_attr.mode        = (SPU_REV_MODE_CLEAR_WA | rev_mode);
	r_attr.depth.left  = 0x0fff;
	r_attr.depth.right = 0x0fff;
	SpuSetReverbModeParam (&r_attr);
	SpuSetReverbDepth (&r_attr);
	SpuSetReverbVoice (SpuOff, SPU_ALLCH);
	SpuSetReverb (SpuOn);
	r_attr.mode = (SPU_REV_MODE_CLEAR_WA | rev_mode);
	SpuSetReverbModeParam (&r_attr);
	SpuSetReverbDepth (&r_attr);

	rev_mode = SPU_REV_MODE_ROOM;
	//rev_mode = SPU_REV_MODE_SPACE;
	r_attr.mode = (SPU_REV_MODE_CLEAR_WA | rev_mode);
	SpuSetReverbModeParam (&r_attr);
	SpuSetReverbDepth (&r_attr);
}

void	masteredPannedVolume( int volume, int pan, short int* Left, short int* Right )
{
int	L=0,
		R=0;

	if( speakerMode==MONO ) pan=0;

	if( volume<1 )
		{
		*Left=0;
		*Right=0;
		return;
		}
	
	if( volume>127 )	volume=127;
	
	volume = (volume<<7)+volume;
	
	if( pan<0 ) pan=0;
	if( pan>64 ) pan=64;

	if( pan<32 )
		{
		L=volume;
		R=(volume>>5)*pan;
		}

	if( pan>32 )
		{
		L=(volume>>5)*(64-pan);		
		R=volume;
		}

	if( pan==32 )
		{
		L=volume;
		R=volume;
		}

	//	and here is the sfx volume slider control
	L=(L*sfxVol)/127;
	R=(R*sfxVol)/127;
		
	*Left = (short)L;
	*Right = (short)R;
}

void	sfxSetMainVolumes( int masVol, int musVol )
{
SpuCommonAttr		c_attr;

	c_attr.mask =
 	(
		SPU_COMMON_MVOLMODEL	|
		SPU_COMMON_MVOLMODER	|
		SPU_COMMON_MVOLL	|
		SPU_COMMON_MVOLR	|
		SPU_COMMON_CDVOLL	|
		SPU_COMMON_CDVOLR |
		SPU_COMMON_CDMIX );

	c_attr.mvolmode.left  = SPU_VOICE_DIRECT;
	c_attr.mvolmode.right  = SPU_VOICE_DIRECT;
	c_attr.mvol.left  = masVol*129;
	c_attr.mvol.right = masVol*129;	
	c_attr.cd.volume.left  = musVol*129;
	c_attr.cd.volume.right = musVol*129;
	c_attr.cd.mix = SPU_ON;
	SpuSetCommonAttr (&c_attr);
}

void	sfxInitVoices( void )
{
VOICETYPE			*voice_ptr;
short int			c;
int					voice_mask;
SpuVoiceAttr		s_attr;

	s_attr.mask = (
		SPU_VOICE_PITCH
	);

	s_attr.pitch = 0x1000;
	
	voice_ptr = voices;
	c = MAXVOICES;
	voice_mask = 1;
	while( c )
		{
		voice_ptr->voice=voice_mask;
		voice_ptr->active=-1;
		voice_ptr->volume=0;
		voice_ptr->pan=0;
		voice_ptr->sfx=-1;
		voice_ptr->pitchEnv=-1;
		voice_ptr->mute=0;
		voice_ptr->startTime=0;
		voice_ptr->timeLength=0;
		voice_ptr->note=0;
		s_attr.voice = voice_mask;
		SpuSetVoiceAttr (&s_attr);
		SpuSetKey( SpuOff, voice_mask );
		voice_mask <<=1;
		c--;
		voice_ptr++;
		}

	SpuSetKey( SpuOff, SPU_ALLCH );
	SpuSetReverbVoice( SpuOff, SPU_ALLCH );
}

void	sfxMod( int	V, int modType, int modValue )
{
SpuVoiceAttr		s_attr;
int					pitch=0;
	
	//if( modType==FIXVOLUME ) voices[V].volume=modValue;
	if( modType==DELTAPITCH )
		{
		voices[V].note=(Sound_Effect[voices[V].sfx].note<<2)+(modValue<<2);
		s_attr.mask = ( SPU_VOICE_SAMPLE_NOTE
							|SPU_VOICE_NOTE );
			
		s_attr.sample_note=UNITY<<8;
		pitch=voices[V].note;
		s_attr.note	 =	((UNITY+(pitch/4))<<8)+((pitch%4)*64);
		s_attr.voice =voices[V].voice;
		SpuSetVoiceAttr (&s_attr);
		}
	
	if( modType==FIXPITCH )
		{
		s_attr.mask = ( SPU_VOICE_SAMPLE_NOTE
							|SPU_VOICE_NOTE );
			
		s_attr.sample_note=UNITY<<8;
		pitch=modValue<<2;
		////printf( "Mod %d\n", modValue );
		s_attr.note	 =	((UNITY+(pitch/4))<<8)+((pitch%4)*64);
		s_attr.voice =voices[V].voice;
		SpuSetVoiceAttr (&s_attr);
		}
}

int	spuNextFreeVoice( void )
{
int			V;

	for( V=0; V<MAXVOICES; ++V )
		{
		if( voices[V].active==-1 )
			{
			numberOfVoicesFree--;
			voices[V].active=1;
			return V;
			}
		}
	return -1;
}

int	sfxHandleOfPartialName( char* name )
{
int	z;

	for( z=0; z<numPartials; ++z )
		{
		if( strcmp( partials[z].name, name )==0 )
			{
			return z;
			}		
		}	
	return -1;
}

void	sfxSpotR( int partialH, int vol )
{
	vol = (vol*((config->sfx_volume*config->master_volume)/255) )/255;

	Sound_Effect[SPOTR].sampleHandle=partialH;
	if( Sound_Effect[SPOTR].sampleHandle<0 ) return;
	Sound_Effect[SPOTR].timeLength=(partials[Sound_Effect[SPOTR].sampleHandle].size/64)+2;
	sfxTrigOn( SPOTR, 1, FIXVOLUME, vol );
}

void	sfxSpot( int partialH, int vol )
{
	vol = (vol*((config->sfx_volume*config->master_volume)/255) )/255;

	Sound_Effect[SPOT].sampleHandle=partialH;
	if( Sound_Effect[SPOT].sampleHandle<0 ) return;
	Sound_Effect[SPOT].timeLength=(partials[Sound_Effect[SPOT].sampleHandle].size/64)+2;
	sfxTrigOn( SPOT, 1, FIXVOLUME, vol );
}

void	sfxEchoOn( int sfxH, int delayTime, int deltaDelayTime, int deltaVolume, int deltaPitch )
{
int	i;

	for( i=0; i<MAXSCRIPTS; ++i )
		{
		if( echos[i].active==0 )
			{
			echos[i].sfxH=sfxH;
			echos[i].active=1;
			echos[i].lastTime=worldClock;
			echos[i].delayTime=(((Sound_Effect[sfxH].timeLength<<8)/100)*delayTime)>>8;
			echos[i].deltaDelayTime=deltaDelayTime;
			echos[i].volume=Sound_Effect[sfxH].volume;
			echos[i].deltaVolume=deltaVolume;
			echos[i].pitch=Sound_Effect[sfxH].note;
			echos[i].deltaPitch=deltaPitch;
			return;
			}
		}
}								

int	sfxTrigOn( int sfxH, int numMod, ... )
{
va_list				arg_ptr;
SpuVoiceAttr		s_attr;
int					V;
int					i;
int					sampleH;
int					sampleAdd;
int					sampleSize;
int					pitch;
int					modType;
int					modValue;
	
	//printf( "TRIG IT\n" );
	
	//printf( "SFXH %d PARTIAL %s\n", sfxH, partials[Sound_Effect[sfxH].sampleHandle].name );
	
	if( masterVol==0 ) return -1;
	
	if( currentScope==2 )
		{
		if( Sound_Effect[sfxH].scope<currentScope ) return -1;
		}
	sampleH=Sound_Effect[sfxH].sampleHandle;
	if( sampleH==-1 )
 		{
		//printf( "SFX=%d\t%s partial unavailable\n", sfxH, Sound_Effect[sfxH].name );
		return -1;
		}
	
	//if(( sfxH>=CROWD1 )&&( sfxH<=BACROWD ))
	//	{
	//	if( totalCrowd>5 ) return -1;
	//	totalCrowd++;
	//	}
	
	V=spuNextFreeVoice();
	//printf( "%d\t", sfxH );
	
	if( V==-1 ) 
		{
//		printf( "**** No free voices **** \n" );
		return -1;
		}
	//if( V>13 ) printf( "V = %d  sfxH = %d\n", V, sfxH );
	s_attr.mask = ( SPU_VOICE_WDSA
						|SPU_VOICE_VOLL
						|SPU_VOICE_VOLR
						|SPU_VOICE_VOLMODEL
						|SPU_VOICE_VOLMODER
						|SPU_VOICE_SAMPLE_NOTE
						|SPU_VOICE_NOTE
						|SPU_VOICE_ADSR_AMODE
						|SPU_VOICE_ADSR_SMODE
						|SPU_VOICE_ADSR_RMODE
						|SPU_VOICE_ADSR_AR 
						|SPU_VOICE_ADSR_DR 
						|SPU_VOICE_ADSR_SR 
						|SPU_VOICE_ADSR_RR 
						|SPU_VOICE_ADSR_SL );

	s_attr.volmode.left	= SPU_VOICE_DIRECT;
	s_attr.volmode.right	= SPU_VOICE_DIRECT;
	s_attr.a_mode      	= SPU_VOICE_LINEARIncN;
	s_attr.ar          	= Sound_Effect[sfxH].ar;
	s_attr.dr          	= Sound_Effect[sfxH].dr;
	s_attr.s_mode      	= SPU_VOICE_LINEARIncN;
	s_attr.sr          	= Sound_Effect[sfxH].sr;
	s_attr.sl          	= Sound_Effect[sfxH].sl;
	s_attr.r_mode      	= SPU_VOICE_LINEARDecN;
	s_attr.rr          	= Sound_Effect[sfxH].rr;
								
	voices[V].volume=Sound_Effect[sfxH].volume;
	voices[V].pan=Sound_Effect[sfxH].pan;
	
	voices[V].startTime=worldClock;
	voices[V].sfx=sfxH;
	voices[V].pitchEnv=0;
	voices[V].mute=0;
	voices[V].note=Sound_Effect[sfxH].note<<2;
	
	// Modifier section / additional data may be passed to modify parameters at Trigger time
	if( numMod>0 )
		{
		va_start( arg_ptr, numMod );
		for( i=0; i<numMod; i++ )
			{
			modType=va_arg(arg_ptr, int );
			modValue=va_arg(arg_ptr, int );
			if( modType==FIXVOLUME ) voices[V].volume=modValue;
			if( modType==DELTAVOLUME ) voices[V].volume+=modValue;
			if( modType==FIXPITCH ) voices[V].note=(modValue<<2);
			if( modType==DELTAPITCH ) voices[V].note+=(modValue<<2);
			if( modType==FIXPARTIAL )
				{
				sampleH=modValue;
				}
			}
		va_end(arg_ptr);
		}

	sampleAdd=partials[sampleH].addr;
	sampleSize=partials[sampleH].size;
	s_attr.addr=sampleAdd;
	//printf( "sample addres %d\n", sampleAdd );
	//printf( "sample handle %d\n", sampleH );
	//printf( "size %d\n", sampleSize );
	if( Sound_Effect[sfxH].timeLength==-1 ) voices[V].timeLength=sampleSize/253;
	else	voices[V].timeLength=Sound_Effect[sfxH].timeLength;
	
	if( Sound_Effect[sfxH].panMode==0 )
		{
		masteredPannedVolume( voices[V].volume, voices[V].pan, &s_attr.volume.left, &s_attr.volume.right );		
		}
	else
		{
		s_attr.volume.left=0;
		s_attr.volume.right=0;
		}
	s_attr.note=((UNITY+(voices[V].note/4))<<8)+((voices[V].note%4)*64);
	s_attr.sample_note=UNITY<<8;
	s_attr.voice=voices[V].voice;
	SpuSetKeyOnWithAttr (&s_attr);
	
	if( Sound_Effect[sfxH].reverb==1 ) SpuSetReverbVoice( SpuOn, voices[V].voice );
	
	return V;
}

void	sfxVoiceOff( int V )
{
SpuVoiceAttr		s_attr;
	
	//if(( voices[V].sfx>=CROWD1 )&&( voices[V].sfx<=BACROWD ))
   //		{
	//	totalCrowd--;
	//	}
	
	s_attr.mask = ( SPU_VOICE_WDSA
						|SPU_VOICE_ADSR_AR 
						|SPU_VOICE_ADSR_DR 
						|SPU_VOICE_ADSR_SR 
						|SPU_VOICE_ADSR_RR 
						|SPU_VOICE_ADSR_SL );

	s_attr.ar      =	0;
	s_attr.dr      =	0;
	s_attr.sr      =	0;
	s_attr.sl      = 	0;
	s_attr.rr      =	0;
	s_attr.addr		=	NULL;
	s_attr.voice	=	voices[V].voice;
	SpuSetVoiceAttr( &s_attr );

	voices[V].active=-1;
	voices[V].startTime=0;
	voices[V].timeLength=0;
	voices[V].mute=0;
	voices[V].sfx=-1;
	voices[V].volume=0;
	voices[V].pan=0;
	voices[V].pitchEnv=-1;
	voices[V].note=0;

	numberOfVoicesFree++;
}

void	sfxKeyOff( int V )
{
	if( V==-1 ) return;
	SpuSetKey( SpuOff, voices[V].voice );
	voices[V].active=0;
}

void	sfxTrigOff( int V )
{
	if( V==-1 ) return;
	SpuSetKey( SpuOff, voices[V].voice );
	SpuSetReverbVoice( SpuOff, voices[V].voice );
	sfxVoiceOff( V );
}

void	sfxAllVoicesOff( void )
{
int	i;
int	delay;
	
	for( i=0; i<MAXVOICES; ++i )
		{
		sfxTrigOff( i );
		for( delay=0; delay<0xffff; ++delay ) {};
		}
	numberOfVoicesFree=MAXVOICES;
}

void	sfxPanControl( int V, int elapsed )
{
int					length;
int					adjustedPan=0;
int					calc;
SpuVoiceAttr		s_attr;
short int			L,R;
short					envx;
int					pitch;
int					panMode;

	if( voices[V].mute>1 ) return; // done mess the volume if the voice is muted
	
	panMode=Sound_Effect[voices[V].sfx].panMode;
	if( panMode==0 ) return;

	length=Sound_Effect[voices[V].sfx].timeLength;	
	
	if( elapsed<0  ) return;

	if(( panMode>0 )&&( panMode<3 ))
		{
		if( length<=0  ) return;
		calc=(64<<11)/length;
		adjustedPan=(elapsed*calc)>>11;
		}
	
	if(( panMode>2 )&&( panMode<5 ))
		{
		SpuGetVoiceEnvelope ( V, &envx );
		adjustedPan=envx/512;
		}

	if( adjustedPan>64 ) adjustedPan=64;
	if( adjustedPan<0 ) adjustedPan=0;
	
	if(( panMode==2 )||( panMode==4 )) adjustedPan=64-adjustedPan;
	
	adjustedPan+=voices[V].pan;
	adjustedPan=adjustedPan%128;
	if( adjustedPan>64 ) adjustedPan=128-adjustedPan;
	
//#if	AUDIODEV
//	if( sfxEdit ) PN( 48+(V*24)+(adjustedPan/4), 24, adjustedPan );
//#endif
	
	if( voices[V].mute==0 ) masteredPannedVolume( voices[V].volume, adjustedPan, &L, &R );		
	if( voices[V].mute==1 ) masteredPannedVolume( voices[V].volume/2, adjustedPan, &L, &R );		
	s_attr.mask = 
	(	SPU_VOICE_VOLL				|
		SPU_VOICE_VOLR
	);
	s_attr.volume.left  = L;
	s_attr.volume.right = R;
	s_attr.voice		  = voices[V].voice;
	SpuSetVoiceAttr( &s_attr );	
}

void	sfxPitchControl( int V, int elapsed )
{
SpuVoiceAttr		s_attr;
int					pitch=0;
int					calc;
int					attackLength,
						attackTarget,
						decayLength,
						decayTarget;
int					sfx;

	sfx=voices[V].sfx;

	attackLength=Sound_Effect[sfx].apt;				// attack pitch time
	attackTarget=Sound_Effect[sfx].app;				// attack pitch target level
	decayLength=Sound_Effect[sfx].dpt;				// decay pitch time
	decayTarget=Sound_Effect[sfx].dpp;				// decay pitch target level

	if( voices[V].pitchEnv==2 ) return;
	if( elapsed>attackLength ) voices[V].pitchEnv=1;
	if( elapsed>attackLength+decayLength ) voices[V].pitchEnv=2;

	if( voices[V].pitchEnv==0 )
		{
		calc=(attackTarget<<10)/attackLength;
		pitch=(elapsed*calc)>>10;
		}

	if( voices[V].pitchEnv==1 )
		{
		calc=(decayTarget-attackTarget<<10)/decayLength;
		pitch=((elapsed-attackLength)*calc)>>10;
		pitch+=attackTarget;
		}

//#if	AUDIODEV
//	if( sfxEdit ) PN( 64+(V*24), (pitch/4)+96, pitch );
//#endif
	
	s_attr.mask = ( SPU_VOICE_SAMPLE_NOTE
						|SPU_VOICE_NOTE );
			
	s_attr.sample_note=UNITY<<8;
	pitch=pitch+voices[V].note;
	s_attr.note	 =	((UNITY+(pitch/4))<<8)+((pitch%4)*64);
	s_attr.voice =voices[V].voice;
	SpuSetVoiceAttr (&s_attr);
}

// AUDIO TESTING AND EDITING

#if	AUDIODEV
void sfxSaveEdit( void )
{
int	fh;
int	i;


	fh=PCcreat( "C:\\STRIKER\\SFX\\FSFX.DAT",0);
	
	if (fh<0)
		{
//		printf("couldn't create file to save SFX\n" );
		return;
		}
	PCclose(fh);
	
	fh=PCopen( "C:\\STRIKER\\SFX\\FSFX.DAT",1,0); // perms 1 for writing
	if (fh<0)
		{
//		printf("SAVING couldn't open file SFX\n" );
		return;
		}

	PCwrite( fh, (char*)&numberOfSfx, sizeof( int ));
	
	for( i=0; i<numberOfSfx; ++i )
		{
		PCwrite( fh, (char*)&Sound_Effect[i], sizeof(SFXPROGRAM));
		}

	PCclose(fh);
}
#endif

void sfxMatchPartialsToLoaded( void )
{
int	i, z;
int	oneFound;
	
	for( i=0; i<numberOfSfx; ++i )
		{
		oneFound=0;
		for( z=0; z<numPartials; ++z )
			{
			if( strcmp( partials[z].name, Sound_Effect[i].partialName )==0 )
				{
				oneFound=1;
				Sound_Effect[i].sampleHandle=z;
				}		
			}	
		if( oneFound==0 ) Sound_Effect[i].sampleHandle=-1;
		}
}

void sfxLoadEdit( void )
{
int		i;
int		numSfxToLoad;
SFXPROGRAM *sfxp;
int		*lptr;

//	load_file( "\\SFX\\FSFX.DAT", load_buffer, LOAD_BUFF_SIZE );
	


	push(0);	
	lptr = (int *)read_datafile_alloc( SFXDAT );
	pop(0);


//	lptr = (int *) load_buffer;
	numSfxToLoad = *lptr;
	lptr ++;
	sfxp = (SFXPROGRAM *) lptr;
	for (i=0; i<numSfxToLoad; ++i )
		{
		memcpy ((char*)&Sound_Effect[i],(char *)sfxp,sizeof(SFXPROGRAM));
		++sfxp;
		}

	sfxMatchPartialsToLoaded();
}

#if	AUDIODEV
void	sfxEditor( void )
{
static int				CurrentField=0;
short int*				alter=0;
char*						charP;
int						Top=0;
int						Bot=0;
short int				value;
int						i;
static int				Mode=0;	// edit sfx, 1 edit sfx name
int						usepad;
static SFXPROGRAM		pasteBuffer;
static int				pasteActive;

//static int	lastSfxVol=127;
//static int	lastMasterVol=127;
	
	if( worldClock<10 ) return;
	usepad=joypad_d[PAD_ALL];

//	MASTER VOLUME CONTROL TEST, PASSED OK	
//	if( usepad & PAD_LL ) sfxVol--;
//	if( usepad & PAD_LR ) sfxVol++;
//	if( usepad & PAD_LU ) masterVol++;
//	if( usepad & PAD_LD ) masterVol--;
//	if( sfxVol>127 ) sfxVol=127;
//	if( sfxVol<0 ) sfxVol=0;
//	if( masterVol>127 ) masterVol=127;
//	if( masterVol<0 ) masterVol=0;
//	
//	if( usepad & PAD_FRT ) speakerMode=MONO;
//	if( usepad & PAD_FRB ) speakerMode=STEREO;
//	
//	if(( sfxVol!=lastSfxVol )||( masterVol!=lastMasterVol ))
//		{
//		lastMasterVol=masterVol;
//		lastSfxVol=sfxVol;
//		musicVol=127-sfxVol;
//		sfxSetMainVolumes( masterVol, musicVol );
//		}
//
//	PS( 32, 56, "LOUDER" );
//	PS( 32, 136, "QUIETER" );
//	PS( 24, 72, "M\nU\nS\nI\nC" );
//	PS( 104, 72, "E\nF\nF\nE\nC\nT\nS" );
//
//	PS( 32+(sfxVol/2), 128-(masterVol/2), "*" );
//
//	if( speakerMode==MONO )	PS( 32, 200, "MONO" );
//	if( speakerMode==STEREO )	PS( 32, 200, "STEREO" );
//
//	return;

	//for( i=0; i<MAXVOICES; ++i )
		//{
		//PN( 64+(i*24), 32, i );
		//PN( 64+(i*24), 40, envx/129 );
		//PN( 64+(i*24), 48, voices[V].mute );
	//	}

 	SpecialP( 128, 24, " JOHNS DOODY SOUND PAGE " );
	
	if( Mode==2 )
		{
		PS( 128, 128, "TRIANGLE = QUIT" );
		PS( 128, 138, "CROSS    = LOAD" );
		PS( 128, 148, "CIRCLE   = SAVE" );
				
		if( usepad & PAD_RR )
			{
			sfxSaveEdit();
			Mode=0;
			}
		
		if( usepad & PAD_RD )
			{
			sfxLoadEdit();
			Mode=0;
			}
		
		if( usepad & PAD_RU )
			{
			Mode=0;
			}
		return;
		}

	if( joypad[PAD_ALL] & PAD_FLT )
		{
		PS( 128, 224, "KEY ROLLER" );
		usepad=joypad[PAD_ALL];
		}
	
	if( Mode==0 )
		{
		if( usepad & PAD_RU )
			{
			Mode=2;
			}
		
		if( pasteActive )
			{
			PS( 128, 128, "Paste Loaded - SQUARE=CANCEL CROSS=PASTE" );
		
			if( usepad & PAD_RD )
				{
				Sound_Effect[sfxTest]=pasteBuffer;
				pasteActive=0;
				}
			if( usepad & PAD_RL )
				{
				pasteActive=0;
				}
			}
		else
			{
			if( usepad & PAD_RD )
				{
				pasteBuffer=Sound_Effect[sfxTest];
				pasteActive=1;
				}
			}

		if( usepad & PAD_FLB )
			{
			PS( 128, 128, "KILL KILL KILL" );
			sfxAllVoicesOff();
			}
		
		PN( 16, 34,  sfxTest );
		PS( 16, 44, Sound_Effect[sfxTest].name );
		if( usepad & PAD_FRT ) sfxTest++;
		if( usepad & PAD_FRB ) sfxTest--;
		if( sfxTest<0 ) sfxTest=0;
		if( sfxTest==numberOfSfx ) sfxTest=numberOfSfx-1;
		
		if( usepad & PAD_LU ) CurrentField--;
		if( usepad & PAD_LD ) CurrentField++;
		if( CurrentField<0 )
			{
			CurrentField=0;
			Mode=1;
			return;
			}

		if( CurrentField==17 ) CurrentField=16;
		
		//if( usepad & PAD_RL ) sfxSpot( "WHISTLE4", 20 );
		
		if( usepad & PAD_RL ) sfxTrigOn( sfxTest, 0 );
		//if( usepad_d & PAD_RL ) sfxEchoOn( HITPOST, 50, 75, 60, 0 );
		
		S( "ATACK" );N( Sound_Effect[sfxTest].ar );	
		S( "DECAY" );N( Sound_Effect[sfxTest].dr );
		S( "SUS-R" );N( Sound_Effect[sfxTest].sr );
		S( "SUS-L" );N( Sound_Effect[sfxTest].sl );
		S( "RELES" );N( Sound_Effect[sfxTest].rr );
		S( "P-ATT" );N( Sound_Effect[sfxTest].apt );
		S( "P-ATL" );N( Sound_Effect[sfxTest].app );
		S( "P-DET" );N( Sound_Effect[sfxTest].dpt );
		S( "P-DEL" );N( Sound_Effect[sfxTest].dpp );
		S( " TIME" );N( Sound_Effect[sfxTest].timeLength );
		if( Sound_Effect[sfxTest].sampleHandle==-1 ) S( "NOT AVAILABLE" );
		else S( Sound_Effect[sfxTest].partialName );
		S( "  VOL" );N( Sound_Effect[sfxTest].volume );
		S( "  PAN" );N( Sound_Effect[sfxTest].pan );
		S( panModes[Sound_Effect[sfxTest].panMode ] );
		S( "RVERB" );N( Sound_Effect[sfxTest].reverb );
		S( sfxScopeNames[Sound_Effect[sfxTest].scope ] );
		S( "B-NOT" );N( Sound_Effect[sfxTest].note );
		
		PS( 128, 54+(CurrentField*10), "<<" );
		
		switch( CurrentField )
			{
			case 0 :	// ATACK
				{
				Top=127;
				Bot=0;
				alter=&Sound_Effect[sfxTest].ar;
				break;
				}
			case 1 : // DECAY
				{
				Top=127;
				Bot=0;
				alter=&Sound_Effect[sfxTest].dr;
				break;
				}
			case 2 : // SUS-R
				{
				Top=127;
				Bot=0;
				alter=&Sound_Effect[sfxTest].sr;
				break;
				}
			case 3 :	// SUS-L
				{
				Top=15;
				Bot=0;
				alter=&Sound_Effect[sfxTest].sl;
				break;
				}
			case 4 : // RELES
				{
				Top=31;
				Bot=0;
				alter=&Sound_Effect[sfxTest].rr;
				break;
				}
			case 5 : // APT
				{
				Top=0xffff;
				Bot=0;
				alter=&Sound_Effect[sfxTest].apt;
				break;
				}
			case 6 : // APP
				{
				Top=256;
				Bot=-256;
				alter=&Sound_Effect[sfxTest].app;
				break;
				}
			case 7 : // DPT
				{			
				Top=0xffff;
				Bot=0;
				alter=&Sound_Effect[sfxTest].dpt;
				break;
				}
			case 8 : // DPP
				{
				Top=256;
				Bot=-256;
				alter=&Sound_Effect[sfxTest].dpp;
				break;
				}
			case 9 : // TimeLength
				{
				Top=0xffff;
				Bot=-1;
				alter=&Sound_Effect[sfxTest].timeLength;
				break;
				}
			case 10 : // Sample Partial
				{
				Top=numPartials-1;
				Bot=0;
				alter=&Sound_Effect[sfxTest].sampleHandle;
				break;
				}
			case 11 : // Volume
				{
				Top=127;
				Bot=0;
				alter=&Sound_Effect[sfxTest].volume;
				break;
				}
			case 12 : // Pan
				{
				Top=64;
				Bot=0;
				alter=&Sound_Effect[sfxTest].pan;
				break;
				}
			case 13 : // Pan Mode
				{
				Top=4;
				Bot=0;
				alter=&Sound_Effect[sfxTest].panMode;
				break;
				}
			case 14 : // Reverb
				{
				Top=1;
				Bot=0;
				alter=&Sound_Effect[sfxTest].reverb;
				break;
				}
			case 15 : // Scope
				{
				Top=2;
				Bot=0;
				alter=&Sound_Effect[sfxTest].scope;
				break;
				}
			case 16 : // note
				{
				Top=127;
				Bot=-127;
				alter=&Sound_Effect[sfxTest].note;
				break;
				}
			break;
			}
		
		value=*alter;	
		
		if( usepad & PAD_LL )
			{
			value--;
			if( value<Bot ) value=Bot;		
			*alter=value;		
			strcpy( Sound_Effect[sfxTest].partialName, partials[Sound_Effect[sfxTest].sampleHandle].name );
			}
		if( usepad & PAD_LR )
			{
			value++;
			if( value>Top ) value=Top;		
			*alter=value;
			strcpy( Sound_Effect[sfxTest].partialName, partials[Sound_Effect[sfxTest].sampleHandle].name );
			}
		}	// end of Mode==0
	else
		{
		if( worldClock%4<2 ) PN( 16, 34,  sfxTest );
		else	PS( 16, 44, Sound_Effect[sfxTest].name );
		
		PS( (CurrentField*8)+16, 52, "<" );
		charP=&Sound_Effect[sfxTest].name[CurrentField];
		if(( usepad & PAD_LU )&&((*charP)<94)) (*charP)++;
		if(( usepad & PAD_LD )&&((*charP)>32)) (*charP)--;
		if( usepad & PAD_LL ) CurrentField--;
		if( usepad & PAD_LR ) CurrentField++;
		if( CurrentField<0 )
			{
			CurrentField=0;
			Mode=0;
			return;
			}

		if( CurrentField==strlen( Sound_Effect[sfxTest].name )) CurrentField--;
		}
}
#endif

void	sfxMuteVoice( int V, int flag )
{
SpuVoiceAttr		s_attr;
short int			L,R;
	
	voices[V].mute=flag;
	
	// Mute
	if( flag==2 ) masteredPannedVolume( 0, voices[V].pan, &L, &R );		
	
	// Duck
	if( flag==1 ) masteredPannedVolume( voices[V].volume/2, voices[V].pan, &L, &R );		
	
	// un-Mute/Duck
	if( flag==0 ) masteredPannedVolume( voices[V].volume, voices[V].pan, &L, &R );		
	
	s_attr.mask = 
	(	SPU_VOICE_VOLL	|
		SPU_VOICE_VOLR
	);
	s_attr.volume.left  = L;
	s_attr.volume.right = R;
	s_attr.voice		  = voices[V].voice;
	SpuSetVoiceAttr( &s_attr );	
}

void	sfxScriptOff( int i )
{
	scripts[i].active=0;
	scripts[i].scriptP=(short int*)-1;
	scripts[i].startTime=-1;
}

int	sfxScriptOn( short int* script )
{
int	i;

	for( i=0; i<MAXSCRIPTS; ++i )
		{
		if( scripts[i].active==0 )
			{
			scripts[i].active=1;
			scripts[i].scriptP=script;
			scripts[i].startTime=worldClock;
			return i;
			}
		}
	// failed to find a free script handler ( MAXSCRIPTS increase if it's a problem )

	#if DEBUG
	printf( "SFX WARNING - no free script struct\n" );
	#endif
	return 0;
}								

void	sfxProcessEchos( void )
{
int				elapsed;
int				i;

	for( i=0; i<MAXECHOS; ++i )
		{
		if( echos[i].active==1 )
			{
			elapsed=worldClock-echos[i].lastTime;
	
			if( elapsed>echos[i].delayTime )
				{
				sfxTrigOn( echos[i].sfxH, 2, FIXVOLUME, echos[i].volume, FIXPITCH, echos[i].pitch );	
				echos[i].volume=(((echos[i].volume<<8)/100)*echos[i].deltaVolume)>>8;
				echos[i].pitch+=echos[i].deltaPitch;
				echos[i].delayTime=(((echos[i].delayTime<<8)/100)*echos[i].deltaDelayTime)>>8;
				echos[i].lastTime=worldClock;
				if(( echos[i].volume<=0 )||( echos[i].volume>127 )) echos[i].active=0;
				if( numberOfVoicesFree<5 )
					{
					echos[i].active=0;
					//printf( "Echo Cancelled\n" );
					}
				}
			}
		}
}

void	sfxProcessScripts( void )
{
int				elapsed;
int				i;
int				handle, volume;

	for( i=0; i<MAXSCRIPTS; ++i )
		{
		if( scripts[i].active==1 )
			{
			elapsed=worldClock-scripts[i].startTime;
	
			while( elapsed>=(int)*scripts[i].scriptP )
				{
				++scripts[i].scriptP;
				handle=(int)*scripts[i].scriptP;
				++scripts[i].scriptP;
				volume=(int)*scripts[i].scriptP;
				
				sfxSpotR( handle, volume );	
				
				++scripts[i].scriptP;
				if( *scripts[i].scriptP==(short int)-1 )
					{
					scripts[i].active=0;
					return;
					}
				}	
			}
		}
}

void	sfxProcessVoices( void )
{
int					V;
int					i;
int					elapsedTime;
SpuVoiceAttr		s_attr;
short					envx;
int					timeLength;
int					scopeOn;
int					time;
//static int			lastReplayed=0;
static int			lasttime;
int					testCrowd;

//int					ox;
//int					oy;
//int					dx=0; 
//int					dy=0;
//
//	ox=ballxpos/4;
//	oy=ballypos/4;
//
//	if( joypad[PAD_ALL] & PAD_LU ) dx=-1;
//	if( joypad[PAD_ALL] & PAD_LR ) dy=1;
//	if( joypad[PAD_ALL] & PAD_LD ) dx=1;
//	if( joypad[PAD_ALL] & PAD_LL ) dy=-1;
//	draw_line( (0<<16)|(0<<8)|255, ox, oy, 0, ox+(dx*metre), oy+(dy*metre), 0 );

	
	frameCounter++;
	
	// this bit of code dealt with replay coming at different frame rates in DBZ
	// may come in useful again
	//if( playing_replay )
	//	{
	//	if( lastReplayed==0 )
	//		{
	//		lastReplayed=1;
  	//			sfxSetMainVolumes( masterVol, 0 );
	//		}
	//
	//	if( frameCounter%2==0 ) worldClock++;
	//	else return;
	//		
  	//		}
  	// else worldClock++;

	
	worldClock+=2;	// modded by dc 11/10/98

	//if( FRAMERATE==60 ) worldClock/1.2;

	scopeOn=0;

	//if( playing_replay ) scopeOn=2;
	
	if( scopeOn!=currentScope )
		{
		for( V=0; V<MAXVOICES; ++V )
			{
			if( voices[V].active>-1 )
				{
				if( scopeOn<currentScope )
					{
					if( Sound_Effect[voices[V].sfx].scope<currentScope ) sfxMuteVoice( V, 0 );	// unmute
					}
				if( scopeOn>currentScope )
					{
					if( Sound_Effect[voices[V].sfx].scope<scopeOn )
						{
						sfxMuteVoice( V, scopeOn ); // you'll never understand this again - scopeOn==1 duck out 0, scopeOn==2 Mute 0 and 1 O.K.
						}
					}
				}
			}
		}
	
	currentScope=scopeOn;	
	
	for( V=0; V<MAXVOICES; ++V  )
		{
		if( voices[V].active>-1 )
			{
			SpuGetVoiceEnvelope ( V, &envx );
			
			elapsedTime=worldClock-voices[V].startTime;
			
			sfxPanControl( V, elapsedTime );
			sfxPitchControl( V, elapsedTime );
			
			timeLength=Sound_Effect[voices[V].sfx].timeLength;

			if( timeLength!=0 )
				{
				elapsedTime=worldClock-voices[V].startTime;
				
				if( elapsedTime>=voices[V].timeLength )
					{
					SpuSetKey( SpuOff, voices[V].voice );
					voices[V].active=0;
					}
				}

			if(( envx==0 )&&( voices[V].active==0 ))
				{
				sfxVoiceOff( V );
				SpuSetReverbVoice( SpuOff, voices[V].voice );
				}			
			}	
		}

	//sfxProcessEchos();
	sfxProcessScripts();		
	
	//if( iAmTalking )
	//	{
	//	if( worldClock>=stopTalkingTime )
	//		{
	//		iAmTalking=0;
	//		sfxSetMainVolumes( masterVol, musicVol );
	//		}
	//	}
	//else
	//	{
	//	if( speechQ>-1 )
	//		{
	//		sfxTalkOnOn( speechQueue[speechQ] );
	//		speechQueue[speechQ]=0;
	//		speechQ--;
	//		}
	//	}
		
	
	//if( lastReplayed )
	//	{
	//	// end of replay
	//	if( !gamevars.replaying )
	//		{
	//		//sfxSetMainVolumes( masterVol, musicVol );
	//		sfxTrigOn( ASTROPOO, 0 );
	//		sfxTrigOn( CROWD1, 0 );
	//		lastReplayed=0;
	//		}
	//	}
	
	//// timer stuff
	//time=(gamevars.gametime+(FRAMERATE>>1))/FRAMERATE;
	////if( (gamevars.quarter==3) && (time!=lasttime ))
	//if( time != lasttime )
	//	{
	//	//if((time>0)&&(time<15)) sfxTrigOn( TIMEOUT, 0 );
	//	if((time>0)&&(time<10)) sfxTrigOn( TIMEOUT, 0 );
	//	if(time==0)
	//		{
	//		sfxTrigOn( OUTOTIME, 0 );
	//		}
	//	lasttime=time;
	//	}
}

int	Sound_FadeOn( int flag )
{
static int			Stage;

	if( masterVol==0 ) return 0;

	if( flag==0 )
		{
		Stage=0;
		return 1;
		}

	if( flag==1 )
		{
		sfxSetMainVolumes( Stage*(masterVol>>4), musicVol );
		if( Stage==16 )
			{
			sfxSetMainVolumes( masterVol, musicVol );
			return 0;
			}
		Stage++;
		return 1;
		}

	return 0;
}


int	Sound_FadeOff( int flag )
{
static int			Stage;
	
	if( masterVol==0 ) return 0;

	if( flag==0 )
		{
		Stage=16;
		return 1;
		}

	if( flag==1 )
		{
		sfxSetMainVolumes( Stage*(masterVol>>4), musicVol );
		
		if( Stage==0 ) 
			{
			sfxSetMainVolumes( 0, musicVol );
			return 0;
			}

		Stage--;
		return 1;
		}
	return 0;
}

// blocking
void	sfxFadeAllSoundOff( void )
{
int	i=0;
int	Fading;
	
	Fading=1;
	Sound_FadeOff( 0 );
	for( i=0; i<17; ++i )
		{
		if( Fading ) Fading=Sound_FadeOff( 1 );		
		}
}

void	sfxFadeAllSoundOn( void )
{
int	i=0;
int	Fading;
	
	Fading=1;
	Sound_FadeOn( 0 );
	for( i=0; i<17; ++i )
		{
		if( Fading ) Fading=Sound_FadeOn( 1 );		
		}
}


void	sfxLoadPartials()
{
int			fileLength;
char			*bankName;
int			*pFile;
PARTIALTYPE	*bufferDat;
char			*bufferVbs;
char*			z;
int			numPartBank;
int			i;
int			n;
int			fp;
int			spuRamUsed=0;
	
//	printf( "Max SPU Ram Size Available %d\n", MAXSPURAM/1024 );
	
	SpuSetTransferMode( SpuTransByDMA );
	numPartials=0;
	push(0);	
	bufferDat = (PARTIALTYPE *)read_datafile_alloc( FRONTDAT );
	
	//fileLength = file_header.file[ FRONTDAT ].size;
	fileLength = 160;
	fileLength = file_header.file[ FRONTDAT2 ].size; 
	
	numPartBank=fileLength/sizeof(PARTIALTYPE);
	//printf( "file size=%d\n", fileLength );
	//printf( "number of partials in this bank=%d\n", numPartBank );
	
	bufferVbs = (char *)read_datafile_alloc( FRONTVBS );
	//fileLength = file_header.file[ FRONTVBS ].size;
	fileLength = 23776;
	fileLength = file_header.file[ FRONTVBS2 ].size; 

	

	spuRamUsed+=fileLength;
	if( spuRamUsed>=MAXSPURAM )
		{
		#if DEBUG
		printf( "out of SPU (sound) RAM in sfxLoadPartials" );
		#endif
		}
	//printf( "file size=%d\n", fileLength );

	for( n=0; n<numPartBank; ++n )
		{
		partials[numPartials].size=bufferDat->size;
		strcpy( partials[numPartials].name, bufferDat->name );
		//printf( "%d\t%d\t%d\t%s\n", numPartials, partials[numPartials].size, partials[numPartials].addr, partials[numPartials].name );
		partials[numPartials].addr=SpuMalloc( partials[numPartials].size );
		//printf( "Sample %d  Address %d Length  %d\n", numPartials, partials[numPartials].addr, partials[numPartials].size );
		SpuSetTransferStartAddr( partials[numPartials].addr );
		SpuWrite( (char*)bufferVbs, partials[numPartials].size );
		SpuIsTransferCompleted( SPU_TRANSFER_WAIT );
		bufferVbs+=bufferDat->size;
		bufferDat++;
		numPartials++;
		if( numPartials>=MALLOCMAX )
			{
			#if DEBUG
			printf( "You need to allocate more MALLOCMAX for sample partials\n" );
			printf( "Execution terminated, sound.c\n" );
			exit( 1 );
			#endif
			}
		}
	
	//printf( "Sound Ram Used K=%d\n", spuRamUsed/1024 );		
	pop(0);

}

void	initSfx( void )
{
int	i;

	for( i=0; i<MAXSFX; ++i )
		{
		Sound_Effect[i].sampleHandle	= 0;	
		Sound_Effect[i].volume			= 100;			
		Sound_Effect[i].pan				= 32;				
		Sound_Effect[i].panMode			= 0;			
		Sound_Effect[i].timeLength		= -1;		
		Sound_Effect[i].reverb			= 0;			
		Sound_Effect[i].ar				= 0;				
		Sound_Effect[i].dr				= 0;				
		Sound_Effect[i].sr				= 0;				
		Sound_Effect[i].sl				= 0; 				
		Sound_Effect[i].rr				= 0;				
		Sound_Effect[i].apt				= 0;				
		Sound_Effect[i].app				= 0;				
		Sound_Effect[i].dpt				= 0;				
		Sound_Effect[i].dpp				= 0;				
		Sound_Effect[i].scope			= 0;				
		Sound_Effect[i].note				= 0;				
		strcpy( Sound_Effect[i].name, "NONAME  " );
		}
	numberOfSfx=MAXSFX;
}

void	initScripts( void )
{
int	i;

	for( i=0; i<MAXSCRIPTS; ++i )
		{
		scripts[i].active			= 0;
		scripts[i].startTime		= 0;	  
		scripts[i].scriptP		= NULL;
		}  
}

void	initEchos( void )
{
int	i;

	for( i=0; i<MAXECHOS; ++i )
		{
		echos[i].active=0;			
		}  
}

void	sfxSpuCleanUp( void )
{
int	i;
	
	for( i=0; i<numPartials; ++i )
		{
		SpuFree( partials[i].addr );
		partials[i].addr=0;
		}
}

void	sfxRestart( void )
{
	sfxFadeAllSoundOff();
	initScripts();
	initEchos();
	sfxAllVoicesOff();
	sfxSpuCleanUp();
	//sfxLoadPartials( 5, "CROWD", vlangs[gdptr->language], "ACTION", "FIGHT", arenaptr->model_file );										
	sfxMatchPartialsToLoaded();
	sfxFadeAllSoundOn();
}

void sfxShutUp( void ) 
{
	sfxFadeAllSoundOff();
	sfxAllVoicesOff();
	sfxSpuCleanUp();
}

void	sfxSetMasterVolume( int masVol )
{
	masterVol=masVol;
	sfxSetMainVolumes( masterVol, musicVol );
}

void	sfxSetMusicVolume( int musVol )
{
	musicVol=musVol;
	sfxSetMainVolumes( masterVol, musicVol );
}

void	sfxSetSfxVolume( int sVol )
{
	sfxVol=sVol;
}

void	sfxSliderNoise( int sfxSlider )
{
static int	sliderVoice=-1;

	if( sfxSlider )
		{
		if( sliderVoice==-1 ) sliderVoice=sfxTrigOn( SLIDLOOP, 0 );
		}
	else
		{
		if( sliderVoice!=-1 )
			{
			sfxTrigOff( sliderVoice );
			sliderVoice=-1;
			}
		}
}

void	initsound( int masVol, int musVol )
{
	
	// HARDWARE HAS BEEN INIT PREVIOUSLY

	masterVol=masVol;
	musicVol=musVol;

	sfxVol=127;
	
	sfxLoadPartials();	// load samples to spu ram
	sfxInitVoices();																// initalise the voices
	sfxSetMainVolumes( masterVol, musicVol );
	sfxInitReverb();																// init the reverb
	initSfx();
	sfxLoadEdit();
	initScripts();
	initEchos();
	numberOfVoicesFree=MAXVOICES;
	
	#if DEBUG
	printf( "init sound O.K.\n" );
	#endif
}

// Added by SW 23.6.98 //
void	init_spu( void )
{
	SpuInit();																		// init Spu
	SpuInitMalloc( MALLOCMAX, spu_malloc_rec );							// setup spu malloc table
}

// END sound.c









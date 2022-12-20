/************************************************************/
/*																				*/
/*	all dependant defs  													*/
/*																				*/
/************************************************************/

typedef signed char BYTE;
typedef unsigned char UBYTE;
typedef signed short WORD;
typedef unsigned short UWORD;
typedef signed long LONG;
typedef unsigned long ULONG;
typedef unsigned int UINT32;
typedef signed int INT32;

//#define MASTER

#ifndef MASTER
#define	DEBUG
#else


#endif


//#define	NTSC

#define	TRUE	-1
#define	FALSE	0

#define	_K *1024
#define	_L <<16
#define BIT(n)	(1<<n)
#define	ABS(v) v>0?v:-v

/* ot size 

 bits		entries
	0		0
	1		2
	2		4
	3		8
	4		16
	5		32
	6		64
	7		128
	8		256
	9		512
	10		1024
	11		2048
	12		4096
	13		8192
	14		16384



*/

#define	OT_SIZE					9
#define	OT_SHIFT					( 15 - OT_SIZE )
#define	ORDERING_TABLE_SIZE	( 1 << OT_SIZE )
#define	NEAREST_DEPTH			1
#define	FURTHEST_DEPTH	  		ORDERING_TABLE_SIZE-1




#ifdef NTSC
#define	SCREEN_HEIGHT	240
#define	VIDEO_MODE	0
#define	DEFAULT_SCREEN_X_OFFSET	0
#define	DEFAULT_SCREEN_Y_OFFSET	0
#else
#define	SCREEN_HEIGHT	256
#define	VIDEO_MODE		1
#define	DEFAULT_SCREEN_X_OFFSET	0
#define	DEFAULT_SCREEN_Y_OFFSET	16
#endif



#define	FB1X	0
#define	FB1Y	0
#define	FB2X	0
#define	FB2Y	256


#define	SCREEN_WIDTH	512
#define	SCREEN_Z			( SCREEN_WIDTH >> 1 )

#define MAX_INPUT_HOLES			2
#define MAX_PADS_PER_MULTITAP	4
#define TOTAL_PADS				( MAX_INPUT_HOLES * MAX_PADS_PER_MULTITAP )
#define MISSING_TAP_0			BIT(0)
#define MISSING_TAP_1			BIT(1)
#define MISSING_PAD				BIT(0)
#define IN_TAP						BIT(1)
#define PADOUT						BIT(0)

/************************************************************/
/*	pad equates																*/
/************************************************************/
#define PAD_UP     	PAD_LU
#define PAD_DOWN   	PAD_LD
#define PAD_LEFT   	PAD_LL
#define PAD_RIGHT  	PAD_LR
#define PAD_TRIANGLE PAD_RU
#define PAD_CROSS	 	PAD_RD
#define PAD_SQUARE	PAD_RL
#define PAD_CIRCLE	PAD_RR

#define PAD_PAUSE   	PAD_SEL
#define PAD_SELECT  	PAD_SEL
#define PAD_TR      	PAD_FRT
#define PAD_TR2     	PAD_FRB
#define PAD_TL      	PAD_FLT
#define PAD_TL2     	PAD_FLB
#define PAD_PLAY    	PAD_START

#define ANYBUTTON		(PAD_TRIANGLE|PAD_CROSS|PAD_SQUARE|PAD_CIRCLE|PAD_START)
#define PAD_RESET		(PAD_TL|PAD_TL2|PAD_TR|PAD_TR2|PAD_UP|PAD_TRIANGLE)

#define	ANY_PAD_DIRECTION			( PAD_UP|PAD_RIGHT|PAD_DOWN|PAD_LEFT )

#define	ANY_PAD_BUTTON				( PAD_TRIANGLE|PAD_CIRCLE|PAD_CROSS|PAD_SQUARE )
#define	ANY_PAD_SHOULDER_BUTTON ( PAD_TL|PAD_TR|PAD_TL2|PAD_TR2 )
#define	ANY_PAD_ACTIVITY 			( ANY_PAD_DIRECTION|ANY_PAD_BUTTON )
#define	BACK							PAD_TRIANGLE
#define	SELECT						PAD_CROSS
#define	TOGGLE						1

#define	Set_rgb(s,red,green,blue)\
	s.r=red;\
	s.g=green;\
	s.b=blue;



#define	MAX_OBJECTS				20

#define	OBJ_STANDBY				BIT(0)
#define	OBJ_FREE					BIT(1)
#define	OBJ_DISPLAY				BIT(2)
#define	OBJ_TRACEGRID			BIT(3)
#define	OBJ_FAR					BIT(4)
#define	OBJ_ONSCREEN			BIT(5)
#define	OBJ_SHADOW				BIT(6)
#define	OBJ_MAT3					BIT(7)
#define	OBJ_CHILD				BIT(8)
#define	OBJ_WHEEL				BIT(9)
#define	OBJ_HOST					BIT(10)
#define	OBJ_BUGGY				BIT(11)
#define	OBJ_NOCOLLISION 		BIT(12)
#define	OBJ_ROTVECTOR			BIT(13)
#define	OBJ_NOLOD				BIT(14)

#define	PRIM_SETUP				BIT(0)
#define	PRIM_ALLOC				BIT(1)
#define	PRIM_DEALLOC  			BIT(2)


#define	MAX_TRI							1300
#define	MAX_QUAD							100
#define	MAX_VERTEX						1300
#define	MAX_CHILDREN					4


#define	MAX_FAST_COLLISION_POINTS	12
#define	MAX_COLLISION_POINTS			100

/********************************************************************/
/*	file control flags															*/
/********************************************************************/
#define	READ					0
#define	WRITE					1
#define	READ_WRITE			2

#define	FROM_START			0
#define	FROM_CURRENT		1
#define	FROM_END				2

#define	FILE_READ_ERROR	0	
#define	FILE_TOO_BIG		2	

#define	NORMAL				0
#define	BAG					1

#define	PAD_SIZE				2048
#define  Sector(s) ((s+PAD_SIZE-1)/PAD_SIZE)
enum	{ 
	ROOT_DIR=0,
	GFX_DIR,
	SFX_DIR,
	MODEL_DIR };

#define	OK 			1
#define	NOT_OK		0

#define M_IDENTITY {{{4096,0,0},{0,4096,0},{0,0,4096}},{0,0,0}}
#define M_CLEAR {{{0,0,0},{0,0,0},{0,0,0}},{0,0,0}}


/*********************************************************/
/*	timebar defs  														*/
/*********************************************************/
#define	TB_ACTIVE	1


#define	FRAMES_PER_UPDATE	2


#define	CALIBRATE			1




#define dot(a,b) (((a).vx * (b).vx) + ((a).vy * (b).vy) + ((a).vz * (b).vz))
#define VECTOR_SET(v,x,y,z) (v)->vx=x;(v)->vy=y;(v)->vz=z
#define	PHYSICS(n,obj) (*physics[n])(obj)

/************************************************************/
/*																				*/
/*	physics defs															*/
/*																				*/
/************************************************************/
#define	PI	(3.141592654)
#define	ANGULAR_VELOCITY_DAMP 98.5			// % angular velocity damping

#define	VRES_SHIFT		8						// velocity fraction bits
#define	AVRES_SHIFT		12						// angular velocity fraction bits
#define	MOMENT_SHIFT	8 						// moment of inertia shift
#define	MASS_SHIFT		8						// mass fraction bits


#define	VRES			(1<<VRES_SHIFT) 				// velocity fraction bits
#define	AVRES			(1<<AVRES_SHIFT)				// velocity fraction bits
#define	GRAVITY  	(VRES * 1)
#define	MASS			(1<<MASS_SHIFT)
#define	MOMENT 		(1 << MOMENT_SHIFT)

#define AV_PRESCALE	(ONE/(2*PI))


#define	MAX_WAVES	64


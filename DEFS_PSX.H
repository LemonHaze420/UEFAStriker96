#define	_4BIT	 0
#define	_8BIT	 1
#define	_16BIT 2

#define	Poly_f3		0
#define	Poly_ft3		1
#define	Poly_f4		2
#define	Poly_ft4		3
#define	Poly_g3		4
#define	Poly_gt3		5
#define	Poly_g4		6
#define	Poly_gt4		7
#define	Sprt_16		8
#define  Poly_rotft4 9


#define M_IDENTITY {{{4096,0,0},{0,4096,0},{0,0,4096}},{0,0,0}}

#define PAD_UP     	PAD_LU
#define PAD_DN 	  	PAD_LD
#define PAD_LT	   	PAD_LL
#define PAD_RT		  	PAD_LR
#define PAD_STA   	PAD_START
#define PAD_TRI		PAD_RU
#define PAD_X		 	PAD_RD
#define PAD_SQU		PAD_RL
#define PAD_CIR		PAD_RR
#define PAD_L1			PAD_FLT
#define PAD_L2			PAD_FLB
#define PAD_R1			PAD_FRT
#define PAD_R2			PAD_FRB


#if NTSC
#define TICKS_PER_SEC 60
#else
#define TICKS_PER_SEC 50
#endif


#define	LEFT_RIGHT			1
#define	RIGHT_LEFT			2
#define	MONO					( LEFT_RIGHT | RIGHT_LEFT )			
#define	STEREO				LEFT_RIGHT			


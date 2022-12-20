/************************************************************/
/*																				*/
/* liddle psx framework													*/
/*																				*/
/************************************************************/


/************************************************************/
/*																				*/
/* defs																		*/
/*																				*/
/************************************************************/


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



typedef struct
{
	float x,y,z;


}VECTORF;

typedef struct
{
	VECTOR	k;
	VECTOR	damper;

}Vspring;

typedef struct
{
	LONG	k;
	LONG	damper;

}Spring;

typedef struct
{
	WORD	x,y,z;

}Maxis;

typedef struct
{
	Maxis		xaxis;
	Maxis		yaxis;
	Maxis		zaxis;

}Rotation;

typedef struct
{
	Rotation	rotate;
	VECTOR	translate;

}RT;

typedef union
{
	MATRIX		pmat;
	RT				rmat;

}Matrix;







typedef struct
{
	WORD	x, y;

}Xy;

typedef struct
{

	ULONG			ot1[ ORDERING_TABLE_SIZE ];
	ULONG			ot2[ ORDERING_TABLE_SIZE ];
	ULONG			*available_ot[2];
	
	DRAWENV		draw[2];			/* drawing environment */
	DISPENV		disp[2];			/* display environment */

}Db;

typedef	struct
{
	WORD	lo;
	WORD	hi;
}Word_access;

typedef	struct
{
	UWORD	lo;
	UWORD	hi;
}Uword_access;

typedef union
{
	LONG			l;
	Word_access		w;

}Pos;

typedef union
{
	ULONG				l;
	Uword_access	w;

}Upos;

typedef struct
{
	LONG	type;
	WORD	width,height;
	WORD	clut[1];

}Image_data;


typedef struct
{
	UBYTE			name[16];
	Image_data	image;

}Gfx_header;

typedef struct
{
	LONG	status;
	WORD	tapslot;
	WORD	tap;

}Missing_pad;

typedef struct
{
	WORD	tap;
	Missing_pad	pad[ TOTAL_PADS ];

}Controller_gone;

typedef struct
{

	WORD	pad;
	WORD	debounce;
	WORD	status;
	WORD	refresh_status;

}Pad;

typedef struct
{
	ControllerPacket pad_buffer[2];

	LONG	current_setup;
	LONG	original_setup;
 	LONG	eh;
	Pad	pad[8];

	Pad	all;
	void	(*task)(void);


}Player_input;

typedef struct
{
	WORD	r;
	WORD	g;
	WORD	b;
	
}Rgb;


typedef struct
{

	LONG	offset;
	LONG	size;

}Bag_file;



typedef struct
{
	WORD	x,y;

}Screen_coord;



typedef struct
{
	VECTOR	pos;
	VECTOR	target;
	SVECTOR	i,j,k;
	LONG		control;
} Camera;



/******************************************************/
/*	linked list														*/
/******************************************************/
typedef struct Link
{
	struct Link	*next;	
	struct Link	*next_active;	
	struct Link	*prev_active;	

}Link;

typedef struct
{
	Link *base_chain;
	Link *free_chain;
	Link *active_chain;
	LONG size;
	LONG many;

}Chain;

/******************************************************/
/*	primitive 														*/
/******************************************************/
typedef union
{
	POLY_GT4	gt4;
	POLY_G4	g4;
	LINE_F4	lf4;
	LINE_G4	lg4;
	POLY_F4	f4;

}Quad_prim;

typedef union
{
	POLY_GT3	gt3;
	POLY_G3	g3;
	LINE_F3	lf3;
	LINE_G3	lg3;
	POLY_F3	f3;

}Tri_prim;

typedef struct Quad
{
	Link			link;
	Quad_prim	prim[2];				// biggest quad primitive
}Quad;

typedef struct Tri
{
	Link			link;
	Tri_prim		prim[2];				// biggest tri primitive
}Tri;

typedef union
{
	Tri	*tri_ptr;
	Quad	*quad_ptr;

}Prim_ptr;

typedef struct
{
	WORD	u,v;
}Uv;

typedef struct
{
	UBYTE	u,v;
}Buv;

typedef struct
{
	WORD		v1,v2,v3,v4;
//	SVECTOR	n0,n1,n2;
//	CVECTOR	c0,c1,c2,c3;

}G4_prim;

typedef struct
{
	WORD		v1,v2,v3, pad;
	SVECTOR	n0,n1,n2;
//	CVECTOR	c0,c1,c2;

}G3_prim;


typedef struct
{
	WORD		v1,v2,v3,v4;
//	CVECTOR	c0,c1,c2,c3;
	Uv		  	uv0,uv1,uv2,uv3;
	SVECTOR	n0,n1,n2,n3;
}GT4_prim;

typedef struct
{
	SVECTOR	a,b,c;

}UVMat;

typedef struct
{
	WORD		v1,v2,v3;
	Buv 	  	uv0,uv1,uv2;
	SVECTOR	n0,n1,n2;

}GT3_prim;

typedef struct
{
	WORD		v1,v2,v3;
	Buv 	  	uv0,uv1,uv2;
	UVMat		bm0,bm1,bm2;

}GT3B_prim;

typedef struct
{
	WORD		v1,v2,v3;
	Buv 	  	uv0,uv1,uv2;
	UVMat		bm0,bm1,bm2;

}GT3RB_prim;


typedef struct
{
	SVECTOR	n0,n1,n2;
	WORD		v1,v2,v3, pad;

}GT3C_prim;

typedef struct
{
	SVECTOR	n0,n1,n2;
	WORD		v1,v2,v3,pad;

}GT3G_prim;

typedef struct
{
	SVECTOR	v;
//	CVECTOR	c;
//	SVECTOR	n;
// 	Xy			t;
}Point;

typedef struct	test_model
{
	UBYTE		name[16];
	WORD		vertex_count;
	WORD		g4_count;
	WORD		g3_count;
	WORD		gt4_count;
	WORD		gt3_count;
	WORD		gt3b_count;
	WORD		gt3r_count;
	WORD		gt3br_count;
	WORD		gt3c_count;
	WORD		gt3g_count;
	WORD		gt3rb_count;
	WORD		gt3pad_count;

	SVECTOR	bound[9];

	Point			*point_list;
	G4_prim		*g4_prim;
	G3_prim		*g3_prim;
	GT4_prim		*gt4_prim;
	GT3_prim		*gt3_prim;
	GT3B_prim	*gt3b_prim;
	GT3_prim		*gt3r_prim;
	GT3C_prim	*gt3c_prim;
	GT3G_prim 	*gt3g_prim;
	GT3RB_prim 	*gt3rb_prim;

//	SVECTOR	vlist[1];
	Point		vlist[1];
}Model_info;




typedef struct
{
	Model_info *detail;

}Lod;

typedef struct
{
	SVECTOR		i,j,k;

}Axis;

typedef union
{
	G4_prim		*g4_prim;
	G3_prim		*g3_prim;
	GT4_prim		*gt4_prim;
	GT3_prim		*gt3_prim;
	GT3B_prim  	*gt3b_prim;
	GT3_prim		*gt3r_prim;
	GT3C_prim  	*gt3c_prim;
	GT3G_prim 	*gt3g_prim;
	GT3RB_prim 	*gt3rb_prim;

}Mprim;

typedef struct
{
	WORD		setup;
	WORD		count;
	Mprim		mprim;

}Pinfo;


typedef struct
{

	Model_info 	*model_info;
	WORD			set;
	WORD			vertex_count;
	WORD			g4_setup;
	WORD			g4_count;
	WORD			gt3r_setup;
	WORD			gt3c_setup;
	WORD			gt3g_setup;
	WORD			gt3rb_setup;
	WORD			gt3pad_setup;
	WORD			pad;

	Pinfo			g3_pinfo;
	Pinfo			gt3b_pinfo;

	Point			*point_list;
	G4_prim		*g4_prim;
	GT4_prim		*gt4_prim;
	GT3_prim		*gt3_prim;
	GT3_prim		*gt3r_prim;
	GT3C_prim  	*gt3c_prim;
	GT3G_prim 	*gt3g_prim;
	GT3RB_prim 	*gt3rb_prim;

//	Quad			*quad_g4;
//	Quad			*quad_last_g4;
//	Quad			*quad_gt4;
//	Quad			*quad_last_gt4;
//	Quad			*quad_lg4;
//	Quad			*quad_last_lg4;

	Tri			*tri_g3;
	Tri			*tri_last_g3;

	Tri			*tri_gt3b;
	Tri			*tri_last_gt3b;
	Tri			*tri_gt3ba;
	Tri			*tri_last_gt3ba;
	Tri			*tri_gt3bs;
	Tri			*tri_last_gt3bs;

}Model_stack;

typedef struct
{
	VECTOR	velocity;	
	VECTOR	position;
	VECTOR	offset;
	VECTOR	last_bottom_offset;
	VECTOR	last_top_offset;
	LONG		last_springforce;
	LONG		springforce;
	LONG		compression;
	Spring	*spring;


}Wheel;

typedef struct
{
	struct Object*	wheels[4];
	LONG	steer;

}Chassis;

typedef union
{
	Chassis	chassis;
	Wheel	wheel;

}Object_pocket;


typedef struct Object
{
	Link			link;
	LONG			control;
	LONG			z;
	LONG			distance_from_camera;
	WORD			current_lod;
	WORD			last_lod;

	struct Object	*pop;
	void 	 		(*task)( struct Object * );	
	Model_info	**lod;

	struct Object *children[4];
	struct Object *obj_ptr;
	LONG				mass;
	VECTOR			moment;
	RT					axis;
	SVECTOR			rotvector;
	VECTOR			angular_velocity;
	VECTOR			acceleration;
	VECTOR			velocity;
	LONG				sliding;
	LONG				static_friction;
	LONG				kinetic_friction;
	Object_pocket	pocket;

	Model_stack		model_stack;


	LONG			timer;

}Object;



typedef struct
{
	Model_info	**details;
	void			(*task)(Object *obj );

}Object_def;


typedef struct
{
//	CVECTOR	c;
	WORD		x,y;
	LONG		z;
//	Xy			t;
//	LONG		z;
}Xformed;

typedef struct
{
	SVECTOR	coord;
	SVECTOR	colour;

}Omnilight;


typedef struct
{
	LONG				control;
	WORD				x,y,h,width;
	CVECTOR			bar_col;
	CVECTOR			back_col;
	LONG				usage;

	LONG				max;
	LONG				val;
	POLY_F4			poly[2];
	POLY_F4			outline[2];


}Timebar;



typedef struct
{
	
	LONG				flag;
	LONG				frames_per_update;
	void 	 			(*vbl)( void );	
	void 	 			(*drawback)( void );	

	LONG				vstart;
	LONG				vend;
	LONG				dstart;
	LONG				dend;

	LONG				last_update;
	volatile	LONG	frame_counter, vsync_counter,
						calibrate, calibrate_base;

	volatile	LONG	draw_counter;


}Time_pocket;


typedef struct
{
	WORD	width;
	WORD	height;
	WORD	size;
	LONG	max, min;
	LONG	x_offset, z_offset;
	WORD	*grid;
	Quad	*first, *last;

}Level;

typedef struct
{
	UBYTE	string[64];
	LONG	timer;
	LONG	control;

}Event;




typedef struct
{
	WORD		r,g,b;
	WORD		speed;
	CVECTOR	col;
}Wave;

typedef struct
{
	LONG	*image_vlc;			// bitstream encoded image
	WORD	colour_depth;		// 16 or 24
	WORD	x,y;
	WORD	image_width, image_height;
	WORD	screen_xres, screen_yres;

}Bs_image;

typedef struct
{
	WORD	tpage;
	WORD	bump_tpage, bump_tpage_a, bump_tpage_s;
	WORD	ref_tpage, ref_tpage_a, ref_tpage_s, ref_tpage_g, ref_tpage_a2;
	WORD	clut,bump_clut,ref_clut;

}Gfx_vram;




#include	"include\mem.h"

#define	FRAME_RATE	1
#define	ASPECT	15								// aspec ratio * 10 



void move_camera( void );
void point_camera_by_vector(void);
void point_cam_z( void );
WORD fade( WORD *c, WORD speed, WORD max, WORD min );
void make_random_number( void );
void process_waves( void );
void init_waves( void );
void control( void );
void reset_free_mem( void );
void vram_peek( void );
void setup_screens( Db *db );
void wait_vbl( void );
void vblank( void );

void clear_vram( WORD x, WORD y, WORD w, WORD h, UBYTE r, UBYTE g, UBYTE b );

void task_planet( Object *obj );
void zero( UBYTE *s, LONG many );


void get_to_v( VECTOR *t, VECTOR *s, LONG shift, LONG speed );
void get_to_sv( SVECTOR *t, SVECTOR *s, LONG shift, WORD speed );
LONG	minmax( LONG v, LONG min, LONG max );


void rv_axis_set( WORD	x, WORD y, WORD z, RT *m1 );
void identity( RT *m );

void render( Object *obj, MATRIX *local );
void draw_objects( void );
void draw_obj( Object *obj, MATRIX *real, MATRIX *light_mat );

void create_obj_axis_yz( Object *obj, MATRIX *dest );
void create_obj_axis( Object *obj, MATRIX *dest );
void draw_shadow_gt3( Object *obj, Xformed *xlist );
WORD xform_shadow_vertex( Object *obj, MATRIX *m, Xformed *x1 );
LONG	distance( VECTOR *p1, MATRIX *p2 );
void alloc_prim( void );
Object	*spawn_object( Object_def *def, WORD set );
void do_test( void );
WORD alloc_g4( Object *obj );
void draw_g4( Object * );
void install_vram_image( void );
WORD alloc_gt4( Object *obj );
void draw_gt4( Object *obj );
WORD alloc_gt3( Object *obj );
void boxtask( Object *obj );
void testup( Object *obj );
LONG	fastdst( LONG xdist, LONG zdist );
void boxtask2( Object *obj );


WORD dealloc_gt3( Object *obj );
void bound( Object *obj );


WORD bung_image( Gfx_header *g, RECT *r );
WORD bung_clut( Gfx_header *g, RECT *r );


void initialise_model( Object_def *def );
void initialise_model_info( Model_info *mi, LONG detail );
WORD xform_vertex( Object *obj, MATRIX *m, Xformed *x1 );


void bump_mat( SVECTOR *magic, UVMat *bm, SVECTOR *v );
void show_matrix( MATRIX *m );



WORD alloc_gt3b( Object *obj );
WORD dealloc_gt3b( Object *obj );
void draw_set_gt3b( Object *obj, Xformed *xlist );
void draw_gt3b( Object *obj, Xformed *xlist );

void draw_set_gt3( Object *obj, Xformed *xlist );
void draw_gt3( Object *obj, Xformed *xlist );

WORD alloc_gt3r( Object *obj );
WORD dealloc_gt3r( Object *obj );
void draw_set_gt3r( Object *obj, Xformed *xlist );
void draw_gt3r( Object *obj, Xformed *xlist );
void getuv3( SVECTOR *n, POLY_GT3 *gt3 );

WORD alloc_gt3c( Object *obj );
WORD dealloc_gt3c( Object *obj );
void draw_set_gt3c( Object *obj, Xformed *xlist );
void draw_gt3c( Object *obj, Xformed *xlist);


WORD alloc_gt3g( Object *obj );
WORD dealloc_gt3g( Object *obj );
void draw_set_gt3g( Object *obj, Xformed *xlist );
void draw_gt3g( Object *obj, Xformed *xlist );


WORD alloc_gt3rb( Object *obj );
WORD dealloc_gt3rb( Object *obj );
void getuv3b( GT3RB_prim *p, POLY_GT3 *gt3 );
void draw_gt3rb( Object *obj, Xformed *xlist );
void draw_set_gt3rb( Object *obj, Xformed *xlist );

void draw_set_g3( Object *obj, Xformed *xlist );
void draw_g3( Object *obj, Xformed *xlist );

WORD dealloc_g3( Object *obj );
WORD alloc_g3( Object *obj );

LONG	asf( LONG *a );

void cap( WORD *v, WORD top, WORD bot );
void install_gfx( Gfx_header *gfx, Gfx_header *bump, WORD no );

void process_objects( void );
Object *get_free_object( Object *obj );
void dmode_line( Object *obj );
void dmode_line_gt4( Object *obj );
void dmode_line_gt3( Object *obj );
void check_lod( Object *obj );




/************************************************************/
/*																				*/
/* constant																	*/
/*																				*/
/************************************************************/
enum
{
	ENGLISH = 0,
	FRENCH,
	SPANISH,
	ITALIAN,
	GERMAN,
	BRAZILIAN,
};

Model_info	*mod_europe[] =
{
	
	&europe_model,
  	NULL

};

Object_def	europe_def =
{
	mod_europe,
	task_planet

};

Model_info	*mod_yank[] =
{
	&yank_model,
  	NULL

};
Object_def	yank_def =
{
	mod_yank,
	task_planet

};


Model_info	*mod_sea[] =
{
	
	&sea_model,
	NULL

};
Object_def	sea_def =
{
	mod_sea,
	task_planet

};


typedef struct
{
	VECTOR	pos;
	SVECTOR	light;
	SVECTOR	rot;

}Planet_pos;

UBYTE	*places[]=
{
	"england",
	"france",
	"spain",
	"italy",
	"germany",
	"brazil",
};

Planet_pos	planet_positions[]=
{
	
	/*********************************************************/
	/*	england																*/
	/*********************************************************/
	26624, -48127, -210444, 0,
	-3897, 101, -1792,0,
	320,0,0,0,

	/*********************************************************/
	/*	france																*/
	/*********************************************************/
	30720, -48127, -243212,0,
	-918, 280, -4253,0,
	-64,320,-288,0,

	/*********************************************************/
	/*	spain																	*/
	/*********************************************************/
	29672,-15786,-244143,0,
	1604,-2132,-3558,0,
	-626,2,400,0,


	/*********************************************************/
	/*	italy																	*/
	/*********************************************************/
	37888, -50175, -221708,0,
	-2385, 3218, -1532,0,
	-16,730,0,0,

	/*********************************************************/
	/*	germany																*/
	/*********************************************************/
	30862, -56758, -244143,0,
	-3308,-1919, -2160,0,
	318,634,192,0,


	/*********************************************************/
	/*	brazil																*/
	/*********************************************************/
	32768, -50175, -222732,0,
	-1750, 2859, -2938,0,
	-144,-1312,0,0,



};

Camera	camera =
{
	-40,-192,-1424,0,
	0,0,0,
	0,

};

WORD	aspectx = ASPECT;
WORD	aspecty = ASPECT;


/************************************************************/
/*																				*/
/* vars																		*/
/*																				*/
/************************************************************/
BYTE		*free_mem_ptr;
LONG		cpu_time;
LONG		buffer;
ULONG		*current_ot;
Object	*planet;
Pos		seed,random_number;
WORD		lang_at;
Wave	*wave;
MATRIX	real;
#define FixAspectRatiox(n)	((n * aspectx) /10)
#define FixAspectRatioy(n)	((n * aspecty) /10)



void init_language_select( void )
{
	printf("Bobs tuckshop. stop yer squabling & get choc chobbling.\n");
 	random_number.l =
	seed.l = 0x12345678;
	lang_at = 0;

	initialise_objects( );
	alloc_prim();
	point_cam_z();
	//initialise_camera( );

	install_vram_image( );
	initialise_model( &europe_def );
	initialise_model( &yank_def );
	initialise_model( &sea_def );

	wave = allocate_mem( 0,sizeof( Wave ) * MAX_WAVES );

	init_waves();

	planet = spawn_object( &europe_def,0 );		
	if( planet )
		{
		planet -> axis.translate.vx = 20480;
		planet -> axis.translate.vy = -49151;
		planet -> axis.translate.vz = -130572;

		planet -> control |= OBJ_ROTVECTOR;
		}

	planet = spawn_object( &yank_def,1 );		
	if( planet )
		{
		planet -> axis.translate.vx = 20480;
		planet -> axis.translate.vy = -49151;
		planet -> axis.translate.vz = -130572;

		planet -> control |= OBJ_ROTVECTOR;
		}



	planet = spawn_object( &sea_def,0 );		
	if( planet )
		{

		planet -> axis.translate.vx = 20480;
		planet -> axis.translate.vy = -49151;
		planet -> axis.translate.vz = -130572;
//		planet -> control |= OBJ_ROTVECTOR;
		}

	SetGeomScreen( 600 ); 									/* distance to viewing-screen 		*/


}

/************************************************************/
/*	point camera along z axis											*/
/************************************************************/
void point_cam_z( void )
{

	camera.i.vx = ONE;
	camera.i.vy = 0;
	camera.i.vz = 0;

	camera.j.vx = 0;
	camera.j.vy = ONE;
	camera.j.vz = 0;

	camera.k.vx = 0;
	camera.k.vy = 0;
	camera.k.vz = ONE;


}


void init_waves( void )
{
Wave	*w;
WORD	c;

	w = wave;	
	c = MAX_WAVES;
	while( c-- )
		{

		w -> r = 0;
		w -> g = 0;
		w -> b = 250 << 4;

		w -> speed = 15 -(random_number.w.hi & 31 );
		if( w -> speed == 0 )
			w -> speed = 1;
		make_random_number();

		w++;
		}


}


void process_waves( void )
{
Wave	*w;
WORD	c,at;

	w = wave;	
	c = MAX_WAVES;
	while( c-- )
		{

		/***************************************/
		/*	adjust colours								*/
		/***************************************/
		at = fade( &w -> r, w -> speed,  500,00 );
		at += fade( &w -> g, w -> speed, 1500,800 );
		at += fade( &w -> b, w -> speed, 4000,1512 );


		/***************************************/
		/*	convert to CVECTORS						*/
		/***************************************/
		w -> col.r = (UBYTE )(w -> r>>4);
		w -> col.g = (UBYTE )(w -> g>>4);
		w -> col.b = (UBYTE )(w -> b>>4);


		if( at == 3 )
			{
			if( w -> speed > 0 )
				{
				w -> speed = -13-(random_number.w.hi & 63 );
				}
			else
				{
				w -> speed = 13+(random_number.w.hi & 63 );
				}
			make_random_number();
			}



		w++;
		}

}

WORD fade( WORD *c, WORD speed, WORD max, WORD min )
{
	*c += speed;
	if( speed > 0 )
		{
		if( *c > max )
			{
			*c = max;
			return 1;
			}

		}
	else
		{
		if( *c < min )
			{
			*c = min;
			return 1;
			}
		}

	return 0;
}


/*****************************************************************/
/*	smart little random number thing										*/
/*****************************************************************/
void make_random_number( void )
{

ULONG b;

	if( seed.l == random_number.l )
		{
		random_number.l = ++seed.l;
		}


	b = random_number.l;
	random_number.l <<= 1;
	b ^= random_number.l;
	if( b & 0x80000000 )
		{
		
		b <<= 1;
		b++;
	
		}
	else
		b <<= 1;

	random_number.l = b;

}


void language_select_exec( void )
{

	move_camera( );
	control();
	process_waves();



}


void control( void )
{ 
Pad	*p;
Planet_pos	*lat;

	p = &controller.all;


	if( p -> debounce & PAD_SELECT )
		{
		vram_peek();
		}


	if( p -> pad & PAD_TL2 )
		{
		/*********************/
		/*	auto locate mode	*/
		/*********************/
		FntPrint("%s\n",places[lang_at]);

		lat = &planet_positions[ lang_at ];
		get_to_sv( (SVECTOR *)&lgtmat.m[0][0], &lat -> light, 2,256 );


		if( p -> debounce & PAD_UP )
			{
			lang_at++;
			}

		if( p -> debounce & PAD_DOWN )
			{
			lang_at--;
			}

		if( lang_at < 0 )
			lang_at = 0;
		if( lang_at > BRAZILIAN )
			lang_at = BRAZILIAN;

		}





}



void setup_screens( Db *db )
{
WORD	c;

	buffer = 1;
	db -> available_ot[0] = db -> ot1;
	db -> available_ot[1] = db -> ot2;


	/*********************************************************************/
	/* i	nitialize environment for double buffer 									*/
	/*********************************************************************/
	SetDefDrawEnv(&db -> draw[0],
					  FB1X, FB1Y,
					  SCREEN_WIDTH, SCREEN_HEIGHT );

	SetDefDispEnv(&db -> disp[0],
					  FB2X, FB2Y,
					  SCREEN_WIDTH, SCREEN_HEIGHT );

	SetDefDrawEnv(&db -> draw[1],
					  FB2X, FB2Y,
					  SCREEN_WIDTH, SCREEN_HEIGHT );

	SetDefDispEnv(&db -> disp[1],
					  FB1X, FB1Y,
					  SCREEN_WIDTH, SCREEN_HEIGHT );



	c = 2;
	while( c-- )
		{
		/*********************************************************************/
		/*	clear bg after use																*/
		/*********************************************************************/
		db -> draw[c].isbg = 1;
		db -> draw[c].dfe = 1;

		db -> draw[c].r0 = 0;
		db -> draw[c].g0 = 0;
		db -> draw[c].b0 = 0;

		db -> disp[c].screen.x = DEFAULT_SCREEN_X_OFFSET;
		db -> disp[c].screen.y = DEFAULT_SCREEN_Y_OFFSET;
		db -> disp[c].screen.w = 255;
		db -> disp[c].screen.h = 256;

		}

}


void setup_screen_640X512( Db *db )
{

	/*********************************************************************/
	/* initialize environment for double buffer 									*/
	/*********************************************************************/
	SetDefDrawEnv(&db -> draw[0],
					  0, 0,
					  640, 512 );

	SetDefDispEnv(&db -> disp[0],
					  0, 0,
					  640, 512 );



		/*********************************************************************/
		/*	clear bg after use																*/
		/*********************************************************************/
		db -> draw[0].isbg = 1;
		db -> draw[0].dfe = 1;

		db -> draw[0].r0 = 0;
		db -> draw[0].g0 = 0;
		db -> draw[0].b0 = 0;

		db -> disp[0].screen.x = DEFAULT_SCREEN_X_OFFSET;
		db -> disp[0].screen.y = DEFAULT_SCREEN_Y_OFFSET;
		db -> disp[0].screen.w = 255;
		db -> disp[0].screen.h = 256;

		db -> disp[0].isrgb24=1;

}


#define	LSP	32
#define	RSP	16
#define	TSP	1024
void task_planet( Object *obj )
{
Pad	*p;
Planet_pos	*lat;


	p = &controller.all;

	if( p -> pad & PAD_TL2 )
		{
		/*********************/
		/*	auto locate mode	*/
		/*********************/

		lat = &planet_positions[ lang_at ];

		get_to_v( &obj -> axis.translate, &lat -> pos, 3,32768 );
		get_to_sv( &obj -> rotvector, &lat -> rot, 3,32 );

		}


	#if 0

	if( p -> pad & PAD_TR )
		{
		FntPrint("Planet move (%d %d %d)\n",obj -> axis.translate.vx,
														obj -> axis.translate.vy,
														obj -> axis.translate.vz );

//		FntPrint("%d %d %d\n",obj -> axis.translate.vx,obj -> axis.translate.vy,obj -> axis.translate.vz  );
		if( p -> pad & PAD_UP )
			{
			obj -> axis.translate.vy -= TSP;
			}

		if( p -> pad & PAD_DOWN )
			{
			obj -> axis.translate.vy += TSP;
			}


		if( p -> pad & PAD_LEFT )
			{
			obj -> axis.translate.vx -= TSP;
			}

		if( p -> pad & PAD_RIGHT )
			{
			obj -> axis.translate.vx += TSP;
			}

		if( p -> pad & PAD_TRIANGLE )
			{
			obj -> axis.translate.vz -= TSP;

			}

		if( p -> pad & PAD_SQUARE )
			{
			obj -> axis.translate.vz += TSP;

			}


		}

	if( p -> pad & PAD_TR2 )
		{
		FntPrint("Planet rotate (%d %d %d)\n",obj->rotvector.vx,
														  obj->rotvector.vy,
														  obj->rotvector.vz );

		
		if( p -> pad & PAD_LEFT )
			{
			obj -> rotvector.vy += RSP;
			}

		if( p -> pad & PAD_RIGHT )
			{
			obj -> rotvector.vy -= RSP;
			}
		
			
		if( p -> pad & PAD_UP )
			{
			obj -> rotvector.vx -= RSP;
			}
			
		if( p -> pad & PAD_DOWN )
			{
			obj -> rotvector.vx += RSP;
			}

		if( p -> pad & PAD_TRIANGLE )
			{
			obj -> rotvector.vz -= RSP;
			}
			
		if( p -> pad & PAD_SQUARE )
			{
			obj -> rotvector.vz += RSP;
			}

			
			
			
			
		}




	if( p -> pad & PAD_TL )
		{
		RT	m;
		VECTOR	v,v1;

		v.vx = lgtmat.m[0][0];
		v.vy = lgtmat.m[0][1];
		v.vz = lgtmat.m[0][2];
		identity( &m );

		FntPrint("Light rotate\n");
		FntPrint("Vector %d %d %d\n",lgtmat.m[0][0],
											  lgtmat.m[0][1],
											  lgtmat.m[0][2] );

		if( p -> pad & PAD_LEFT )
			{
			rv_axis_set( 0, LSP, 0, &m );

			ApplyMatrixLV( (MATRIX *)&m, &v, &v1 );

			lgtmat.m[0][0] = v1.vx;
			lgtmat.m[0][1] = v1.vy;
			lgtmat.m[0][2] = v1.vz;

			}

		if( p -> pad & PAD_RIGHT )
			{
		
			rv_axis_set( 0, -LSP, 0, &m );

			ApplyMatrixLV( (MATRIX *)&m, &v, &v1 );

			lgtmat.m[0][0] = v1.vx;
			lgtmat.m[0][1] = v1.vy;
			lgtmat.m[0][2] = v1.vz;

			}


		if( p -> pad & PAD_DOWN )
			{
			rv_axis_set( LSP, 0, 0, &m );

			ApplyMatrixLV( (MATRIX *)&m, &v, &v1 );

			lgtmat.m[0][0] = v1.vx;
			lgtmat.m[0][1] = v1.vy;
			lgtmat.m[0][2] = v1.vz;

			}

		if( p -> pad & PAD_UP )
			{
			rv_axis_set( -LSP, 0, 0, &m );

			ApplyMatrixLV( (MATRIX *)&m, &v, &v1 );

			lgtmat.m[0][0] = v1.vx;
			lgtmat.m[0][1] = v1.vy;
			lgtmat.m[0][2] = v1.vz;

			}



		}

	#endif

}


void get_to_v( VECTOR *t, VECTOR *s, LONG shift, LONG speed )
{

	t -> vx += minmax( (( s -> vx - t -> vx ) >> shift ), -speed, speed );
	t -> vy += minmax( (( s -> vy - t -> vy ) >> shift ), -speed, speed );
	t -> vz += minmax( (( s -> vz - t -> vz ) >> shift ), -speed, speed );


}

void get_to_sv( SVECTOR *t, SVECTOR *s, LONG shift, WORD speed )
{
	t -> vx += (WORD)minmax( (LONG)(( s -> vx - t -> vx ) >> shift ), -speed, speed );
	t -> vy += (WORD)minmax( (LONG)(( s -> vy - t -> vy ) >> shift ), -speed, speed );
	t -> vz += (WORD)minmax( (LONG)(( s -> vz - t -> vz ) >> shift ), -speed, speed );

//	t -> vx += (( s -> vx - t -> vx ) >> shift );
//	t -> vy += (( s -> vy - t -> vy ) >> shift );
//	t -> vz += (( s -> vz - t -> vz ) >> shift );

}


LONG	minmax( LONG v, LONG min, LONG max )
{
	if( v < min )
		return min;
	if( v > max )
		return max;

	return v;

}







#define	PWIDTH	512
#define	PHEIGHT	512
#define	PEEK_SPEED			8
#define	PEEK_X_MAX			(1024-PWIDTH)
#define	PEEK_Y_MAX			(512-PHEIGHT)
#define	PEEK_X_MIN			0
#define	PEEK_Y_MIN			0

LONG	xv,yv;
WORD	screenh = 255;
WORD	screenw = 255;
WORD	screenx = 0;
WORD	screeny = 11;

/* debug vram peek		*/

void vram_peek( void )
{
UWORD	quit;
WORD	c;
LONG	old_framerate;

LONG	x,y,width,height;
Pad	*p;
Db	*db;

	old_framerate = time.frames_per_update;
	time.frames_per_update = 1;
	p = &controller.all;

	x = 
	y = 0;

	width = PWIDTH;
	height = PHEIGHT;
	db = &double_buffer;
//	double_buffer = (DB *)screen_buffers;	
	/*********************************************************************/
	/* initialize environment for double buffer 									*/
	/*********************************************************************/
	SetDefDrawEnv(&db -> draw[0], x, 000, width, height);
	SetDefDrawEnv(&db -> draw[1], x, 000, width, height);
	SetDefDispEnv(&db -> disp[0], x, 000, width, height);
	SetDefDispEnv(&db -> disp[1], x, 000, width, height);


	quit = FALSE;
 	xv = yv = 0;


	update_pads();

	while( quit == FALSE )
		{
		update_pads();



		if( p -> debounce & PAD_SELECT )
			{
			quit = TRUE;
			update_pads();

			}


		if( p -> pad & PAD_RIGHT )
			{
			x += PEEK_SPEED;
			if( x > PEEK_X_MAX )
				x = PEEK_X_MAX;
			}

		if( p -> pad & PAD_LEFT )
			{
			x -= PEEK_SPEED;
			if( x < PEEK_X_MIN )
				x = PEEK_X_MIN;
			}

		if( p -> pad & PAD_DOWN )
			{
			y += PEEK_SPEED;
			if( y > PEEK_Y_MAX )
				y = PEEK_Y_MAX;
			}

		if( p -> pad & PAD_UP )
			{
			y -= PEEK_SPEED;
			if( y < PEEK_Y_MIN )
				y = PEEK_Y_MIN;
			}




		SetDefDispEnv( &db -> disp[0], x, y, width, height);
		SetDefDispEnv( &db -> disp[1], x, y, width, height);

		c = 2;
		while( c-- )
			{
			db -> disp[c].screen.h = screenh;
			db -> disp[c].screen.w = screenw;
			db -> disp[c].screen.x = screenx;
			db -> disp[c].screen.y = screeny;

			}

		DrawSync( 0 );
		wait_vblank();
	
		PutDrawEnv( &db -> draw[ buffer ] ); /* update drawing environnment */
		PutDispEnv( &db -> disp[ buffer ] ); /* update display environnment */

	  	}

	setup_screens( &double_buffer );

	time.frames_per_update = old_framerate;


}

void clear_vram( WORD x, WORD y, WORD w, WORD h, UBYTE r, UBYTE g, UBYTE b )
{
RECT	rc;
	
	rc.x = x;
	rc.y = y;
	rc.w = w;
	rc.h = h;

	ClearImage( &rc, r,g,b );
	VSync( 0 );
	ClearImage( &rc, r,g,b );


}

void reset_free_mem( void )
{
	free_mem_ptr = &free_mem;
}




void zero( UBYTE *s, LONG many )
{
LONG	*ls, lmany;

	while( (LONG)s & 3 )
		{
		*s++ = 0;
 		many--;
		}

	ls = ( LONG *)s;
	lmany = many>>2;
	many -= ( lmany<<2);
	while( lmany-- )
		{
		*ls++ = 0;
		}

	s = (UBYTE *)ls;
	while( many-- )
		{
		*s++ = 0;
		}


}









/************************************************************************/
/*																							  	*/
/*	Module: link list control	 													  	*/
/*																							  	*/
/*	Author: 																				  	*/
/*																							  	*/
/************************************************************************/

/************************************************************************/
/*																							  	*/
/*	include default headers															  	*/
/*																							  	*/
/************************************************************************/
//#include "include\libincs.h"

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
#include	"ptypes\link.p"

/************************************************************************/
/*																							  	*/
/*	defs																					  	*/
/*																							  	*/
/************************************************************************/

/************************************************************************/
/*																							  	*/
/*	vars																					  	*/
/*																							  	*/
/************************************************************************/

/************************************************************************/
/*																							  	*/
/*	constant data																		  	*/
/*																							  	*/
/************************************************************************/
void *link( Chain *c )
{

Link	*free = NULL;


	/*********************************************************************/
	/*	make sure theres something to link to										*/
	/*********************************************************************/
	if( c -> free_chain != NULL )
		{

		/******************************************************************/
		/*	save our free one!															*/
		/******************************************************************/
		free = c -> free_chain;

		/******************************************************************/
		/*	unlink it from chain 														*/
		/******************************************************************/
		c -> free_chain = free -> next;
		free -> next = NULL;

		/******************************************************************/
		/*	now link into active chain													*/
		/******************************************************************/
		if( c -> active_chain != NULL )
			{
			c -> active_chain -> prev_active = free;
			free -> next_active = c -> active_chain;
			c -> active_chain = free;

			}
		else
			{
			c -> active_chain = free;
			free -> prev_active = NULL;
			free -> next_active = NULL;
			}

		}

	return ( void *)free;

}

void unlink( void *object, Chain *c )
{
Link	*obj;

	obj = ( Link *)object;

	/*********************************************************************/
	/*	link back into free chain														*/
	/*********************************************************************/
	obj -> next = c -> free_chain;
	c -> free_chain = obj;

	/*********************************************************************/
	/*	remove from active chain														*/
	/*********************************************************************/
	if( obj -> prev_active != NULL )
		{
		if( obj -> next_active != NULL )
			{
			obj -> prev_active -> next_active = obj -> next_active;
			obj -> next_active -> prev_active = obj -> prev_active;
			}
		else
			{			 
			obj -> prev_active -> next_active = NULL;


			}

		obj -> prev_active = obj -> next_active = NULL;

		}
	else
		{
		/******************************************************************/
		/*	iddle debug check																*/
		/******************************************************************/
		if( obj != c -> active_chain )
			{
			printf("first active link's previous link is not free active chain!\n");

			}

		if( obj -> next_active != NULL )
			{
			obj -> next_active -> prev_active = NULL;
			c -> active_chain = obj -> next_active;
			obj -> next_active = NULL;
			}
		else
			{
			c -> active_chain = NULL;
			}


		}
 

}


void unlink_group( Chain *chain, Link *first, Link *last )
{

	if( (first != NULL) && ( last != NULL ) )
		{

		last -> next = chain -> free_chain;
		chain -> free_chain = first;

		}

}

void link_group( Chain *chain, Link **first, Link **last, WORD many )
{
Link	*f, *t, *l;

	f = chain -> free_chain;
	t = f;
	if( t )
		{

		while( many-- )
			{


			if( !t  )
				{
				*first = *last = NULL;
				return;
				}
			
			l = t;
			t = t -> next;

			}

		}


	*first = f;
	*last = l;
	chain -> free_chain = t;



}


/************************************************************************/
/*	initialise a linklist chain														*/
/************************************************************************/
void initialise_chain( LONG size, WORD many, Chain *chain )
{
WORD	c;
void	*i;
Link	*l;

	/*********************************************************************/
	/*	allocate chain workspace 														*/
	/*********************************************************************/
	i = 
	chain -> free_chain = 
	chain -> base_chain = allocate_mem( 0, size * many );

	chain -> active_chain = NULL;

	if( i != NULL )
		{
		chain -> size = size;
		chain -> many = many;
 		c = many;
		while( c-- )
			{
			l = i;
			l -> next = ( i+size );
			l -> next_active = NULL;
			l -> prev_active = NULL;

			i += size;
			}

		l -> next = NULL;

		}

}

/************************************************************************/
/*	initialise a linklist chain														*/
/************************************************************************/
void reset_chain( Chain *chain )
{
WORD	c;
void	*i;
Link	*l;

	/*********************************************************************/
	/*	allocate chain workspace 														*/
	/*********************************************************************/
	i = chain -> free_chain = chain -> base_chain;
	chain -> active_chain = NULL;

	if( i != NULL )
		{
 		c = chain -> many;
		while( c-- )
			{
			l = i;
			l -> next = ( i + chain -> size );
			l -> next_active = NULL;
			l -> prev_active = NULL;

			i += chain -> size;
			}

		l -> next = NULL;

		}

}

/***************************************************************************/
/*	count remaining links in a chain														*/
/***************************************************************************/
WORD	count_chain_free_links( Chain *chain )
{
Link	*l;
WORD	c = 0;

	l = chain -> free_chain;
	while( l != NULL )
		{
		c++;
		l = l -> next;
		}

	return c;

}

/***************************************************************************/
/*	count links in an active chain 														*/
/***************************************************************************/
WORD	count_chain_active_links( Chain *chain )
{
Link	*l;
WORD	c = 0;

	l = chain -> active_chain;
	while( l != NULL )
		{
		c++;
		l = l -> next_active;
		}

	return c;

}








/************************************************************************/
/*																							  	*/
/*	Module: vector & matrix operations	 											*/
/*																							  	*/
/*																							  	*/
/************************************************************************/

/************************************************************************/
/*																							  	*/
/*	include default headers															  	*/
/*																							  	*/
/************************************************************************/
//#include "include\libincs.h"

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


/************************************************************************/
/*																							  	*/
/*	defs																					  	*/
/*																							  	*/
/************************************************************************/

/************************************************************************/
/*																							  	*/
/*	vars																					  	*/
/*																							  	*/
/************************************************************************/

/************************************************************************/
/*																							  	*/
/*	constant data																		  	*/
/*																							  	*/
/************************************************************************/


void rv_axis_set( WORD	x, WORD y, WORD z, RT *m1 )
{
MATRIX	m;
SVECTOR	t;

	t.vx = x;
	t.vy = y;
	t.vz = z;

	RotMatrix_gte( &t, &m );
	gte_MulMatrix0( &m,(MATRIX *)m1, (MATRIX *)m1 ); 


}



/*********************************************/
/*	make matrix identity								*/
/*********************************************/
void identity( RT *m )
{

	m -> rotate.xaxis.x = 
	m -> rotate.yaxis.y = 
	m -> rotate.zaxis.z = ONE;

	m -> rotate.xaxis.y = 
	m -> rotate.xaxis.z = 
	m -> rotate.yaxis.x = 
	m -> rotate.yaxis.z = 
	m -> rotate.zaxis.x = 
	m -> rotate.zaxis.y = 0;

}

void vector_descale( VECTOR *v, LONG scale, VECTOR *o )
{

	if( (scale == 1) || (scale == 0) )
		return;

	o -> vx = v -> vx / scale;
	o -> vy = v -> vy / scale;
	o -> vz = v -> vz / scale;

}

/*********************************************/
/*	calculate the magnitude of a vector			*/
/*********************************************/
LONG vector_magnitude( VECTOR *v )
{
LONG	s;

	s = ( v -> vx * v -> vx ) +
		 ( v -> vy * v -> vy ) +
		 ( v -> vz * v -> vz );

	return SquareRoot0( s );


}

#if 0
void vector_matrix( VECTOR *t, RT *om )
{
MATRIX	av;
SVECTOR	sv;

	sv.vx = (WORD)(t -> vx & 4095);
	sv.vy = (WORD)(t -> vy & 4095);
	sv.vz = (WORD)(t -> vz & 4095);
	
	RotMatrix_gte( &sv, &av );
	gte_MulMatrix0( (MATRIX *)om, &av, (MATRIX *)om ); 

}


void svector_matrix( SVECTOR *sv, RT *om )
{
MATRIX	av;

	RotMatrix_gte( sv, &av );
	gte_MulMatrix0( (MATRIX *)om, &av, (MATRIX *)om ); 

}



void svector_X_matrix( SVECTOR *t, RT *m, VECTOR *ot )
{

	gte_ApplyMatrix( (MATRIX *)m, t, ot );
	ot -> vx += (m->translate.vx >> VRES_SHIFT);
	ot -> vy += (m->translate.vy >> VRES_SHIFT);
	ot -> vz += (m->translate.vz >> VRES_SHIFT);

}

void vector_X_matrix( VECTOR *t, RT *m, VECTOR *ot )
{

	ApplyMatrixLV( (MATRIX *)m, t, ot );
	ot -> vx += m->translate.vx;
	ot -> vy += m->translate.vy;
	ot -> vz += m->translate.vz;

}


void rot_axis_set( SVECTOR *t, RT *m1 )
{
MATRIX	m;

	RotMatrix_gte( t, &m );
	gte_MulMatrix0( &m,(MATRIX *)m1, (MATRIX *)m1 ); 



}


void rv_axis_set_local( WORD	x, WORD y, WORD z, RT *m1 )
{
RT		m;

SVECTOR	t;

	t.vx = x;
	t.vy = y;
	t.vz = z;

	/*********************************************/
	/*	get local orientation							*/
	/*********************************************/
	RotMatrix_gte( &t, (MATRIX *)&m );

	/*********************************************/
	/*	get back to local space							*/
	/*********************************************/
	gte_MulMatrix0( (MATRIX *)m1,(MATRIX *)&m, (MATRIX *)m1 ); 
	

}


/*********************************************************/
/* translate matrix													*/
/*********************************************************/
void matrix_translate( RT *vo, VECTOR *vi )
{
//	vo -> translate.vx += vi -> vx >> VRES_SHIFT;
//	vo -> translate.vy += vi -> vy >> VRES_SHIFT;
//	vo -> translate.vz += vi -> vz >> VRES_SHIFT;

	vo -> translate.vx += vi -> vx;
	vo -> translate.vy += vi -> vy;
	vo -> translate.vz += vi -> vz;

}

/*********************************************************/
/*	rotate matrix														*/
/*********************************************************/
void matrix_rotate( RT *m, VECTOR *r )
{
MATRIX	av;
SVECTOR	rsv;
	
	rsv.vx = (WORD)(r -> vx );
	rsv.vy = (WORD)(r -> vy );
	rsv.vz = (WORD)(r -> vz );
	
	RotMatrix_gte( &rsv, &av );
	gte_MulMatrix0( (MATRIX *)m, &av, (MATRIX *)m ); 
								
}

/*********************************************************/
/*	vector cross product												*/
/*********************************************************/
void vector_cross( VECTOR *i, VECTOR *j, VECTOR *k  )
{
VECTOR	r;


	gte_OuterProduct0( i, j, &r);

	#if 0
	r.vx = (i -> vy * j -> vz) - ( i -> vz * j -> vy );
	r.vy = -((i -> vx * j -> vz) - ( i -> vz * j -> vx ));
	r.vz = (i -> vx * j -> vy) - ( i -> vy * j -> vx );
	#endif

	*k = r;



}

LONG vector_dot( VECTOR *a, VECTOR *b )
{

	return (((a)->vx * (b)->vx) + ((a)->vy * (b)->vy) + ((a)->vz * (b)->vz));

}

void vector_scale( VECTOR *v, LONG scale, VECTOR *o )
{
	if( (scale == 1) || (scale == 0) )
		return;
		
	o -> vx = v -> vx * scale;
	o -> vy = v -> vy * scale;
	o -> vz = v -> vz * scale;

}



void vector_shift( VECTOR *v, LONG shift, VECTOR *o )
{

	o -> vx = v -> vx << shift;
	o -> vy = v -> vy << shift;
	o -> vz = v -> vz << shift;

}

void vector_deshift( VECTOR *v, LONG shift, VECTOR *o )
{

	o -> vx = v -> vx >> shift;
	o -> vy = v -> vy >> shift;
	o -> vz = v -> vz >> shift;

}

/******************************************/
/*	vector addition								*/
/******************************************/
void vector_add( VECTOR *a, VECTOR *b, VECTOR *c )
{
	c -> vx = a -> vx + b -> vx;
	c -> vy = a -> vy + b -> vy;
	c -> vz = a -> vz + b -> vz;

}


/******************************************/
/*	vector addition								*/
/******************************************/
void vector_subtract( VECTOR *a, VECTOR *b, VECTOR *c )
{
	c -> vx = a -> vx - b -> vx;
	c -> vy = a -> vy - b -> vy;
	c -> vz = a -> vz - b -> vz;

}




/*********************************************/
/*	calculate the magnitude of a vector			*/
/*********************************************/
LONG vector_size( VECTOR *v )
{
LONG	s;

	s = (v -> vx + v -> vy  + v -> vz)/3;

	return s;


}

/*********************************************/
/*	multiply two vectors								*/
/*********************************************/
void vector_multiply( VECTOR *v, VECTOR *v1, VECTOR *o )
{

	o -> vx = v -> vx * v1 -> vx;
	o -> vy = v -> vy * v1 -> vy;
	o -> vz = v -> vz * v1 -> vz;

}

/*********************************************/
/*	divide two vectors								*/
/*********************************************/
void vector_divide( VECTOR *v, VECTOR *v1, VECTOR *o )
{

	o -> vx = v -> vx / v1 -> vx;
	o -> vy = v -> vy / v1 -> vy;
	o -> vz = v -> vz / v1 -> vz;

}

/*********************************************/
/*	negate a vectors									*/
/*********************************************/
void vector_negate( VECTOR *v, VECTOR *vo )
{

	vo -> vx = -v -> vx;
	vo -> vy = -v -> vy;
	vo -> vz = -v -> vz;

}


/*********************************************/
/*	create an inverse matrix						*/
/*********************************************/
void inverse_matrix( RT *mat, RT *imat )
{
RT	workmat;	

	TransposeMatrix( (MATRIX *)mat, (MATRIX *)imat );
	imat -> translate.vx =
	imat -> translate.vy =
	imat -> translate.vz = 0;

	identity( &workmat );
	workmat.translate.vx = -mat -> translate.vx;
	workmat.translate.vy = -mat -> translate.vy;
	workmat.translate.vz = -mat -> translate.vz;

	CompMatrixLV( (MATRIX *)imat,(MATRIX *)&workmat, (MATRIX *)imat );
	

}

void extract_x_component( Rotation *r, VECTOR *v )
{

	v -> vx = (WORD)r -> xaxis.x;
	v -> vy = (WORD)r -> yaxis.x;
	v -> vz = (WORD)r -> zaxis.x;

}

void extract_y_component( Rotation *r, VECTOR *v )
{

	v -> vx = (WORD)r -> xaxis.y;
	v -> vy = (WORD)r -> yaxis.y;
	v -> vz = (WORD)r -> zaxis.y;



}

void extract_z_component( Rotation *r, VECTOR *v )
{

	v -> vx = (WORD)r -> xaxis.z;
	v -> vy = (WORD)r -> yaxis.z;
	v -> vz = (WORD)r -> zaxis.z;

}
#endif




/************************************************************************/
/*																							  	*/
/*	Module: draw					 													  	*/
/*																							  	*/
/*																							  	*/
/*																							  	*/
/************************************************************************/

/************************************************************************/
/*																							  	*/
/*	include default headers															  	*/
/*																							  	*/
/************************************************************************/

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
#include	"ptypes\draw.p"
#include	"include\main.h"


/************************************************************************/
/*																							  	*/
/*	defs																					  	*/
/*																							  	*/
/************************************************************************/
#define	ZIP_STASH	50
#define	S 128
#define	NEAREST_Z	0
#define	FURTHEST_Z	20000
#define	FALLOFF		2048
#define	ALV			69
#define	SLV			69

#define	SCREEN_TOPBIT		BIT(31)
#define	SCREEN_LEFTBIT		BIT(15)
#define	SCREEN_BOTTOMBIT	BIT(24)
#define	SCREEN_RIGHTBIT	BIT(9)
#define	CLIPBITS	(SCREEN_TOPBIT|SCREEN_LEFTBIT|SCREEN_BOTTOMBIT|SCREEN_RIGHTBIT )


#define	BUMP_PIXEL_SIZE	2
#define	BUMPSHIFT	0
//#define	SHADE_BUMPMAP	

enum
{ DUDE, WEENER };


/************************************************************************/
/*	a few re-worked gte macros															*/
/************************************************************************/
#define rgte_RotTransPers(r1,r2,r3)   			\
{							\
	gte_ldv0(r1);   	\
   gte_rtps();     	\
   gte_stsxy(r2); 		\
	gte_stsz(r3);	}

#define rgte_getz(r1,r2)   			\
{							\
	gte_ldv0(r1);   	\
   gte_rtps();     	\
	gte_stsz(r2);	}


#define rgte_Square0(r1)					\
				{	gte_ldlvl(r1);		\
					gte_sqr0();		\
					gte_stlvnl(r1);	}	

#define rgte_RotTransPers3noz(r1,r2,r3,r4,r5,r6 )		\
				{	gte_ldv3(r1,r2,r3);	\
					gte_rtpt();		\
					gte_stsxy3(r4,r5,r6);}

#define rgte_RotTransPers3(r1,r2,r3,r4,r5,r6,r7,r8,r9 )		\
				{	gte_ldv3(r1,r2,r3);	\
					gte_rtpt();		\
					gte_stsxy3(r4,r5,r6);\
					gte_stsz3( r7,r8,r9);}


/************************************************************************/
/*																							  	*/
/*	vars																					  	*/
/*																							  	*/
/************************************************************************/
Chain	chain_tri,chain_quad;
Xformed	xform_list[ MAX_VERTEX ];
Gfx_vram	installed_gfx[ 10 ];
SVECTOR	*last_magic;

typedef struct
{
	WORD (*alloc)( Object *obj );
	WORD (*dealloc)( Object *obj );
	void (*draw_set)( Object *obj, Xformed *t );
	void (*draw)( Object *obj, Xformed *t);

}Prim_setup;


enum
{
	THE_G3S,
	THE_GT3BS,

	THEMS_DONE,
};

Prim_setup	prim_setup[] =
{
	alloc_g3,
	dealloc_g3,
	draw_set_g3,
	draw_g3,

	alloc_gt3b,
	dealloc_gt3b,
	draw_set_gt3b,
	draw_gt3b,


};


/************************************************************************/
/*																							  	*/
/*	constant data																		  	*/
/*																							  	*/
/************************************************************************/
RECT		gfx_rect =
{
	512,0,0,0,

};

RECT		clut_rect =
{
	512,256,0,0,

};

MATRIX	cmat =
{
		ONE/4,  0,  0, 	/* R */
		ONE/4,  0,  0,   	/* G */
		ONE/4,  0,  0,   	/* B */
};


/******************************************************************/
/*																						*/
/*	alloacte prims for model drawer											*/
/*																						*/
/******************************************************************/
void alloc_prim( void )
{
	
	initialise_chain( sizeof( Tri ), MAX_TRI, &chain_tri );
	initialise_chain( sizeof( Quad ), MAX_TRI, &chain_quad );

	SetBackColor(0, 0, 0);	
	SetColorMatrix(&cmat);	  

}





/******************************************************************/
/*																						*/
/*	setup model information level of detail shared pointlists.		*/
/*																						*/
/******************************************************************/
void initialise_model( Object_def *def )
{
Model_info	**detail, *mi;
Point			*point_list;

	detail = def -> details;
 	
	mi = *detail;
	if( mi )
		{
		/******************************************************/
		/*	initialise model information								*/
		/******************************************************/
		initialise_model_info( mi, DUDE );

		/******************************************************/
		/*	1st model has pointlist for the rest					*/
		/******************************************************/
		point_list = mi -> point_list;

		detail++;
		while( (mi = *detail) )
			{
			/******************************************************/
			/*	init lower level detail obs to highlevel pointlist	*/
			/******************************************************/
			initialise_model_info( mi, WEENER );
			mi -> point_list = point_list;

			detail++;
			}

		}
	else
		{
		/******************************************************/
		/*	model has 0 levels of detail								*/
		/******************************************************/
		printf("model has no detail levels!! EH ? \n");

		}

}


/************************************************************************/
/*	initialise model info poinlist, quad, tri information pointers			*/
/************************************************************************/
void initialise_model_info( Model_info *mi, LONG detail )
{

	if( detail == DUDE )
		{
		/*********************************************************************/
		/*	'DUDE' model has pointlist informatioin for all levels of detail	*/
		/*********************************************************************/
		mi -> point_list = ( Point *)&(mi -> vlist[ 0 ]);
		mi -> g4_prim = ( G4_prim *)&(mi -> point_list[ mi -> vertex_count ]);
		mi -> g3_prim = ( G3_prim *)&(mi -> g4_prim[ mi -> g4_count ]);
		mi -> gt4_prim = ( GT4_prim *)&(mi -> g3_prim[ mi -> g3_count ]);
		mi -> gt3_prim = ( GT3_prim *)&(mi -> gt4_prim[ mi -> gt4_count ]);
		mi -> gt3b_prim = ( GT3B_prim *)&(mi -> gt3_prim[ mi -> gt3_count ]);
		mi -> gt3r_prim = ( GT3_prim *)&(mi -> gt3b_prim[ mi -> gt3b_count ]);
		mi -> gt3c_prim = ( GT3C_prim *)&(mi -> gt3r_prim[ mi -> gt3r_count ]);
		mi -> gt3g_prim = ( GT3G_prim *)&(mi -> gt3c_prim[ mi -> gt3c_count ]);
		mi -> gt3rb_prim = ( GT3RB_prim *)&(mi -> gt3g_prim[ mi -> gt3g_count ]);
		}
	else
		{
		/*********************************************************************/
		/*	'WEENER' model takes pointlist information from the 'DUDE'			*/
		/*********************************************************************/
		mi -> g4_prim = ( G4_prim *)&(mi ->vlist[ 0 ]);
		mi -> g3_prim = ( G3_prim *)&(mi -> g4_prim[ mi -> g4_count ]);
		mi -> gt4_prim = ( GT4_prim *)&(mi -> g3_prim[ mi -> g3_count ]);
		mi -> gt3_prim = ( GT3_prim *)&(mi -> gt4_prim[ mi -> gt4_count ]);
		mi -> gt3b_prim = ( GT3B_prim *)&(mi -> gt3_prim[ mi -> gt3_count ]);
		mi -> gt3r_prim = ( GT3_prim *)&(mi -> gt3b_prim[ mi -> gt3b_count ]);
		mi -> gt3c_prim = ( GT3C_prim *)&(mi -> gt3r_prim[ mi -> gt3r_count ]);
		mi -> gt3g_prim = ( GT3G_prim *)&(mi -> gt3c_prim[ mi -> gt3c_count ]);
		mi -> gt3rb_prim = ( GT3RB_prim *)&(mi -> gt3g_prim[ mi -> gt3g_count ]);

		}


}
/*********************************************************************/
/*	model draw																			*/
/*********************************************************************/
void draw_obj( Object *obj, MATRIX *real, MATRIX *light_mat )
{
MATRIX	light, local;
RT			axis;
SVECTOR	blight, magic;
//LONG		child;

	PushMatrix();

	if( obj -> control & OBJ_ROTVECTOR )
		{
		/***************************************************************/
		/*	object uses rotation vector instead of axis set					*/
		/***************************************************************/
		RotMatrix(  &obj -> rotvector, (MATRIX *)&axis.rotate );
		}
	else
		{
		axis.rotate = obj -> axis.rotate;
		}

//	MatrixNormal_1( (MATRIX *)&obj -> axis, (MATRIX *)&obj -> axis );
	vector_descale( &obj -> axis.translate, VRES, &axis.translate );

  	/***************************************************************/
	/* create object matrix														*/
	/***************************************************************/
	CompMatrix( real, (MATRIX *)&axis, &local );

	/***************************************************************/
	/*	setup lights																*/
	/***************************************************************/
	MulMatrix0( light_mat, (MATRIX *)&axis, &light );
	gte_SetLightMatrix(&light);

	/***************************************************************/
	/*																					*/
	/*	setup bump light															*/
	/*																					*/
	/***************************************************************/

	/***************************************************************/
	/*	get bumplight vector														*/
	/***************************************************************/
	blight.vx = light_mat->m[0][0];
	blight.vy = light_mat->m[0][1];
	blight.vz = light_mat->m[0][2];

	TransposeMatrix( (MATRIX *)&axis, (MATRIX *)&fmatrix );
	ApplyMatrixSV( (MATRIX *)&fmatrix, &blight, &magic );

	/***************************************************************/
	/*	now get obj matrix														*/
	/***************************************************************/
	gte_SetRotMatrix( &local );
	gte_SetTransMatrix( &local );

	last_magic = &magic;
	render( obj, &local );

	#if 0
	child = MAX_CHILDREN;
	while( child-- )
		{
		if( obj -> children[ child ]  )
			{
			draw_obj( obj -> children[ child ], &local, &light );
			}
		}
	#endif

	
	gte_SetLightMatrix(light_mat);
	PopMatrix();
}


void render( Object *obj, MATRIX *local )
{
WORD		setup, flag;
Xformed	*t_buffer;

WORD	c;
Pinfo			*p;
Prim_setup	*ps;

//	bound( obj );
	if( obj -> control & OBJ_ONSCREEN )
		{
		/************************************************************/
		/*	new distance get														*/
		/************************************************************/
//		obj -> distance_from_camera = vector_magnitude( &camera.pos, &obj -> axis.translate ) >> 3;

		t_buffer = xform_list;

//		if( (obj -> z >= 0) && (obj -> z < FURTHEST_Z) )
			{

			flag = xform_vertex( obj, local, t_buffer );

			if( flag == TRUE )
				{

				#if 0

				if( obj -> z >= FURTHEST_Z )
					{
					/***************************************************/
					/*	object has left the building							*/
					/***************************************************/
//					obj -> model_stack.gt3b_setup = PRIM_DEALLOC;
					obj -> model_stack.gt3b_pinfo.setup = PRIM_DEALLOC;
					obj -> control |= OBJ_FAR;
					}
				else
					{
					if( obj -> control & OBJ_FAR )
						{
						/************************************************/
						/*	obj is back from far-clip							*/
						/************************************************/
						obj -> control &= ~OBJ_FAR;
//						obj -> model_stack.gt3_setup = (PRIM_SETUP|PRIM_ALLOC);
						obj -> model_stack.gt3_pinfo.setup = (PRIM_SETUP|PRIM_ALLOC);

						}
					}

				#endif

				c = THEMS_DONE;
				p = &obj -> model_stack.gt3b_pinfo;
				while( c-- )
					{

					ps = &prim_setup[c];

					if( p -> count )
						{
						setup = p -> setup;

						if( setup & PRIM_SETUP )
							{
							if( setup & PRIM_ALLOC )
								{
								/************************************************/
								/*	need to alloc											*/
								/************************************************/
								(*ps -> alloc)(obj);
								}

							(*ps -> draw_set)( obj, t_buffer );
					
							setup &= (PRIM_SETUP|PRIM_ALLOC);
							setup >>= 1;

							p -> setup &= ~(PRIM_SETUP|PRIM_ALLOC);
							p -> setup |= setup;

							}
						else
						if( setup & PRIM_DEALLOC )
							{
							/************************************************/
							/*	need to deallocate model prims					*/
							/************************************************/
							(*ps -> dealloc)(obj);
							p -> setup &= ~PRIM_DEALLOC;

							}
						else
							{
							(*ps -> draw)( obj, t_buffer );
							}
						}



					p--;

					}

				}
			}
		}


}


#if 0
/***************************************************************/
/*	create onscreen boundbox coords										*/
/***************************************************************/
void bound( Object *obj )
{
Xy	scrxy[9];
LONG	z[9];
LONG	code,z1,z2,z3;
WORD	c;

	code = 0;
	c = 0;
	while( c < 8 )
		{

		rgte_RotTransPers3( &obj -> model_stack.model_info -> bound[c],
							     &obj -> model_stack.model_info -> bound[c+1],
							  	  &obj -> model_stack.model_info -> bound[c+2],

							  	  (ULONG *)&scrxy[c],
							  	  (ULONG *)&scrxy[c+1],
							  	  (ULONG *)&scrxy[c+2],
								  
								  &z[c],
								  &z[c+1],
								  &z[c+2] );



		gte_stsxy3( &z1,&z2,&z3 );
		if( z1 & SCREEN_TOPBIT )
			z1 &= ~SCREEN_BOTTOMBIT;
		if( z1 & SCREEN_LEFTBIT )
			z1 &= ~SCREEN_RIGHTBIT;

		if( z2 & SCREEN_TOPBIT )
			z2 &= ~SCREEN_BOTTOMBIT;
		if( z2 & SCREEN_LEFTBIT )
			z2 &= ~SCREEN_RIGHTBIT;
			
		if( z3 & SCREEN_TOPBIT )
			z3 &= ~SCREEN_BOTTOMBIT;
		if( z3 & SCREEN_LEFTBIT )
			z3 &= ~SCREEN_RIGHTBIT;


		z1 ^= CLIPBITS;
		z2 ^= CLIPBITS;
		z3 ^= CLIPBITS;

		code |= z1;
		code |= z2;
		code |= z3;

		#if 0
		/*********************************/
		/*	if poly doesn't get too big	*/
		/*********************************/
		code |= ((z1^CLIPBITS));
		code |= ((z2^CLIPBITS));
		code |= ((z3^CLIPBITS));
		#endif

		c += 3;

		}



	if( (code & CLIPBITS) == CLIPBITS )
		{
		obj -> control |= OBJ_ONSCREEN;
		obj -> z = (z[0]+z[1]+z[2]+z[3]+z[4]+z[5]+z[6]+z[7]) >> 3;

		}
	else
		obj -> control &= ~OBJ_ONSCREEN;


	obj -> control |= OBJ_ONSCREEN;

}

/*********************************************************/
/*	get distance between 2 vectors								*/
/*********************************************************/
LONG	distance( VECTOR *p1, MATRIX *p2 )
{
VECTOR	v;
LONG		a;

	v.vx = p1 -> vx - p2 -> t[0];
	v.vy = p1 -> vy - p2 -> t[1];
	v.vz = p1 -> vz - p2 -> t[2];

	gte_Square0( &v, &v );
	a = v.vx + v.vy + v.vz;
	return SquareRoot0( a );


}

/******************************************************************/
/*	faster ( but a bit fuzzy ) distance calculation						*/
/******************************************************************/
LONG	fastdst( LONG xdist, LONG zdist )
{
ULONG	xdiff, zdiff;

	if( xdist >=0 )
		xdiff = xdist;
	else
		xdiff = -xdist;

	if( zdist >=0 )
		zdiff = zdist;
	else
		zdiff = -zdist;

	return( xdiff+zdiff-(((zdiff>zdiff) ? zdiff : xdiff) >> 1 ));

}


#endif


/*********************************************************/
/*	vertex roaster														*/
/*********************************************************/
WORD xform_vertex( Object *obj, MATRIX *m, Xformed *x1 )
{
Point		*p;
LONG		many, many3;

//	if( obj -> z > NEAREST_Z)
		{

		p = obj -> model_stack.point_list;
		many3 = obj -> model_stack.vertex_count / 3;
		many = obj -> model_stack.vertex_count - (many3*3);
		
		while( many3-- )
			{
			/***************************************************************/
			/*	roast em																		*/
			/***************************************************************/
			gte_ldv3( &p -> v,
							&(p+1) -> v,
							&(p+2) -> v );

			gte_rtpt();	


			gte_stsxy3( (ULONG *)&x1 -> x,
							(ULONG *)&(x1+1) -> x,
							(ULONG *)&(x1+2) -> x );

			gte_stsz3( &x1 -> z,
							&(x1+1) -> z,
							&(x1+2) -> z  );


			x1 += 3;
			p += 3;



			}

		while( many-- )
			{

			rgte_RotTransPers( &p -> v, (ULONG *)&x1 -> x, &x1 -> z );

			p++;
			x1++;

			}
		}
//	else
//		return FALSE;


	return TRUE;

}




/***************************************************************/
/*																					*/
/*	non textured tri, pre setup,& draw 									*/
/*																					*/
/***************************************************************/
void draw_set_g3( Object *obj, Xformed *xlist )
{
Tri		*t;
G3_prim	*g3_prim;
POLY_G3	*g3;
LONG		n,depth,c;	
Point		*p;


	c = obj -> model_stack.g3_pinfo.count;
	t = obj -> model_stack.tri_g3;
	g3_prim =  obj -> model_stack.g3_pinfo.mprim.g3_prim;
	while( c-- )			
		{
		gte_ldsxy3( *(LONG *)&xlist[ g3_prim -> v1 ].x,
					   *(LONG *)&xlist[ g3_prim -> v2 ].x,
					   *(LONG *)&xlist[ g3_prim -> v3 ].x );

		gte_nclip();
		gte_stopz(&n);

		g3 = &t -> prim[ buffer ].g3;
		SetPolyG3( g3 );
		SetSemiTrans( g3,0 );
//		g3 -> tpage = tpage;
//		g3 -> clut = clut_id;

		/******************************************/
		/*	uv's												*/
		/******************************************/
		p =  obj -> model_stack.point_list;
		
		g3 -> r0 =
		g3 -> r1 =
		g3 -> r2 =

		g3 -> g0 =
		g3 -> g1 =
		g3 -> g2 =

		g3 -> b0 =
		g3 -> b1 =
		g3 -> b2 = 128;

		if( n > 0 )
			{

			g3 -> x0 = xlist[ g3_prim -> v1 ].x;
			g3 -> y0 = xlist[ g3_prim -> v1 ].y;
			g3 -> x1 = xlist[ g3_prim -> v2 ].x;
			g3 -> y1 = xlist[ g3_prim -> v2 ].y;
			g3 -> x2 = xlist[ g3_prim -> v3 ].x;
			g3 -> y2 = xlist[ g3_prim -> v3 ].y;

			depth  = xlist[ g3_prim -> v1 ].z;
			depth += xlist[ g3_prim -> v2 ].z;
			depth += xlist[ g3_prim -> v3 ].z;
			depth /= 3;

			if( depth > NEAREST_Z )
				{
				depth >>= 2;

				addPrim( current_ot + depth, g3 );


				}

			}

		g3_prim++;
		t = (Tri *)t -> link.next;

		}


}



/***************************************************************/
/*																					*/
/*	nontextured tri, primitive draw										*/
/*																					*/
/***************************************************************/
void draw_g3( Object *obj, Xformed *xlist )
{
LONG		c;
Tri		*t;
G3_prim	*g3_prim;
POLY_G3	*g3;
LONG		depth;	
LONG		n;	
LONG		code;


	c = obj -> model_stack.g3_pinfo.count;
	t = obj -> model_stack.tri_g3;
	g3_prim = obj -> model_stack.g3_pinfo.mprim.g3_prim;
	while( c-- )			
		{

		gte_ldsxy3( *(LONG *)&xlist[ g3_prim -> v1 ].x,
					   *(LONG *)&xlist[ g3_prim -> v2 ].x,
					   *(LONG *)&xlist[ g3_prim -> v3 ].x );

		gte_nclip();
		g3 = &t -> prim[ buffer ].g3;

		gte_stopz(&n);
	  	if( n > 0 )
			{

			#if 1
	  		depth = xlist[ g3_prim -> v1 ].z;
			depth += xlist[ g3_prim -> v2 ].z;
			depth += xlist[ g3_prim -> v3 ].z;
			depth /= 3;
			#endif


			if( depth > NEAREST_Z )
				{
	
				depth >>= 2;
				depth = 2;

				code = ((*(LONG *)&xlist[ g3_prim -> v1 ].x)^CLIPBITS);
				code |= ((*(LONG *)&xlist[ g3_prim -> v2 ].x)^CLIPBITS);
				code |= ((*(LONG *)&xlist[ g3_prim -> v3 ].x)^CLIPBITS);

				if( (code & CLIPBITS) == CLIPBITS )
					{

					code = g3 -> code;
					gte_ldv0( &g3_prim -> n0 );
					gte_ldrgb( &wave[ g3_prim -> v1 & (MAX_WAVES-1) ].col );
					gte_nccs();
					gte_strgb((CVECTOR *)&g3 -> r0);
					g3 -> code = code;

					gte_ldv0( &g3_prim -> n1 );
					gte_ldrgb( &wave[ g3_prim -> v2 & (MAX_WAVES-1) ].col );
					gte_nccs();
					gte_strgb((CVECTOR *)&g3 -> r1);

					gte_ldv0( &g3_prim -> n2 );
					gte_ldrgb( &wave[ g3_prim -> v3 & (MAX_WAVES-1) ].col );
					gte_nccs();
					gte_strgb((CVECTOR *)&g3 -> r2);

					gte_stsxy3_g3( g3 );

					addPrim( current_ot + depth, g3 );



					}
				}
			}


		g3_prim++;
		t = (Tri *)t -> link.next;

		}

}


/***************************************************************/
/*																					*/
/*	bumpmap textured tri, pre setup,& draw								*/
/*																					*/
/***************************************************************/
void draw_set_gt3b( Object *obj, Xformed *xlist )
{
WORD			c;
Tri			*t, *tba, *tbs;
GT3B_prim	*gt3b_prim;
POLY_GT3		*gt3, *gt3ba,*gt3bs;
LONG			n,depth;	
Point			*p;
Gfx_vram		*gv;


	gv = &installed_gfx[ obj -> model_stack.set ];
//	c = obj -> model_stack.gt3b_count;
	c = obj -> model_stack.gt3b_pinfo.count;
	t = obj -> model_stack.tri_gt3b;
	tba = obj -> model_stack.tri_gt3ba;
	tbs = obj -> model_stack.tri_gt3bs;
//	gt3b_prim =  obj -> model_stack.gt3b_prim;
	gt3b_prim =  obj -> model_stack.gt3b_pinfo.mprim.gt3b_prim;
	while( c-- )			
		{
		gte_ldsxy3( *(LONG *)&xlist[ gt3b_prim -> v1 ].x,
					   *(LONG *)&xlist[ gt3b_prim -> v2 ].x,
					   *(LONG *)&xlist[ gt3b_prim -> v3 ].x );

		gte_nclip();
		gte_stopz(&n);

		gt3 = &t -> prim[ buffer ].gt3;
		gt3 -> tpage = gv -> tpage;
		gt3 -> clut = gv -> clut;
		SetSemiTrans( gt3,0 );
		SetPolyGT3( gt3 );

		gt3ba = &tba -> prim[ buffer ].gt3;
		SetPolyGT3( gt3ba );
		gt3ba -> tpage = gv -> bump_tpage_a;
		gt3ba -> clut = gv -> bump_clut;
		SetSemiTrans( gt3ba,1 );
	
		gt3bs = &tbs -> prim[ buffer ].gt3;
		SetPolyGT3( gt3bs );
		gt3bs -> tpage = gv -> bump_tpage_s;
		gt3bs -> clut = gv -> bump_clut;
		SetSemiTrans( gt3bs,1 );


		/******************************************/
		/*	uv's												*/
		/******************************************/
		p =  obj -> model_stack.point_list;

		*((WORD *)&gt3 -> u0) = *((WORD *)&gt3b_prim -> uv0);
		*((WORD *)&gt3 -> u1) = *((WORD *)&gt3b_prim -> uv1);
		*((WORD *)&gt3 -> u2) = *((WORD *)&gt3b_prim -> uv2);
		
		*((WORD *)&gt3ba -> u0) = *((WORD *)&gt3b_prim -> uv0);
		*((WORD *)&gt3ba -> u1) = *((WORD *)&gt3b_prim -> uv1);
		*((WORD *)&gt3ba -> u2) = *((WORD *)&gt3b_prim -> uv2);

		*((WORD *)&gt3bs -> u0) = *((WORD *)&gt3b_prim -> uv0);
		*((WORD *)&gt3bs -> u1) = *((WORD *)&gt3b_prim -> uv1);
		*((WORD *)&gt3bs -> u2) = *((WORD *)&gt3b_prim -> uv2);

		
		gt3 -> r0 =
		gt3 -> r1 =
		gt3 -> r2 =

		gt3 -> g0 =
		gt3 -> g1 =
		gt3 -> g2 =

		gt3 -> b0 =
		gt3 -> b1 =
		gt3 -> b2 = 128;

		//alv = 69;
		//slv = 69;

		gt3ba -> r0 =
		gt3ba -> r1 =
		gt3ba -> r2 =

		gt3ba -> g0 =
		gt3ba -> g1 =
		gt3ba -> g2 =

		gt3ba -> b0 =
		gt3ba -> b1 =
		gt3ba -> b2 = ALV;

		gt3bs -> r0 =
		gt3bs -> r1 =
		gt3bs -> r2 =

		gt3bs -> g0 =
		gt3bs -> g1 =
		gt3bs -> g2 =

		gt3bs -> b0 =
		gt3bs -> b1 =
		gt3bs -> b2 = SLV;

		

		if( n > 0 )
			{

			gt3 -> x0 = xlist[ gt3b_prim -> v1 ].x;
			gt3 -> y0 = xlist[ gt3b_prim -> v1 ].y;
			gt3 -> x1 = xlist[ gt3b_prim -> v2 ].x;
			gt3 -> y1 = xlist[ gt3b_prim -> v2 ].y;
			gt3 -> x2 = xlist[ gt3b_prim -> v3 ].x;
			gt3 -> y2 = xlist[ gt3b_prim -> v3 ].y;

			gt3ba -> x0 = xlist[ gt3b_prim -> v1 ].x;
			gt3ba -> y0 = xlist[ gt3b_prim -> v1 ].y;
			gt3ba -> x1 = xlist[ gt3b_prim -> v2 ].x;
			gt3ba -> y1 = xlist[ gt3b_prim -> v2 ].y;
			gt3ba -> x2 = xlist[ gt3b_prim -> v3 ].x;
			gt3ba -> y2 = xlist[ gt3b_prim -> v3 ].y;

			gt3bs -> x0 = xlist[ gt3b_prim -> v1 ].x;
			gt3bs -> y0 = xlist[ gt3b_prim -> v1 ].y;
			gt3bs -> x1 = xlist[ gt3b_prim -> v2 ].x;
			gt3bs -> y1 = xlist[ gt3b_prim -> v2 ].y;
			gt3bs -> x2 = xlist[ gt3b_prim -> v3 ].x;
			gt3bs -> y2 = xlist[ gt3b_prim -> v3 ].y;

			depth = xlist[ gt3b_prim -> v1 ].z;
			depth += xlist[ gt3b_prim -> v2 ].z;
			depth += xlist[ gt3b_prim -> v3 ].z;
			depth /= 3;

			if( depth > NEAREST_Z )
				{
				depth >>= 2;

				addPrim( current_ot + depth, gt3ba );
				addPrim( current_ot + depth, gt3bs );
				addPrim( current_ot + depth, gt3 );
				}

			}

		gt3b_prim++;
		t = (Tri *)t -> link.next;
		tba = (Tri *)tba -> link.next;
		tbs = (Tri *)tbs -> link.next;

		}


}


/***************************************************************/
/*																					*/
/*	bumpmap textured tri, primitive draw								*/
/*																					*/
/***************************************************************/
void draw_gt3b( Object *obj, Xformed *xlist )
{
Tri			*t, *tba, *tbs;
GT3B_prim	*gt3b_prim;
POLY_GT3		*gt3, *gt3ba,*gt3bs;
LONG			depth,n,code,c;	
SVECTOR		v1,v2,v3, *magic;
WORD			flag;


	magic = last_magic;


//	c = obj -> model_stack.gt3b_count;
	c = obj -> model_stack.gt3b_pinfo.count;
	t = obj -> model_stack.tri_gt3b;
	tba = obj -> model_stack.tri_gt3ba;
	tbs = obj -> model_stack.tri_gt3bs;
//	gt3b_prim =  obj -> model_stack.gt3b_prim;
	gt3b_prim =  obj -> model_stack.gt3b_pinfo.mprim.gt3b_prim;
	while( c-- )			
		{

		gte_ldsxy3( *(LONG *)&xlist[ gt3b_prim -> v1 ].x,
					   *(LONG *)&xlist[ gt3b_prim -> v2 ].x,
					   *(LONG *)&xlist[ gt3b_prim -> v3 ].x );

		gte_nclip();
		gt3 = &t -> prim[ buffer ].gt3;
		gt3ba = &tba -> prim[ buffer ].gt3;
		gt3bs = &tbs -> prim[ buffer ].gt3;


		gte_stopz(&n);
	  	if( n > 0 )
			{
		

			#if 1
	  		depth = xlist[ gt3b_prim -> v1 ].z;
			depth += xlist[ gt3b_prim -> v2 ].z;
			depth += xlist[ gt3b_prim -> v3 ].z;
			depth /= 3;
			#endif


			if( depth > NEAREST_Z )
				{
	
				depth >>= 2;

				depth = 1;

				code = ((*(LONG *)&xlist[ gt3b_prim -> v1 ].x)^CLIPBITS);
				code |= ((*(LONG *)&xlist[ gt3b_prim -> v2 ].x)^CLIPBITS);
				code |= ((*(LONG *)&xlist[ gt3b_prim -> v3 ].x)^CLIPBITS);

				if( (code & CLIPBITS) == CLIPBITS )
					{

					code = gt3 -> code;
					gte_ldv0( &gt3b_prim -> bm0.a );
					gte_ncs();
					gte_strgb((CVECTOR *)&gt3 -> r0);
					gt3 -> code = code;

					gte_ldv0( &gt3b_prim -> bm1.a );
					gte_ncs();
					gte_strgb((CVECTOR *)&gt3 -> r1);

					gte_ldv0( &gt3b_prim -> bm2.a );
					gte_ncs();
					gte_strgb((CVECTOR *)&gt3 -> r2);

					gte_stsxy3_gt3( gt3 );
					gte_stsxy3_gt3( gt3ba );
					gte_stsxy3_gt3( gt3bs );

					bump_mat( magic, &gt3b_prim -> bm0, &v1 );
					//gt3bs -> u0 = gt3b_prim -> uv0.u + (UBYTE)v1.vy;
					//gt3bs -> v0 = gt3b_prim -> uv0.v + (UBYTE)v1.vz;

					bump_mat( magic, &gt3b_prim -> bm1, &v2 );
					//gt3bs -> u1 = gt3b_prim -> uv1.u + (UBYTE)v2.vy;
					//gt3bs -> v1 = gt3b_prim -> uv1.v + (UBYTE)v2.vz;

					bump_mat( magic, &gt3b_prim -> bm2, &v3 );
				  	//gt3bs -> u2 = gt3b_prim -> uv2.u + (UBYTE)v3.vy;
					//gt3bs -> v2 = gt3b_prim -> uv2.v + (UBYTE)v3.vz;

					#if 1

					flag = (WORD)gt3b_prim -> uv0.u + v1.vy;
					cap( &flag, 255,0 );
					gt3bs -> u0 = (UBYTE )flag;
					flag = (WORD)gt3b_prim -> uv0.v + v1.vz;
					cap( &flag, 255,0 );
					gt3bs -> v0 = (UBYTE )flag;

					flag = (WORD)gt3b_prim -> uv1.u + v2.vy;
					cap( &flag, 255,0 );
					gt3bs -> u1 = (UBYTE )flag;
					flag = (WORD)gt3b_prim -> uv1.v + v2.vz;
					cap( &flag, 255,0 );
					gt3bs -> v1 = (UBYTE )flag;


					flag = (WORD)gt3b_prim -> uv2.u + v3.vy;
					cap( &flag, 255,0 );
					gt3bs -> u2 = (UBYTE )flag;
					flag = (WORD)gt3b_prim -> uv2.v + v3.vz;
					cap( &flag, 255,0 );
					gt3bs -> v2 = (UBYTE )flag;

					#endif

					#ifdef SHADE_BUMPMAP
					gt3ba -> r0 =
					gt3bs -> r0 =
					gt3   -> r0 >> BUMPSHIFT;
					gt3ba -> g0 =
					gt3bs -> g0 =
					gt3   -> g0 >> BUMPSHIFT;
					gt3ba -> b0 =
					gt3bs -> b0 =
					gt3   -> b0 >> BUMPSHIFT;

					gt3ba -> r1 =
					gt3bs -> r1 =
					gt3   -> r1 >> BUMPSHIFT;
					gt3ba -> g1 =
					gt3bs -> g1 =
					gt3   -> g1 >> BUMPSHIFT;
					gt3ba -> b1 =
					gt3bs -> b1 =
					gt3   -> b1 >> BUMPSHIFT;

					gt3ba -> r2 =
					gt3bs -> r2 =
					gt3   -> r2 >> BUMPSHIFT;
					gt3ba -> g2 =
					gt3bs -> g2 =
					gt3   -> g2 >> BUMPSHIFT;
					gt3ba -> b2 =
					gt3bs -> b2 =
					gt3   -> b2 >> BUMPSHIFT;
					#endif


					#if 0
				  	gt3ba -> x0 += alx;
				  	gt3ba -> x1 += alx;
				  	gt3ba -> x2 += alx;

				  	gt3ba -> y0 += aly;
				  	gt3ba -> y1 += aly;
				  	gt3ba -> y2 += aly;
					#endif

					#ifndef SHADE_BUMPMAP
				  	gt3ba -> r0 = 
				  	gt3ba -> r1 = 
				  	gt3ba -> r2 = 
				  	gt3ba -> g0 = 
				  	gt3ba -> g1 = 
				  	gt3ba -> g2 = 
				  	gt3ba -> b0 = 
				  	gt3ba -> b1 = 
				  	gt3ba -> b2 = ALV;

				  	gt3bs -> r0 = 
				  	gt3bs -> r1 = 
				  	gt3bs -> r2 = 
				  	gt3bs -> g0 = 
				  	gt3bs -> g1 = 
				  	gt3bs -> g2 = 
				  	gt3bs -> b0 = 
				  	gt3bs -> b1 = 
				  	gt3bs -> b2 = SLV;
					#endif


					#if 0
				  	gt3bs -> x0 += slx;
				  	gt3bs -> x1 += slx;
				  	gt3bs -> x2 += slx;

				  	gt3bs -> y0 += sly;
				  	gt3bs -> y1 += sly;
				  	gt3bs -> y2 += sly;
					#endif


			 		addPrim( current_ot + depth, gt3bs );
					addPrim( current_ot + depth, gt3ba );
					addPrim( current_ot + depth, gt3 );

					}
				}
			}


		gt3b_prim++;
		t = (Tri *)t -> link.next;
		tba = (Tri *)tba -> link.next;
		tbs = (Tri *)tbs -> link.next;


		}

}

/***************************************************************/
/*																					*/
/*	calculate bumpmap subtractive layer offest						*/
/*																					*/
/***************************************************************/
void bump_mat( SVECTOR *magic, UVMat *bm, SVECTOR *v )
{
MATRIX	uvmat;

	uvmat.m[0][0] = bm -> a.vx;
	uvmat.m[1][0] = bm -> b.vx;
	uvmat.m[2][0] = bm -> c.vx;

	uvmat.m[0][1] = bm -> a.vy;
	uvmat.m[1][1] = bm -> b.vy;
	uvmat.m[2][1] = bm -> c.vy;

	uvmat.m[0][2] = bm -> a.vz;
	uvmat.m[1][2] = bm -> b.vz;
	uvmat.m[2][2] = bm -> c.vz;

	ApplyMatrixSV( &uvmat, magic, v );

	v -> vx >>= 12-BUMP_PIXEL_SIZE;
	v -> vy >>= 12-BUMP_PIXEL_SIZE;
	v -> vz >>= 12-BUMP_PIXEL_SIZE;


}

void cap( WORD *v, WORD top, WORD bot )
{

	if( *v  > top )
		*v = top;
	if( *v < bot )
		*v = bot;

}



#if 0
/***************************************************************/
/*																					*/
/*	textured tri, primitive draw											*/
/*																					*/
/***************************************************************/
void draw_gt3( Object *obj, Xformed *xlist )
{
LONG		c;
Tri		*t;
GT3_prim	*gt3_prim;
POLY_GT3	*gt3;
LONG		depth;	
LONG		n;	
LONG		code;


//	c = obj -> model_stack.gt3_count;
	c = obj -> model_stack.gt3_pinfo.count;
	t = obj -> model_stack.tri_gt3;
	gt3_prim =  obj -> model_stack.gt3_prim;
	while( c-- )			
		{

		gte_ldsxy3( *(LONG *)&xlist[ gt3_prim -> v1 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v2 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v3 ].x );

		gte_nclip();
		gt3 = &t -> prim[ buffer ].gt3;

		gte_stopz(&n);
	  	if( n > 0 )
			{
		

			#if 1
	  		depth = xlist[ gt3_prim -> v1 ].z;
			depth += xlist[ gt3_prim -> v2 ].z;
			depth += xlist[ gt3_prim -> v3 ].z;
			depth /= 3;
			#endif


			if( depth > NEAREST_Z )
				{
	
				depth >>= 2;

				code = ((*(LONG *)&xlist[ gt3_prim -> v1 ].x)^CLIPBITS);
				code |= ((*(LONG *)&xlist[ gt3_prim -> v2 ].x)^CLIPBITS);
				code |= ((*(LONG *)&xlist[ gt3_prim -> v3 ].x)^CLIPBITS);

				if( (code & CLIPBITS) == CLIPBITS )
					{

					code = gt3 -> code;
					gte_ldv0( &gt3_prim -> n0 );
					gte_ncs();
					gte_strgb((CVECTOR *)&gt3 -> r0);
					gt3 -> code = code;

					gte_ldv0( &gt3_prim -> n1 );
					gte_ncs();
					gte_strgb((CVECTOR *)&gt3 -> r1);

					gte_ldv0( &gt3_prim -> n2 );
					gte_ncs();
					gte_strgb((CVECTOR *)&gt3 -> r2);

					gte_stsxy3_gt3( gt3 );

					addPrim( current_ot + depth, gt3 );



					}
				}
			}


		gt3_prim++;
		t = (Tri *)t -> link.next;


		}

}

/***************************************************************/
/*																					*/
/*	textured tri, primitive draw											*/
/*																					*/
/***************************************************************/
void draw_shadow_gt3( Object *obj, Xformed *xlist )
{
LONG		c;
Tri		*t;
GT3_prim	*gt3_prim;
POLY_GT3	*gt3;
LONG		depth;	
LONG		n;	
LONG		code;


//	c = obj -> model_stack.gt3_count;
	c = obj -> model_stack.gt3_pinfo.count;
	t = obj -> model_stack.tri_gt3;
	gt3_prim =  obj -> model_stack.gt3_prim;
	while( c-- )			
		{

		gte_ldsxy3( *(LONG *)&xlist[ gt3_prim -> v1 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v2 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v3 ].x );

		gte_nclip();
		gt3 = &t -> prim[ buffer ].gt3;

		gte_stopz(&n);
	  	if( n > 0 )
			{
		

			#if 1
	  		depth = xlist[ gt3_prim -> v1 ].z;
			depth += xlist[ gt3_prim -> v2 ].z;
			depth += xlist[ gt3_prim -> v3 ].z;
			depth /= 3;
			#endif


			if( depth > NEAREST_Z )
				{
	
				depth >>= 2;

				code = ((*(LONG *)&xlist[ gt3_prim -> v1 ].x)^CLIPBITS);
				code |= ((*(LONG *)&xlist[ gt3_prim -> v2 ].x)^CLIPBITS);
				code |= ((*(LONG *)&xlist[ gt3_prim -> v3 ].x)^CLIPBITS);

				if( (code & CLIPBITS) == CLIPBITS )
					{

					gt3 -> r0 =
					gt3 -> g0 =
					gt3 -> b0 =

					gt3 -> r1 =
					gt3 -> g1 =
					gt3 -> b1 =

					gt3 -> r2 =
					gt3 -> g2 =
					gt3 -> b2 = 0;


					gte_stsxy3_gt3( gt3 );

					addPrim( current_ot + depth, gt3 );


					}
				}
			}


		gt3_prim++;
		t = (Tri *)t -> link.next;


		}

}






/***************************************************************/
/*																					*/
/*	reflective textured tri, primitive draw							*/
/*																					*/
/***************************************************************/
void draw_gt3r( Object *obj, Xformed *xlist )
{
LONG		c;
Tri		*t, *tr;
GT3_prim	*gt3_prim;
POLY_GT3	*gt3, *gt3r;
LONG		depth;	
LONG		n;	
LONG		code, rcode;

	c = obj -> model_stack.gt3r_count;
	t = obj -> model_stack.tri_gt3r;
	tr = obj -> model_stack.tri_gt3ra;
	gt3_prim =  obj -> model_stack.gt3r_prim;
	while( c-- )			
		{

		gte_ldsxy3( *(LONG *)&xlist[ gt3_prim -> v1 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v2 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v3 ].x );

		gte_nclip();
		gt3 = &t -> prim[ buffer ].gt3;
		gt3r = &tr -> prim[ buffer ].gt3;

		gte_stopz(&n);

	  	if( n > 0 )
			{
		

			#if 1
	  		depth = xlist[ gt3_prim -> v1 ].z;
			depth += xlist[ gt3_prim -> v2 ].z;
			depth += xlist[ gt3_prim -> v3 ].z;
			depth /= 3;
			#endif


			if( depth > NEAREST_Z )
				{
	
				depth >>= 2;

				code = ((*(LONG *)&xlist[ gt3_prim -> v1 ].x)^CLIPBITS);
				code |= ((*(LONG *)&xlist[ gt3_prim -> v2 ].x)^CLIPBITS);
				code |= ((*(LONG *)&xlist[ gt3_prim -> v3 ].x)^CLIPBITS);

				if( (code & CLIPBITS) == CLIPBITS )
					{

					code = gt3 -> code;
					rcode = gt3r -> code;
					gte_ldv0( &gt3_prim -> n0 );
					gte_ncs();
				  	gte_strgb((CVECTOR *)&gt3 -> r0);
				  	gte_strgb((CVECTOR *)&gt3r -> r0);
					gt3 -> code = code;
					gt3r -> code = rcode;

					gte_ldv0( &gt3_prim -> n1 );
					gte_ncs();
					gte_strgb((CVECTOR *)&gt3 -> r1);
					gte_strgb((CVECTOR *)&gt3r -> r1);

					gte_ldv0( &gt3_prim -> n2 );
					gte_ncs();
					gte_strgb((CVECTOR *)&gt3 -> r2);
					gte_strgb((CVECTOR *)&gt3r -> r2);

					gte_stsxy3_gt3( gt3 );
					gte_stsxy3_gt3( gt3r );


					PushMatrix();
					SetRotMatrix( (MATRIX *)&obj -> axis );
				
					getuv3( &gt3_prim -> n0, gt3r );

					PopMatrix();


					addPrim( current_ot + depth, gt3r );
					addPrim( current_ot + depth, gt3 );

					}
				}
			}


		gt3_prim++;
		t = (Tri *)t -> link.next;
		tr = (Tri *)tr -> link.next;


		}

}



/***************************************************************/
/*																					*/
/*	chrome textured tri, primitive draw									*/
/*																					*/
/***************************************************************/
void draw_gt3c( Object *obj, Xformed *xlist )
{
Tri			*t;
GT3C_prim	*gt3_prim;
POLY_GT3		*gt3c;
LONG			depth,n,code,c;	

	c = obj -> model_stack.gt3c_count;
	t = obj -> model_stack.tri_gt3c;
	gt3_prim =  obj -> model_stack.gt3c_prim;
	while( c-- )			
		{

		gte_ldsxy3( *(LONG *)&xlist[ gt3_prim -> v1 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v2 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v3 ].x );

		gte_nclip();
		gt3c = &t -> prim[ buffer ].gt3;

		gte_stopz(&n);

	  	if( n > 0 )
			{
		

			#if 1
	  		depth = xlist[ gt3_prim -> v1 ].z;
			depth += xlist[ gt3_prim -> v2 ].z;
			depth += xlist[ gt3_prim -> v3 ].z;
			depth /= 3;
			#endif


			if( depth > NEAREST_Z )
				{
	
				depth >>= 2;

				code = ((*(LONG *)&xlist[ gt3_prim -> v1 ].x)^CLIPBITS);
				code |= ((*(LONG *)&xlist[ gt3_prim -> v2 ].x)^CLIPBITS);
				code |= ((*(LONG *)&xlist[ gt3_prim -> v3 ].x)^CLIPBITS);

				if( (code & CLIPBITS) == CLIPBITS )
					{

					code = gt3c -> code;
					gte_ldv0( &gt3_prim -> n0 );
					gte_ncs();
				  	gte_strgb((CVECTOR *)&gt3c -> r0);
					gt3c -> code = code;

					gte_ldv0( &gt3_prim -> n1 );
					gte_ncs();
					gte_strgb((CVECTOR *)&gt3c -> r1);

					gte_ldv0( &gt3_prim -> n2 );
					gte_ncs();
					gte_strgb((CVECTOR *)&gt3c -> r2);

					gte_stsxy3_gt3( gt3c );

					PushMatrix();
					SetRotMatrix( (MATRIX *)&obj -> axis );
				
					getuv3( &gt3_prim -> n0, gt3c );

					PopMatrix();


					addPrim( current_ot + depth, gt3c );

					}
				}
			}


		gt3_prim++;
		t = (Tri *)t -> link.next;

		}

}


/***************************************************************/
/*																					*/
/*	chrome textured tri, primitive draw									*/
/*																					*/
/***************************************************************/
void draw_gt3g( Object *obj, Xformed *xlist )
{
Tri			*t;
GT3G_prim	*gt3_prim;
POLY_GT3		*gt3g;
LONG			depth,n,code,c;	

	c = obj -> model_stack.gt3g_count;
	t = obj -> model_stack.tri_gt3g;
	gt3_prim =  obj -> model_stack.gt3g_prim;
	while( c-- )			
		{

		gte_ldsxy3( *(LONG *)&xlist[ gt3_prim -> v1 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v2 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v3 ].x );

		gte_nclip();
		gt3g = &t -> prim[ buffer ].gt3;

		gte_stopz(&n);

		n = 1;
	  	if( n > 0 )
			{
		

			#if 1
	  		depth = xlist[ gt3_prim -> v1 ].z;
			depth += xlist[ gt3_prim -> v2 ].z;
			depth += xlist[ gt3_prim -> v3 ].z;
			depth /= 3;
			#endif


			if( depth > NEAREST_Z )
				{
	
				depth >>= 2;

				code = ((*(LONG *)&xlist[ gt3_prim -> v1 ].x)^CLIPBITS);
				code |= ((*(LONG *)&xlist[ gt3_prim -> v2 ].x)^CLIPBITS);
				code |= ((*(LONG *)&xlist[ gt3_prim -> v3 ].x)^CLIPBITS);

			 	//if( (code & CLIPBITS) == CLIPBITS )
					{

					code = gt3g -> code;
					gte_ldv0( &gt3_prim -> n0 );
					gte_ncs();
				  	gte_strgb((CVECTOR *)&gt3g -> r0);
					gt3g -> code = code;

					gte_ldv0( &gt3_prim -> n1 );
					gte_ncs();
					gte_strgb((CVECTOR *)&gt3g -> r1);

					gte_ldv0( &gt3_prim -> n2 );
					gte_ncs();
					gte_strgb((CVECTOR *)&gt3g -> r2);

					gte_stsxy3_gt3( gt3g );

					PushMatrix();
					SetRotMatrix( (MATRIX *)&obj -> axis );
				
					getuv3( &gt3_prim -> n0, gt3g );

					PopMatrix();


					addPrim( current_ot + depth, gt3g );

					}
				}
			}


		gt3_prim++;
		t = (Tri *)t -> link.next;

		}

}


/*********************************************************************************/
/*																											*/
/*	reflective bumpmaped textured lightsourced (!!)tri, primitive draw				*/
/*																											*/
/*********************************************************************************/
void draw_gt3rb( Object *obj, Xformed *xlist )
{
Tri			*t, *tba, *tbs, *tbr;
GT3RB_prim	*gt3rb_prim;
POLY_GT3		*gt3, *gt3rba,*gt3rbs,*gt3rbr;
LONG			depth,n,code,coder,c;	
SVECTOR		v1,v2,v3, *magic;

	magic = last_magic;

	c = obj -> model_stack.gt3rb_count;
	t = obj -> model_stack.tri_gt3rb;
	tba = obj -> model_stack.tri_gt3rba;
	tbs = obj -> model_stack.tri_gt3rbs;
	tbr = obj -> model_stack.tri_gt3rbr;
	gt3rb_prim =  obj -> model_stack.gt3rb_prim;
	while( c-- )			
		{

		gte_ldsxy3( *(LONG *)&xlist[ gt3rb_prim -> v1 ].x,
					   *(LONG *)&xlist[ gt3rb_prim -> v2 ].x,
					   *(LONG *)&xlist[ gt3rb_prim -> v3 ].x );

		gte_nclip();
		gt3 = &t -> prim[ buffer ].gt3;
		gt3rba = &tba -> prim[ buffer ].gt3;
		gt3rbs = &tbs -> prim[ buffer ].gt3;
		gt3rbr = &tbr -> prim[ buffer ].gt3;


		gte_stopz(&n);
	  	if( n > 0 )
			{
		

			#if 1
	  		depth = xlist[ gt3rb_prim -> v1 ].z;
			depth += xlist[ gt3rb_prim -> v2 ].z;
			depth += xlist[ gt3rb_prim -> v3 ].z;
			depth /= 3;
			#endif


			if( depth > NEAREST_Z )
				{
	
				depth >>= 2;

				code = ((*(LONG *)&xlist[ gt3rb_prim -> v1 ].x)^CLIPBITS);
				code |= ((*(LONG *)&xlist[ gt3rb_prim -> v2 ].x)^CLIPBITS);
				code |= ((*(LONG *)&xlist[ gt3rb_prim -> v3 ].x)^CLIPBITS);

				if( (code & CLIPBITS) == CLIPBITS )
					{

					code = gt3 -> code;
					coder = gt3rbr -> code;
					gte_ldv0( &gt3rb_prim -> bm0.a );
					gte_ncs();
					gte_strgb((CVECTOR *)&gt3 -> r0);
					gte_strgb((CVECTOR *)&gt3rbr -> r0);
					gt3 -> code = code;
					gt3rbr -> code = coder;

					gte_ldv0( &gt3rb_prim -> bm1.a );
					gte_ncs();
					gte_strgb((CVECTOR *)&gt3 -> r1);
					gte_strgb((CVECTOR *)&gt3rbr -> r1);

					gte_ldv0( &gt3rb_prim -> bm2.a );
					gte_ncs();
					gte_strgb((CVECTOR *)&gt3 -> r2);
					gte_strgb((CVECTOR *)&gt3rbr -> r2);

					gte_stsxy3_gt3( gt3 );
					gte_stsxy3_gt3( gt3rba );
					gte_stsxy3_gt3( gt3rbs );
					gte_stsxy3_gt3( gt3rbr );

					bump_mat( magic, &gt3rb_prim -> bm0, &v1 );
					gt3rbs -> u0 = gt3rb_prim -> uv0.u - (UBYTE)v1.vy;
					gt3rbs -> v0 = gt3rb_prim -> uv0.v - (UBYTE)v1.vz;

					bump_mat( magic, &gt3rb_prim -> bm1, &v2 );
					gt3rbs -> u1 = gt3rb_prim -> uv1.u - (UBYTE)v2.vy;
					gt3rbs -> v1 = gt3rb_prim -> uv1.v - (UBYTE)v2.vz;

					bump_mat( magic, &gt3rb_prim -> bm2, &v3 );
				  	gt3rbs -> u2 = gt3rb_prim -> uv2.u - (UBYTE)v3.vy;
					gt3rbs -> v2 = gt3rb_prim -> uv2.v - (UBYTE)v3.vz;


					#if 0

				  	gt3rba -> x0 += alx;
				  	gt3rba -> x1 += alx;
				  	gt3rba -> x2 += alx;

				  	gt3rba -> y0 += aly;
				  	gt3rba -> y1 += aly;
				  	gt3rba -> y2 += aly;

				  	gt3rba -> r0 = 
				  	gt3rba -> r1 = 
				  	gt3rba -> r2 = 
				  	gt3rba -> g0 = 
				  	gt3rba -> g1 = 
				  	gt3rba -> g2 = 
				  	gt3rba -> b0 = 
				  	gt3rba -> b1 = 
				  	gt3rba -> b2 = ALV;



				  	gt3rbs -> x0 += slx;
				  	gt3rbs -> x1 += slx;
				  	gt3rbs -> x2 += slx;

				  	gt3rbs -> y0 += sly;
				  	gt3rbs -> y1 += sly;
				  	gt3rbs -> y2 += sly;

				  	gt3rbs -> r0 = 
				  	gt3rbs -> r1 = 
				  	gt3rbs -> r2 = 
				  	gt3rbs -> g0 = 
				  	gt3rbs -> g1 = 
				  	gt3rbs -> g2 = 
				  	gt3rbs -> b0 = 
				  	gt3rbs -> b1 = 
				  	gt3rbs -> b2 = slv;

					#endif

					gt3rba -> r0 =
					gt3rbs -> r0 =
					gt3   -> r0 >> BUMPSHIFT;
					gt3rba -> g0 =
					gt3rbs -> g0 =
					gt3   -> g0 >> BUMPSHIFT;
					gt3rba -> b0 =
					gt3rbs -> b0 =
					gt3   -> b0 >> BUMPSHIFT;

					gt3rba -> r1 =
					gt3rbs -> r1 =
					gt3   -> r1 >> BUMPSHIFT;
					gt3rba -> g1 =
					gt3rbs -> g1 =
					gt3   -> g1 >> BUMPSHIFT;
					gt3rba -> b1 =
					gt3rbs -> b1 =
					gt3   -> b1 >> BUMPSHIFT;

					gt3rba -> r2 =
					gt3rbs -> r2 =
					gt3   -> r2 >> BUMPSHIFT;
					gt3rba -> g2 =
					gt3rbs -> g2 =
					gt3   -> g2 >> BUMPSHIFT;
					gt3rba -> b2 =
					gt3rbs -> b2 =
					gt3   -> b2 >> BUMPSHIFT;


					PushMatrix();
					SetRotMatrix( (MATRIX *)&obj -> axis );
					getuv3b( gt3rb_prim, gt3rbr );
					PopMatrix();


			 		addPrim( current_ot + depth, gt3rbr );
			 		addPrim( current_ot + depth, gt3rbs );
					addPrim( current_ot + depth, gt3rba );
					addPrim( current_ot + depth, gt3 );


					}
				}
			}


		gt3rb_prim++;
		t = (Tri *)t -> link.next;
		tba = (Tri *)tba -> link.next;
		tbs = (Tri *)tbs -> link.next;
		tbr = (Tri *)tbr -> link.next;


		}

}




/***************************************************************/
/*	calculate reflection map uv's											*/
/***************************************************************/
void getuv3( SVECTOR *n, POLY_GT3 *gt3 )
{
VECTOR	in;

	ApplyRotMatrix( n++, &in );
	gt3 -> u0 = ( in.vx >> 5 ) + 128;
	gt3 -> v0 = ( in.vy >> 5 ) + 128;

	ApplyRotMatrix( n++, &in );
	gt3 -> u1 = ( in.vx >> 5 ) + 128;
	gt3 -> v1 = ( in.vy >> 5 ) + 128;

	ApplyRotMatrix( n++, &in );
	gt3 -> u2 = ( in.vx >> 5 ) + 128;
	gt3 -> v2 = ( in.vy >> 5 ) + 128;

}

/***************************************************************/
/*	calculate reflection map uv's											*/
/***************************************************************/
void getuv3b( GT3RB_prim *p, POLY_GT3 *gt3 )
{
VECTOR	in;

	ApplyRotMatrix( &p -> bm0.a, &in );
	gt3 -> u0 = ( in.vx >> 5 ) + 128;
	gt3 -> v0 = ( in.vy >> 5 ) + 128;

	ApplyRotMatrix( &p -> bm1.a, &in );
	gt3 -> u1 = ( in.vx >> 5 ) + 128;
	gt3 -> v1 = ( in.vy >> 5 ) + 128;

	ApplyRotMatrix( &p -> bm2.a, &in );
	gt3 -> u2 = ( in.vx >> 5 ) + 128;
	gt3 -> v2 = ( in.vy >> 5 ) + 128;

}




/***************************************************************/
/*																					*/
/*	textured tri, pre setup,& draw										*/
/*																					*/
/***************************************************************/
void draw_set_gt3( Object *obj, Xformed *xlist )
{
Tri		*t;
GT3_prim	*gt3_prim;
POLY_GT3	*gt3;
LONG		n,depth,c;	
Point		*p;
Gfx_vram	*gv;


	
	gv = &installed_gfx[ obj -> model_stack.set ];
//	c = obj -> model_stack.gt3_count;
	c = obj -> model_stack.gt3_pinfo.count;
	t = obj -> model_stack.tri_gt3;
	gt3_prim =  obj -> model_stack.gt3_prim;
	while( c-- )			
		{
		gte_ldsxy3( *(LONG *)&xlist[ gt3_prim -> v1 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v2 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v3 ].x );

		gte_nclip();
		gte_stopz(&n);

		gt3 = &t -> prim[ buffer ].gt3;
		SetPolyGT3( gt3 );
		SetSemiTrans( gt3,0 );
		gt3 -> tpage = gv -> tpage;
		gt3 -> clut = gv -> clut;


		/******************************************/
		/*	uv's												*/
		/******************************************/
		p =  obj -> model_stack.point_list;

		*((WORD *)&gt3 -> u0) = *((WORD *)&gt3_prim -> uv0);
		*((WORD *)&gt3 -> u1) = *((WORD *)&gt3_prim -> uv1);
		*((WORD *)&gt3 -> u2) = *((WORD *)&gt3_prim -> uv2);
		
		gt3 -> r0 =
		gt3 -> r1 =
		gt3 -> r2 =

		gt3 -> g0 =
		gt3 -> g1 =
		gt3 -> g2 =

		gt3 -> b0 =
		gt3 -> b1 =
		gt3 -> b2 = 128;

		if( n > 0 )
			{

			gt3 -> x0 = xlist[ gt3_prim -> v1 ].x;
			gt3 -> y0 = xlist[ gt3_prim -> v1 ].y;
			gt3 -> x1 = xlist[ gt3_prim -> v2 ].x;
			gt3 -> y1 = xlist[ gt3_prim -> v2 ].y;
			gt3 -> x2 = xlist[ gt3_prim -> v3 ].x;
			gt3 -> y2 = xlist[ gt3_prim -> v3 ].y;

			depth = xlist[ gt3_prim -> v1 ].z;
			depth += xlist[ gt3_prim -> v2 ].z;
			depth += xlist[ gt3_prim -> v3 ].z;
			depth /= 3;

			if( depth > NEAREST_Z )
				{
				depth >>= 2;

				addPrim( current_ot + depth, gt3 );


				}

			}

		gt3_prim++;
		t = (Tri *)t -> link.next;

		}


}



/***************************************************************/
/*																					*/
/*	textured reflective, pre setup,& draw 								*/
/*																					*/
/***************************************************************/
void draw_set_gt3r( Object *obj, Xformed *xlist )
{
Tri		*t, *tr;
GT3_prim	*gt3_prim;
POLY_GT3	*gt3r, *gt3ra;
LONG		n,depth,c;	
Point		*p;
Gfx_vram	*gv;

	gv = &installed_gfx[ obj -> model_stack.set ];
	c = obj -> model_stack.gt3r_count;
	t = obj -> model_stack.tri_gt3r;
	tr = obj -> model_stack.tri_gt3ra;
	gt3_prim =  obj -> model_stack.gt3r_prim;
	while( c-- )			
		{
		gte_ldsxy3( *(LONG *)&xlist[ gt3_prim -> v1 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v2 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v3 ].x );

		gte_nclip();
		gte_stopz(&n);

		gt3r = &t -> prim[ buffer ].gt3;
		SetPolyGT3( gt3r );
		SetSemiTrans( gt3r,0 );
		gt3r -> tpage = gv -> tpage;
		gt3r -> clut = gv -> clut;

		gt3ra = &tr -> prim[ buffer ].gt3;
		SetPolyGT3( gt3ra );
		SetSemiTrans( gt3ra,1 );
		gt3ra -> tpage = gv -> ref_tpage_a;
		gt3ra -> clut = gv -> ref_clut;

		/******************************************/
		/*	uv's												*/
		/******************************************/
		p =  obj -> model_stack.point_list;

		*((WORD *)&gt3r -> u0) = *((WORD *)&gt3_prim -> uv0);
		*((WORD *)&gt3r -> u1) = *((WORD *)&gt3_prim -> uv1);
		*((WORD *)&gt3r -> u2) = *((WORD *)&gt3_prim -> uv2);
		
		gt3r -> r0 =
		gt3r -> r1 =
		gt3r -> r2 =

		gt3r -> g0 =
		gt3r -> g1 =
		gt3r -> g2 =

		gt3r -> b0 =
		gt3r -> b1 =
		gt3r -> b2 = 128;



		gt3ra -> r0 =
		gt3ra -> r1 =
		gt3ra -> r2 =

		gt3ra -> g0 =
		gt3ra -> g1 =
		gt3ra -> g2 =

		gt3ra -> b0 =
		gt3ra -> b1 =
		gt3ra -> b2 = 128;

		if( n > 0 )
			{

			gt3ra -> x0 = gt3r -> x0 = xlist[ gt3_prim -> v1 ].x;
			gt3ra -> y0 = gt3r -> y0 = xlist[ gt3_prim -> v1 ].y;
			gt3ra -> x1 = gt3r -> x1 = xlist[ gt3_prim -> v2 ].x;
			gt3ra -> y1 = gt3r -> y1 = xlist[ gt3_prim -> v2 ].y;
			gt3ra -> x2 = gt3r -> x2 = xlist[ gt3_prim -> v3 ].x;
			gt3ra -> y2 = gt3r -> y2 = xlist[ gt3_prim -> v3 ].y;


			depth = xlist[ gt3_prim -> v1 ].z;
			depth += xlist[ gt3_prim -> v2 ].z;
			depth += xlist[ gt3_prim -> v3 ].z;
			depth /= 3;

			if( depth > NEAREST_Z )
				{
				depth >>= 2;

				addPrim( current_ot + depth, gt3ra );
				addPrim( current_ot + depth, gt3r );
				}

			}

		gt3_prim++;
		t = (Tri *)t -> link.next;
		tr = (Tri *)tr -> link.next;

		}


}


/***************************************************************/
/*																					*/
/*	textured chrome, pre setup,& draw 									*/
/*																					*/
/***************************************************************/
void draw_set_gt3c( Object *obj, Xformed *xlist )
{
Tri			*t;
GT3C_prim	*gt3_prim;
POLY_GT3		*gt3c;
LONG			n,depth,c;	
Point			*p;
Gfx_vram		*gv;

	gv = &installed_gfx[ obj -> model_stack.set ];
	c = obj -> model_stack.gt3c_count;
	t = obj -> model_stack.tri_gt3c;
	gt3_prim =  obj -> model_stack.gt3c_prim;
	while( c-- )			
		{
		gte_ldsxy3( *(LONG *)&xlist[ gt3_prim -> v1 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v2 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v3 ].x );

		gte_nclip();
		gte_stopz(&n);

		gt3c = &t -> prim[ buffer ].gt3;
		SetPolyGT3( gt3c );
		SetSemiTrans( gt3c,0 );
		gt3c -> tpage = gv -> ref_tpage;
		gt3c -> clut = gv -> ref_clut;

		/******************************************/
		/*	uv's												*/
		/******************************************/
		p =  obj -> model_stack.point_list;

		gt3c -> r0 =
		gt3c -> r1 =
		gt3c -> r2 =

		gt3c -> g0 =
		gt3c -> g1 =
		gt3c -> g2 =

		gt3c -> b0 =
		gt3c -> b1 =
		gt3c -> b2 = 128;


		if( n > 0 )
			{

			gt3c -> x0 = xlist[ gt3_prim -> v1 ].x;
			gt3c -> y0 = xlist[ gt3_prim -> v1 ].y;
			gt3c -> x1 = xlist[ gt3_prim -> v2 ].x;
			gt3c -> y1 = xlist[ gt3_prim -> v2 ].y;
			gt3c -> x2 = xlist[ gt3_prim -> v3 ].x;
			gt3c -> y2 = xlist[ gt3_prim -> v3 ].y;


			depth = xlist[ gt3_prim -> v1 ].z;
			depth += xlist[ gt3_prim -> v2 ].z;
			depth += xlist[ gt3_prim -> v3 ].z;
			depth /= 3;

			if( depth > NEAREST_Z )
				{
				depth >>= 2;

				addPrim( current_ot + depth, gt3c );
				}

			}

		gt3_prim++;
		t = (Tri *)t -> link.next;

		}


}

/***************************************************************/
/*																					*/
/*	glass prim, pre setup,& draw 											*/
/*																					*/
/***************************************************************/
void draw_set_gt3g( Object *obj, Xformed *xlist )
{
Tri			*t;
GT3G_prim	*gt3_prim;
POLY_GT3		*gt3g;
LONG			n,depth,c;	
Point			*p;
Gfx_vram		*gv;

	gv = &installed_gfx[ obj -> model_stack.set ];
	c = obj -> model_stack.gt3g_count;
	t = obj -> model_stack.tri_gt3g;
	gt3_prim =  obj -> model_stack.gt3g_prim;
	while( c-- )			
		{
		gte_ldsxy3( *(LONG *)&xlist[ gt3_prim -> v1 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v2 ].x,
					   *(LONG *)&xlist[ gt3_prim -> v3 ].x );

		gte_nclip();
		gte_stopz(&n);

		gt3g = &t -> prim[ buffer ].gt3;
		SetPolyGT3( gt3g );
		SetSemiTrans( gt3g,1 );
		gt3g -> tpage = gv -> ref_tpage_g;
		gt3g -> clut = gv -> ref_clut;

		/******************************************/
		/*	uv's												*/
		/******************************************/
		p =  obj -> model_stack.point_list;

		gt3g -> r0 =
		gt3g -> r1 =
		gt3g -> r2 =

		gt3g -> g0 =
		gt3g -> g1 =
		gt3g -> g2 =

		gt3g -> b0 =
		gt3g -> b1 =
		gt3g -> b2 = 128;


		if( n > 0 )
			{

			gt3g -> x0 = xlist[ gt3_prim -> v1 ].x;
			gt3g -> y0 = xlist[ gt3_prim -> v1 ].y;
			gt3g -> x1 = xlist[ gt3_prim -> v2 ].x;
			gt3g -> y1 = xlist[ gt3_prim -> v2 ].y;
			gt3g -> x2 = xlist[ gt3_prim -> v3 ].x;
			gt3g -> y2 = xlist[ gt3_prim -> v3 ].y;


			depth = xlist[ gt3_prim -> v1 ].z;
			depth += xlist[ gt3_prim -> v2 ].z;
			depth += xlist[ gt3_prim -> v3 ].z;
			depth /= 3;

			if( depth > NEAREST_Z )
				{
				depth >>= 2;

				addPrim( current_ot + depth, gt3g );
				}

			}

		gt3_prim++;
		t = (Tri *)t -> link.next;

		}


}


/***************************************************************/
/*																					*/
/*	reflective bumpmap textured tri, pre setup,& draw 				*/
/*																					*/
/***************************************************************/
void draw_set_gt3rb( Object *obj, Xformed *xlist )
{
WORD			c;
Tri			*t, *tba, *tbs, *tbr;
GT3RB_prim	*gt3rb_prim;
POLY_GT3		*gt3, *gt3rba,*gt3rbs, *gt3rbr;
LONG			n,depth;	
Point			*p;
Gfx_vram		*gv;

	gv = &installed_gfx[ obj -> model_stack.set ];
	c = obj -> model_stack.gt3rb_count;
	t = obj -> model_stack.tri_gt3rb;
	tba = obj -> model_stack.tri_gt3rba;
	tbs = obj -> model_stack.tri_gt3rbs;
	tbr = obj -> model_stack.tri_gt3rbr;
	gt3rb_prim =  obj -> model_stack.gt3rb_prim;
	while( c-- )			
		{
		gte_ldsxy3( *(LONG *)&xlist[ gt3rb_prim -> v1 ].x,
					   *(LONG *)&xlist[ gt3rb_prim -> v2 ].x,
					   *(LONG *)&xlist[ gt3rb_prim -> v3 ].x );

		gte_nclip();
		gte_stopz(&n);

		gt3 = &t -> prim[ buffer ].gt3;
		SetPolyGT3( gt3 );
		SetSemiTrans( gt3,0 );
		gt3 -> tpage = gv -> tpage;
		gt3 -> clut = gv -> clut;

		gt3rba = &tba -> prim[ buffer ].gt3;
		SetPolyGT3( gt3rba );
		SetSemiTrans( gt3rba,1 );
		gt3rba -> tpage = gv -> bump_tpage_a;
		gt3rba -> clut = gv -> bump_clut;
	
		gt3rbs = &tbs -> prim[ buffer ].gt3;
		SetPolyGT3( gt3rbs );
		SetSemiTrans( gt3rbs,1 );
		gt3rbs -> tpage = gv -> bump_tpage_s;
		gt3rbs -> clut = gv -> bump_clut;

		gt3rbr = &tbr -> prim[ buffer ].gt3;
		SetPolyGT3( gt3rbr );
		SetSemiTrans( gt3rbr,1 );
		gt3rbr -> tpage = gv -> ref_tpage_a;
		gt3rbr -> clut = gv -> ref_clut;

		/******************************************/
		/*	uv's												*/
		/******************************************/
		p =  obj -> model_stack.point_list;

		*((WORD *)&gt3 -> u0) = *((WORD *)&gt3rb_prim -> uv0);
		*((WORD *)&gt3 -> u1) = *((WORD *)&gt3rb_prim -> uv1);
		*((WORD *)&gt3 -> u2) = *((WORD *)&gt3rb_prim -> uv2);
		
		*((WORD *)&gt3rba -> u0) = *((WORD *)&gt3rb_prim -> uv0);
		*((WORD *)&gt3rba -> u1) = *((WORD *)&gt3rb_prim -> uv1);
		*((WORD *)&gt3rba -> u2) = *((WORD *)&gt3rb_prim -> uv2);

		*((WORD *)&gt3rbs -> u0) = *((WORD *)&gt3rb_prim -> uv0);
		*((WORD *)&gt3rbs -> u1) = *((WORD *)&gt3rb_prim -> uv1);
		*((WORD *)&gt3rbs -> u2) = *((WORD *)&gt3rb_prim -> uv2);

		
		gt3 -> r0 =
		gt3 -> r1 =
		gt3 -> r2 =

		gt3 -> g0 =
		gt3 -> g1 =
		gt3 -> g2 =

		gt3 -> b0 =
		gt3 -> b1 =
		gt3 -> b2 = 128;

		//alv = 69;
		//slv = 69;

		gt3rba -> r0 =
		gt3rba -> r1 =
		gt3rba -> r2 =

		gt3rba -> g0 =
		gt3rba -> g1 =
		gt3rba -> g2 =

		gt3rba -> b0 =
		gt3rba -> b1 =
		gt3rba -> b2 = ALV;

		gt3rbs -> r0 =
		gt3rbs -> r1 =
		gt3rbs -> r2 =

		gt3rbs -> g0 =
		gt3rbs -> g1 =
		gt3rbs -> g2 =

		gt3rbs -> b0 =
		gt3rbs -> b1 =
		gt3rbs -> b2 = SLV;

		if( n > 0 )
			{

			gt3rbr -> x0 = gt3rbs -> x0 = gt3rba -> x0 = gt3 -> x0 = xlist[ gt3rb_prim -> v1 ].x;
			gt3rbr -> y0 = gt3rbs -> y0 = gt3rba -> y0 = gt3 -> y0 = xlist[ gt3rb_prim -> v1 ].y;
			gt3rbr -> x1 = gt3rbs -> x1 = gt3rba -> x1 = gt3 -> x1 = xlist[ gt3rb_prim -> v2 ].x;
			gt3rbr -> y1 = gt3rbs -> y1 = gt3rba -> y1 = gt3 -> y1 = xlist[ gt3rb_prim -> v2 ].y;
			gt3rbr -> x2 = gt3rbs -> x2 = gt3rba -> x2 = gt3 -> x2 = xlist[ gt3rb_prim -> v3 ].x;
			gt3rbr -> y2 = gt3rbs -> y2 = gt3rba -> y2 = gt3 -> y2 = xlist[ gt3rb_prim -> v3 ].y;

			depth = xlist[ gt3rb_prim -> v1 ].z;
			depth += xlist[ gt3rb_prim -> v2 ].z;
			depth += xlist[ gt3rb_prim -> v3 ].z;
			depth /= 3;

			if( depth > NEAREST_Z )
				{
				depth >>= 2;

				addPrim( current_ot + depth, gt3rbr );
				addPrim( current_ot + depth, gt3rba );
				addPrim( current_ot + depth, gt3rbs );
				addPrim( current_ot + depth, gt3 );
				}

			}

		gt3rb_prim++;
		t = (Tri *)t -> link.next;
		tba = (Tri *)tba -> link.next;
		tbs = (Tri *)tbs -> link.next;
		tbr = (Tri *)tbr -> link.next;

		}


}

#endif



/******************************************************/
/*	allocate object & prims & the like						*/
/******************************************************/
Object	*spawn_object( Object_def *def, WORD set )
{
Model_info	*mi;
Object		*obj;
WORD			stat,d;
//VECTOR		v_identity = { 16 * 65535,32 * 65535,0,0 };
VECTOR		v_identity = { 0,0,0,0 };

	stat = 0;
	obj = get_object();
	if( obj )
		{
	
		mi = *def -> details;
		if( mi )
			{
			obj -> model_stack.set = set;
			obj -> model_stack.model_info = mi;
			obj -> task = def -> task;

			/******************************************************/
			/*	init child list												*/
			/******************************************************/
			d = MAX_CHILDREN;
			while( d-- )
				{
				obj -> children[d] = NULL;
				}

			identity( &obj -> axis );

			/*********************************************************/
			/*	setup level of detail											*/
			/*********************************************************/
			obj -> model_stack.point_list = mi -> point_list;
			obj -> lod = def -> details;

//			obj -> distance_from_camera = vector_magnitude( &camera.pos, &obj -> axis.translate ) >> 3;
			
			obj -> last_lod = -1;
			obj -> z = -1;
			check_lod( obj );


			obj -> angular_velocity.vx = 
			obj -> angular_velocity.vy = 
			obj -> angular_velocity.vz = 0;

			obj -> velocity.vx =
			obj -> velocity.vy =
			obj -> velocity.vz = 0;

			obj -> rotvector.vx =
			obj -> rotvector.vy =
			obj -> rotvector.vz = 0;

			obj -> angular_velocity = v_identity;

			obj -> model_stack.g3_pinfo.count = mi -> g3_count;
			obj -> model_stack.g3_pinfo.setup = PRIM_SETUP|PRIM_ALLOC;

			obj -> model_stack.gt3b_pinfo.count = mi -> gt3b_count;
			obj -> model_stack.gt3b_pinfo.setup = PRIM_SETUP|PRIM_ALLOC;

			obj -> model_stack.vertex_count = mi -> vertex_count;

			obj -> control = OBJ_DISPLAY;
			obj -> control |= OBJ_ONSCREEN;

		}
		else
			{
			obj -> control = OBJ_FREE;
			obj = NULL;
			}

		}

	return obj;
}


/******************************************************************/
/*	link neccesary gt3 prims													*/
/******************************************************************/
WORD alloc_g3( Object *obj )
{
	
	/***************************************************************/
	/*	first, need to de-alloc what was there before					*/
	/***************************************************************/
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_g3 -> link,
					&obj -> model_stack.tri_last_g3 -> link );

	link_group( &chain_tri,
					(Link **)&obj -> model_stack.tri_g3,
					(Link **)&obj -> model_stack.tri_last_g3,
					obj -> model_stack.g3_pinfo.count );

	if( obj -> model_stack.tri_g3 )
		{
		/************************************************************/
		/*	alloc succesfull														*/
		/************************************************************/
		obj -> model_stack.g3_pinfo.count = obj -> model_stack.model_info -> g3_count;

		}


}

/******************************************************************/
/*	unlink neccesary gt3 prims													*/
/******************************************************************/
WORD dealloc_g3( Object *obj )
{
	
	/***************************************************************/
	/*	de-alloc what was there before										*/
	/***************************************************************/
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_g3 -> link,
					&obj -> model_stack.tri_last_g3 -> link );
	
	obj -> model_stack.tri_g3 =
	obj -> model_stack.tri_last_g3 = NULL;

}



/******************************************************************/
/*	link neccesary gt3 prims													*/
/******************************************************************/
WORD alloc_gt3b( Object *obj )
{
	
	/***************************************************************/
	/*	first, need to de-alloc what was there before					*/
	/***************************************************************/
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3b -> link,
					&obj -> model_stack.tri_last_gt3b -> link );

	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3ba -> link,
					&obj -> model_stack.tri_last_gt3ba -> link );

	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3bs -> link,
					&obj -> model_stack.tri_last_gt3bs -> link );
	

	link_group( &chain_tri,
					(Link **)&obj -> model_stack.tri_gt3b,
					(Link **)&obj -> model_stack.tri_last_gt3b,
					obj -> model_stack.model_info -> gt3b_count );
	link_group( &chain_tri,
					(Link **)&obj -> model_stack.tri_gt3ba,
					(Link **)&obj -> model_stack.tri_last_gt3ba,
					obj -> model_stack.model_info -> gt3b_count );
	link_group( &chain_tri,
					(Link **)&obj -> model_stack.tri_gt3bs,
					(Link **)&obj -> model_stack.tri_last_gt3bs,
					obj -> model_stack.model_info -> gt3b_count );


	if( obj -> model_stack.tri_gt3b )
		{
		/************************************************************/
		/*	alloc succesfull														*/
		/************************************************************/
	  //	obj -> model_stack.gt3b_count = obj -> model_stack.model_info -> gt3b_count;
		obj -> model_stack.gt3b_pinfo.count = obj -> model_stack.model_info -> gt3b_count;

		}


}

/******************************************************************/
/*	link neccesary gt3 prims													*/
/******************************************************************/
WORD dealloc_gt3b( Object *obj )
{

	/***************************************************************/
	/*	first, need to de-alloc what was there before					*/
	/***************************************************************/
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3b -> link,
					&obj -> model_stack.tri_last_gt3b -> link );

	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3ba -> link,
					&obj -> model_stack.tri_last_gt3ba -> link );

	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3bs -> link,
					&obj -> model_stack.tri_last_gt3bs -> link );
	
	
	obj -> model_stack.tri_gt3b =
	obj -> model_stack.tri_last_gt3b =
	obj -> model_stack.tri_gt3ba =
	obj -> model_stack.tri_last_gt3ba =
	obj -> model_stack.tri_gt3bs =
	obj -> model_stack.tri_last_gt3bs = NULL;


}


#if 0
/******************************************************************/
/*	link neccesary gt3 prims													*/
/******************************************************************/
WORD alloc_gt3( Object *obj )
{
	
	/***************************************************************/
	/*	first, need to de-alloc what was there before					*/
	/***************************************************************/
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3 -> link,
					&obj -> model_stack.tri_last_gt3 -> link );

	link_group( &chain_tri,
					(Link **)&obj -> model_stack.tri_gt3,
					(Link **)&obj -> model_stack.tri_last_gt3,
					obj -> model_stack.gt3_pinfo.count );

	if( obj -> model_stack.tri_gt3 )
		{
		/************************************************************/
		/*	alloc succesfull														*/
		/************************************************************/
		obj -> model_stack.gt3_pinfo.count = obj -> model_stack.model_info -> gt3_count;

		}


}

/******************************************************************/
/*	link neccesary gt3 prims													*/
/******************************************************************/
WORD dealloc_gt3( Object *obj )
{
	
	/***************************************************************/
	/*	de-alloc what was there before										*/
	/***************************************************************/
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3 -> link,
					&obj -> model_stack.tri_last_gt3 -> link );
	
	obj -> model_stack.tri_gt3 =
	obj -> model_stack.tri_last_gt3 = NULL;

//	obj -> gt3_count = 0;

}


/******************************************************************/
/*	link neccesary refmap gt3 prims											*/
/******************************************************************/
WORD alloc_gt3r( Object *obj )
{
	
	/***************************************************************/
	/*	first, need to de-alloc what was there before					*/
	/***************************************************************/
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3r -> link,
					&obj -> model_stack.tri_last_gt3r -> link );

	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3ra -> link,
					&obj -> model_stack.tri_last_gt3ra -> link );


	link_group( &chain_tri,
					(Link **)&obj -> model_stack.tri_gt3r,
					(Link **)&obj -> model_stack.tri_last_gt3r,
					obj -> model_stack.model_info -> gt3r_count );

	link_group( &chain_tri,
					(Link **)&obj -> model_stack.tri_gt3ra,
					(Link **)&obj -> model_stack.tri_last_gt3ra,
					obj -> model_stack.model_info -> gt3r_count );


	if( obj -> model_stack.tri_gt3r )
		{
		/************************************************************/
		/*	alloc succesfull														*/
		/************************************************************/
		obj -> model_stack.gt3r_count = obj -> model_stack.model_info -> gt3r_count;

		}


}

/******************************************************************/
/*	link neccesary gt3 prims													*/
/******************************************************************/
WORD dealloc_gt3r( Object *obj )
{

	/***************************************************************/
	/*	first, need to de-alloc what was there before					*/
	/***************************************************************/
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3r -> link,
					&obj -> model_stack.tri_last_gt3r -> link );

	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3ra -> link,
					&obj -> model_stack.tri_last_gt3ra -> link );

	
	obj -> model_stack.tri_gt3r =
	obj -> model_stack.tri_last_gt3r =
	obj -> model_stack.tri_gt3ra =
	obj -> model_stack.tri_last_gt3ra = NULL;

}



/******************************************************************/
/*	link neccesary chromemap gt3 prims										*/
/******************************************************************/
WORD alloc_gt3c( Object *obj )
{
	
	/***************************************************************/
	/*	first, need to de-alloc what was there before					*/
	/***************************************************************/
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3c -> link,
					&obj -> model_stack.tri_last_gt3c -> link );


	link_group( &chain_tri,
					(Link **)&obj -> model_stack.tri_gt3c,
					(Link **)&obj -> model_stack.tri_last_gt3c,
					obj -> model_stack.model_info -> gt3c_count );

	if( obj -> model_stack.tri_gt3c )
		{
		/************************************************************/
		/*	alloc succesfull														*/
		/************************************************************/
		obj -> model_stack.gt3c_count = obj -> model_stack.model_info -> gt3c_count;

		}


}

/******************************************************************/
/*	link neccesary gt3 prims													*/
/******************************************************************/
WORD dealloc_gt3c( Object *obj )
{

	/***************************************************************/
	/*	first, need to de-alloc what was there before					*/
	/***************************************************************/
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3c -> link,
					&obj -> model_stack.tri_last_gt3c -> link );

	obj -> model_stack.tri_gt3c=
	obj -> model_stack.tri_last_gt3c = NULL;
}


/******************************************************************/
/*	link neccesary chromemap gt3 prims										*/
/******************************************************************/
WORD alloc_gt3g( Object *obj )
{
	
	/***************************************************************/
	/*	first, need to de-alloc what was there before					*/
	/***************************************************************/
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3g -> link,
					&obj -> model_stack.tri_last_gt3g -> link );


	link_group( &chain_tri,
					(Link **)&obj -> model_stack.tri_gt3g,
					(Link **)&obj -> model_stack.tri_last_gt3g,
					obj -> model_stack.model_info -> gt3g_count );

	if( obj -> model_stack.tri_gt3g )
		{
		/************************************************************/
		/*	alloc succesfull														*/
		/************************************************************/
		obj -> model_stack.gt3g_count = obj -> model_stack.model_info -> gt3g_count;

		}


}

/******************************************************************/
/*	link neccesary gt3 prims													*/
/******************************************************************/
WORD dealloc_gt3g( Object *obj )
{

	/***************************************************************/
	/*	first, need to de-alloc what was there before					*/
	/***************************************************************/
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3g -> link,
					&obj -> model_stack.tri_last_gt3g -> link );

	obj -> model_stack.tri_gt3g =
	obj -> model_stack.tri_last_gt3g = NULL;
}




/******************************************************************/
/*	link neccesary gt3 reflective bumpy prims								*/
/******************************************************************/
WORD alloc_gt3rb( Object *obj )
{
	
	/***************************************************************/
	/*	first, need to de-alloc what was there before					*/
	/***************************************************************/
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3rb -> link,
					&obj -> model_stack.tri_last_gt3rb -> link );
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3rba -> link,
					&obj -> model_stack.tri_last_gt3rba -> link );
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3rbs -> link,
					&obj -> model_stack.tri_last_gt3rbs -> link );
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3rbr -> link,
					&obj -> model_stack.tri_last_gt3rbr -> link );


	link_group( &chain_tri,
					(Link **)&obj -> model_stack.tri_gt3rb,
					(Link **)&obj -> model_stack.tri_last_gt3rb,
					obj -> model_stack.model_info -> gt3rb_count );
	link_group( &chain_tri,
					(Link **)&obj -> model_stack.tri_gt3rba,
					(Link **)&obj -> model_stack.tri_last_gt3rba,
					obj -> model_stack.model_info -> gt3rb_count );
	link_group( &chain_tri,
					(Link **)&obj -> model_stack.tri_gt3rbs,
					(Link **)&obj -> model_stack.tri_last_gt3rbs,
					obj -> model_stack.model_info -> gt3rb_count );
	link_group( &chain_tri,
					(Link **)&obj -> model_stack.tri_gt3rbr,
					(Link **)&obj -> model_stack.tri_last_gt3rbr,
					obj -> model_stack.model_info -> gt3rb_count );


	if( obj -> model_stack.tri_gt3rb )
		{
		/************************************************************/
		/*	alloc succesfull														*/
		/************************************************************/
		obj -> model_stack.gt3rb_count = obj -> model_stack.model_info -> gt3rb_count;

		}


}

/******************************************************************/
/*	link neccesary gt3 prims													*/
/******************************************************************/
WORD dealloc_gt3rb( Object *obj )
{

	/***************************************************************/
	/*	first, need to de-alloc what was there before					*/
	/***************************************************************/
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3rb -> link,
					&obj -> model_stack.tri_last_gt3rb -> link );
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3rba -> link,
					&obj -> model_stack.tri_last_gt3rba -> link );
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3rbs -> link,
					&obj -> model_stack.tri_last_gt3rbs -> link );
	unlink_group( &chain_tri, 
					&obj -> model_stack.tri_gt3rbr -> link,
					&obj -> model_stack.tri_last_gt3rbr -> link );

	
	obj -> model_stack.tri_gt3rb =
	obj -> model_stack.tri_last_gt3rb =
	obj -> model_stack.tri_gt3rbr =
	obj -> model_stack.tri_last_gt3rbr =
	obj -> model_stack.tri_gt3rba =
	obj -> model_stack.tri_last_gt3rba =
	obj -> model_stack.tri_gt3rbs =
	obj -> model_stack.tri_last_gt3rbs = NULL;


}


#endif




void dealloc_object( Object *obj )
{

	obj -> control |= OBJ_FREE;

}


void install_vram_image( void )
{

	install_gfx( &europe_gfx, 
					 &europe_bump,
					 0 );

	install_gfx( &yank_gfx, 
					 &yank_bump,
					 1 );

}


void install_gfx( Gfx_header *gfx, Gfx_header *bump, WORD no )
{
Gfx_vram	*i;


	i = &installed_gfx[ no ];
	i -> tpage = bung_image( gfx, &gfx_rect );
	gfx_rect.x += 64;

	i -> bump_tpage = bung_image( bump, &gfx_rect );
	i -> bump_tpage_a = GetTPage( 0,1,gfx_rect.x,gfx_rect.y );
	i -> bump_tpage_s = GetTPage( 0,2,gfx_rect.x,gfx_rect.y );
	gfx_rect.x += 64;
	
	
	i -> clut = bung_clut( gfx, &clut_rect );
	clut_rect.y += 1;
	
	i -> bump_clut = bung_clut( bump, &clut_rect );
	clut_rect.y += 1;


}



WORD bung_image( Gfx_header *g, RECT *rect )
{
Image_data	*i;
WORD	tp;

	i = &g -> image;

	if( i -> type == 8 )
		{
		/***************************************************************/
		/*	8bit image																	*/
		/***************************************************************/
		rect ->w = i -> width>>1;
		rect ->h = i -> height;
		LoadImage( rect, (ULONG *)&i -> clut[256] );
		tp = GetTPage( 1,0,rect ->x,rect ->y );

		}

	else
	if( i -> type == 4 )
		{
		/***************************************************************/
		/*	4bit image																	*/
		/***************************************************************/
		rect ->w = i -> width>>2;
		rect ->h = i -> height;
		LoadImage( rect, (ULONG *)&i -> clut[16] );
		tp = GetTPage( 0,0,rect ->x,rect ->y );

		}


	return tp;
}

WORD bung_clut( Gfx_header *g, RECT *rect )
{
Image_data	*i;
WORD	id;

	i = &g -> image;

	if( i -> type == 8 )
		{
		/***************************************************************/
		/*	8bit clut																	*/
		/***************************************************************/
		rect ->w = 256;
		rect ->h = 1;
		LoadImage( rect ,(ULONG *)&i -> clut[0] );
		id = GetClut( rect ->x, rect ->y );
		}
	else
	if( i -> type == 4 )
		{
		/***************************************************************/
		/*	4bit clut																	*/
		/***************************************************************/
		rect ->w = 16;
		rect ->h = 1;
		LoadImage( rect ,(ULONG *)&i -> clut[0] );
		id = GetClut( rect ->x, rect ->y );
		}

	return id;

}

WORD	obx,oby,obz;
WORD	obrx,obry,obrz;


void boxtask( Object *obj )
{
	
	obz = obj -> z;


}

void create_m3_axis( MATRIX *mx, MATRIX *my, MATRIX *mz, MATRIX *dest )
{

	MulMatrix0( mx, my, dest ); 
	MulMatrix0( dest,mz, dest ); 

}








/************************************************************************/
/*																							  	*/
/*	Module: object handling					 											*/
/*																							  	*/
/*																							  	*/
/************************************************************************/

/************************************************************************/
/*																							  	*/
/*	include default headers															  	*/
/*																							  	*/
/************************************************************************/

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


extern ULONG	zipstack;
/************************************************************************/
/*																							  	*/
/*	defs																					  	*/
/*																							  	*/
/************************************************************************/

/************************************************************************/
/*																							  	*/
/*	vars																					  	*/
/*																							  	*/
/************************************************************************/
Chain	chain_object;
LONG	object_count;

/************************************************************************/
/*																							  	*/
/*	constant data																		  	*/
/*																							  	*/
/************************************************************************/
MATRIX lgtmat =
{
	0,  0,  -ONE,			// light angle #1
	0,  0,  0,			// light angle #2
	0,  0,  0			// light angle #3

};

#if 0

WORD	z_swap[] =
{

	30000,
	450,
	600,
	750,
	900,
	1050,
	1200,
	1350
};
#endif



/************************************************************************/
/*	object chain & mem init																*/
/************************************************************************/
void initialise_objects( void )
{

	initialise_chain( sizeof( Object ), MAX_OBJECTS, &chain_object );



}


/************************************************************************/
/*	get a free object																		*/
/************************************************************************/
Object *get_object( void )
{
Object	*obj;

	obj = (Object *)link( &chain_object );

	return obj;

}


/************************************************************************/
/*	get a free object																		*/
/************************************************************************/
void free_object( Object *obj )
{
Model_stack	*ms;

	ms = &obj -> model_stack;
	
	unlink( obj, &chain_object );
//	unlink_group( &chain_quad,(Link *)ms -> quad_g4,  (Link *)ms -> quad_last_g4 );
//	unlink_group( &chain_quad,(Link *)ms -> quad_gt4, (Link *)ms -> quad_last_gt4 );
//	unlink_group( &chain_tri, (Link *)ms -> tri_g3,  (Link *)ms -> tri_last_g3 );
//	unlink_group( &chain_tri, (Link *)ms -> tri_gt3, (Link *)ms -> tri_last_gt3 );

}

/************************************************************************/
/*	process & draw objects																*/
/************************************************************************/
void process_objects( void )
{
Object	*obj, *next_object;


	/*********************************************************************/
	/*	first process																		*/
	/*********************************************************************/
	obj = (Object *)chain_object.active_chain;
	while( obj != NULL )
		{
		next_object = (Object *)obj -> link.next_active;

		if( !(obj -> control & OBJ_NOLOD) )
			{
			if( obj -> lod )
				{
				/***************************************************************/
				/*	need to do level of detail 											*/
				/***************************************************************/
				check_lod( obj );
				}
			}

		if( !(obj -> control & OBJ_STANDBY) )
			{
			if( obj -> task )
				{
				(*obj -> task)( obj );
				}
			}

		obj = next_object;

		}


	/*********************************************************************/
	/*	now draw em																			*/
	/*********************************************************************/
	obj = (Object *)chain_object.active_chain;
	while( obj != NULL )
		{
		next_object = (Object *)obj -> link.next_active;

		if( !( obj -> control & OBJ_FREE ) )
			{

			if( !(obj -> control & OBJ_CHILD) )
				{
				if( obj -> control & OBJ_DISPLAY )
					{

					draw_obj( obj, &real, &lgtmat );

					}
				}

			}
		else
			{
			free_object( obj );

			}

		obj = next_object;

		}



//	FntPrint("fast stash %x\n",zipstack);


}



/***************************************************************/
/*																					*/
/*	check & alter object detail level									*/
/*																					*/
/***************************************************************/
void check_lod( Object *obj )
{
Model_info		*mi;
Model_info		**lod;
WORD				done, detail_on;

	detail_on = 0;
	lod = obj -> lod;
	done = FALSE;

	while( done == FALSE )
		{
//		if( obj -> distance_from_camera < z_swap[ detail_on ] )
			{
			/*********************************************************/
			/*	nailed the sucker													*/
			/*********************************************************/
			done = TRUE;
			obj -> current_lod = detail_on;
			if( obj -> current_lod != obj -> last_lod )
				{
				/******************************************************/
				/*	need to re-initialise to different model				*/
				/******************************************************/
				obj -> last_lod = obj -> current_lod;

				/******************************************************/
				/*	get new detail model											*/
				/******************************************************/
				mi = *lod;

				//obj -> point_list = mi -> point_list;
				obj -> model_stack.g4_prim = mi -> g4_prim;
				obj -> model_stack.g3_pinfo.mprim.g3_prim =  mi -> g3_prim;
				obj -> model_stack.gt4_prim = mi -> gt4_prim;
				obj -> model_stack.gt3_prim = mi -> gt3_prim;
				obj -> model_stack.gt3b_pinfo.mprim.gt3b_prim = mi -> gt3b_prim;
				obj -> model_stack.gt3r_prim = mi -> gt3r_prim;
				obj -> model_stack.gt3c_prim = mi -> gt3c_prim;
				obj -> model_stack.gt3g_prim = mi -> gt3g_prim;
				obj -> model_stack.gt3rb_prim = mi -> gt3rb_prim;


				obj -> model_stack.vertex_count = mi -> vertex_count;
				done = TRUE;

				}

			}

		lod++;
		detail_on++;

		if( !(*lod) )
			{
			/******************************************************/
			/*	object has no more levels of detail						*/
			/******************************************************/
			done = TRUE;

			}

		}

}




void move_camera( void )
{

	point_camera_by_vector();

}


void point_camera_by_vector(void)
{
	VECTOR v1;
	MATRIX m1 = M_IDENTITY, m2 = M_IDENTITY;

	m1.m[0][0] = camera.i.vx;
	m1.m[0][1] = camera.i.vy;
	m1.m[0][2] = camera.i.vz;

	m1.m[1][0] = camera.j.vx;
	m1.m[1][1] = camera.j.vy;
	m1.m[1][2] = camera.j.vz;

	m1.m[2][0] = camera.k.vx;
	m1.m[2][1] = camera.k.vy;
	m1.m[2][2] = camera.k.vz;


	#if 0

	FntPrint("%d,%d,%d\n%d %d %d \n%d %d %d\n",
		camera.i.vx,
		camera.i.vy,
		camera.i.vz,

		camera.j.vx,
		camera.j.vy,
		camera.j.vz,

		camera.k.vx,
		camera.k.vy,
		camera.k.vz );

	#endif


	v1.vx = -camera.pos.vx;
	v1.vy = -camera.pos.vy;
	v1.vz = -camera.pos.vz;

	TransMatrix(&m2, &v1);

	CompMatrix(&m1, &m2, &real);



	#if 1
	real.m[0][0] = FixAspectRatiox(real.m[0][0]);
	real.m[0][1] = FixAspectRatiox(real.m[0][1]);
	real.m[0][2] = FixAspectRatiox(real.m[0][2]);
	real.t[0]	  = FixAspectRatiox(real.t[0]);


	#endif


	SetRotMatrix(&real);
	SetTransMatrix(&real);


}







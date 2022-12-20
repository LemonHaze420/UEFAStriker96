


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


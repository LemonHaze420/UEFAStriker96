#define MAX_LASERS 8		// holographic lasers

#define NO_OVERRIDE		0
#define COLOR_OVERRIDE	1
#define TINT_OVERRIDE	2


/******************************************
 *														*
 * Scene Object Descriptor block structs	*
 *														*
 ******************************************/

typedef struct
{
	WORD	sdt_filenum;			// S01 data file for this scene
	WORD	tex_filenum;			// TEX ( gfx ) file for this scene
	WORD	mdt_filenum;			// M01 data file for this scene
	WORD	ldt_filenum;			// L01 data file for this scene
	SVECTOR envrot_vec;			// environment map rotated by this vector
	WORD	*model_list;			// addr of list of objects to include in this scene
}Scene_startup;

typedef struct
{
	WORD	polys_per_object;	// max num of laser beams to draw per object
	WORD	laser_name;			// laser object name from ( .DEF file )
	BYTE	r,g,b,transp_rate;// laser color, transparency rate -1, 0, 1, 2
	WORD	*object_list;		// list of objects to draw with this laser...
	WORD	active;				// yes or no
	WORD  max_laser_beams;	// maximum no. of lasers beams for this laser
	SVECTOR pos;				// laser beam base pos
}Laser_data;


typedef struct
{
	MATRIX  anim_matrix;			// matrix output by anim.c
	MATRIX  user_matrix_a;		// additional matrix ( multiplied with anim_matrix )
	MATRIX  user_matrix_b;		// additional matrix ( multiplied with user_matrix_a )
	MATRIX  s01_matrix;			// matrix for this object from S01 file
	SVECTOR rot;					// additional position,rotation of scene object
	BYTE	  name[32];	
	WORD anim_seq;					// anim list to play on this SOD 0,1 or 2

	M01_anim  *mdt_indexlist1;	// addr of indexlist ( index to rots and trans ) for this object (0=no anim)
	M01_rot   *mdt_rot1;		 	// addr of rotlist for this obj
	M01_trans *mdt_trans1;	 	// addr of translist for this obj
	WORD mdt_blocks1;				// num of animation blocks
	WORD current_anim_frame1;
	WORD last_anim_frame1;
	WORD anim_initialised1;		// yes= this object's animlist has already been initialised
	WORD anim_timer1;
	WORD matrix_control1;		// see bit defs in anim.h

	M01_anim  *mdt_indexlist2;	// addr of indexlist ( index to rots and trans ) for this object (0=no anim)
	M01_rot   *mdt_rot2;		 	// addr of rotlist for this obj
	M01_trans *mdt_trans2;	 	// addr of translist for this obj
	WORD mdt_blocks2;				// num of animation blocks
	WORD current_anim_frame2;
	WORD last_anim_frame2;
	WORD anim_initialised2;		// yes= this object's animlist has already been initialised
	WORD anim_timer2;
	WORD matrix_control2;		// see bit defs in anim.h

	WORD active;					// user can enable disable this object...
	WORD color_override;			//	YES = use colors from sod , not from face->c0 c1 c2 etc
	CVECTOR tint_c0;
	WORD obj_num;
	WORD exclude_from_scene;	// YES = this object wont be loaded to memory
	WORD user_depth;				// depth added to otz
}SOD;

typedef struct
{
	WORD num_objects;
	SOD *sod_list;								// pointer to list of sods
	SVECTOR pos,rot;							// additional position/rotation of Scene
	SVECTOR envrot_vec;						// additional rotation to environment matrix
	ULONG	anim_frame_counter;				// current animation frame no.
	ULONG total_frames;						// total number of frames for animation
	Laser_data laser_data[ MAX_LASERS ];// laser info 
	WORD lasers_active;						// number of lasers active
}SOD_header; 










#if 0
typedef struct
{
	SVECTOR rot;					// additional position,rotation of scene object

	MATRIX	 *matrix_list;		// addr of matrices

	WORD anim_frames;				// num animations per object
	WORD anim_matrices;			// num matrices per frame 
	
	WORD mdt_blocks1;				// num of frames
	WORD current_anim_frame;
	WORD last_anim_frame;
	WORD anim_initialised;		// yes= this object's animlist has already been initialised
	WORD anim_timer;

	WORD active;					// user can enable disable this object...
	WORD depth;						// depth added to otz

	struct Skin_obj *skin_obj;			// skinned object to draw....
}Skin_sod;
#endif

#define MAX_SKINFILE_SIZE 180000

typedef struct
{
	Skin_svector v0,v1,v2; 	//vertices
	CVECTOR c0,c1,c2;			//vertex colors
}Skin_g3_face;

typedef struct
{
	Skin_svector v0,v1,v2,v3; //vertices
	CVECTOR c0,c1,c2,c3;	//vertex colors
}Skin_g4_face;

typedef struct
{
	Skin_svector v0,v1,v2; //vertices
	CVECTOR c0,c1,c2;	//vertex colors
	UV_pos  t0,t1,t2;
	UWORD tpage;
}Skin_gt3_face;

typedef struct
{
	Skin_svector v0,v1,v2,v3; //vertices
	CVECTOR c0,c1,c2,c3;	//vertex colors
	UV_pos  t0,t1,t2,t3;
	UWORD tpage,pad;
}Skin_gt4_face;

typedef struct
{
	Skin_g3_face  *g3_face;
	Skin_g4_face  *g4_face;
	Skin_gt3_face *gt3_face;
	Skin_gt4_face *gt4_face;	
	POLY_GT3 *g3_ptr[2];			// allocated Polys for this scene
	POLY_GT4 *g4_ptr[2];
	POLY_GT3 *gt3_ptr[2];
	POLY_GT4 *gt4_ptr[2];

	WORD g3_cnt,g4_cnt,gt3_cnt,gt4_cnt;

	WORD real_lit;					// Object affected by real lights Yes ( uses rl_ functions ) else pl_ functions
	MATRIX user_matrix;			// object matrix

	SVECTOR rot,rot2;				// additional position,rotation of scene object
	VECTOR pos,pos2;
	MATRIX *matrix_list;			// addr of matrices
	WORD anim_frames;				// num animations per object
	WORD anim_matrices;			// num matrices per frame 
	WORD current_anim_frame;
	WORD last_anim_frame;
	WORD anim_initialised;		// yes= this object's animlist has already been initialised
	WORD anim_timer;
	WORD active;					// user can enable disable this object...
	WORD depth;						// depth added to otz
}Skin_obj;






void load_skin_file( WORD file_num, Skin_obj *obj );
void setup_skinned_object( Skin_header_src *header_src, Skin_obj *obj );
void pl_draw_skin_g3s( Skin_obj *obj );
void pl_draw_skin_g4s( Skin_obj *obj );
void pl_draw_skin_gt3s( Skin_obj *obj );
void pl_draw_skin_gt4s( Skin_obj *obj );



void draw_skin_objs(Skin_obj *obj );

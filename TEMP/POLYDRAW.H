






typedef struct
{
	SVECTOR v0,v1,v2; //vertices
	SVECTOR n0,n1,n2;	//vertex normals
	CVECTOR c0,c1,c2;	//vertex colors
}G3_face;

typedef struct
{
	SVECTOR v0,v1,v2,v3; //vertices
	SVECTOR n0,n1,n2,n3;	//vertex normals
	CVECTOR c0,c1,c2,c3;	//vertex colors
}G4_face;

typedef struct
{
	SVECTOR v0,v1,v2; //vertices
	SVECTOR n0,n1,n2;	//vertex normals
	CVECTOR c0,c1,c2;	//vertex colors
	UV_pos  t0,t1,t2;
//	UV_pos  t0_mip1,t1_mip1,t2_mip1; 	 //texture pos
//	UV_pos  t0_mip2,t1_mip2,t2_mip2; 	 //texture pos
	UWORD tpage;
//	UWORD tpage_mip1;
//	UWORD tpage_mip2;
}GT3_face;

typedef struct
{
	SVECTOR v0,v1,v2,v3; //vertices
	SVECTOR n0,n1,n2,n3;	//vertex normals
	CVECTOR c0,c1,c2,c3;	//vertex colors
	UV_pos  t0,t1,t2,t3;
//	UV_pos  t0_mip1,t1_mip1,t2_mip1,t3_mip1; 	 //texture pos
//	UV_pos  t0_mip2,t1_mip2,t2_mip2,t3_mip2; 	 //texture pos
	UWORD tpage,pad;
//	UWORD tpage_mip1;
//	UWORD tpage_mip2;
}GT4_face;










typedef struct
{
	SOD		*sod;				// addr of SOD associated with this object

	G3_face  *g3_face;
	G4_face  *g4_face;
	GT3_face *gt3_face;
	GT4_face *gt4_face;	
	POLY_GT3 *g3_ptr[2];			// allocated Polys for this scene
	POLY_GT4 *g4_ptr[2];
	POLY_GT3 *gt3_ptr[2];
	POLY_GT4 *gt4_ptr[2];

	POLY_GT3 *egt3_ptr[2];			// environment mapped polys for GT3 polys
	POLY_GT4 *egt4_ptr[2];			// environment mapped polys for GT4 polys

	POLY_GT3 *eg3_ptr[2];			// environment mapped polys for G3 polys
	POLY_GT4 *eg4_ptr[2];			// environment mapped polys for G4 polys

	WORD g3_cnt,g4_cnt,gt3_cnt,gt4_cnt;

//mipmap stuff
	SVECTOR centre_vertex;	// centre point of this object
	WORD mip_mapping;			// active? Yes or No
	WORD mip_level;			// level of mipmapping 0,1 or 2
	WORD do_mipmap_swap;		// if !=0, set uv,tp to mip_level
	WORD pad;

	WORD real_lit;					// Object affected by real lights Yes ( uses rl_ functions ) else pl_ functions
	WORD environment_mapping; 	// Yes or No

	void (*upd_rtn)(void *scene);

	MATRIX matrix;		// copy of gte's matrix for polydraw

	WORD current_laser_face;
}Scene_obj;



typedef struct
{
	Scene_obj	*scene_obj_list;
}Scene_header;	






typedef union
{
	LINE_F2		line_f2;
	LINE_F3		line_f3;
	LINE_G2		line_g2;
	LINE_G3		line_g3;
	POLY_G3		poly_g3;
	POLY_G4		poly_g4;
	POLY_GT3		poly_gt3;
	POLY_GT4		poly_gt4;
	SPRT			sprt;
	SPRT_16		sprt16;
	SPRT_8		sprt8;
	TILE			tile;
	TILE_1		tile_1;
	TILE_8		tile_8;
	TILE_16		tile_16;
	DR_AREA		dr_area;
}Prim;






void setup_scene_gt4s( Scene_obj *scene_obj, Scene_obj_src *scene_obj_src );
void setup_scene_g4s ( Scene_obj *scene_obj, Scene_obj_src *scene_obj_src );
void setup_scene_gt3s( Scene_obj *scene_obj, Scene_obj_src *scene_obj_src );
void setup_scene_g3s ( Scene_obj *scene_obj, Scene_obj_src *scene_obj_src );

void rl_draw_scene_g3s ( Scene_obj *scene_obj, SVECTOR *lv, WORD light0_type );
void rl_draw_scene_g4s ( Scene_obj *scene_obj, SVECTOR *lv, WORD light0_type );
void rl_draw_scene_gt3s( Scene_obj *scene_obj, SVECTOR *lv, WORD light0_type );
void rl_draw_scene_gt4s( Scene_obj *scene_obj, SVECTOR *lv, WORD light0_type );
void pl_draw_scene_g3s ( Scene_obj *scene_obj );
void pl_draw_scene_g4s ( Scene_obj *scene_obj );
void pl_draw_scene_gt3s( Scene_obj *scene_obj );
void pl_draw_scene_gt4s( Scene_obj *scene_obj );

void rl_phong_draw_scene_g3s( Scene_obj *scene_obj, MATRIX *local_mat );
void rl_phong_draw_scene_g4s( Scene_obj *scene_obj, MATRIX *local_mat );
void rl_phong_draw_scene_gt3s( Scene_obj *scene_obj, MATRIX *local_mat );
void rl_phong_draw_scene_gt4s( Scene_obj *scene_obj, MATRIX *local_mat );


void rottrans_centre_vertex( Scene_obj *obj );

extern Scene_header *scene_header;
extern Scene_header_src *scene_header_src;





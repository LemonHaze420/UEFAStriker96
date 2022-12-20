extern	Object	*spawn_object( Object_def *def, WORD set );
extern	void create_m3_axis( MATRIX *mx, MATRIX *my, MATRIX *mz, MATRIX *dest );
extern	void create_obj_axis_yz( Object *obj, MATRIX *dest );
extern	void create_obj_axis( Object *obj, MATRIX *dest );
extern	void draw_obj( Object *obj, MATRIX *real, MATRIX *light_mat );


extern	void alloc_prim( void );
extern	void draw_objects( void );

extern	Omnilight olight, olight2;
extern	Chain	chain_tri,chain_quad;
extern	Object_def	bdef, bdef2;


extern	WORD	obx,oby,obz;
extern	WORD	obrx,obry,obrz;

extern	WORD	max_light;
extern	Omnilight	ol[];

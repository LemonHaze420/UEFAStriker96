


#define TARGET_CAM 0
#define NORMAL_CAM 1


typedef struct
{
	SVECTOR	xa,ya,za;

	SVECTOR	angle;
	VECTOR	pos;
	SVECTOR	dest;
	WORD 		type,mdt_blocks;		// num of animation blocks
	BYTE		name[CAMERA_NAME_LEN];
	MATRIX 	scene_matrix;

	M01_anim  *mdt_indexlist;		// addr of indexlist ( index to rots and trans ) for this object (0=no anim)
	M01_rot   *mdt_rot;				// addr of rotlist for this obj
	M01_trans *mdt_trans;			// addr of translist for this obj

	WORD current_anim_frame,last_anim_frame;
	WORD anim_timer,animation_active;	// yes= this cam has matrix animation
	WORD anim_suspended,pad;	// yes or no
	MATRIX anim_mat;

}Camera_data;

typedef struct
{
	MATRIX output_mat;
	MATRIX matrix;	//scene_matrix * rotation placed here
	WORD current;

	Camera_data data[MAX_CAMERAS];
	WORD aspect_correction,x_aspect;		//aspect ratio correction
	WORD num_cameras;
}Camera;

void reset_cameras();
void set_camera_matrix( WORD camera_num );
void fix_aspect_ratio(MATRIX *mat);
void move_matrix_camera( void );

extern Camera camera;
extern MATRIX	light_matrix;
extern MATRIX	color_matrix;
extern WORD 	aspect_correction;








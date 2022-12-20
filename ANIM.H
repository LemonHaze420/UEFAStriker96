
#define MATRIX_ANIM_ACTIVE			1	// matrix animation active
#define MATRIX_A						2	// output to sod->user_matrix_a
#define MATRIX_B						4	// output to sod->user_matrix_b
#define MATRIX_ANIM					8	// output to sod->anim_matrix
#define MATRIX_ANIM_SINGLE_SHOT	16	// 0 = repeat
#define MATRIX_ANIM_DEACTIVATE	32	//	0 = stays active when animation done



void load_m01_file ( WORD filenum, WORD slot );
void load_l01_file ( WORD filenum, WORD slot );
void start_scene_anim ( WORD anim_seq, WORD slot, WORD matrix_control ); 
void stop_scene_anim ( WORD slot );
void start_camera_anim ( WORD slot );
void start_light_anim ( WORD slot );
void do_camera_animation();
void do_light_animation();
void reset_animation();
void do_object_animation1 ( SOD *sod );
void do_object_animation2 ( SOD *sod );

void play_obj_m01 ( WORD object_num, WORD block_num, WORD mdt_slot_num, WORD matrix_type );
void start_obj_m01_anim1 ( SOD *sod, WORD mdt_slot_num, WORD matrix_control );
void update_obj_m01_anim ( SOD *sod );

extern WORD all_animation_paused;

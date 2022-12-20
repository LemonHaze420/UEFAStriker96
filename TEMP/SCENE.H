#define GORDON_PRESENT NO

#define MAX_SCENE_TEXTURE_SIZE	612000
#define MAX_SCENE_SDT_SIZE			510000

//#define MATERIAL_NAME_LEN 16

#define DUMMY_TEXTURE_X   (512)
#define DUMMY_TEXTURE_Y   (511)
#define DUMMY_TEXTURE_BPP (2)	// always 16-bit




void load_scene_data( WORD scene_file_num , WORD texture_file_num );
void init_scene_draw( Scene_startup *ss );
void set_scene_ambience ( BYTE r, BYTE g, BYTE b );
void draw_scene();
void draw_scene_x();
void start_scene( Scene_startup *scene_startup );
void store_sod_mat( WORD obj_num );

extern SOD_header *sod_header;
void load_scene_sdt( Scene_startup *scene_startup ) ;



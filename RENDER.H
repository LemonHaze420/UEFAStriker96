
// ted slots


#if 1
enum
{
	TED_PLAYER_FACE,
	TED_PLAYER_ARMS,
	TED_PLAYER_LEGS,
	TED_SHIRT_BACK,
	TED_SHIRT_FRONT,
	TED_SHORT,
	TED_SOCK,
	TED_BOOT,
	TED_SLEEVE,

//	TED_TVK_SHIA,
//	TED_TVK_SHIB,
//	TED_TVK_SHOA,
//	TED_TVK_SHOB,
//	TED_TVK_SLEA,
//	TED_TVK_SLEB,

//	TED_TVBACK1,
//	TED_TVBACK2,

//	TED_FLAG1 ,TED_FLAG2 ,TED_FLAG3 ,TED_FLAG4 ,TED_FLAG5 ,TED_FLAG6 ,TED_FLAG7 ,TED_FLAG8 ,TED_FLAG9 ,TED_FLAG10,
//	TED_FLAG11,TED_FLAG12,TED_FLAG13,TED_FLAG14,TED_FLAG15,TED_FLAG16,TED_FLAG17,TED_FLAG18,TED_FLAG19,TED_FLAG20,
//	TED_FLAG21,TED_FLAG22,TED_FLAG23,TED_FLAG24,TED_FLAG25,TED_FLAG26,TED_FLAG27,TED_FLAG28,TED_FLAG29,TED_FLAG30,
//	TED_FLAG31,TED_FLAG32,TED_FLAG33,TED_FLAG34,TED_FLAG35,TED_FLAG36,TED_FLAG37,TED_FLAG38,TED_FLAG39,TED_FLAG40,
//	TED_FLAG41,TED_FLAG42,TED_FLAG43,TED_FLAG44,TED_FLAG45,TED_FLAG46,TED_FLAG47,TED_FLAG48,TED_FLAG49,TED_FLAG50,
//	TED_FLAG51,TED_FLAG52,TED_FLAG53,TED_FLAG54,TED_FLAG55,TED_FLAG56,TED_FLAG57,TED_FLAG58,TED_FLAG59,TED_FLAG60,
//	TED_FLAG61,TED_FLAG62,TED_FLAG63,TED_FLAG64,TED_FLAG65,TED_FLAG66,TED_FLAG67,TED_FLAG68,TED_FLAG69,TED_FLAG70,
//	TED_FLAG71,TED_FLAG72,TED_FLAG73,TED_FLAG74,TED_FLAG75,TED_FLAG76,TED_FLAG77,TED_FLAG78,TED_FLAG79,TED_FLAG80,


};
#endif



typedef struct
{
	WORD x,y,w,h,bpp;
	WORD clut_x, clut_y;
}Ted_block;

extern Ted_block ted_block[];



void setup_ted_list();

//extern void draw_plasma_to_model_object ( Plasma *plasma );

void render_scene();
void init_all_lasers();
//void add_laser( Laser_data *laser_data, WORD laser_num );
void draw_lasers();

// enviroment map vars
extern WORD scene_envmap_u0, scene_envmap_v0, scene_envmap_vshift;

extern void load_ted_info();

extern void setup_scene_objects();

//void read_ted_data( BYTE *ted_addr, BYTE *fname, WORD slot );
void download_texture( BYTE *addr, WORD ted_slot );
//void overlay_textures( UBYTE *src1,WORD mod1, UBYTE *src2,WORD mod2, UBYTE *src3,WORD mod3, WORD ted_slot );

void overlay_textures( UBYTE *src1, UBYTE *src2, UBYTE *src3, WORD ted_slot );

void download_clut( BYTE *addr, WORD ted_slot, WORD src_offset, WORD dest_offset, WORD len );




void overlay_texture2( UBYTE *src1, WORD x1, WORD y1, WORD w1, WORD h1, UBYTE *src2, WORD x2, WORD y2, WORD w2, WORD h2, UBYTE *src3, WORD x3, WORD y3, WORD w3, WORD h3, WORD dest_x, WORD dest_y );
void download_texture2( BYTE *src_addr, WORD src_wd, WORD src_ht, WORD dest_x, WORD dest_y );

void move_texture( WORD ted_slot1, WORD ted_slot2 );
void move_clut( WORD ted_slot1, WORD ted_slot2 );
WORD get_ted_pal_color( WORD ted_slot );
void load_scene_materials( WORD filenum, WORD clear );


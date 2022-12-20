							
#define OT_SIZE	(4096+100)	// ordering table size
#define OT_SHIFT 2			// otsize << OT_SHIFT should = 16384
#define	OVERLAY_DEPTH		0	/* very closest prim to screen */
#define	NEAREST_DEPTH		2
#define  MIDDLE_DEPTH		(OT_SIZE/2)
#define	SPRITE_DEPTH  		(MIDDLE_DEPTH+10)
#define	PLASMA_DEPTH		OT_SIZE-3
#define	FURTHEST_DEPTH		OT_SIZE-2
#define  ENVPOLY_DEPTH		OT_SIZE-1 // depth for ft3s to set env tpage

#define SCREEN_Z 256		// distance to view plane

#define SCREEN_X		0

#if NTSC == YES
	#define	FRAME_HT		256
	#define 	SCREEN_Y		0
	#define	FPS			60
#else
	#define  FRAME_HT    256
	#define 	SCREEN_Y		0 //45
	#define	FPS			50
#endif

#define DEFAULT_XRES	512	// screen x/y res
#define DEFAULT_YRES	256	// default values...

/** see spritehw.c 'draw_sprite_prim' if xres,yres changes... ***/
#define DEFAULT_XRES_SHIFT 9	// 2^xshift should = default xres
#define DEFAULT_YRES_SHIFT 8  // 2^yshift should = default yres

#define SCREEN_UNIT_WD 1024	// width of screen in buffalos / tramps / etc
#define SCREEN_UNIT_HT 1024

typedef struct
{		
DRAWENV		draw;			/* drawing environment */
DISPENV		disp;			/* display environment */
ULONG			*ot;
WORD			buffnum;	//buffer number
}DB;	//dislay buffer






//***** Functions *****
void clear_vram( UWORD x, UWORD y, UWORD w,UWORD h ,WORD r,WORD g,WORD b);
void wait_vblank();
void swap_display_buffers();
void setup_system_font();
void clear_ordering_table();
void update_display();
void allocate_drawbuffers();
void display_system_font();
void switch_debug_info();
void print_error ( WORD error_num, BYTE *text_string, WORD num );
void init_display( WORD wd,WORD ht, BYTE bgr, BYTE bgg, BYTE bgb, WORD clear1, WORD clear2, WORD mode24 );
void enable_display( WORD time );
void disable_display();




extern DB *db[];
extern DB *cdb;
extern WORD frame_update;
extern WORD print_debug_info;
extern WORD screen_z;
extern WORD x_aspect;
extern WORD enable_display_timer;

void adjust_hw_screen_pos();
void swap_screens();
void set_defdispdraw();
extern WORD animating_man_active;


extern void show_loadscn( WORD load_it );
extern void stop_animating_man();
extern void start_animating_man();

extern WORD screen_xres, screen_yres;



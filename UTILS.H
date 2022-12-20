//***** Vars *****
extern WORD gpu_speed,gpu_bar_stop,gpu_bar_start;
extern WORD cpu_speed,cpu_bar_stop,cpu_bar_start;
extern volatile WORD draw_counter;
extern DB *cdb;			/* current draw buffer */
//extern WORD frame_counter;
extern WORD step_cnt;
extern WORD item_num,item_select;


void display_error( BYTE *error_string_addr );

void show_message( BYTE *string );
void setup_system_font();
void init_timer_bars( void );
void draw_cpu_bar( void );
void show_cpugpu_time();
void draw_gpu_bar( void );
void single_step_pause( void );
void init_debug_menu();
void debug_menu();
void adjust_vector ( VECTOR *sv, WORD amount );
void adjust_svector ( SVECTOR *sv, WORD amount );
void fntprint_mat ( BYTE *name, MATRIX *mat );
extern LONG show_vc( BYTE *string );
extern void start_vc();
void move_camera();
void select_a_scenecam();
void select_a_freecam();
void switch_aspect_correction();
void debug_lights_on_off();
void pause_animation();

void show_message( BYTE *string );
void scene_debug();

enum
	{
	ADJUST_RDB_POS,
	ADJUST_RDB_SIZE,
	ADJUST_TS_POS,

	PLAY_SFX,
	PLAY_XA,
	ITEMS,
	};




void init_lines();
void draw_line( WORD x0, WORD y0, WORD x1, WORD y1, UBYTE r, UBYTE g, UBYTE b );
void show_border_area();
void debug_choose_winner();


void debug_print_mess(BYTE *mess, WORD time );


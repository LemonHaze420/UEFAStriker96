#define TEXTMAN_TSLOTS 	 160 // max num tslots

//*** Text manager slots ***

enum {


 LOLLY_TSLOT,
 SHELL_TSLOT,
 REQUEST_TSLOT,
 MAIN_TSLOT,
};




enum 
{
	TINY_FONT,
	MILFORD_FONT,
	TITLE_FONT,
	LOLLY_FONT,
	FT13_FONT,
	MAX_FONTS,
};





// font commands
#define FONT_XC		 1
#define FONT_YC		 2
#define FONT_LJUST	 4
#define FONT_RJUST	 8
#define FONT_NORMAL	 16

#define MAX_TEXT_STRINGS_ACTIVE 32	// in a single group


typedef struct
{
	UBYTE fr;
	UBYTE wd;
}Font_adjust;

typedef struct
{
	UBYTE graphic_type;
	UBYTE	alpha_lower;
	UBYTE	alpha_upper;
	UBYTE numeric;
	UBYTE comma;
	UBYTE stop;
	UBYTE exclamation;
	UBYTE double_quote;
	UBYTE single_quote;
	UBYTE dollar;
	UBYTE pound;
	UBYTE hat;
	UBYTE ampersand;
	UBYTE asterisk;
	UBYTE open_bracket;
	UBYTE	close_bracket;
	UBYTE	plus;
	UBYTE	minus;
	UBYTE	hash;
	UBYTE	question_mark;
	UBYTE colon;
	UBYTE less_than;
	UBYTE greater_than;
	UBYTE ret;
	UBYTE backspace;
	UBYTE space;			// rectangle shape for enter name
	UBYTE dummy_space;	// used to get width for space sprite (not displayed)
	BYTE  x_gap_size;		// x gap between characters
	BYTE  y_gap_size;		// y gap between characters
//	UBYTE x_space_size;	// space character size
	Font_adjust adjust[32];	// list of frames that require adjustment
}Font_data;

typedef struct
{
	Font_data *font_data;
	UBYTE lookup_list[256];
}Font;



void add_font ( Font_data *font_data, UWORD font_num );
void init_fonts();
void unspawn_text_string( Sprite *sprite );
void unspawn_all_text_strings();
void set_text_string_velocity( Sprite *sprite, LONG xv1, LONG yv1, LONG xv2, LONG yv2 );
void set_text_string_upd( Sprite *sprite, void *upd );

void apply_to_sprite_string( Sprite *sprite );
void get_group_sprite ( WORD group );

//extern Sprite *tail_font_sprite;
//extern Font_data font1_data;
//extern Font_data font2_data;
extern Font *resident_fonts[];


void italic_font_logic ( Sprite *sprite );
void milford_font_logic ( Sprite *sprite );
void milford_font_logic_solid ( Sprite *sprite );
void milford_font_logic_sub ( Sprite *sprite );

WORD count_word_size( UBYTE *ptr, WORD font_type );

#define FONT_INIT			0
#define FONT_STAGE1		1
#define FONT_STAGE2		2
#define FONT_STAGE3		3
#define FONT_SELECTED	4	
#define FONT_DYING		5	
#define FONT_DEAD			6







typedef struct Textman_struct
{
	WORD	 active;
 	WORD	 to_die; //kill_flag;
	Sprite *head_sprite;
	Sprite *tail_sprite;
	WORD	x,y;
	void	*font_logic;
	WORD	font_num;
	WORD	font_type;
	WORD	justify_type;
	WORD	spawn_group;

	WORD  max_brightness;

	WORD  x1,x2,y1,y2;

	UBYTE	string[512];
	WORD rt_edge_pos;
	struct Gadget *gadget;
	WORD fade_up_speed;
	WORD die_speed;
	WORD depth;
	CVECTOR text_color; //tint;
	WORD dim_if_requester_box_active;
	WORD delay_until_fade_up;
	WORD flash_text;	/* Yes = flash it */
}Textman_struct;

typedef struct
{
	Textman_struct *text_string_list;
	WORD max_text_strings;
	WORD   slot_cnt;

}Textman_header;

extern Textman_header textman_header;


void start_text_manager( WORD text_strings );
void adjust_text_string_pos ( Sprite *sprite, WORD xamount, WORD yamount );





Textman_struct *add_text_string ( WORD slot, UBYTE *string, WORD font_type, void *font_logic, WORD x, WORD y, WORD justify_type, WORD spawn_group, Gadget *gadget, WORD fade_up_speed, WORD die_speed );
void spawn_text_string ( Textman_struct *textman );


void set_textman_slot( WORD slot_pos, WORD num_slots );
extern WORD tslot_poses[];

void set_rdb_size_from_tm( Rdb_gadget *rdb, Textman_struct *tm );



void update_text_manager();





// gadget subtypes
enum
{
RDB_GADGET,	// rounded dialog gadget
SLIDER_GADGET,
};


typedef struct
{
	Pos  xvel1;
	Pos  yvel1;
	Pos  xvel2;
	Pos  yvel2;
	WORD xsize,ysize;
	RECT old_size;
	RECT size, grow_size;
	WORD grow_cnt;

	WORD depth;
	WORD tint;			// box brightness
	WORD r,g,b;
	WORD max_tint;		// tint fades up to this level...
	WORD flash_tint_default;	// tint level when this rdb is NOT selected
	WORD flash_tint_max;	// brightest tint level when this rdb selected

//	WORD quad_w,quad_h;
	WORD lt_h, bot_h, top_w, cen_w, cen_h, rt_w;
	WORD x1,y1,x2,y2;

	Sprite *cen;

	WORD xoffs,yoffs; //, tint_offs;
	WORD transp_rate;


	BYTE *text;
	WORD font_type;
	void *font_logic;
	WORD group;
	WORD justify_type;

	WORD parent_selected_num; // copy of select num on parent gadget

	struct Gadget *gadget;

	WORD update;


}Rdb_gadget;




typedef struct
{
	WORD x;
	WORD y;
	BYTE *val_addr;
	Pos val;
	Sprite *slide_spr;
//	Sprite *bar_spr;
	Sprite *lt_arrow;
	Sprite *rt_arrow;
	WORD group;
	WORD base_frame;
	LONG in_min, in_max;
	struct Gadget *gadget;
}Slider_gadget;


typedef struct Gadget
{
	WORD type;	// gadget type
	WORD mode;	// GADGET_OFF, GADGET_INIT, GADGET_UPDATE, GADGET_KILL
	WORD sequence_num;
	WORD id;
	WORD selected_num;
	WORD old_selected_num;

//** gadget can only be ONE of the below types **

	Rdb_gadget *rdb_gadget;
	Slider_gadget *slider;

	void	(*upd_rtn)( void * );	// Pointer to update routine of object

	struct Gadget *parent;

}Gadget;

typedef struct Gadget_header
{
	Gadget	*gadget_list;
	WORD 		total_gadgets;
	WORD		current_gadget;
	WORD		gadgets_active;
}Gadget_header;



extern Gadget_header gadget_header;
extern Gadget *gadget;	

void init_gadgets( WORD num_gadgets );
void kill_gadget ( Gadget *gadget );

Gadget *get_gadget( WORD gad_num );

void spawn_tw_gadget( Gadget *gadget );
void spawn_rdb_gadget( Gadget *gadget );


Gadget *set_rdb_gadget( Gadget *parent, Rdb_gadget *rdb, WORD x, WORD y, WORD w, WORD h, WORD group );

extern Gadget *get_free_gadget();


WORD modify_byte( BYTE *word_addr, WORD min, WORD max );
WORD modify_word_ud( WORD *word_addr, WORD min, WORD max );
WORD modify_word( WORD *word_addr, WORD min, WORD max );


Gadget *set_slider_gadget( WORD x, WORD y, Slider_gadget *slider, WORD in_min, WORD in_max, UBYTE *slider_val_addr, WORD group_num );


void ztest();


void set_rdb_box( Rdb_gadget *rdb, WORD x, WORD y, WORD w, WORD h );
void kill_rdb_gadget( Gadget *gad );
void kill_slider_gadget( Gadget *gad );




extern WORD requester_box_active;


extern WORD do_request( BYTE *request_text, WORD *request_button_list, WORD ypos, WORD default_button );


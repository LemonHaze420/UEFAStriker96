#define SHELL_ITEM_BASE 		256
#define SHELL_NEXT 				1
#define SHELL_PREV 				2
#define SHELL_CANCEL 			3
#define SHELL_PL_ED  			4
#define SHELL_TE_ED  			5
#define SHELL_YES					6
#define SHELL_NO					7
#define SHELL_OK					8
#define SHELL_QUIT				9
#define SHELL_LOAD				10
#define SHELL_SAVE				11
#define SHELL_STORE				12
#define SHELL_TYPES_LIST		13

#define TITLE 0
#define FRONTEND 1

#define MAX_SHELL_BUTTONS 6
typedef struct
{
	WORD current_item;
	WORD current_item_pressed;	// if X or SQU pressed, current item dumped here
	WORD old_item;
	WORD sequence_num;
	WORD keyboard_active;
	WORD module_abort;
	WORD master_option_temp;	// option selected by user
	WORD master_option;			//option selected by user when X pressed
	WORD master_option_active;
	WORD total_master_options;			// number of options 
	UWORD *types_list;				// word list for bottom buttons
	UWORD disable_button[ SHELL_TYPES_LIST ];
	UWORD current_button;
	UWORD last_button;
	UWORD drop_buttons;	// yes makes buttons fall off screen
	WORD  button_dest_x,button_dest_y;
	void (*old_init)();		// Pointer to init function
	void (*init)();		// Pointer to init function
	void (*update)();		// Pointer to update function
	void (*kill)();		// Pointer to kill func
	void (*prev)();		// previous func
	WORD lollies_moving; // yes or no
	WORD requester_box_active;
//	Button_gadget	gads[6];
//	Rdb_gadget		rdbs[6];
	WORD called_before;	// YES if exec called init function previously

	Sprite *spr[MAX_SHELL_BUTTONS];
	Sprite *shad[MAX_SHELL_BUTTONS];
	Sprite *lt_indicator_sprite;
	Sprite *rt_indicator_sprite;
	WORD timer;
	UWORD *types_list_req;
	WORD default_button_req;
	WORD buttons_dropping;
	WORD mode;	// frontend or title
	WORD drop_indicators;

	Sprite *lolly[2];
	WORD lolly_mode;
	BYTE lolly_text_buffer[2][128];
	WORD lolly_text_x[2];
	WORD lolly_text_dest_x[2];
	WORD lolly_xvel[2];

	Sprite *bot_bar[3];
	WORD bot_bar_text_x;
	Sprite *logo;
}Shell;

extern Shell *shell;

void modify_current_item_array( WORD base_num, WORD wd, WORD ht );
void modify_current_item( WORD total_items );

void drop_indicators();

extern void indicator_upd( Sprite *sprite );





extern Shell shell_struct;








void goto_shell_buttons();



void set_shell_bottom_buttons( UWORD *types_list, WORD def );
void kill_shell_bottom_buttons();
WORD get_shell_type();
void init_shell();
void set_title_lolly( BYTE *text1, BYTE *text2 );


/********************************************************************
 *																						  *
 * Pad structures																	  *
 *																						  *
 ********************************************************************/

typedef struct
{
	WORD c;			// constant contents of pad 
	WORD db;			// debounce pad contents (one frame) 
	WORD last_a;	// contents 1 frame ago 
	WORD last_b;	// contents 2 frames ago 
	WORD last;		// contents 1 frame ago if last_a == last_b 
	WORD r;			// key repeat signal 
	WORD r_wait;	// initial time to wait	(set for initial wait) 
	WORD r_delay;	// delay until next repeat (set for following repeat speed 
	WORD r_cnt[14];// repeat counter (  1 for each button )
	WORD alx;		// analog left stick x pos
	WORD aly;		// analog left stick y pos
	WORD arx;		// analog right stick x pos
	WORD ary;		// analog right stick y pos
}Padstruct;



void initialise_control_pads( WORD repeat_wait, WORD repeat_speed );
extern void read_pads( void );
extern void read_a_pad ( WORD num );
extern WORD joy_curr,joy_curr_store, joy_curr_output;

extern WORD joydata[];
extern WORD analog_lx[];
extern WORD analog_ly[];
extern WORD analog_rx[];
extern WORD analog_ry[];

extern Padstruct joy[8];
extern Padstruct *cjoy;

extern volatile WORD pads_inactive_cnt;
extern WORD pad_disabled[];	//yes=this pad is inactive
void set_pad_repeat_speed( WORD repeat_wait, WORD repeat_speed );
void restore_repeat_speed();

extern WORD pads_connected;


void pad_assign();


extern WORD pads_connected_list[];
extern WORD multitaps_connected_list[];


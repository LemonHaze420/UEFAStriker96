/************************************************************************
 *																							  	*
 *																							  	*
 *	PSX Control pad initialisation & read										  	*
 *																							  	*
 *																							  	*
 ************************************************************************/

#include "incs.h"

#include "pad_hw.c"

#ifdef PSX
#include "..\..\global\striker.def" // defs for PSX striker
#include "..\..\global\s01.h"	// structs used by scene "s01" format
#include "..\..\striker\frontend\mod.def"
#include "..\..\striker\src\anim.def"

#include "teamedit.h"
#include "preset.h"
#include	"sod.h"
#include	"scene.h"
#include "datafile.h"
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include "hardware.h"
#include "polydraw.h"	//psx structs
#include "text.h"
#include	"font.h"
#include "plasma.h"
#include	"render.h"
#include	"mem.h"
#include	"fvars.h"
#include	"pad.h"
#include "utils.h"
#include "entname.h"
#include "skindraw.h"
#include "..\..\global\image.h"
#include "psxmacro.h"

#endif
/************************************************************************
 *																							  	*
 *	prototypes																			  	*
 *																							  	*
 ************************************************************************/

void read_a_pad ( WORD num );
void joy_repeat( Padstruct *joy_ptr, WORD pad_bit, WORD bit_num );


/************************************************************************
 *																							  	*
 *	vars																					  	*
 *																							  	*
 ************************************************************************/

WORD joy_curr;			/* Specify pad to read. -1 = read current pad pressed */
WORD joy_curr_output;	/* num of last pad pressed */
WORD joy_curr_store;
WORD initial_repeat_speed, initial_repeat_wait;
Padstruct joy[8];

Padstruct *cjoy;	// addr of current pad. Set joy_curr
						// as 0-n for pad or -1 to use current pad pressed.

volatile WORD pads_inactive_cnt;
WORD pads_connected;
WORD pads_connected_list[8];	// 0,1,2,3 = left side ; 4,5,6,7=right side
WORD multitaps_connected_list[2];	// 0 = left size ; 1=right side

void initialise_control_pads( WORD repeat_wait, WORD repeat_speed )
{
WORD cnt;

	init_pad_hardware();

	pads_inactive_cnt=0;

//	for ( cnt=0;cnt<8;cnt++)
//		share->pad_disabled[cnt]=NO;

	set_pad_repeat_speed( repeat_wait, repeat_speed );

//***** store rep wait and speed *****
	initial_repeat_wait = repeat_wait;
	initial_repeat_speed = repeat_speed;
}

void read_pads( void )
{
WORD cnt;

//	FntPrint("reading pads\n");

/* joy1-4 = pad data */
/* joy1-4_last = last frame pad data */
/* joy_curr_output = pad num 0-3 of pad currently pressed */

	if ( pads_inactive_cnt )
		{
		pads_inactive_cnt--;
		}


	read_machine_pad_data(); // read pad data from hardware to joydata array


// dump joydata into joy[] arrays

	for ( cnt=0;cnt<8;cnt++)
		{

//		if ( share->pad_disabled[cnt]==NO )
			read_a_pad ( cnt );		// output into pad structs

//		FntPrint("lxy=%d %d rxy=%d %d dig=%d\n",joy[cnt].alx, joy[cnt].aly,joy[cnt].arx, joy[cnt].ary,joy[cnt].c );

		}

	if ( joy_curr == -1 )
		cjoy=&joy[ joy_curr_output ];
	else
		cjoy=&joy[ joy_curr ];


}

void read_a_pad ( WORD num )
{
Padstruct *joy_ptr;

	joy_ptr = &joy[num];

	joy_ptr->last_b = joy_ptr->last_a;
	joy_ptr->last_a = joy_ptr->c;
	
	if ( pads_inactive_cnt == 0 )
		{
		joy_ptr->c = joydata[num]; //only read pads if inactive cnt=0
		joy_ptr->alx = analog_lx[num];
		joy_ptr->aly = analog_ly[num];
		joy_ptr->arx = analog_rx[num];
		joy_ptr->ary = analog_ry[num];
		}

	joy_ptr->last = joy_ptr->last_a | joy_ptr->last_b;

	joy_ptr->db = joy_ptr->c & (~joy_ptr->last);

	if ( pads_inactive_cnt )
		return;

	if ( joy_ptr->c )
		{
		joy_curr_output=num;
		}

//buttons that have pad repeat feature....
	joy_repeat ( joy_ptr, PAD_LT,0 );
	joy_repeat ( joy_ptr, PAD_RT,1 );
	joy_repeat ( joy_ptr, PAD_UP,2 );
	joy_repeat ( joy_ptr, PAD_DN,3 );
	joy_repeat ( joy_ptr, PAD_L1,4 );
	joy_repeat ( joy_ptr, PAD_R1,5 );
	joy_repeat ( joy_ptr, PAD_L2,6 );
	joy_repeat ( joy_ptr, PAD_R2,7 );
	joy_repeat ( joy_ptr, PAD_X ,8 );
	joy_repeat ( joy_ptr, PAD_SQU ,9 );
	joy_repeat ( joy_ptr, PAD_TRI ,10 );
	joy_repeat ( joy_ptr, PAD_CIR ,11 );
}

void joy_repeat( Padstruct *joy_ptr, WORD pad_bit, WORD bit_num )
{
	if ( joy_ptr->c & pad_bit )
		{

		if ( joy_ptr->r_cnt[bit_num] == 0 )
			{
			joy_ptr->r_cnt[bit_num] = joy_ptr->r_delay;
			joy_ptr->r |= pad_bit;
			}
		else
			{
			joy_ptr->r_cnt[bit_num]--;
			joy_ptr->r &= ~pad_bit;
			}
		}

	else
		{
		joy_ptr->r_cnt[bit_num] = joy_ptr->r_wait;
		joy_ptr->r&=~pad_bit;
		}


	if ( joy_ptr->db & pad_bit )
		joy_ptr->r |= joy_ptr->db & pad_bit;

}




void set_pad_repeat_speed( WORD repeat_wait, WORD repeat_speed )
{
WORD cnt;


	for ( cnt=0;cnt<8;cnt++)
		{
		joy[cnt].r_wait = repeat_wait;	//initial wait before repeat occurs
		joy[cnt].r_delay = repeat_speed;
		}
}



void restore_repeat_speed()
{
	set_pad_repeat_speed( initial_repeat_wait, initial_repeat_speed );
}












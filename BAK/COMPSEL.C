/****************************************************
 *																	 *
 *	 																 *
 * Competition select										 *
 *	( from main wheel )										 *
 *																	 *
 ****************************************************/
#if 0
#include "incs.h"

/****************************************************
 *																	 *
 *	 Defines														 *
 *																	 *
 ****************************************************/

/****************************************************
 *																	 *
 *	 Structs														 *
 *																	 *
 ****************************************************/

/****************************************************
 *																	 *
 *	 Prototypes													 *
 *																	 *
 ****************************************************/

/****************************************************
 *																	 *
 *  Vars															 *
 *																	 *
 ****************************************************/

Gadget *compsel_gadget;

WORD compsel_objects[]={

//rlE_Comp_Fren,
//rlE_Comp_KO,
//rlE_Comp_Sea,
//rlE_Comp_Wold,
//rlE_Comp_Euro,
//rlE_Comp_CLe,
0 };

SOD *sod;

void init_competition_select()
{
// load m01 for trophy
	load_m01_file ( COMPSEL_IN, COMPSEL_IN_SLOT );
	load_m01_file ( COMPSEL_OUT, COMPSEL_OUT_SLOT );



}

void update_competition_select()
{


	compsel_gadget = gadget_header.gadget_list + COMPSEL_GADGET;
	gadget_init.transp_rate = 0;


	if ( gadget_header.current_gadget == -1 )
		{
		gadget_header.current_gadget = 0;
		}


	set_gadget_text_attr ( 0,  0,110, FONT1, 0, FONT_XC, 0 , competition_text_ptrs[share->lang], italic_font_logic );

	if ( main_wheel.sequence_num == 0 )
		{
		compsel_gadget->preset_val = share->competition_type;
		main_wheel.sequence_num++;
		set_gadget_box_attr ( 0, 0, -1, -1,  0,0,0 );
		gadget_init.transp_rate = 0;
		gadget_init.xvel1.l = 4350000;
		spawn_gadget ( DIALOG_BOX_GADGET, compsel_gadget );
		//opt_gads[1] = gadget;

		sod = &sod_header->sod_list[ compsel_objects[ gadget->preset_val ] ] ;
		start_obj_m01_anim1 ( sod , COMPSEL_IN_SLOT , MATRIX_ANIM_SINGLE_SHOT + MATRIX_ANIM );

		}


	sod_header->sod_list[ compsel_objects[ compsel_gadget->preset_val ] ].active = NO;
	sod_header->sod_list[ compsel_objects[  compsel_gadget->preset_val ] ].rot.vy += 30;


	if ( main_wheel.active == NO )
		{
		FntPrint("Competition select\n\npress down to exit\n");
		if ( joy[0].r & PAD_UP )
			{

			if ( gadget_header.current_gadget  )
				gadget_header.current_gadget--;
			}

		if ( joy[0].r & PAD_DN )
			{
			gadget_header.current_gadget++;
			if ( gadget_header.current_gadget == 1 )
				{
				main_wheel.active = YES;
				}
			}


		if ( gadget_header.current_gadget == 1 )
			{
			}




		mod_gad_preset_lr ( compsel_gadget );
		share->competition_type = compsel_gadget->preset_val;


		}

	sod_header->sod_list[ compsel_objects[ compsel_gadget->preset_val ] ].active = YES;

	FntPrint("cg=%d\b",compsel_gadget->preset_val);



}

void kill_competition_select()
{

		kill_gadget( compsel_gadget );

//		sod_header->sod_list[ compsel_objects[ compsel_gadget->preset_val ] ].active = NO;


		sod = &sod_header->sod_list[ compsel_objects[ gadget->preset_val ] ] ;
		start_obj_m01_anim1 ( sod , COMPSEL_OUT_SLOT ,  MATRIX_ANIM_DEACTIVATE + MATRIX_ANIM + MATRIX_ANIM_SINGLE_SHOT );


}


#endif

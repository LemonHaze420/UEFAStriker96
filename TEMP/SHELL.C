#include "incs.h"

#ifdef PSX
#include "..\..\striker\frontend\mod.def"
#include "..\..\striker\src\anim.def"
#include "datafile.h"
#include "plasma.h"
#include "sprite.h"
#include "gadget.h"
#include "shell.h"
#include	"pad.h"
#include	"frontend.h"
#include "hardware.h"
#include "font.h"
#endif


#define LOLLY_INACTIVE	0
#define LOLLY_ACTIVE		1
#define LOLLY_DYING		2 
#define LOLLY_INIT		3

WORD bot_butt_frames[]={ 0, 3, 0, 18, 24, 27, 15, 18, 15, 6, 9, 12, 21 };



/****************************************************
 *																	 *
 *	 Prototypes													 *
 *																	 *
 ****************************************************/

void spawn_title_lollies();
void reset_title_lollies();
void run_title_lollies();
void exec_shell( void *init, void *update, void *kill );
void init_shell();
void run_shell();
void shell_control();
void do_shell_bottom_buttons();
void indicator_upd( Sprite *sprite );
void shell_icon_upd( Sprite *sprite );
void shell_shad_upd( Sprite *sprite );
void set_shell_bottom_buttons_req();
void bar_shad_upd( Sprite *sprite );
WORD find_shell_prev_button();




/****************************************************
 *																	 *
 *  Vars															 *
 *																	 *
 ****************************************************/

Shell shell_struct;
Shell *shell;

WORD shell_button_types[12];


void goto_shell_buttons()
{
UWORD *types_list;
WORD index;

	sfxSpot( HIT1, 30 );
	shell->old_item = shell->current_item;
	shell->current_item = 256;
	shell->master_option_active=YES;
	types_list = shell->types_list;

	index = SHELL_ITEM_BASE;
	while( *types_list )
		{
		if ( *types_list == SHELL_NEXT )
			{
			shell->current_item = index;
			break;
			}
		index++;
		types_list++;
		}

}

void exit_shell_buttons()
{
	sfxSpot( HIT1, 30 );
	shell->current_item = shell->old_item;
	shell->master_option_active = NO;
}



void modify_current_item( WORD total_items )
{

//	FntPrint("current item=%d mopt=%d\n",shell->current_item,shell->master_option );

	if ( cjoy->db & PAD_UP )
		{
		if ( shell->master_option_active == YES )
			{
			// user pressed up from main wheel
		 	shell->master_option_active = NO;
			shell->current_item = total_items-1;
			sfxSpot( HIT1, 30 );
			}
		else
			{
			if ( shell->current_item != 0 )
				{
				sfxSpot( HIT1, 30 );

				shell->current_item--;
				flash_counter=0;
				}
			}
		}

	if ( cjoy->db & PAD_DN )
		{
		if ( shell->master_option_active == NO )	
			{
		
			flash_counter=0;
			shell->current_item++;
			if ( shell->current_item >= total_items )
				{
				goto_shell_buttons();
				}
			else
				sfxSpot( HIT1, 30 );
				
					
			}
		}


}

void modify_current_item_array( WORD base_num, WORD wd, WORD ht )
{
//
// Select items arranged as an array
// as X, Y


WORD total_items = (wd * ht) + base_num;
WORD lt,rt;
WORD null_word=0;

	if ( cjoy->db & PAD_UP )
		{
		flash_counter=0;

		sfxSpot( HIT1, 30 );

		if ( shell->master_option_active == YES )
			{
			// user pressed up from main wheel
		 	shell->current_item = total_items-1;
		 	shell->master_option_active = NO;

			}
		else
			{

			shell->current_item-= wd;
			if ( shell->current_item < 0 )
				shell->current_item += total_items;

			}
		}


	if ( shell->master_option_active == NO )
		{
		if ( cjoy->db & PAD_DN )
			{

			flash_counter=0;
			shell->current_item+=wd;
			if ( shell->current_item >= total_items )
				{
				shell->current_item-=wd;
				goto_shell_buttons();
				return;
				//shell->master_option_active = NO;
				}
			else
				sfxSpot( HIT1, 30 );


			}


		if ( cjoy->db & PAD_LT )
			{
			sfxSpot( HIT1, 30 );

			lt = shell->current_item % wd;
			shell->current_item--;

			if ( shell->current_item < 0 ) shell->current_item = wd-1;

			flash_counter=0;

			rt = shell->current_item % wd;
			if ( rt > lt )
				{
				shell->current_item += wd;
				}
			}

		if ( cjoy->db & PAD_RT )
			{
			sfxSpot( HIT1, 30 );


			lt = shell->current_item % wd;
			shell->current_item++;
			rt = shell->current_item % wd;
			flash_counter=0;

			if ( rt < lt )
				{
				shell->current_item -= wd;
				}
			}
		}
}


void run_shell()
{
  
	push(0);	
	store_processes();

	abort_game=NO;


	while ( abort_game == NO )
		{
		bg_loop();
		}

	restore_processes();

	pop(0);
}




/**********/



void exec_shell( void *init, void *update, void *kill )
{

	push(0);

	shell->init 	= init;
	shell->update 	= update;
	shell->kill		= kill;
	shell->module_abort = NO;

	read_pads();

	store_processes();


	if ( shell->init == shell->old_init )
		{
		shell->called_before = YES;
		}
	else
		{
		shell->called_before = NO;
		shell->old_init = shell->init;
		}

	(*shell->init)();

//	add_process( update );


	(*shell->update)();

	while( (shell->module_abort == NO) )
		{
  		(*shell->update)();
 		bg_loop();



		shell_control();
		}

	restore_processes();


	(*shell->kill)();


//	shell->current_button=0;

 //	update_text_manager();

	
	read_pads();
	pop(0);


}



void shell_control()
{
WORD cnt;
UWORD type, index, disable_type;
WORD x,xp,w;
Textman_struct *ts;
BYTE *text;
Rdb_gadget *rdb;

//	FntPrint("item=%d\n",shell->current_item);


	shell->current_item_pressed = -1;


	if ( shell->master_option_active == YES )
		{

		if ( cjoy->db & PAD_LT )
			{
			if ( shell->current_item > SHELL_ITEM_BASE ) 
				{
				sfxSpot( HIT1, 30 );
				shell->current_item--;
				flash_counter=0;
				}
			}

		if ( cjoy->db & PAD_RT )
			{
			if ( shell->current_item < (SHELL_ITEM_BASE-1+shell->total_master_options) )
				{
				sfxSpot( HIT1, 30 );

				shell->current_item++;
				flash_counter=0;
				}
			}

		index = shell->current_item - SHELL_ITEM_BASE;
		type = shell->types_list[ index ];
		disable_type = shell->disable_button[ type ];

		if ( (cjoy->db & (PAD_X|PAD_SQU|PAD_STA|PAD_TRI)) && (shell->timer==0) )
			{
			if ( cjoy->db & PAD_TRI )
				{
				if ( find_shell_prev_button() )
					{
					shell->timer=20;
					shell->current_button=0;
					}
				}
			else
				{
				shell->timer=20;
				shell->current_button=0;
				}

			}

		if ( shell->timer )
			{
			shell->timer--;

			if ( (shell->timer==15) )
				{
				if ( disable_type != type )
					{
					shell->drop_buttons=YES;

					if ( type != SHELL_QUIT )
						shell->lolly_mode = LOLLY_DYING;
					}
				else
					{
					shell->timer=5;
					}
				}

			if ( (shell->timer==5) )
				{
				shell->timer=0;
				shell->master_option = shell->master_option_temp;
				shell->current_button = type;
				shell->last_button = type;

				if ( (type == SHELL_NEXT) && ( disable_type == 0 ) )
					{
					sfxSpot( BUTVOC, 30 );
					share->thread_pos++;
					shell->module_abort = YES;
					}
				else
					{
					sfxSpot( BALL1, 30 );
					}

				if ( (type == SHELL_SAVE) || (type == SHELL_OK) || (type == SHELL_LOAD) || (type == SHELL_CANCEL) || ( type == SHELL_QUIT ) || ( type == SHELL_STORE) ||( type == SHELL_YES ))
					{
					sfxSpot( HIT1, 30 );
					shell->module_abort = YES;
					}

				if ( type == SHELL_PREV )
					{
					sfxSpot( HIT1, 30 );
					share->thread_pos--;
					shell->module_abort = YES;
					}


				if ( type == SHELL_TE_ED )
					{
					shell->module_abort = YES;
					sfxSpot( HIT1, 30 );
					}

				if ( type == SHELL_PL_ED )
					{
					shell->module_abort = YES;
					sfxSpot( HIT1, 30 );
					}
				}
			}
		}
	else
		{
		if ( cjoy->db & (PAD_X|PAD_SQU) )
			{
			shell->current_item_pressed = shell->current_item;
			}
		if ( (cjoy->db & (PAD_STA)) && ( shell->keyboard_active == NO ) )
			{
			goto_shell_buttons();
			}
		}
}


WORD get_shell_type()
{
/* Returns shell bottom button type pressed 
	on exitting shell */

	return shell->types_list[ shell->current_item - SHELL_ITEM_BASE ];
}




void init_shell()
{
WORD cnt;
WORD xp;
	shell=&shell_struct;
	shell->master_option_active=YES;
	shell->drop_indicators = NO;
	shell->current_item = SHELL_ITEM_BASE;
	shell->lollies_moving=YES;
	shell->default_button_req = 0;
	shell->timer = 0;
	shell->buttons_dropping=NO;
	init_process_executor();
	init_gadgets(60);

	start_text_manager( TEXTMAN_TSLOTS );	// call before update_frontend
	set_textman_slot( LOLLY_TSLOT, 4 );	// for lolly text and its dropshadow
	set_textman_slot( SHELL_TSLOT, 10 );	// for shell buttons
	set_textman_slot( REQUEST_TSLOT, 4 );		
	set_textman_slot( MAIN_TSLOT, 96 );		// reserve 6 slots


	for( cnt=0; cnt<SHELL_TYPES_LIST; cnt++ )
		{
		shell->disable_button[cnt] = 0;
		}

	shell->last_button = 0;

	shell->called_before = NO;
	shell->old_init = 0;
	add_process ( set_shell_bottom_buttons_req );	/* Checks to see if new buttons need set up */


	load_ted_info();	// load ted file and store in ted_block[]

	init_display( 512, 256, 0,0,0, YES, YES, NO );	//screen xy res & bg color
	init_sprites ( FRONTEND_SAN, 1024 );


	init_debug_menu();

	joy_curr=-1;	// read current pad pressed into *cjoy


	load_scene_materials ( FRONTEND_TEX );
	setup_system_font();	
	init_fonts();	// ** call after 'load scene materials' ***


	shell->lt_indicator_sprite = spawn_sprite( ICON );
	sprite->upd_rtn = ( void *)indicator_upd;
	sprite->type2= 0;	// its the left sprite
	shell->rt_indicator_sprite = spawn_sprite( ICON );
	sprite->upd_rtn = ( void *)indicator_upd;
	sprite->type2= 1;	// its the right sprite ( at the right price )




	add_backdrop( TIT_BG );

	frame_update=2;



/** Setup icon bottom buttons **/
	xp = 340;
	for( cnt=0; cnt<MAX_SHELL_BUTTONS;cnt++)
		{
		spawn_sprite( ICON );
		shell->spr[cnt] = sprite;
		sprite->use_frame_offs = YES;
		sprite->display = NO;
		sprite->x.w.hi = xp;
		sprite->depth = NEAREST_DEPTH+10;
  		sprite->mode = 0;

		spawn_sprite( SHELSHD );
		sprite->prev = shell->spr[cnt];
		sprite->use_frame_offs = YES;
		sprite->display = NO;
		sprite->x.w.hi = xp;
		sprite->upd_rtn = (void *)bar_shad_upd;
		sprite->depth = NEAREST_DEPTH+11;
		sprite->transp_rate = 2;
		sprite->tint.r=
		sprite->tint.g=
		sprite->tint.b=44;

		}

/** Bottom bar left sprite ***/
	shell->bot_bar[0] = spawn_sprite( BAR );
	sprite->frame = 0;
	sprite->upd_rtn = (void *)shell_icon_upd;
	sprite->mode = 1;
	sprite->group_num = SHELL_ITEM_BASE-1;
	sprite->depth = MIDDLE_DEPTH;
	sprite->display=NO;

/** Bottom bar middle sprite ***/
	shell->bot_bar[1] = spawn_sprite( BAR );
	sprite->depth = MIDDLE_DEPTH;
	sprite->display=NO;
	sprite->frame = 1;

/** Bottom bar right sprite ***/
	shell->bot_bar[2] = spawn_sprite( BAR );
	sprite->depth = MIDDLE_DEPTH;
	sprite->frame = 2;
	sprite->display=NO;


/** Bottom bar shadow sprites ***/

	for(cnt=0;cnt<3;cnt++)
		{
		spawn_sprite( BAR );
		sprite->frame = cnt+3;
		sprite->prev = shell->bot_bar[cnt];
		sprite->display=NO;
		sprite->depth = MIDDLE_DEPTH+1;
		sprite->transp_rate = 2;
		sprite->tint.r = sprite->tint.g = sprite->tint.b = 44;
		sprite->upd_rtn = (void *)bar_shad_upd;
		sprite->centre_if_scaled = NO;
		}



/*** Logo ***/
	spawn_sprite( LOGO );
	sprite->display = NO;
	shell->logo = sprite;

	spawn_sprite( SHELSHD );
	sprite->display = NO;
	shell->logo->prev = sprite;
	sprite->frame = 1;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 44;

	spawn_title_lollies();

}


void set_shell_bottom_buttons( UWORD *types_list, WORD default_button )
{

//	if ( (shell->types_list == types_list) && () )
//		{
//		return;
//		}

	shell->types_list_req = types_list;
	shell->default_button_req = default_button;
	shell->current_button=0;
}

void set_shell_bottom_buttons_req()
{
/************************
 *
 * This func is constantly polled and sets up buttons when old buttons have finished moving *
 *
 ************************/

WORD default_button;
UWORD *types_list;
WORD index;
WORD found_button;

//	shell->current_button=0;


	if ( (shell->timer) || (shell->types_list_req == 0) || (shell->buttons_dropping))
		{
		return;
		}

	types_list = shell->types_list_req;
	default_button = shell->default_button_req;
	shell->types_list_req = 0;


	if ( shell->requester_box_active == NO )
		{
		shell->button_dest_x = 340;
		shell->button_dest_y = 230+(16*4);
		}

	shell->drop_buttons = NO;
	shell->types_list = types_list;

	shell->current_button=0;


//***** Count num of buttons in list ***** 

	shell->total_master_options=0;
	while( *types_list )
		{
		types_list++;
		shell->total_master_options++;
		}

	if ( default_button )
		{
		shell->last_button = default_button;
		}

	if ( shell->last_button == 0 )
		{
		// default to next
		shell->last_button = SHELL_NEXT;
		}

	found_button=NO;
	//*** Last button was never set, so look for NEXT ***
	types_list = shell->types_list;
	index = SHELL_ITEM_BASE;
	while( *types_list )
		{
		if ( *types_list == shell->last_button )
			{
			shell->current_item = index;
			found_button=YES;
			break;
			}
		index++;
		types_list++;
		}

	if ( found_button == NO )
		{
		/*** Could not find button matching previously pressed, so default to first one in list ***/
		shell->current_item = SHELL_ITEM_BASE;
		}

	do_shell_bottom_buttons();


	shell->master_option_active=YES;
}


void do_shell_bottom_buttons()
{
Spr_anim_frame_san *fr;
//Textman_struct *ts;
//Gadget *gad;
WORD x,xp,w;
BYTE *text;
UWORD type;
WORD cnt,gr,x1,x2;

//*** Shell buttons ***

	xp = shell->button_dest_x;
	cnt=0;

									  
	for( x=0; x< MAX_SHELL_BUTTONS; x++ )
		{
		shell->spr[x]->display=NO;
		}

	gr = shell->total_master_options + SHELL_ITEM_BASE - 1;
	for( x=shell->total_master_options-1;x>=0;x--)
		{
		type = *(shell->types_list+x);

		sprite = shell->spr[ x ];
		sprite->display=YES;
		sprite->group_num = gr;

		sprite->y.w.hi = 1200 + (cnt*2400); 
		sprite->x.w.hi = xp;
		sprite->frame = bot_butt_frames[ type ];	// set base frame num
		sprite->type2 = sprite->frame;

		sprite->temp = sprite->y.w.hi;
		sprite->upd_rtn = (void *)shell_icon_upd;
	  
		fr = get_sprite_frame_addr( ICON );
		fr += sprite->frame;

		xp -= (fr->real_w+8)*2;
		gr--;

		cnt++;
		}

	shell->bot_bar[0]->x.w.hi = -454;
	shell->bot_bar[2]->x.w.hi = xp;

	fr = get_sprite_frame_addr( BAR );
	fr += shell->bot_bar[0]->frame;
	x1 = shell->bot_bar[0]->x.w.hi + (fr->real_w*2);
	x2 = shell->bot_bar[2]->x.w.hi;

	shell->bot_bar[1]->x.w.hi = x1;
	shell->bot_bar[1]->centre_if_scaled=NO;

	shell->bot_bar_text_x = x1 + ((x2-x1)/2);

	shell->bot_bar[0]->y.w.hi = 1200 + (cnt*2400); 
	shell->bot_bar[0]->temp = shell->bot_bar[0]->y.w.hi;
	shell->bot_bar[0]->display=YES;

	shell->bot_bar[1]->xscale = (x2-x1)*32;



}


void kill_shell_bottom_buttons()
{
WORD x;
	for ( x=0; x<shell->total_master_options; x++ )
		{
//		kill_gadget( shell->gads[x].gadget );
//		kill_rdb_gadget( shell->rdbs[x].gadget );
		}

//	printf("killed %d rdbs\n",shell->total_master_options);
}


void indicator_upd( Sprite *sprite )
{
WORD dest;
WORD stay_dead;
WORD xd;
WORD yo;


	if ( sprite->textman->font_type == TINY_FONT )
		{
		yo=-8;
		}
	else
		yo = 4;

	stay_dead=NO;
	if ( shell->mode == FRONTEND )
		{
		if ( (sprite->textman->active == NO) && ( shell->master_option_active==NO ) )
			{
			sprite->sequence_num = 0;	/* Make indicators fly off screen and stay there */
			stay_dead = YES;
			}
		if ( sprite->textman->active && (sprite->textman->spawn_group<0) )
			{
			stay_dead = YES;
			}
		}
	

	if ( sprite->type2 == 0 )
		{
		sprite->x2.w.hi = sprite->textman->x1 - ( 8*2 ) - (17*2);
		}
	else
		{
		sprite->x2.w.hi = sprite->textman->x2 + ( 8*2 );
		}

	sprite->y2.w.hi = sprite->textman->y1; // - ( 8*2 ) - (17*2);


	if ( (shell->mode == FRONTEND) && (shell->master_option_active==YES) )
		{
//		if ( sprite->sequence_num == 2 )
			sprite->sequence_num = 0;
			stay_dead = YES;
		}
	else
		{
		if ( !stay_dead )
		sprite->display=YES;
		}


	if ( sprite->initialised == NO )
		{
		sprite->mode=0;
		sprite->xvel1.l = 0;

		sprite->x.w.hi = (sprite->type2==0)?-542:542;

		sprite->sequence_num = 0;
		sprite->frame = 36;
		return;
		}

	switch( sprite->sequence_num )
		{
		case 0:
		/** Indicator is flying off screen **/
			if ( sprite->type2==0 )
				dest=-6;
			else
				dest=6;

			sprite->x.w.hi += sprite->xvel1.w.hi + (dest*4);
			sprite->xvel1.w.hi += dest;
			
			if ( sprite->xvel1.w.hi > 70 )
				sprite->xvel1.w.hi = 70;
			if ( sprite->xvel1.w.hi < -70 )
				sprite->xvel1.w.hi = -70;

			if ( (shell->lt_indicator_sprite->x.w.hi < -1000) )
				{
				if ( stay_dead )
					{
					sprite->display=NO;
					//sprite->x.w.hi = (sprite->type2==0)?-512:512;
					sprite->y.w.hi = sprite->textman->y1 + yo;
					return;
					}

			/** sprite has moved completely off screen **/
				sprite->sequence_num++;

				if ( sprite->type2 == 0 )
					{
					sprite->x2.w.hi = sprite->textman->x1 - ( 8*2 ) - (16*2);
					}
				else
					{
					sprite->x2.w.hi = sprite->textman->x2 + ( 8*2 );
					}
				sprite->y.w.hi = sprite->textman->y1 + yo;
				sprite->xvel1.l = 0;
				}
		break;

		case 1:
			sprite->sequence_num++;
		break;

		case 2:
			/** User is choosing an option **/
			if ( sprite->type2 == 0 )
				{
				xd = sprite->textman->x1 - ( 6*2 ) - (15*2);
				}
			else
				{
				xd = (sprite->textman->x2 + ( 6*2 ));
				}

			sprite->x.w.hi -= (sprite->x.w.hi - xd)/3;
			sprite->x.w.hi = xd;


//			sprite->x2.w.hi = sprite->x.w.hi;
	
			sprite->display = YES; //1-shell->master_option_active;


			sprite->frame = 36;
			if ( ((flash_counter/8)&1)==0)
				{
				sprite->frame++;
				}

			sprite->y.w.hi = sprite->textman->y1 + yo;
		break;


		case 3:
			sprite->xvel1.l=0;
			sprite->sequence_num=0;
		break;

		}


//	FntPrint("x=%d\n",sprite->x.w.hi );

	sprite->textman=0;


//	sprite->display=NO;

	if ( shell->master_option_active )
		{
		if ( sprite->x.w.hi < -600 )
			sprite->x.w.hi = -600;
		else
			{
			if ( sprite->x.w.hi > 600 )
				sprite->x.w.hi = 600;
			}

		}

//	FntPrint("x=%d\n",sprite->x.w.hi);
}





void shell_icon_upd( Sprite *sprite )
{
	
Textman_struct *ts;
	if ( shell->drop_buttons == NO )
		{
		if ( sprite->mode == 0 )
			{
		/** Set frame only if an icon **/
			sprite->frame = sprite->type2;
			if ( (sprite->group_num == shell->current_item) && ( shell->master_option_active ) )
				{
				if ( ((flash_counter/8)&1) == 0 )
					{
					sprite->frame++;
					}
				if ( shell->timer > 17 )
					sprite->frame = sprite->type2+2;
				}
			}
		sprite->y.w.hi -= ( sprite->y.w.hi - shell->button_dest_y )/2;

		}
	else
		{
	/** sprites are flagged to fall off screen **/

		if ( sprite->y.w.hi > 600 )
			{
			sprite->display = NO;
			shell->buttons_dropping=NO;	// button have stopped

			}
		else
			{
			sprite->y.w.hi += (( sprite->y.w.hi - shell->button_dest_y )/2)+((sprite->group_num-SHELL_ITEM_BASE)*4) + 6;
			}
		}

	if ( sprite->mode )
		{
		/** Its a bottom bar sprite, link the other two sprs to it **/

		shell->bot_bar[1]->y.l = sprite->y.l;
		shell->bot_bar[2]->y.l = sprite->y.l;

	/** Bottom lolly text **/
		if ( (shell->requester_box_active == NO) && ( shell->mode == FRONTEND ) )
			{
			ts=add_text_string ( tslot_poses[LOLLY_TSLOT]+2 , &shell->lolly_text_buffer[0][0] , TITLE_FONT, milford_font_logic, shell->bot_bar_text_x, shell->bot_bar[0]->y.w.hi, FONT_XC, -2, 0, 0,0 );
			ts->depth = NEAREST_DEPTH-1;
			ts=add_text_string ( tslot_poses[LOLLY_TSLOT]+3 , &shell->lolly_text_buffer[0][0] , TITLE_FONT, milford_font_logic_sub, shell->bot_bar_text_x-6, shell->bot_bar[0]->y.w.hi+12, FONT_XC, -2, 0, 0,0 );
			ts->depth = NEAREST_DEPTH-0;
			}

		}

}

void shell_shad_upd( Sprite *sprite )
{
Sprite *prev = (Sprite *)sprite->prev;

	sprite->display = prev->display;
	sprite->x.w.hi = prev->x.w.hi - 15*2;
	sprite->y.w.hi = prev->y.w.hi + 3*4;

	sprite->xoffs = ((shell->button_dest_y - prev->y.w.hi)/4) + 8;
	sprite->yoffs = 12;

	if ( (shell->timer>17 ) && (prev->group_num == shell->current_item) )
		{
		sprite->xscale = 220;
		sprite->yscale = 220;
		sprite->centre_if_scaled = YES;
		sprite->xoffs += 12;
		sprite->yoffs -= 4;
		}
	else
		{
		sprite->xscale = 256;
		sprite->yscale = 256;
		}

}

void bar_shad_upd( Sprite *sprite )
{
Sprite *prev = (Sprite *)sprite->prev;

	sprite->display = prev->display;
	sprite->x.w.hi = prev->x.w.hi - 15*2;
	sprite->y.w.hi = prev->y.w.hi + 3*4;
	sprite->xscale = prev->xscale;




}



void drop_indicators()
{
	shell->lt_indicator_sprite->sequence_num=3;
	shell->rt_indicator_sprite->sequence_num=3;
}




/*******************************
 *
 * Top and bottom lollies
 *
 *******************************/



void set_title_lolly( BYTE *text1, BYTE *text2 )
{
	strcpy( &shell->lolly_text_buffer[0][0], text1 );

	if ( text2 )
		strcpy( &shell->lolly_text_buffer[1][0], text2 );
	else
		shell->lolly_text_buffer[1][0] = 0;

	if ( text1 )
		strcpy( &shell->lolly_text_buffer[0][0], text1 );
	else
		shell->lolly_text_buffer[0][0] = 0;


	shell->lolly_mode = LOLLY_INIT;
	reset_title_lollies();
}

void spawn_title_lollies()
{

	spawn_sprite( -1 );
 	sprite->transp_rate = 2;
	sprite->depth = MIDDLE_DEPTH-100;
	shell->lolly[0] = sprite;


	spawn_sprite( -1 );
 	sprite->transp_rate = 2;
	sprite->depth = MIDDLE_DEPTH-100;
	shell->lolly[1] = sprite;


	shell->lolly[0]->r1 = shell->lolly[0]->r3 = 
	shell->lolly[0]->g1 = shell->lolly[0]->g3 =
	shell->lolly[0]->b1 = shell->lolly[0]->b3 = 128;
	shell->lolly[0]->r0 = shell->lolly[0]->r2 =
	shell->lolly[0]->g0 = shell->lolly[0]->g2 =
	shell->lolly[0]->b0 = shell->lolly[0]->b2 = 0;
	shell->lolly[0]->x.w.hi =  512-585;
	shell->lolly[0]->y.w.hi = -480;
	shell->lolly[0]->untextured_frame.w = 585; 
	shell->lolly[0]->untextured_frame.h = 32*4;
	shell->lolly[0]->gouraud = YES;

	shell->lolly[1]->r0 = shell->lolly[1]->r2 =
	shell->lolly[1]->g0 = shell->lolly[1]->g2 =
	shell->lolly[1]->b0 = shell->lolly[1]->b2 = 128;
	shell->lolly[1]->r1 = shell->lolly[1]->r3 = 
	shell->lolly[1]->g1 = shell->lolly[1]->g3 =
	shell->lolly[1]->b1 = shell->lolly[1]->b3 = 0;
	shell->lolly[1]->x.w.hi = -512;
	shell->lolly[1]->y.w.hi = 296; 
	shell->lolly[1]->untextured_frame.w = 600; 
	shell->lolly[1]->untextured_frame.h = 200; 
	shell->lolly[1]->gouraud = YES;

	reset_title_lollies();

	add_process( run_title_lollies );

	run_title_lollies();
}

void reset_title_lollies()
{
WORD wd;
Textman_struct *ts;

	shell->lolly[0]->brightness = 40;
	shell->lolly[1]->brightness = 0;

//	shell->lolly_text_x[0] = 512;

//	ts = add_text_string ( tslot_poses[LOLLY_TSLOT]+0 , &shell->lolly_text_buffer[1][0] , MILFORD_FONT, milford_font_logic, shell->lolly_text_dest_x[0], -472, FONT_NORMAL, -2, 0, 0,0 );

//	shell->lolly_text_dest_x[0] = 512 - (ts->x2 - ts->x1) - (32*2);



	run_title_lollies();
}


void run_title_lollies()
{
Textman_struct *ts;
Sprite *shad;

	shad = (Sprite *)shell->logo->prev;


	if ( shell->mode == FRONTEND )
		{
		shell->logo->display = YES;
		shell->logo->x.w.hi = -480;
		shell->logo->y.w.hi = -500;
		shell->bot_bar[0]->display =
		shell->bot_bar[1]->display =
		shell->bot_bar[2]->display = 1-shell->requester_box_active;
		shell->logo->frame = 1;

		shad->display= NO;
		}
	else
		{
		shell->logo->display = YES;
		shell->bot_bar[0]->display=
		shell->bot_bar[1]->display=
		shell->bot_bar[2]->display=NO;
		shell->logo->frame = 0;
		shell->logo->x.w.hi = -(256+208);
		shell->logo->y.w.hi = -(350+(22*4));

		shad->x.w.hi = -256-(4*2)-12-208;
		shad->y.w.hi = -350+(4*4)-(0*4)+(16)-(22*4);
		shad->display = YES;
		shad->transp_rate = 2;
		}



	if ( shell->mode != FRONTEND )
		shell->lolly[0]->display = NO;
	else
		{
		shell->lolly[0]->display = YES;
		ts=add_text_string ( tslot_poses[LOLLY_TSLOT]+0 , &shell->lolly_text_buffer[1][0] , TITLE_FONT, milford_font_logic, -60, -472, FONT_RJUST, -2, 0, 0,0 );
		ts->depth = NEAREST_DEPTH+0;
		ts=add_text_string ( tslot_poses[LOLLY_TSLOT]+1 , &shell->lolly_text_buffer[1][0] , TITLE_FONT, milford_font_logic_sub, -60-6, -472+12, FONT_RJUST, -2, 0, 0,0 );
		ts->depth = NEAREST_DEPTH+1;
		}


	if ( shell->lolly_mode == LOLLY_INIT )
		{
		if ( shell->lolly[1]->brightness < 40 )
			{
			shell->lolly[1]->brightness+=4;
			if ( shell->lolly[1]->brightness > 40 )
				shell->lolly_mode = LOLLY_ACTIVE;
			}
		}


	if ( shell->lolly_mode == LOLLY_DYING )
		{
		if ( shell->lolly[1]->brightness > 0 )
			{
			shell->lolly[1]->brightness-=4;
			if ( shell->lolly[1]->brightness < 0 )
				shell->lolly_mode = LOLLY_INACTIVE;
			}
		}
}



WORD find_shell_prev_button()
{
WORD *but=shell->types_list;
WORD found_it=NO;
WORD store;

	store = shell->current_item;

	shell->current_item=256;
	while( *but )
		{
		if ( *but == SHELL_PREV )
			{
			found_it = YES;
			break;
			}
		else
			{
			shell->current_item++;
			but++;
			}
		}

	if ( found_it == NO )
		{
		shell->current_item = store; /* didnt find PREV button */
		}
	return found_it;
}

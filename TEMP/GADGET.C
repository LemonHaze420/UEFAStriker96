/****************************************************
 *																	 *
 *	 Gadgets														 *
 *																	 *
 ****************************************************/

#include "incs.h"

#ifdef PSX
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include	"shell.h"
#include	"font.h"
#include "datafile.h"
#include "hardware.h"
#include "text.h"
#include	"mem.h"
#include	"fvars.h"
#include	"pad.h"
#include "..\..\global\image.h"
#include "psxmacro.h"
#include "anim.def"
#endif




/****************************************************
 *																	 *
 *	 Defines														 *
 *																	 *
 ****************************************************/

#define GADGET_OFF		0
#define GADGET_INIT		1
#define GADGET_UPDATE	2
#define GADGET_KILL		3



//#define SLIDER_SPR_LEN (252)
#define SLIDER_MIN (4*65536)
#define SLIDER_MAX (254*65536)

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


void spawn_rdb_gadget( Gadget *gadget );
void update_rdb_gadget( Gadget *gadget );
void update_slider_gadget( Gadget *gadget );


void process_gadgets();
void refresh_gadget( Gadget *gadget );

void update_slider_gadget( Gadget *gadget );


Gadget *find_free_gadget();

/****************************************************
 *																	 *
 *  Vars															 *
 *																	 *
 ****************************************************/

Gadget_header gadget_header;
Gadget *gadget;

/****************************************************
 *																	 *
 *  Functions													 *
 *																	 *
 ****************************************************/



void init_gadgets( WORD num_gadgets )
{
WORD cnt;
Gadget *gadget;

	shell->requester_box_active = NO;
	add_process ( process_gadgets );

	gadget_header.gadget_list = allocate_mem ( 0, sizeof ( Gadget ) * num_gadgets);
	gadget_header.total_gadgets = num_gadgets;

	gadget = gadget_header.gadget_list;

	for ( cnt=0; cnt<num_gadgets; cnt++ )
		{
		gadget->mode = GADGET_OFF;
		gadget++;
		}
}





void process_gadgets()
{
Gadget *gadget = gadget_header.gadget_list;
WORD upd,cnt;

	gadget_header.gadgets_active=0;

	for ( cnt=0; cnt< gadget_header.total_gadgets; cnt++ )
		{
		if ( gadget->mode )
			{
			gadget_header.gadgets_active++;


			if ( gadget->upd_rtn )
	  			(*gadget->upd_rtn)( gadget );	//execute gadget update func


			}
		gadget++;
		}

//	FntPrint("gadgets active=%d\n",gadget_header.gadgets_active );
}


Gadget *find_free_gadget()
{

Gadget *gadget = gadget_header.gadget_list;

	while ( gadget->mode != GADGET_OFF )
		{
		gadget++;
		}

	return gadget;
	

}



void kill_gadget ( Gadget *gadget )
{
	gadget->mode = GADGET_KILL;
	gadget->sequence_num=0;
}







/**************************************************************************
 *																								  *
 *																								  *
 * Gadgets																					  *
 * sliderbar, textwheel, dialog box etc...										  *
 *																								  *
 *																								  *
 **************************************************************************/



ULONG set_slider_val( WORD in_val, WORD in_min, WORD in_max )
{
ULONG slider_steps = SLIDER_MAX-SLIDER_MIN;

	return in_val * 65536;

}

Gadget *get_gadget( WORD gad_num )
{
	return ( gadget_header.gadget_list + gad_num );
}



Gadget *set_slider_gadget( WORD x, WORD y, Slider_gadget *slider, WORD in_min, WORD in_max, UBYTE *slider_val_addr, WORD group_num )
{
LONG mod_val;


/*** Don't set an in_max greater than 330 ***/

	gadget = find_free_gadget();
	gadget->slider = slider;

	gadget->mode = GADGET_INIT;


	slider->gadget = gadget;
	slider->x = x;
	slider->y = y;
	slider->in_min = in_min;
	slider->in_max = in_max;
	slider->val_addr = slider_val_addr;

	mod_val = *slider_val_addr;

	//printf("input val=%d\n",mod_val);

/*** Convert inputed variable into Slider var ***/
	gadget->slider->val.l = set_slider_val( mod_val, in_min, in_max );


//*** Spawn scaling bar slider ***
	gadget->slider->slide_spr = spawn_sprite ( ICON );
	sprite->frame = 40;
	sprite->depth = NEAREST_DEPTH+10;
	sprite->centre_if_scaled = NO; // dont auto centre when scaling

//*** Spawn left arrow ***
	gadget->slider->lt_arrow = spawn_sprite ( ICON );
	sprite->frame = 39;
	sprite->depth = NEAREST_DEPTH+10;

//*** Spawn right arrow ***
	gadget->slider->rt_arrow = spawn_sprite ( ICON );
	sprite->frame = 41;
	sprite->depth = NEAREST_DEPTH+9;


	gadget->slider->group = group_num;

	gadget->upd_rtn = (void *)&update_slider_gadget;
	gadget->type = SLIDER_GADGET;
	gadget->mode = GADGET_UPDATE;



}

void update_slider_gadget( Gadget *gadget )
{
Slider_gadget *slider;
WORD col;
WORD slide_val;
WORD spd;
WORD *val;
static Pos vel;
WORD upd;


	slider = gadget->slider;

	slider->slide_spr->x.w.hi = slider->x;
	slider->slide_spr->y.w.hi = slider->y;
	slider->slide_spr->xscale = ((slider->val.l+0)>>16)*20;

	//FntPrint("scale=%d\n",slider->slide_spr->xscale );

	slider->lt_arrow->x.w.hi = slider->x - 20;
	slider->lt_arrow->y.w.hi = slider->y;

	slider->rt_arrow->x.w.hi = (slider->x + ( slider->val.l / (0x10000000/(20*16*20) ) )) - 4;
	slider->rt_arrow->y.w.hi = slider->y;

	slider->lt_arrow->frame =  slider->base_frame;
	slider->slide_spr->frame = slider->base_frame+1;
	slider->rt_arrow->frame =  slider->base_frame+2;

	upd=NO;

	if ( shell->current_item == slider->group )
		{
		if ( ((flash_counter/8)&1)==0 )
			{
			slider->lt_arrow->frame += 3;
			slider->rt_arrow->frame += 3;
			slider->slide_spr->frame += 3;
			}

		if ( !(cjoy->c & (PAD_LT|PAD_RT)) )
			{
			vel.l=220000;
			}
		else
			{
			if ( vel.l < 600000 )
				vel.l += 40000; 
			}

		vel.l /= 256;
		vel.l *= 256;


		if ( cjoy->c & PAD_LT )
			{
			slider->val.l-=vel.l;
			if ( slider->val.l < SLIDER_MIN )
				slider->val.l = SLIDER_MIN;	
			upd=YES;

			}

		if ( cjoy->c & PAD_RT )
			{
			slider->val.l+=vel.l;

			if ( slider->val.l > SLIDER_MAX )
				slider->val.l = SLIDER_MAX;
			upd=YES;
			}


		}
	else
		col = 128;


	upd=YES;
	if ( upd )
		{
		*slider->val_addr = slider->val.w.hi; //get_slider_val( slider->val.l, slider->in_min, slider->in_max );

//		FntPrint("val=%d\n",slider->val.l );
		}


	if ( gadget->mode == GADGET_KILL )
		{


		//*** get gadget value and write back to chosen variable addr ***
		*slider->val_addr = slider->val.w.hi; //get_slider_val( slider->val.l, slider->in_min, slider->in_max );


//		printf("output val=%d\n",*slider->val_addr);

		}



}






void rdb_sprite_upd( Sprite *sprite )
{					 
Rdb_gadget *rdb = sprite->gadget->rdb_gadget;
WORD box_x1, box_x2, box_y1, box_y2;

WORD x1,y1,x2,y2;

	if ( sprite->mode == GADGET_KILL )
		{
		kill_sprite( sprite );
		return;
		}


	x1 = rdb->x1;
	x2 = rdb->x2;
	y1 = rdb->y1;
	y2 = rdb->y2;


	sprite->x.w.hi = x1; // + (4*4);
	sprite->y.w.hi = y1; // + (4*4);

	sprite->untextured_frame.w = x2-x1; //(x2-x1-32);
	sprite->untextured_frame.h = y2-y1; //((y2-y1-(rdb->bot_h*2))/1);
}



Gadget *set_rdb_gadget( Gadget *parent, Rdb_gadget *rdb, WORD x, WORD y, WORD w, WORD h, WORD group )
{
Textman_struct *ts;
Gadget *gadget;

	gadget = find_free_gadget();
	rdb->gadget = gadget;

	gadget->rdb_gadget = rdb;
	
	rdb->group = group;
	rdb->old_size.x = 
	rdb->old_size.y = 
	rdb->old_size.w = 
	rdb->old_size.h = -1;
//	rdb->xoffs = rdb->yoffs = rdb->tint_offs = 0;
	rdb->max_tint = 150;	// default box brightness / sub level
	rdb->flash_tint_default = 128;
	rdb->flash_tint_max = 255;
	rdb->depth = MIDDLE_DEPTH;

	gadget->parent = parent;

	if ( x != -1 )
		rdb->size.x = x;

	if ( y != -1 )
		rdb->size.y = y;

	if ( w != -1 )
		rdb->size.w = w;

	if ( h != -1 )
		rdb->size.h = h;


	spawn_rdb_gadget( gadget );


	return gadget;
}





void spawn_rdb_gadget( Gadget *gadget )
{
WORD quad_w, quad_h;
WORD box_x1,box_x2,box_y1,box_y2;
Spr_anim_frame_san *fr;	
Sprite *quad;
Gadget *parent = gadget->parent;
Rdb_gadget *rdb = gadget->rdb_gadget;
Textman_struct *ts;
WORD transp_rate;
WORD shad_transp;


// box is positioned so that
// 0,0 = centre point in box
// and 0,0 is centre of screen

	gadget->upd_rtn = (void *)&update_rdb_gadget;
	gadget->type = RDB_GADGET;

	transp_rate = 0;

	rdb->tint = 0;			// subtract amount = 0
	gadget->mode = GADGET_INIT;	// start fade up
	rdb->transp_rate = transp_rate;


//** centre **
	spawn_sprite( -1 );	//BOX
	rdb->cen = sprite;
	sprite->centre_if_scaled=NO;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->gadget = gadget;
	sprite->mode = GADGET_INIT;
	sprite->transp_rate = 2;
	sprite->depth = MIDDLE_DEPTH;
	sprite->gouraud = YES;
	sprite->untextured_frame.w = 128;
	sprite->untextured_frame.h = 128;

	sprite->r0 = sprite->g0 = sprite->b0 = 
	sprite->r1 = sprite->g1 = sprite->b1 = 
	sprite->r2 = sprite->g2 = sprite->b2 = 
	sprite->r3 = sprite->g3 = sprite->b3 = 50;
//	sprite->xoffs = 512;
//	sprite->yoffs = 512;




	rdb->x1 = rdb->size.x;
	rdb->y1 = rdb->size.y-4;
	rdb->x2 = rdb->x1 + rdb->size.w;
	rdb->y2 = rdb->y1 + rdb->size.h+8;
	rdb->grow_size.x = -850;
	rdb->grow_size.y = rdb->size.y;
	rdb->grow_size.w = rdb->size.w; 
	rdb->grow_size.h = rdb->size.h;
	rdb->grow_cnt=2;

}







void update_rdb_gadget( Gadget *gadget )
{
Rdb_gadget *rdb = gadget->rdb_gadget; //[num];
Textman_struct *ts;
Gadget *new_gadget, *gad_store;
WORD box_x1,box_x2,box_y1,box_y2, type, quad_w, quad_h;
Spr_anim_frame_san *fr;	
Sprite *sprite2, *quad;
WORD tint;


	if ( gadget->mode == GADGET_INIT )
		{
		#if 0
		rdb->tint += 5;
		if ( rdb->tint >= rdb->max_tint )
			{
			rdb->tint = rdb->max_tint;
			gadget->mode = GADGET_UPDATE;
			}
		#endif

		rdb->cen->depth = rdb->depth;


//		rdb->grow_size.x += rdb->grow_cnt;

//		if ( rdb->grow_size.x > rdb->size.x )
//			rdb->grow_size.x = rdb->size.x;


//		rdb->grow_cnt = (rdb->size.x - -850) / 10;
		
		}



	if ( rdb->update==YES ) 
		rdb->update=NO;

//	if ( (rdb->size.x != rdb->old_size.x) || (rdb->size.y != rdb->old_size.y) || (rdb->size.w != rdb->old_size.w) || (rdb->size.h != rdb->old_size.h) )
//		{
//		rdb->old_size = rdb->size;
//		rdb->update = YES;
//		}

//	FntPrint("xy1=%d %d  xy2=%d %d\n",rdb->x1, rdb->y1, rdb->x2, rdb->y2 );
	


//	rdb->x1 = rdb->grow_size.x;
//	rdb->y1 = rdb->grow_size.y-4;
//	rdb->x2 = (rdb->x1 + rdb->grow_size.w);
//	rdb->y2 = rdb->y1 + (((rdb->grow_size.h+8)|3)+1);

	rdb->x1 = rdb->size.x;
	rdb->y1 = rdb->size.y-4;
	rdb->x2 = (rdb->x1 + rdb->size.w);
	rdb->y2 = rdb->y1 + (((rdb->size.h+8)|3)+1);


	rdb->x1 = (rdb->x1 /2) *2;
	rdb->y1 = (rdb->y1 /4) *4;
	rdb->x2 = (rdb->x2 /2) *2;
	rdb->y2 = (rdb->y2 /4) *4;


	if ( rdb->group == shell->current_item && (((flash_counter/8)&1)==0) )
		{
		tint = rdb->flash_tint_max;
		}
	else
		tint = rdb->flash_tint_default;


#if 0
	if ( requester_box_active == YES )
		{
		if ( rdb->dim_if_requester_box_active == YES )
			tint /= 2;
		}
#endif




}





void kill_rdb_gadget( Gadget *gad )
{
Rdb_gadget *rdb = gad->rdb_gadget;


	gad->mode = GADGET_OFF;

	rdb->cen->mode = GADGET_KILL;


	rdb->cen->display = 0;

	return;


}

void kill_slider_gadget( Gadget *gad )
{
	gad->mode = GADGET_OFF;
	kill_sprite( gad->slider->slide_spr );
	kill_sprite( gad->slider->lt_arrow );
	kill_sprite( gad->slider->rt_arrow );
}					 



/**************************************************************************/



WORD modify_word( WORD *word_addr, WORD min, WORD max )
{
WORD upd=NO;

	if ( cjoy->r & PAD_X )
		{
		sfxSpot( BUTVOC, 30 );
		(*word_addr)++;
		upd= YES;
		}

	if ( cjoy->r & PAD_SQU )
		{
		sfxSpot( BUTVOC, 30 );
		(*word_addr)--;
		upd= YES;
		}

	if ( *word_addr > max )
		*word_addr = min;
	else
		{
		if ( *word_addr < min )	
			*word_addr = max;
		}


	return upd;

}

WORD modify_byte( BYTE *word_addr, WORD min, WORD max )
{
WORD upd=NO;


	if ( cjoy->r & PAD_X )
		{
		sfxSpot( BUTVOC, 30 );
		(*word_addr)++;
		upd=YES;
		}

	if ( cjoy->r & PAD_SQU )
		{
		sfxSpot( BUTVOC, 30 );
		(*word_addr)--;
		upd=YES;
		}

	if ( *word_addr > max )
		*word_addr = min;
	else
		{
		if ( *word_addr < min )	
			*word_addr = max;
		}

	return upd;

}


WORD modify_word_ud( WORD *word_addr, WORD min, WORD max )
{
WORD upd=NO;
	if ( cjoy->r & PAD_DN )
		{
		sfxSpot( BUTVOC, 30 );
		(*word_addr)++;
		upd=YES;

		}

	if ( cjoy->r & PAD_UP )
		{
		sfxSpot( BUTVOC, 30 );
		(*word_addr)--;
		upd=YES;
		}

	if ( *word_addr > max )
		*word_addr = min;
	else
		{
		if ( *word_addr < min )	
			*word_addr = max;
		}
	return upd;
}





/***********************************************************************/







/*
	I'm going to tear off you're manhoods
   and throw them on the dessert flour
   for aunts to eat.
*/


Rdb_gadget message_requester_rdb;




WORD do_request( BYTE *request_text, WORD *request_button_list, WORD ypos, WORD default_button )
{
Textman_struct *ts;
WORD button_pressed=NO;

WORD rdb_gadget_set=NO;
WORD cnt, text_num;
BYTE *text;
WORD store[5];
WORD buttons;
Rdb_gadget *rdb;
Gadget *gadget;

WORD info_box_y = ypos;
WORD info_box_h = 16*8;	// this is the size of border around the standard box
Rdb_gadget *info_box_rdb;
WORD ht,y,hv, wd,x,wv;
Spr_anim_frame_san *fr;
WORD icon_wd;
WORD *waddr;

	read_pads();
	buttons=0;

/*** Store shell info before changing it ***/
 	store[0] = shell->current_item;
	store[1] = shell->master_option_active;
	store[2] = shell->button_dest_x;
	store[3] = shell->button_dest_y;
	store[4] = shell->module_abort;

	shell->master_option_active = YES;
	shell->current_item=0;


/** Count number of buttons **/
	shell->total_master_options=0;
	waddr=request_button_list;
	while( (*waddr) )
		{
		shell->total_master_options++;
		waddr++;
		}

	fr = get_sprite_frame_addr( ICON );
	icon_wd = (fr->real_w+8)*2;

	shell->button_dest_x = -(icon_wd*(2-shell->total_master_options))/2;
	set_shell_bottom_buttons( request_button_list, default_button);	





	shell->requester_box_active = YES;

/** Set item to default item **/
	while(1)
		{
		if ( request_button_list[ shell->current_item ] == default_button )
			break;
		shell->current_item++;
		}

/**** Info box *****/
	gadget = set_rdb_gadget( 0, &message_requester_rdb, 0, 0,0,0,  -1 );
	info_box_rdb = gadget->rdb_gadget;
	info_box_rdb->depth = MIDDLE_DEPTH - 10;





	ts = add_text_string ( tslot_poses[REQUEST_TSLOT]+0, request_text , MILFORD_FONT, milford_font_logic, 0, info_box_y, FONT_XC+FONT_YC, -2, 0, 0,0 );


	shell->current_button=0;
	while( shell->current_button == 0 )
		{
		ts = add_text_string ( tslot_poses[REQUEST_TSLOT]+0, request_text , MILFORD_FONT, milford_font_logic, 0, info_box_y, FONT_XC, -2, 0, 0,0 );
		set_rdb_size_from_tm( info_box_rdb, ts );
		info_box_rdb->size.y -= (info_box_h/2);
		info_box_rdb->size.h += (info_box_h/1);

		if ( shell->buttons_dropping == NO )
			shell->button_dest_y = ts->y2 + (12*4);

		bg_loop();
		shell_control();

		}

	ht = message_requester_rdb.size.h;
	wd = message_requester_rdb.size.w;
	x = message_requester_rdb.size.x;
	y = message_requester_rdb.size.y;

	hv = (ht-48)/8;
	wv = (wd-48)/8;

	shell->drop_buttons = YES;

	for(cnt=0;cnt<8;cnt++)
		{
		ht -= hv;
		y += hv/2;

		wd -= wv;
		x += wv/2;

		message_requester_rdb.size.h = ht;
		message_requester_rdb.size.y = y;
		message_requester_rdb.size.w = wd;
		message_requester_rdb.size.x = x;

		bg_loop();

		shell_control();

		}

	kill_rdb_gadget( message_requester_rdb.gadget );
	bg_loop();
	bg_loop();

	read_pads();

 	shell->current_item = store[0];
	shell->master_option_active = store[1]; 
	shell->button_dest_x = store[2];
	shell->button_dest_y = store[3];
	shell->requester_box_active = NO;
	shell->module_abort = store[4];

	return shell->current_button;
}



void set_rdb_box( Rdb_gadget *rdb, WORD x, WORD y, WORD w, WORD h )
{
	if ( x!=-1)
		rdb->size.x = x;
	if ( y!=-1)
		rdb->size.y = y;
	if ( w!=-1)
		rdb->size.w = w;
	if ( h!=-1)
		rdb->size.h = h;
}




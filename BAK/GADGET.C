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

#define RDB_TL				0
#define RDB_TR				1
#define RDB_BL				2
#define RDB_BR				3
#define RDB_LT				4
#define RDB_RT				5
#define RDB_TOP			6
#define RDB_BOT			7
#define RDB_CEN			8
#define RDB_SHAD_TR		9
#define RDB_SHAD_BR		10
#define RDB_SHAD_BL		11
#define RDB_SHAD_BOT		12
#define RDB_SHAD_RT		13
#define RDB_SHAD_FILLER	14

#define LOLLY_INACTIVE	0
#define LOLLY_ACTIVE		1


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


//void spawn_textstring_gadget( Gadget *gadget );
void spawn_rdb_gadget( Gadget *gadget );
void spawn_tw_gadget( Gadget *gadget );

void update_rdb_gadget( Gadget *gadget );
void update_text_wheel_gadget( Gadget *gadget );
//void update_textstring_gadget( Gadget *gadget );
void update_slider_gadget( Gadget *gadget );


void process_gadgets();
void refresh_gadget( Gadget *gadget );

void init_textline_gad ( Gadget *gadget );
void upd_textline_gad( Gadget *gadget );

//void process_dialog_box_gadget( Gadget *gadget );
//void process_rdb( Gadget *gadget, WORD num );
void process_text_wheel( Gadget *gadget );

void update_slider_gadget( Gadget *gadget );


Gadget *find_free_gadget();

/****************************************************
 *																	 *
 *  Vars															 *
 *																	 *
 ****************************************************/

Gadget_header gadget_header;
Gadget *gadget;
WORD requester_box_active;

/****************************************************
 *																	 *
 *  Functions													 *
 *																	 *
 ****************************************************/



void init_gadgets( WORD num_gadgets )
{
WORD cnt;
Gadget *gadget;

	requester_box_active = NO;
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

//			if ( gadget->text_wheel.mode )
//				process_text_wheel( gadget );

				

			//if ( gadget->dialog_box.mode )
			//	process_dialog_box( gadget );

			//if ( gadget->rounded_dialog_box[0].mode )
			//	process_rounded_dialog_box( gadget,0 );
			//if ( gadget->rounded_dialog_box[1].mode )
			//	process_rounded_dialog_box( gadget,1 );

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

//	return( ((slider_steps * (in_val-in_min)) / (in_max-in_min)) + SLIDER_MIN );

	return in_val * 65536;

}

#if 0
ULONG get_slider_val( LONG slider_val, LONG mod_min, LONG mod_max )
{
ULONG val;

	val = slider_val - SLIDER_MIN;
	val = val * 256; //(mod_max-mod_min );
	val = val / (SLIDER_MAX-SLIDER_MIN);
	val += mod_min;

	//printf("get, val=%d  min=%d max=%d\n",val, mod_min, mod_max);
	return val;
}
#endif

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

	slider->x = x;
	slider->y = y;
	slider->in_min = in_min;
	slider->in_max = in_max;
	slider->val_addr = slider_val_addr;

	mod_val = *slider_val_addr;

	//printf("input val=%d\n",mod_val);

/*** Convert inputed variable into Slider var ***/
	gadget->slider->val.l = set_slider_val( mod_val, in_min, in_max );


//*** Spawn transparent bar under slider ***
	gadget->slider->bar_spr = spawn_sprite ( SLIDER );
	sprite->frame = 1;
	sprite->depth = MIDDLE_DEPTH;
	sprite->transp_rate = 0;
	sprite->tint.r = 
	sprite->tint.g = 
	sprite->tint.b = 32;

//*** Spawn scaling bar slider ***
	gadget->slider->slide_spr = spawn_sprite ( SLIDER );
	sprite->frame = 0;
	sprite->depth = MIDDLE_DEPTH-1;
	sprite->centre_if_scaled = NO; // dont auto centre when scaling

//*** Spawn left arrow ***
	gadget->slider->lt_arrow = spawn_sprite ( SLIDER );
	sprite->frame = 2;
	sprite->depth = MIDDLE_DEPTH;
	sprite->transp_rate = 1;

//*** Spawn right arrow ***
	gadget->slider->rt_arrow = spawn_sprite ( SLIDER );
	sprite->frame = 3;
	sprite->depth = MIDDLE_DEPTH;
	sprite->transp_rate = 1;




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

	slider->bar_spr->x.w.hi = slider->x;
	slider->bar_spr->y.w.hi = slider->y;

	slider->slide_spr->x.w.hi = slider->x;
	slider->slide_spr->y.w.hi = slider->y;
	slider->slide_spr->xscale = (slider->val.l+0)>>16;

	//FntPrint("scale=%d\n",slider->slide_spr->xscale );

	slider->lt_arrow->x.w.hi = slider->x - 40;
	slider->lt_arrow->y.w.hi = slider->y;

	slider->rt_arrow->x.w.hi = slider->x + (236*2);
	slider->rt_arrow->y.w.hi = slider->y;


	upd=NO;

	if ( shell->current_item == slider->group )
		{
		if ( ((flash_counter/8)&1)==0 )
			{
			col = 40;
			}
		else
			{
			col = 128;
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

		FntPrint("val=%d\n",slider->val.l );
		}


	if ( gadget->mode == GADGET_KILL )
		{
		kill_sprite( slider->bar_spr );
		kill_sprite( slider->slide_spr );
		kill_sprite( slider->lt_arrow );
		kill_sprite( slider->rt_arrow );


		//*** get gadget value and write back to chosen variable addr ***
		*slider->val_addr = slider->val.w.hi; //get_slider_val( slider->val.l, slider->in_min, slider->in_max );


//		printf("output val=%d\n",*slider->val_addr);

		}



#if 0
	slider->lt_arrow->tint.r = 
	slider->lt_arrow->tint.g = 
	slider->lt_arrow->tint.b = 
	slider->rt_arrow->tint.r = 
	slider->rt_arrow->tint.g = 
	slider->rt_arrow->tint.b = col;
#endif


}






void rdb_sprite_upd( Sprite *sprite )
{					 
Rdb_gadget *rdb = sprite->gadget->rdb_gadget;
//	FntPrint("t=%d\n",sprite->tint.r);
WORD box_x1, box_x2, box_y1, box_y2;

WORD x1,y1,x2,y2;

//	FntPrint("wh=%d %d\n",rdb->x2-rdb->x1, rdb->y2-rdb->y1 );

	if ( sprite->mode == GADGET_KILL )
		{
		kill_sprite( sprite );
		return;
		}

//	sprite->transp_rate = -1; //rdb->transp_rate;

//	sprite->tint.r = sprite->tint.g = sprite->tint.b = 128; //sprite->temp;


	x1 = rdb->x1;
	x2 = rdb->x2;
	y1 = rdb->y1;
	y2 = rdb->y2;


	switch ( sprite->type2 )
		{
		case RDB_TL:
			{
			sprite->x.w.hi = x1;
			sprite->y.w.hi = y1;
			break;
			}

//** tr quad ***
		case RDB_TR:
			{
			sprite->x.w.hi = x2 - rdb->quad_w;
			sprite->y.w.hi = y1;
			break;
			}


//** bl quad ***
		case RDB_BL:
			{
			sprite->x.w.hi = x1;
			sprite->y.w.hi = y2 - rdb->quad_h;
			break;
			}


//** br quad ***
		case RDB_BR:
			{
			sprite->x.w.hi = x2 - rdb->quad_w;
			sprite->y.w.hi = y2 - rdb->quad_h;
			break;
			}


//*** top ***
		case RDB_TOP:
			{
			sprite->x.w.hi = x1 + rdb->quad_w;
			sprite->y.w.hi = y1;
			sprite->xscale = ((x2-x1-(rdb->quad_w*2)) * 256)/rdb->top_w;
			break;
			}


//*** bot ***
		case RDB_BOT:
			{
			sprite->x.w.hi = x1 + rdb->quad_w;
			sprite->y.w.hi = y2 - rdb->bot_h;
			sprite->xscale = ((x2-x1-(rdb->quad_w*2)) * 256)/rdb->top_w;
			break;
			}


//*** lt ***
		case RDB_LT:
			{
			sprite->x.w.hi = x1;
			sprite->y.w.hi = y1+2 + rdb->quad_h;
			sprite->yscale = ( (y2-y1-(rdb->quad_h*2)) * 256) / rdb->lt_h;
			break;
			}

//*** rt ***
		case RDB_RT:
			{
			sprite->x.w.hi = x2 - rdb->rt_w;
			sprite->y.w.hi = y1 + rdb->quad_h;
			sprite->yscale = ( (y2-y1-(rdb->quad_h*2)) * 256) / rdb->lt_h;
			break;
			}

//*** Centre *****
		case RDB_CEN:
			{
			sprite->x.w.hi = x1 + (4*4);
			sprite->y.w.hi = y1 + (4*4);
//			sprite->xscale = ( (x2-x1-32) * rdb->cen_w ) / 256;
//			sprite->yscale = ( ((y2-y1-(rdb->bot_h*2))/1) * 256 ) / rdb->cen_h;

			sprite->untextured_frame.w = (x2-x1-32);
			sprite->untextured_frame.h = ((y2-y1-(rdb->bot_h*2))/1);
//			sprite->xscale = 256;
//			sprite->yscale = 256;
//			sprite->w = 64;
//			sprite->h = 64;

			break;
			}


//** shad tr quad ***
		case RDB_SHAD_TR:
			{
			sprite->x.w.hi = x2 - rdb->quad_w + (8*2);
			sprite->y.w.hi = y1 + (5*4);
	//		sprite->transp_rate = -1;
	//		sprite->transp_rate = 2;
			break;
			}

//** shad br quad ***
		case RDB_SHAD_BR:
			{
			sprite->x.w.hi = x2 - rdb->quad_w + ( 8*2 );
			sprite->y.w.hi = y2 - rdb->quad_h + ( 5*4 );
	//		sprite->transp_rate = -1;
	//		sprite->transp_rate = 2;
			break;

			}

//** bl quad ***
		case RDB_SHAD_BL:
			{
			sprite->x.w.hi = x1 + ( 4*4 );
			sprite->y.w.hi = y2 - rdb->quad_h + ( 5*4 );
	//		sprite->transp_rate = -1;
	//		sprite->transp_rate = 2;
			break;
			}

		case RDB_SHAD_BOT:
			{
			sprite->x.w.hi = x1 + ( 4*4 ) +  rdb->quad_w;
			sprite->y.w.hi = y2 - rdb->quad_h + ( 5*4);
			sprite->untextured_frame.w = rdb->shad_br->x.w.hi - sprite->x.w.hi;
			sprite->untextured_frame.h = 8*4;
			break;
			}

		case RDB_SHAD_RT:
			{
			sprite->x.w.hi = rdb->shad_tr->x.w.hi;
			sprite->y.w.hi = rdb->shad_tr->y.w.hi + rdb->quad_h + 4;
			sprite->untextured_frame.w = rdb->quad_w; // + (3*2);
			sprite->untextured_frame.h = rdb->shad_br->y.w.hi - (rdb->shad_tr->y.w.hi + rdb->quad_h) - 2;
			break;
			}

		case RDB_SHAD_FILLER:
			{
			sprite->x.w.hi = rdb->shad_br->x.w.hi - (6*2);
			sprite->y.w.hi = rdb->shad_br->y.w.hi - (6*4);
			sprite->untextured_frame.w = 6*2;
			sprite->untextured_frame.h = 6*4;
			break;
			}
		}
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
	rdb->dim_if_requester_box_active = YES;
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




//*** tl quad ***
	spawn_sprite( BOX );
	sprite->frame = 0;
	fr = sprite->spr_anim_frame_san+ sprite->frame;
	rdb->tl = sprite;
	rdb->quad_w = fr->w*2;
	rdb->quad_h = fr->h*4;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->type2 = RDB_TL;
	sprite->gadget = gadget;
	sprite->mode = GADGET_INIT;
	sprite->depth = MIDDLE_DEPTH;
	sprite->transp_rate = transp_rate;
 

//** tr quad ***
	spawn_sprite( BOX );
	rdb->tr = sprite;
	sprite->frame = 2;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->type2 = RDB_TR;
	sprite->gadget = gadget;
	sprite->mode = GADGET_INIT;
	sprite->depth = MIDDLE_DEPTH;
	sprite->transp_rate = transp_rate;



//** bl quad ***
	spawn_sprite( BOX );
	rdb->bl = sprite;
	sprite->frame = 5;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->type2 = RDB_BL;
	sprite->gadget = gadget;
	sprite->mode = GADGET_INIT;
	sprite->depth = MIDDLE_DEPTH;
	sprite->transp_rate = transp_rate;



//** br quad ***
	spawn_sprite( BOX );
	rdb->br = sprite;
	sprite->frame = 7;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->type2 = RDB_BR;
	sprite->gadget = gadget;
	sprite->mode = GADGET_INIT;
	sprite->depth = MIDDLE_DEPTH;
	sprite->transp_rate = transp_rate;


//** top centre ***
	spawn_sprite( BOX );
	sprite->frame = 1;
	fr = sprite->spr_anim_frame_san + sprite->frame;
	rdb->top_w = fr->w*2;
	rdb->top = sprite;
	sprite->centre_if_scaled = NO;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->type2 = RDB_TOP;
	sprite->gadget = gadget;
	sprite->mode = GADGET_INIT;
	sprite->depth = MIDDLE_DEPTH;
	sprite->transp_rate = transp_rate;

//** bot centre ***
	spawn_sprite( BOX );
	rdb->bot = sprite;
	sprite->frame = 6;
	fr = sprite->spr_anim_frame_san + sprite->frame;
	sprite->centre_if_scaled = NO;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->type2 = RDB_BOT;
	sprite->gadget = gadget;
	sprite->mode = GADGET_INIT;
	rdb->bot_h = fr->h*4;
	sprite->depth = MIDDLE_DEPTH;
	sprite->transp_rate = transp_rate;


//** lt ***
	spawn_sprite( BOX );
	sprite->frame = 3;
	fr = sprite->spr_anim_frame_san + sprite->frame;
	rdb->lt = sprite;
	rdb->lt_h = fr->h*4;
	sprite->centre_if_scaled = NO;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->type2 = RDB_LT;
	sprite->gadget = gadget;
	sprite->mode = GADGET_INIT;
	sprite->depth = MIDDLE_DEPTH;
	sprite->transp_rate = transp_rate;

//** rt ***
	spawn_sprite( BOX );
	rdb->rt = sprite;
	sprite->frame = 4;
	fr = sprite->spr_anim_frame_san + sprite->frame;
	rdb->rt_w = fr->w*2;
	sprite->centre_if_scaled = NO;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->type2 = RDB_RT;
	sprite->gadget = gadget;
	sprite->mode = GADGET_INIT;
	sprite->depth = MIDDLE_DEPTH;
	sprite->transp_rate = transp_rate;

//** centre **
	spawn_sprite( -1 );	//BOX
	rdb->cen = sprite;
// 	sprite->frame = 8;
//	fr = sprite->spr_anim_frame_san + sprite->frame;
	sprite->centre_if_scaled=NO;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
//	rdb->cen_w = fr->w*2;
//	rdb->cen_h = fr->h*4;
	sprite->type2 = RDB_CEN;
	sprite->gadget = gadget;
	sprite->mode = GADGET_INIT;
	sprite->transp_rate = transp_rate;
	sprite->depth = MIDDLE_DEPTH;
	sprite->gouraud = YES;
//	sprite->w = 128;
//	sprite->h = 128;

	sprite->untextured_frame.w = 128;
	sprite->untextured_frame.h = 128;

	sprite->r0 = 54; sprite->g0 = 49; sprite->b0 = 118;
	sprite->r1 = 23; sprite->g1 = 21; sprite->b1 = 52;
	sprite->r2 = 36; sprite->g2 = 33; sprite->b2 = 79;
	sprite->r3 = 24; sprite->g3 = 22; sprite->b3 = 53;
//	sprite->xoffs = 512;
//	sprite->yoffs = 512;

//** shad tr quad ***
	spawn_sprite( BOXSHAD );
	shad_transp = 2;
	rdb->shad_tr = sprite;
	sprite->frame = 2;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->type2 = RDB_SHAD_TR;
	sprite->gadget = gadget;
	sprite->mode = GADGET_INIT;
	sprite->depth = MIDDLE_DEPTH+1;
	sprite->transp_rate = shad_transp;
//	sprite->tint.r = sprite->tint.g = sprite->tint.b = 80;

//** shad br quad ***


	spawn_sprite( BOXSHAD );
	rdb->shad_br = sprite;
	sprite->frame = 0;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->type2 = RDB_SHAD_BR;
	sprite->gadget = gadget;
	sprite->mode = GADGET_INIT;
	sprite->depth = MIDDLE_DEPTH+1;
	sprite->transp_rate = shad_transp;
//	sprite->tint.r = sprite->tint.g = sprite->tint.b = 80;

//** shad bl quad ***
	spawn_sprite( BOXSHAD );
	rdb->shad_bl = sprite;
	sprite->frame = 1;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->type2 = RDB_SHAD_BL;
	sprite->gadget = gadget;
	sprite->mode = GADGET_INIT;
	sprite->depth = MIDDLE_DEPTH+1;
	sprite->transp_rate = shad_transp;
//	sprite->tint.r = sprite->tint.g = sprite->tint.b = 80;

//** shad bot ***	 
	spawn_sprite( -1 );
	rdb->shad_bot = sprite;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->type2 = RDB_SHAD_BOT;
	sprite->gadget = gadget;
	sprite->mode = GADGET_INIT;
	sprite->depth = MIDDLE_DEPTH+1;
	sprite->transp_rate = shad_transp;
	sprite->gouraud = YES;
	sprite->r0 = sprite->g0 = sprite->b0 = 
	sprite->r1 = sprite->g1 = sprite->b1 = 255; //180;
	sprite->r2 = sprite->g2 = sprite->b2 = 
	sprite->r3 = sprite->g3 = sprite->b3 = 0;


//** shad rt ***
	spawn_sprite( -1 );
	rdb->shad_rt = sprite;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->type2 = RDB_SHAD_RT;
	sprite->gadget = gadget;
	sprite->mode = GADGET_INIT;
	sprite->depth = MIDDLE_DEPTH+1;
	sprite->transp_rate = shad_transp;
	sprite->gouraud = YES;
	sprite->r0 = sprite->g0 = sprite->b0 = 255;
	sprite->r1 = sprite->g1 = sprite->b1 = 0;
	sprite->r2 = sprite->g2 = sprite->b2 = 255; 
	sprite->r3 = sprite->g3 = sprite->b3 = 0;

//** shad filler ***
	spawn_sprite( -1 );
	rdb->shad_filler = sprite;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->type2 = RDB_SHAD_FILLER;
	sprite->gadget = gadget;
	sprite->mode = GADGET_INIT;
	sprite->depth = MIDDLE_DEPTH+1;
	sprite->transp_rate = -1; //shad_transp;
	sprite->gouraud = NO;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 0; //255;







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

		rdb->cen->depth =
		rdb->tl->depth = 
		rdb->tr->depth = 
		rdb->bl->depth = 
		rdb->br->depth = 
		rdb->lt->depth = 
		rdb->rt->depth = 
		rdb->top->depth =
		rdb->bot->depth = rdb->depth;
		rdb->shad_tr->depth =
		rdb->shad_br->depth =
		rdb->shad_bl->depth =
		rdb->shad_bot->depth =
		rdb->shad_rt->	depth =
		rdb->shad_filler->depth = rdb->depth+1;


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


	if ( requester_box_active == YES )
		{
		if ( rdb->dim_if_requester_box_active == YES )
			tint /= 2;
		}

//	rdb->cen->tint.r = rdb->cen->tint.g = rdb->cen->tint.b = 
	rdb->tl->tint.r = rdb->tl->tint.g = rdb->tl->tint.b = 
	rdb->tr->tint.r = rdb->tr->tint.g = rdb->tr->tint.b = 
	rdb->bl->tint.r = rdb->bl->tint.g = rdb->bl->tint.b = 
	rdb->br->tint.r = rdb->br->tint.g = rdb->br->tint.b = 
	rdb->lt->tint.r = rdb->lt->tint.g = rdb->lt->tint.b = 
	rdb->rt->tint.r = rdb->rt->tint.g = rdb->rt->tint.b = 
	rdb->top->tint.r = rdb->top->tint.g = rdb->top->tint.b = 
	rdb->bot->tint.r = rdb->bot->tint.g = rdb->bot->tint.b = tint+50;

	rdb->cen->r0 = rdb->cen->g0 = 64;
	rdb->cen->b0 = 128;
	rdb->cen->r1 = rdb->cen->g1 = 40;
	rdb->cen->b1 = 80;
	rdb->cen->r2 = rdb->cen->g2 = 65;
	rdb->cen->b2 = 100;
	rdb->cen->r3 = rdb->cen->g3 = 32;
	rdb->cen->b3 = 80;



//	rdb->cen->g0 = rdb->cen->g1 = rdb->cen->g2 = rdb->cen->g3 = 
//	rdb->cen->b0 = rdb->cen->b1 = rdb->cen->b2 = rdb->cen->b3 = 96;

#if 0
	rdb->tl->display = 
	rdb->tr->display = 
	rdb->bl->display = 
	rdb->br->display = 
	rdb->lt->display = 
	rdb->rt->display = 
	rdb->top->display = 
	rdb->bot->display = NO;
#endif

#if 1
	rdb->shad_tr->display =
	rdb->shad_br->display =
	rdb->shad_bl->display =
	rdb->shad_bot->display =
	rdb->shad_rt->	display =
	rdb->shad_filler->display = NO;
#endif
}





void kill_rdb_gadget( Gadget *gad )
{
Rdb_gadget *rdb = gad->rdb_gadget;


	gad->mode = GADGET_OFF;
	rdb->tl->mode = GADGET_KILL;
	rdb->tr->mode = GADGET_KILL;
	rdb->bl->mode = GADGET_KILL;
	rdb->br->mode = GADGET_KILL;
	rdb->lt->mode = GADGET_KILL;
	rdb->rt->mode = GADGET_KILL;
	rdb->top->mode = GADGET_KILL;
	rdb->bot->mode = GADGET_KILL;
	rdb->cen->mode = GADGET_KILL;
	rdb->shad_tr->mode = GADGET_KILL;
	rdb->shad_br->mode = GADGET_KILL;
	rdb->shad_bl->mode = GADGET_KILL;
	rdb->shad_bot->mode = GADGET_KILL;
	rdb->shad_rt->mode = GADGET_KILL;
	rdb->shad_filler->mode = GADGET_KILL;

#if 1
	rdb->tl->display = 0;
	rdb->tr->display = 0;
	rdb->bl->display = 0;
	rdb->br->display = 0;
	rdb->lt->display = 0;
	rdb->rt->display = 0;
	rdb->top->display = 0;
	rdb->bot->display = 0;
	rdb->cen->display = 0;
	rdb->shad_bl->display = 0;
	rdb->shad_br->display = 0;
	rdb->shad_tr->display = 0;
	rdb->shad_bot->display = 0;
	rdb->shad_rt->display = 0;
	rdb->shad_filler->display = 0;
#endif

	return;


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



void spawn_title_lollies();
void reset_title_lollies();
void run_title_lollies();


Sprite *lolly[9];
BYTE lolly_text_buffer[2][128];
WORD lolly_mode;

void set_title_lolly( BYTE *text1, BYTE *text2 )
{
	strcpy( &lolly_text_buffer[0][0], text1 );

	if ( text2 )
		strcpy( &lolly_text_buffer[1][0], text2 );
	else
		lolly_text_buffer[1][0] = 0;

	lolly_mode = LOLLY_ACTIVE;
	reset_title_lollies();
}

void spawn_title_lollies()
{
#if 1
	lolly_mode = LOLLY_INACTIVE;
	spawn_sprite( TITBAR );
	sprite->frame = 0;
	lolly[0] = sprite;
	sprite->depth = MIDDLE_DEPTH;

	spawn_sprite( TITBAR );
	sprite->frame = 1;
	lolly[1] = sprite;
	sprite->depth = MIDDLE_DEPTH;
	sprite->xscale = 256+128+64+128+512;
	sprite->centre_if_scaled = NO;

	spawn_sprite( TITBAR );
	sprite->frame = 2;
	lolly[2] = sprite;
	sprite->depth = MIDDLE_DEPTH;
//	sprite->xscale = 256+128+64+128+512;

	spawn_sprite( TITBAR );
	sprite->frame = 1;
	lolly[5] = sprite;
	sprite->depth = MIDDLE_DEPTH;
	sprite->xscale = 256+128+64+128+512;
	sprite->centre_if_scaled = NO;

	spawn_sprite( TITSHAD );
	sprite->frame = 0;
	lolly[3] = sprite;
	sprite->depth = MIDDLE_DEPTH+1;
 	sprite->transp_rate = 2;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 100;
	sprite->centre_if_scaled = NO;

	spawn_sprite( TITSHAD );
	sprite->frame = 1;
	lolly[6] = sprite;
	sprite->depth = MIDDLE_DEPTH+1;
 	sprite->transp_rate = 2;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 100;
	sprite->xscale = 4096;
	sprite->centre_if_scaled = NO;

	spawn_sprite( TITSHAD );
	sprite->frame = 2;
	lolly[4] = sprite;
	sprite->depth = MIDDLE_DEPTH+1;
 	sprite->transp_rate = 2;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 60;
	sprite->xflip = YES;

	spawn_sprite( TITSHAD );
	sprite->frame = 1;
	lolly[8] = sprite;
	sprite->depth = MIDDLE_DEPTH+1;
 	sprite->transp_rate = 2;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 60;
	sprite->xflip = YES;
	sprite->xscale = 4096;

	reset_title_lollies();

#endif
	add_process( run_title_lollies );

	run_title_lollies();
}

void reset_title_lollies()
{
Textman_struct *ts;
WORD wd;

	lolly[0]->xvel1.w.hi = 240;
	lolly[2]->xvel1.w.hi = 240;


	ts=add_text_string ( tslot_poses[LOLLY_TSLOT]+0 , &lolly_text_buffer[0][0], LOLLY_FONT, milford_font_logic,  512, 0, FONT_NORMAL, -2, 0, 10,0 );
	wd = ts->x2-ts->x1 ;
	lolly[0]->x.w.hi =  1024+256+128-wd; //474*2;


	if ( lolly_text_buffer[1][0] != 0 )
		{
		ts=add_text_string ( tslot_poses[LOLLY_TSLOT]+1 , &lolly_text_buffer[1][0], LOLLY_FONT, milford_font_logic,  512, 0, FONT_NORMAL, -2, 0, 10,0 );
		wd = ts->x2-ts->x1 ;
		}
	else
		wd = 172*2;

	lolly[2]->x.w.hi =  (-420*2) + wd - 512 - 256 - 128 - 32;

	lolly[0]->y.w.hi = -115*4;
	lolly[1]->y.w.hi = -115*4;

	lolly[2]->y.w.hi =   80*4;
	lolly[5]->y.w.hi =   80*4;

	run_title_lollies();
}


void run_title_lollies()
{
Textman_struct *ts1;
Textman_struct *ts2;
WORD lx;


WORD wd;

	if ( lolly_mode == LOLLY_ACTIVE )
		{
		}


	lolly[0]->x.w.hi -= lolly[0]->xvel1.w.hi;
	lolly[2]->x.w.hi += lolly[2]->xvel1.w.hi;


	lolly[1]->x.w.hi = lolly[0]->x.w.hi + 254*2;
	lolly[5]->x.w.hi = lolly[2]->x.w.hi - 186*2;


//shadow sprite for top lolly
	lolly[3]->x.w.hi = lolly[0]->x.w.hi+(9*2);
	lolly[6]->x.w.hi = lolly[0]->x.w.hi+(9*2) + (16*2);
	lolly[3]->y.w.hi = lolly[0]->y.w.hi+(8*4);
	lolly[6]->y.w.hi = lolly[3]->y.w.hi;

//shadow sprite for bottom lolly
	lolly[4]->x.w.hi = lolly[2]->x.w.hi+256+64+32;
	lolly[8]->x.w.hi = lolly[2]->x.w.hi-256-(16*2)+64+32;
	lolly[4]->y.w.hi = lolly[2]->y.w.hi+(8*4); // - 128;
	lolly[8]->y.w.hi = lolly[4]->y.w.hi;

//	FntPrint("x=%d xvel=%d\n",lolly[0]->x.w.hi, lolly[0]->xvel1.w.hi);


//** Print text that appears on lolly stick **

	if ( lolly_mode == LOLLY_ACTIVE )
		{
		/** Top lolly text **/
		lx = lolly[0]->x.w.hi & -(1+1);
		ts1 = add_text_string ( tslot_poses[LOLLY_TSLOT]+0 , &lolly_text_buffer[0][0], LOLLY_FONT, milford_font_logic,     lx+16, -117*4, FONT_NORMAL, -2, 0, 0,0 );
		ts1->dim_if_requester_box_active = NO;
		ts1->depth = NEAREST_DEPTH+2;
		ts2 = add_text_string ( tslot_poses[LOLLY_TSLOT]+1 , &lolly_text_buffer[0][0], LOLLY_FONT, milford_font_logic_sub, lx+20, -115*4, FONT_NORMAL, -2, 0, 0,0 );
		ts2->dim_if_requester_box_active = NO;
		ts2->max_brightness = 48;
		ts2->depth = NEAREST_DEPTH+3;	// shadow must go behind normal text

		/** Bottom lolly text **/

		if ( lolly_text_buffer[1][0] != 0 ) 
			{
			lx = (lolly[2]->x.w.hi & -(1+1)) - (98*2);
			ts1 = add_text_string ( tslot_poses[LOLLY_TSLOT]+2 , &lolly_text_buffer[1][0], LOLLY_FONT, milford_font_logic,     lx+16, 78*4, FONT_RJUST, -2, 0, 0,0 );
			ts1->dim_if_requester_box_active = NO;
			ts1->depth = NEAREST_DEPTH+2;
			ts2 = add_text_string ( tslot_poses[LOLLY_TSLOT]+3 , &lolly_text_buffer[1][0], LOLLY_FONT, milford_font_logic_sub, lx+20, 80*4, FONT_RJUST, -2, 0, 0,0 );
			ts2->dim_if_requester_box_active = NO;
			ts2->max_brightness = 48;
			ts2->depth = NEAREST_DEPTH+3;	// shadow must go behind normal text
			}

		lolly[0]->display =
		lolly[1]->display =
		lolly[2]->display =		  
		lolly[3]->display =
		lolly[4]->display =
		lolly[5]->display =
		lolly[6]->display =
		lolly[7]->display =
		lolly[8]->display = YES;
		}
	else
		{
		lolly[0]->display =
		lolly[1]->display =
		lolly[2]->display =
		lolly[3]->display =
		lolly[4]->display = 
		lolly[5]->display = 
		lolly[6]->display = 
		lolly[7]->display = 
		lolly[8]->display = NO;
		}


	lolly[0]->xvel1.l -= (lolly[0]->xvel1.l/4);

	if ( lolly[0]->xvel1.w.hi < 0 )
		lolly[0]->xvel1.l = 0;


	lolly[2]->xvel1.l -= (lolly[2]->xvel1.l/4);

	if ( lolly[2]->xvel1.w.hi < 0 )
		lolly[2]->xvel1.l = 0;

	if ( lolly[0]->xvel1.w.hi < 10 )
		{
		shell->lollies_moving=NO;
		}
	else
		shell->lollies_moving=YES;



//	lolly[2]->display = NO;



}






/*
	I'm going to tear off you're manhoods
   and throw them on the dessert flour
   for aunts to eat.
*/


#define MAX_REQUESTER_RDBS 6
Rdb_gadget requester_rdb[MAX_REQUESTER_RDBS];
Rdb_gadget message_requester_rdb;




WORD do_request( BYTE *request_text, WORD *request_button_list, WORD ypos, WORD default_button )
{
Textman_struct *ts;
WORD button_pressed=NO;

WORD rdb_gadget_set=NO;
WORD cnt, text_num;
BYTE *text;
WORD store[2];
WORD buttons;
Rdb_gadget *rdb;
Gadget *gadget;

WORD info_box_y = ypos;
WORD info_box_h = 16*8;	// this is the size of border around the standard box
Rdb_gadget *info_box_rdb;
WORD button_boxes_x, button_boxes_y, button_boxes_w;
WORD ht,y,hv, wd,x,wv;

	read_pads();
	buttons=0;

/*** Store shell info before changing it ***/
 	store[0] = shell->current_item;
	store[1] = shell->master_option_active;


	shell->master_option_active = YES;
	shell->current_item=0;

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
	info_box_rdb->dim_if_requester_box_active = NO;




/******* Draw button texts to determine their widths ******/
	button_boxes_w = 0;
	for(cnt=0;cnt<MAX_REQUESTER_RDBS;cnt++)
		{
		text_num = request_button_list[cnt];
		if ( text_num == 0 )
			{
			buttons = cnt;
			break;
			}
		else
			{
			text = shell_type_word_list[ config->language ][ text_num ];
			ts = add_text_string ( tslot_poses[REQUEST_TSLOT]+cnt, text , MILFORD_FONT, milford_font_logic, 0,512, FONT_NORMAL, -2, 0, 0,0 );
			button_boxes_w += (ts->x2 - ts->x1) + 64;
			}
		}
	button_boxes_w -= 64;


	requester_box_active = YES;

	while( button_pressed == NO )
		{

		ts = add_text_string ( tslot_poses[REQUEST_TSLOT]+4, request_text , MILFORD_FONT, milford_font_logic, 0, info_box_y, FONT_XC, -2, 0, 0,0 );
		ts->dim_if_requester_box_active = NO;
		set_rdb_size_from_tm( info_box_rdb, ts );
		info_box_rdb->size.y -= (info_box_h/2);
		info_box_rdb->size.h += (info_box_h/1);

		button_boxes_y = info_box_rdb->size.y + info_box_rdb->size.h + ( 10*4 );


		button_boxes_x = (-button_boxes_w)/2;

		if ( cjoy->db & PAD_LT )
			{
			if ( shell->current_item != 0 )
				{
				flash_counter=0;
				shell->current_item--;
				}
			}

		if ( cjoy->db & PAD_RT )
			{
			if ( shell->current_item < buttons-1 )
				{
				flash_counter=0;
				shell->current_item++;
				}
			}

		if ( cjoy->db & PAD_X )
			{
			button_pressed = request_button_list[ shell->current_item ];
			}


		for(cnt=0; cnt<buttons; cnt++)
			{
			text_num = request_button_list[cnt];
			text = shell_type_word_list[ config->language ][ text_num ];
			ts = add_text_string ( tslot_poses[REQUEST_TSLOT]+cnt, text , MILFORD_FONT, milford_font_logic, button_boxes_x ,  button_boxes_y,  FONT_NORMAL, cnt, 0, 0,0 );


			ts->dim_if_requester_box_active = NO;
			button_boxes_x += (ts->x2 - ts->x1) + 64;

			if ( rdb_gadget_set == NO )
				{
				gadget = set_rdb_gadget( 0, &requester_rdb[cnt], -1, button_boxes_y, -1, 20*4, cnt );
				rdb = gadget->rdb_gadget;
				rdb->depth = MIDDLE_DEPTH - 10;
				set_rdb_size_from_tm( rdb, ts );
				rdb -> dim_if_requester_box_active = NO;
				}
			}
		rdb_gadget_set = YES;
		bg_loop();
		}

	ht = message_requester_rdb.size.h;
	wd = message_requester_rdb.size.w;
	x = message_requester_rdb.size.x;
	y = message_requester_rdb.size.y;

	for(cnt=0;cnt<buttons;cnt++)
		kill_rdb_gadget( requester_rdb[cnt].gadget );

	hv = (ht-48)/8;
	wv = (wd-48)/8;
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

		}

	kill_rdb_gadget( message_requester_rdb.gadget );
	bg_loop();
	bg_loop();

	read_pads();

 	shell->current_item = store[0];
	shell->master_option_active = store[1]; 

	requester_box_active = NO;

	return	button_pressed; // return SHELL_type 
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
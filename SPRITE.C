/****************************************************************************
 *																									 *																																										  *
 *  .SAN file loader ( output by Maptims )											 *																						  *
 *	 ( Sprite ANim )																		    *																							  *
 *  3rd October 1997																			 *																							  *
 *																									 *
 ****************************************************************************/

#include	"incs.h"


#ifdef PSX
#include "..\..\global\striker.def" // defs for PSX striker
#include "..\..\striker\frontend\mod.def"
#include "..\..\striker\src\anim.def"
//#include "..\..\striker\movies\fruit\mod.def"
//#include "..\..\striker\movies\hotdog\mod.def"
#include "..\..\global\s01.h"	// structs used by scene "s01" format
#include	"sod.h"
#include	"scene.h"
#include "datafile.h"
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include "hardware.h"
#include "polydraw.h"	//psx structs
#include	"font.h"
//#include "text.h"
#include "plasma.h"
#include	"render.h"
//#include "anim.h"	
//#include "test.h"
#include	"mem.h"
#include	"fvars.h"
#include	"pad.h"
#include	"camera.h"
#include	"light.h"
#include "utils.h"
#include	"io.h"
//#include	"sound.h"
//#include "teamsel.h"
#include "options.h"
//#include "..\..\global\image.h"
#endif

#include "spritehw.c"

/********************************************************************
 *																						  *
 * Structs																			  *
 *																						  *
 ********************************************************************/






/********************************************************************************************************************************
 *																						 																	  			  *
 * Prototypes																		 																	  			  *
 *																						 																	  			  *
 ********************************************************************************************************************************/

Sprite *find_empty_sprite_slot();
Sprite *spawn_sprite( WORD spr_type );
void set_sprite( Sprite *sprite, WORD spr_type );




/********************************************************************
 *																						  *
 * Vars																				  *
 *																						  *
 ********************************************************************/


LONG *sprite_animation_file;	// .SAN file addr
//LONG sprites_spawned;			// num of sprites currently spawned
LONG max_sprites;					// max allocated amount of sprites
LONG sprite_search_pos;
LONG max_sprs_spawned;			// max num of sprites that were active at once
Sprite *spawned_sprites_list;
Sprite *sprite;		// addr of spawned sprite structure ( Global )

void init_sprites( WORD san_filenum, LONG max_spr )
{
//*** pass... sprite animation ( .san ) file to load
//*** and max no. of sprites
LONG cnt;

	max_sprites = max_spr;	// set global var max_sprites

	sprite_animation_file = read_datafile_alloc ( san_filenum );

//** Alloc mem for sprite structures **
	spawned_sprites_list = allocate_mem ( 0, sizeof ( Sprite ) * max_sprites );

//** Alloc mem for sprite primitives **
	allocate_sprite_primitive_list( max_sprites );

//** Alloc mem for dummy DR_TPAGE primitives **
	allocate_drtp_list( max_sprites );


	for ( cnt=0; cnt<max_sprites; cnt++ )
		{
		spawned_sprites_list[cnt].active = NO;
		spawned_sprites_list[cnt].prim = (Prim *)spawned_sprite_primitive_list+(cnt*2);
		spawned_sprites_list[cnt].dummy = (POLY_FT3 *)drtp_list+(cnt*2);
		spawned_sprites_list[cnt].prev = 0;
		}

	//sprites_spawned = 0;
	sprite_search_pos=0;

	add_process( draw_sprites );
	max_sprs_spawned = 0;

}

void sprite_null_func()
{
}

Sprite *spawn_null_clip_sprite( WORD depth )
{
RECT r={0,0,512,256};
DR_AREA *dr;

//**** restore clip window to normal ****

	spawn_sprite(0);
	sprite->special=YES;
	sprite->depth = depth;

	dr = (DR_AREA *)sprite->prim + 0;
	SetDrawArea( dr, &r );

	r.y += 256;
	dr++;
	SetDrawArea( dr, &r );

	return sprite;
}

Sprite *spawn_clip_sprite( RECT *rect, WORD depth )
{
DR_AREA *dr;

//**** set clip window on primitives BEFORE this depth ****

	spawn_sprite(0);
	sprite->special=YES;
	sprite->depth = depth;

	dr = (DR_AREA *)sprite->prim + 0;
	SetDrawArea( dr, rect );
	rect->y += 256;

	dr++;
	SetDrawArea( dr, rect );

	rect->y -= 256;

	return sprite;

}

Sprite *spawn_sprite_after ( WORD spr_type )
{
Sprite *previous_spr;

	previous_spr = sprite;
	spawn_sprite ( spr_type );
	sprite->prev = previous_spr;

	previous_spr->next = sprite;
	return sprite;
}

Spr_anim_frame_san *get_sprite_frame_addr ( WORD type )
{
Spr_anim_header_san *spr_anim_header_san;
BYTE *byte_addr;
LONG offs;
Spr_anim_frame_san *fr;

	offs = *(sprite_animation_file + type);
	byte_addr = (BYTE *)sprite_animation_file + offs;
	spr_anim_header_san = (Spr_anim_header_san *)byte_addr;
	byte_addr += sizeof ( Spr_anim_header_san );

	fr = (Spr_anim_frame_san *)byte_addr;
	return fr;

}

Sprite *spawn_sprite( WORD spr_type )
{
	sprite = find_empty_sprite_slot();
	set_sprite( sprite, spr_type );
	return sprite;
}

Sprite *spawn_sprite_at_this_slot( Sprite *sprite, WORD spr_type )
{
//*** spawn at indicated sprite slot ***
	set_sprite( sprite, spr_type );
	return sprite;
}


void set_sprite( Sprite *sprite, WORD spr_type )
{
Spr_anim_header_san *spr_anim_header_san;
BYTE *byte_addr;
LONG offs;

	if ( spr_type == 0 )
		{
//		printf("gfjklgfjgk\n");
		}

	if ( spr_type > -1 )
		{
		// sprite has a type, and therefore is textured

		offs = *(sprite_animation_file + spr_type);
		byte_addr = (BYTE *)sprite_animation_file + offs;
		spr_anim_header_san = (Spr_anim_header_san *)byte_addr;
		byte_addr += sizeof ( Spr_anim_header_san );
		sprite->num_frames = spr_anim_header_san->frames;

		//printf("type %d has %d frames\n",spr_type, sprite->num_frames );
		sprite->clut = spr_anim_header_san->clut;
		sprite->spr_anim_frame_san = (Spr_anim_frame_san *)byte_addr;
		}
	else
		{
		sprite->spr_anim_frame_san = &sprite->untextured_frame;
		sprite->num_frames = 1;
		}


	sprite->brightness	=	128;
	sprite->centre_if_scaled = YES;
	sprite->type			= 0;
	sprite->mode			= 0;
	sprite->special		= NO;
	sprite->gadget			= 0;
	sprite->type			= spr_type;
	sprite->update_cnt 	= 2;
	sprite->prev 			= 0;
	sprite->next 			= 0;
	sprite->active 		= YES;
	sprite->display		= YES;
	sprite->x2.l			=
	sprite->y2.l			= 0;

	sprite->initialised	= NO;
	sprite->transp_rate	= -1;				// transparency off by default
	sprite->depth 			= NEAREST_DEPTH;
	sprite->upd_rtn 		= sprite_null_func;
	sprite->frame 			=
	sprite->timer1 		= 
	sprite->timer2 		= 
	sprite->temp 			= 
	sprite->sequence_num = 
	sprite->xoffs 			= 
	sprite->yoffs 			= 0;
	sprite->gouraud		= NO; // off by default
	sprite->textman		= 0;

	sprite->xflip = 0;

	sprite->x.l 	 = 
	sprite->y.l 	 =
	sprite->xvel1.l = 
//	sprite->xvel2.l = 
	sprite->yvel1.l = 0;
//	sprite->yvel2.l = 0;

	sprite->tint.r = 
	sprite->tint.g = 
	sprite->tint.b = 128;

//	sprite->xflip = 0;
//	sprite->old_xflip = 15;

	sprite->xscale =
	sprite->yscale = 256;
	sprite->use_frame_offs = NO; //YES;

//  	(*sprite->upd_rtn)( sprite );	//execute sprite update func

//	printf("spr=%x\n",sprite);
}



void draw_sprites()
{
Sprite *sprite;
WORD cnt;
WORD spawn_cnt;

#if DEBUG
BYTE err_string[256];
WORD sprs_visible=0;
#endif

	sprite = spawned_sprites_list;
	spawn_cnt=0;

	for ( cnt=0; cnt< max_sprites; cnt++ )
		{
		if ( sprite->active )
			{
  			(*sprite->upd_rtn)( sprite );	//execute sprite update func

			spawn_cnt++;

			if ( sprite->special )
				draw_special_sprite_primitive( sprite );
			else
				{
				if ( sprite->type >-1 )
					{
					#if DEBUG
			 		if ( sprite->frame >= sprite->num_frames )
			 			{
						sprintf ( err_string, "Illegal sprite frame (%d)",sprite->frame );
			 			display_error ( err_string );
						}
					#endif
					}

				draw_sprite_primitive( sprite );

				if ( sprite->display )
					{

					#if DEBUG
					sprs_visible++;
					#endif
					}

				}
			}
		sprite++;
		}

	if ( spawn_cnt > max_sprs_spawned )
		max_sprs_spawned = spawn_cnt;

#if DEBUG
	FntPrint("sprs; active=%d tot=%d visible=%d\n",spawn_cnt, max_sprs_spawned,sprs_visible );
#endif

}






	
		

Sprite *find_empty_sprite_slot()
{
BYTE temp[64];
Sprite *sprite =  spawned_sprites_list + sprite_search_pos;
WORD trys=2;

	while(1)
		{
		if ( !sprite->active )
			{
			return sprite;
			}
		else
			{
			sprite++;
			sprite_search_pos++;
			if ( sprite_search_pos >= max_sprites )
				{
				sprite_search_pos = 0;
				sprite = spawned_sprites_list;
				trys--;

				#if DEBUG
				if ( trys < 0 )
					{
					sprintf( temp, "too many sprites spawned..max = %d\n",max_sprites );
					display_error ( temp );
					}
				#endif
				}
			}
		}


	return 0;
	

}








void kill_sprite( Sprite *sprite )
{
	sprite->active = NO;
}


void kill_sprite_group( WORD group_num )
{
//*** search all sprites and decativate those with the indicated group num ***

Sprite *sprite =  spawned_sprites_list;

WORD cnt = max_sprites;

	while( cnt-- )
		{
		if ( sprite->group_num == group_num )
			{
			kill_sprite ( sprite );
			}
		sprite++;
		}

}





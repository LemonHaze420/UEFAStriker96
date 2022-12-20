/********************************************************************************************************************************
 *																																										  *
 *  SPRITE hardware file																																		  *
 *	 #included by SPRITE.c																																        *	
 *																																										  *
 ********************************************************************************************************************************/

#include "spritehw.h"

void allocate_sprite_primitive_list( LONG max_sprites );
Prim *spawned_sprite_primitive_list;
POLY_FT3 *drtp_list;

void update_ingame_backdrop();
void draw_g4( Sprite *sprite );
void draw_gt4_dummy_texture( Sprite *sprite );
void draw_gt4_texture( Sprite *sprite );
void draw_ft4_dummy_texture( Sprite *sprite );
void draw_ft4_texture( Sprite *sprite );
void draw_ft4_font_texture( Sprite *sprite );

/******************************************************************													
 *																						*
 * NOTES																				*
 *																						*
 * Sprites are moved in a 1024*1024 window...							*
 * Untextured sprites sizes are treated as follows						*
 * A 1024x1024 sprite would occupy the full screen						*
 * Textured sprites are displayed at their true size in pixels 	*
 * but placed within the 1024*1024 window									*
 *	- no scaling of untextured sprites yet									*
 *																						*
 ******************************************************************/




void draw_sprite_primitive( Sprite *sprite )
{

POLY_G4 *g4;
POLY_GT4 *poly;
Spr_anim_frame_san *frame;
UWORD tp;
Prim *prim;
WORD xp,yp;
WORD xo,yo,xd, yd;
//WORD store;


	if ( sprite->gouraud )
		{
		if (sprite->transp_rate == -1)
			{
			draw_g4( sprite );
			}
		else
			{
			if ( sprite->type >-1 )
				{
				draw_gt4_texture( sprite );
				}
			else
				{
				draw_g4( sprite );
//				draw_gt4_dummy_texture( sprite );	// g4 with a dummy texture so it can be drawn with transparency
				}
			}
		}
	else
		{
		if ( sprite->type >-1 )
			{
			if ( (sprite->xscale == 256) && (sprite->yscale == 256) )
				draw_ft4_font_texture( sprite );
			else
				draw_ft4_texture( sprite );
			}
		else
			{
			draw_ft4_dummy_texture( sprite );
			}
		}
}


void draw_gt4_texture( Sprite *sprite )
{
POLY_GT4 *poly;
Spr_anim_frame_san *frame;
UWORD tp;
Prim *prim;
WORD xp,yp;
WORD xo,yo,xd, yd;
//WORD store;


	poly  = (POLY_GT4 *)sprite->prim + cdb->buffnum;

	frame = sprite->spr_anim_frame_san + sprite->frame;

	if ( sprite->display )
		{
		xp = (( sprite->x.w.hi + sprite->xoffs +(SCREEN_UNIT_WD/2)) >> 1);
		yp = (( sprite->y.w.hi + sprite->yoffs +(SCREEN_UNIT_HT/2)) >> 2);


		if ( (sprite->xscale != 256) || ( sprite->yscale != 256 ) )
			{
			xd = ( frame->w * sprite->xscale ) / 256;
			yd = ( frame->h * sprite->yscale ) / 256;

			if ( sprite->centre_if_scaled )
				{
				xo = (xd - frame->w)/2;
				yo = (yd - frame->h)/2;
				}
			else
				xo=yo=0;


			// ensure poly is always centered
		 	poly->x0 = 
		 	poly->x2 = xp - xo;
			poly->x1 = 
			poly->x3 = xp + xd - xo;

		 	poly->y0 = 
		 	poly->y1 = yp - yo;
			poly->y2 = 
			poly->y3 = yp + yd - yo;
			}
		else
			{
			if ( sprite->use_frame_offs )
				{
				poly->x0 = 
				poly->x2 = xp + frame->xoffs; // - 256;
				poly->x1 = 
				poly->x3 = poly->x0 + frame->w;


				poly->y0 = 
				poly->y1 = yp + frame->yoffs; // - 128; //; // - 512;
				poly->y2 = 
				poly->y3 = poly->y1 + frame->h;


				}
			else
				{
				poly->x0 = 
				poly->x2 = xp;
				poly->x1 = 
				poly->x3 = xp + frame->w;

				poly->y0 = 
				poly->y1 = yp;
				poly->y2 = 
				poly->y3 = yp + frame->h;
				}
			}

		if ( (sprite->old_transp_rate != sprite->transp_rate) || ( sprite->frame != sprite->old_frame ) || ( *(ULONG *)&sprite->tint.r != *(ULONG *)&sprite->old_tint.r )  )
			{
			sprite->update_cnt=2;	// do 2 updates to set uv stuff
			}

		}


	if ( ( sprite->xscale != sprite->old_xscale ) || ( sprite->yscale != sprite->old_yscale ) )
		{
		sprite->update_cnt = 2;
		}

	if ( sprite->update_cnt )
		{
		sprite->update_cnt--; 	
		if ( sprite->initialised == NO )
			{
			SetPolyGT4 ( poly );
			if ( !(sprite->update_cnt) )	// initialisation performed for 2 frames
				sprite->initialised++;
			}

		//** This data is set only when transparency or frame have changed.. **
		//** and requires 2 frames to set it **


		poly->r0 = sprite->r0;
		poly->g0 = sprite->g0;
		poly->b0 = sprite->b0;
		poly->r1 = sprite->r1;
		poly->g1 = sprite->g1;
		poly->b1 = sprite->b1;
		poly->r2 = sprite->r2;
		poly->g2 = sprite->g2;
		poly->b2 = sprite->b2;
		poly->r3 = sprite->r3;
		poly->g3 = sprite->g3;
		poly->b3 = sprite->b3;


		if ( sprite->xscale!=256 ) 
			{
			poly->u0 = poly->u2 = frame->u;
			poly->u1 = poly->u3 = frame->u + frame->w-1;
			}
		else
			{
			poly->u0 = poly->u2 = frame->u;
			poly->u1 = poly->u3 = frame->u + frame->w;
			}

		if ( sprite->yscale!=256 ) 
			{
			poly->v0 = poly->v1 = frame->v;
			poly->v2 = poly->v3 = frame->v + frame->h-1;
			}
		else
			{
			poly->v0 = poly->v1 = frame->v;
			poly->v2 = poly->v3 = frame->v + frame->h;
			}
								
		sprite->old_xscale		= sprite->xscale;
		sprite->old_yscale		= sprite->yscale;
		sprite->old_frame       = sprite->frame;
		sprite->old_transp_rate = sprite->transp_rate;
		sprite->old_tint 			= sprite->tint;
		poly->clut = sprite->clut;


		if ( sprite->transp_rate == -1 )
			{
			poly->tpage = frame->tpage; //&(-97));
			SetSemiTrans ( poly, 0 );
			}
		else
			{
			poly->tpage = (frame->tpage&(-97)) + (sprite->transp_rate*32);
			SetSemiTrans ( poly, 1 );
			}
		}


	if ( sprite->display )
		AddPrim ( cdb->ot + sprite->depth, poly );

}


void draw_gt4_dummy_texture( Sprite *sprite )
{
POLY_GT4 *poly;
Spr_anim_frame_san *frame;
UWORD tp;
Prim *prim;
WORD xp,yp;
WORD xo,yo,xd, yd;
//WORD store;


	poly  = (POLY_GT4 *)sprite->prim + cdb->buffnum;


	frame = sprite->spr_anim_frame_san + sprite->frame;

	if ( sprite->display )
		{
		xp = (( sprite->x.w.hi + sprite->xoffs +(SCREEN_UNIT_WD/2)) >> 1);
		yp = (( sprite->y.w.hi + sprite->yoffs +(SCREEN_UNIT_HT/2)) >> 2);

		poly->x0 = 
		poly->x2 = xp;
		poly->x1 = 
		poly->x3 = xp +  ( frame->w >> 1);
		poly->y0 = 
		poly->y1 = yp;
		poly->y2 = 
		poly->y3 = yp + ( frame->h >> 2); //zz>>2

		if ( (sprite->old_transp_rate != sprite->transp_rate) || ( sprite->frame != sprite->old_frame ) || ( *(ULONG *)&sprite->tint.r != *(ULONG *)&sprite->old_tint.r )  )
			{
			sprite->update_cnt=2;	// do 2 updates to set uv stuff
			}

		}


	if ( ( sprite->xscale != sprite->old_xscale ) || ( sprite->yscale != sprite->old_yscale ) )
		{
		sprite->update_cnt = 2;
		}

	if ( sprite->update_cnt )
		{
		sprite->update_cnt--; 	
		if ( sprite->initialised == NO )
			{
			SetPolyGT4 ( poly );
			if ( !(sprite->update_cnt) )	// initialisation performed for 2 frames
				sprite->initialised++;
			}

		//** This data is set only when transparency or frame have changed.. **
		//** and requires 2 frames to set it **


		setRGB0( poly, sprite->r0, sprite->g0, sprite->b0 );
		setRGB1( poly, sprite->r1, sprite->g1, sprite->b1 );
		setRGB2( poly, sprite->r2, sprite->g2, sprite->b2 );
		setRGB3( poly, sprite->r3, sprite->g3, sprite->b3 );

		SetSemiTrans ( poly, 1 );

	// Point poly to a dummy texture

		poly -> tpage = GetTPage ( DUMMY_TEXTURE_BPP, sprite->transp_rate, DUMMY_TEXTURE_X,DUMMY_TEXTURE_Y ); 
		poly -> u0 = 
		poly -> u1 = 
		poly -> u2 = 
		poly -> u3 = ( DUMMY_TEXTURE_X <<(2-DUMMY_TEXTURE_BPP)) % (64<<(2-DUMMY_TEXTURE_BPP));
		poly -> v0 = 
		poly -> v1 = 
		poly -> v2 = 
		poly -> v3 = DUMMY_TEXTURE_Y % 256;
		}



	if ( sprite->display )
		{
		AddPrim ( cdb->ot + sprite->depth, poly );
		}


//	FntPrint("w=%d h=%d\n",frame->w,frame->h);

//	FntPrint("tp=%d clut=%d\n",poly->tpage,poly->clut);
//	FntPrint("xy0=%d %d xy3=%d %d\n",poly->x0,poly->y0, poly->x3,poly->y3 );
}


void draw_g4( Sprite *sprite )
{
POLY_G4 *poly;
POLY_FT3 *dr_dummy;
WORD dummy;
UWORD tp;
Prim *prim;
WORD xp,yp;
WORD xo,yo,xd, yd;
//WORD store;

	poly  = (POLY_G4 *)sprite->prim + cdb->buffnum;
	dr_dummy = sprite->dummy + cdb->buffnum;

	
	if ( sprite->transp_rate == -1 )
		dummy=NO;
	else
		dummy=YES;

	if ( sprite->display )
		{
		xp = (( sprite->x.w.hi + sprite->xoffs +(SCREEN_UNIT_WD/2)) >> 1);
		yp = (( sprite->y.w.hi + sprite->yoffs +(SCREEN_UNIT_HT/2)) >> 2);


		poly->x0 = 
		poly->x2 = xp;
		poly->x1 = 
		poly->x3 = xp + (sprite->untextured_frame.w>>1);

		poly->y0 = 
		poly->y1 = yp;
		poly->y2 = 
		poly->y3 = yp + (sprite->untextured_frame.h>>2);

		if ( (sprite->old_transp_rate != sprite->transp_rate) || ( sprite->frame != sprite->old_frame )  )
			{
			sprite->update_cnt=2;	// do 2 updates to set uv stuff
			}

		}


	if ( sprite->update_cnt )
		{
		sprite->update_cnt--; 	
		if ( sprite->initialised == NO )
			{
			SetPolyG4 ( poly );

			dr_dummy->tpage = GetTPage( 0, sprite->transp_rate, 0, 0 );
			SetPolyFT3( dr_dummy );
			dr_dummy->x0 = 560;
			dr_dummy->x1 = 561;
			dr_dummy->x2 = 560;
			dr_dummy->y0 = 560;
			dr_dummy->y1 = 560;
			dr_dummy->y2 = 561;

			if ( !(sprite->update_cnt) )	// initialisation performed for 2 frames
				sprite->initialised++;
			}

		//** This data is set only when transparency or frame have changed.. **
		//** and requires 2 frames to set it **




		sprite->old_frame       = sprite->frame;
		sprite->old_transp_rate = sprite->transp_rate;
		sprite->old_tint 			= sprite->tint;


		if ( dummy == NO )
			SetSemiTrans ( poly, 0 );
		else
			SetSemiTrans ( poly, 1 );


		}

	if ( sprite->brightness != 128 )
		{
		poly->r0 = (sprite->r0 * sprite->brightness)/128;
		poly->g0 = (sprite->g0 * sprite->brightness)/128;
		poly->b0 = (sprite->b0 * sprite->brightness)/128;
		poly->r1 = (sprite->r1 * sprite->brightness)/128;
		poly->g1 = (sprite->g1 * sprite->brightness)/128;
		poly->b1 = (sprite->b1 * sprite->brightness)/128;
		poly->r2 = (sprite->r2 * sprite->brightness)/128;
		poly->g2 = (sprite->g2 * sprite->brightness)/128;
		poly->b2 = (sprite->b2 * sprite->brightness)/128;
		poly->r3 = (sprite->r3 * sprite->brightness)/128;
		poly->g3 = (sprite->g3 * sprite->brightness)/128;
		poly->b3 = (sprite->b3 * sprite->brightness)/128;
		}
	else
		{
		poly->r0 = sprite->r0;
		poly->g0 = sprite->g0;
		poly->b0 = sprite->b0;
		poly->r1 = sprite->r1;
		poly->g1 = sprite->g1;
		poly->b1 = sprite->b1;
		poly->r2 = sprite->r2;
		poly->g2 = sprite->g2;
		poly->b2 = sprite->b2;
		poly->r3 = sprite->r3;
		poly->g3 = sprite->g3;
		poly->b3 = sprite->b3;
		}


	if ( sprite->display )
		{
		AddPrim ( cdb->ot + sprite->depth, poly );

		if ( dummy )
			AddPrim ( cdb->ot + sprite->depth+0 , dr_dummy );
		}

}





void draw_ft4_texture( Sprite *sprite )
{
POLY_FT4 *poly;
Spr_anim_frame_san *frame;
UWORD tp;
Prim *prim;
WORD xp,yp;
WORD xo,yo,xd, yd;
//WORD store;
//ULONG col;

	poly  = (POLY_FT4 *)sprite->prim + cdb->buffnum;

	frame = sprite->spr_anim_frame_san + sprite->frame;

	if ( sprite->display )
		{
		xp = (( sprite->x.w.hi + sprite->xoffs +(SCREEN_UNIT_WD/2)) >> 1);
		yp = (( sprite->y.w.hi + sprite->yoffs +(SCREEN_UNIT_HT/2)) >> 2);


		if ( (sprite->xscale != 256) || ( sprite->yscale != 256 ) )
			{
			xd = ( frame->w * sprite->xscale ) / 256;
			yd = ( frame->h * sprite->yscale ) / 256;

			if ( sprite->centre_if_scaled )
				{
				xo = (xd - frame->w)/2;
				yo = (yd - frame->h)/2;
				}
			else
				xo=yo=0;

			// ensure poly is always centered
		 	poly->x0 = 
		 	poly->x2 = xp - xo;
			poly->x1 = 
			poly->x3 = xp + xd - xo;

		 	poly->y0 = 
		 	poly->y1 = yp - yo;
			poly->y2 = 
			poly->y3 = yp + yd - yo;
			}
		else
			{
			if ( sprite->use_frame_offs )
				{
				poly->x0 = 
				poly->x2 = xp + frame->xoffs; // - 256;
				poly->x1 = 
				poly->x3 = poly->x0 + frame->w;


				poly->y0 = 
				poly->y1 = yp + frame->yoffs; // - 128; //; // - 512;
				poly->y2 = 
				poly->y3 = poly->y1 + frame->h;


				}
			else
				{
				poly->x0 = 
				poly->x2 = xp;
				poly->x1 = 
				poly->x3 = xp + frame->w;

				poly->y0 = 
				poly->y1 = yp;
				poly->y2 = 
				poly->y3 = yp + frame->h;
				}
			}


//		col = ;
		if ( (sprite->old_transp_rate != sprite->transp_rate) || ( sprite->frame != sprite->old_frame ) || ( *(ULONG *)&sprite->tint.r != *(ULONG *)&sprite->old_tint.r) )
			{
			sprite->update_cnt=2;	// do 2 updates to set uv stuff
			}

		}


	if ( ( sprite->xscale != sprite->old_xscale ) || ( sprite->yscale != sprite->old_yscale ) )
		{
		sprite->update_cnt = 2;
		}

	if ( sprite->update_cnt )
		{
		sprite->update_cnt--; 	
		if ( sprite->initialised == NO )
			{
			SetPolyFT4 ( poly );
			if ( !(sprite->update_cnt) )	// initialisation performed for 2 frames
				sprite->initialised++;
			}

		//** This data is set only when transparency or frame have changed.. **
		//** and requires 2 frames to set it **


		poly->r0 = sprite->tint.r;
		poly->g0 = sprite->tint.g;
		poly->b0 = sprite->tint.b;


		if ( sprite->xscale!=256 ) 
			{
			poly->u0 = poly->u2 = frame->u;
			poly->u1 = poly->u3 = frame->u + frame->w-1;
			}
		else
			{
			poly->u0 = poly->u2 = frame->u;
			poly->u1 = poly->u3 = frame->u + frame->w;
			}

		if ( sprite->yscale!=256 ) 
			{
			poly->v0 = poly->v1 = frame->v;
			poly->v2 = poly->v3 = frame->v + frame->h-1;
			}
		else
			{
			poly->v0 = poly->v1 = frame->v;
			poly->v2 = poly->v3 = frame->v + frame->h;
			}
								
		sprite->old_xscale		= sprite->xscale;
		sprite->old_yscale		= sprite->yscale;
		sprite->old_frame       = sprite->frame;
		sprite->old_transp_rate = sprite->transp_rate;
		sprite->old_tint 			= sprite->tint;
		poly->clut 					= sprite->clut;

		if ( sprite->transp_rate == -1 )
			{
			poly->tpage = frame->tpage;
			SetSemiTrans ( poly, 0 );
			}
		else
			{
			poly->tpage = (frame->tpage&(-97)) + (sprite->transp_rate*32);
			SetSemiTrans ( poly, 1 );
			}
			
		}


	if ( sprite->display )
		AddPrim ( cdb->ot + sprite->depth, poly );

}


void draw_ft4_dummy_texture( Sprite *sprite )
{
POLY_FT4 *poly;
Spr_anim_frame_san *frame;
UWORD tp;
Prim *prim;
WORD xp,yp;
//WORD xo,yo;
//WORD store;


	poly  = (POLY_FT4 *)sprite->prim + cdb->buffnum;

	frame = sprite->spr_anim_frame_san + sprite->frame;

	if ( sprite->display )
		{
		xp = (( sprite->x.w.hi + sprite->xoffs +(SCREEN_UNIT_WD/2)) >> 1);
		yp = (( sprite->y.w.hi + sprite->yoffs +(SCREEN_UNIT_HT/2)) >> 2);

			if ( sprite->use_frame_offs )
				{
				poly->x0 = 
				poly->x2 = xp + frame->xoffs; // - 256;
				poly->x1 = 
				poly->x3 = poly->x0 + frame->w;

				poly->y0 = 
				poly->y1 = yp + frame->yoffs; // - 128;
				poly->y2 = 
				poly->y3 = poly->y1 + frame->h;

				}
			else
				{

				poly->x0 = 
				poly->x2 = xp;
				poly->x1 = 
				poly->x3 = xp +  ( frame->w >> 1);
				poly->y0 = 
				poly->y1 = yp;
				poly->y2 = 
				poly->y3 = yp + ( frame->h >> 2); //zz>>2
				}
		if ( (sprite->old_transp_rate != sprite->transp_rate) || ( sprite->frame != sprite->old_frame ) || ( *(ULONG *)&sprite->tint.r != *(ULONG *)&sprite->old_tint.r )  )
			{
			sprite->update_cnt=2;	// do 2 updates to set uv stuff
			}

		}


	if ( ( sprite->xscale != sprite->old_xscale ) || ( sprite->yscale != sprite->old_yscale ) )
		{
		sprite->update_cnt = 2;
		}

	if ( sprite->update_cnt )
		{
		sprite->update_cnt--; 	
		if ( sprite->initialised == NO )
			{
			SetPolyFT4 ( poly );
			if ( !(sprite->update_cnt) )	// initialisation performed for 2 frames
				sprite->initialised++;
			}

		//** This data is set only when transparency or frame have changed.. **
		//** and requires 2 frames to set it **


		poly->r0 = sprite->tint.r;
		poly->g0 = sprite->tint.g;
		poly->b0 = sprite->tint.b;

		SetSemiTrans ( poly, (sprite->transp_rate>-1)?1:0 );

		// Point poly to a dummy texture

		poly -> tpage = GetTPage ( DUMMY_TEXTURE_BPP, sprite->transp_rate, DUMMY_TEXTURE_X,DUMMY_TEXTURE_Y ); 
		poly -> u0 = 
		poly -> u1 = 
		poly -> u2 = 
		poly -> u3 = ( DUMMY_TEXTURE_X <<(2-DUMMY_TEXTURE_BPP)) % (64<<(2-DUMMY_TEXTURE_BPP));
		poly -> v0 = 
		poly -> v1 = 
		poly -> v2 = 
		poly -> v3 = DUMMY_TEXTURE_Y % 256;
		}



	if ( sprite->display )
		{
		AddPrim ( cdb->ot + sprite->depth, poly );
		}

}



void draw_ft4_font_texture( Sprite *sprite )
{
POLY_FT4 *poly;
Spr_anim_frame_san *frame;
UWORD tp;
Prim *prim;
WORD xp,yp;


	poly  = (POLY_FT4 *)sprite->prim + cdb->buffnum;

	frame = sprite->spr_anim_frame_san + sprite->frame;

	if ( sprite->display )
		{
		xp = (( sprite->x.w.hi + sprite->xoffs +(SCREEN_UNIT_WD/2)) >> 1);
		yp = (( sprite->y.w.hi + sprite->yoffs +(SCREEN_UNIT_HT/2)) >> 2);


			if ( sprite->use_frame_offs )
				{
				poly->x0 = 
				poly->x2 = xp + frame->xoffs; // - 256;
				poly->x1 = 
				poly->x3 = poly->x0 + frame->w;


				poly->y0 = 
				poly->y1 = yp + frame->yoffs; // - 128; //; // - 512;
				poly->y2 = 
				poly->y3 = poly->y1 + frame->h;


				}
			else
				{


				poly->x0 = 
				poly->x2 = xp;
				poly->x1 = 
				poly->x3 = xp + frame->w;

				poly->y0 = 
				poly->y1 = yp;
				poly->y2 = 
				poly->y3 = yp + frame->h;
				}
		if ( (sprite->old_transp_rate != sprite->transp_rate) || ( sprite->frame != sprite->old_frame ) || ( *(ULONG *)&sprite->tint.r != *(ULONG *)&sprite->old_tint.r )  )
			{
			sprite->update_cnt=2;	// do 2 updates to set uv stuff
			}

		}


	if ( sprite->update_cnt )
		{
		sprite->update_cnt--; 	
		if ( sprite->initialised == NO )
			{
			SetPolyFT4 ( poly );
			if ( !(sprite->update_cnt) )	// initialisation performed for 2 frames
				sprite->initialised++;
			}

		//** This data is set only when transparency or frame have changed.. **
		//** and requires 2 frames to set it **


		poly->r0 = sprite->tint.r;
		poly->g0 = sprite->tint.g;
		poly->b0 = sprite->tint.b;


		poly->u0 = poly->u2 = frame->u;
		poly->u1 = poly->u3 = frame->u + frame->w;
		poly->v0 = poly->v1 = frame->v;
		poly->v2 = poly->v3 = frame->v + frame->h;
								
		sprite->old_frame       = sprite->frame;
		sprite->old_transp_rate = sprite->transp_rate;
		sprite->old_tint 			= sprite->tint;
		poly->clut 					= sprite->clut;

		if ( sprite->transp_rate == -1 )
			{
			poly->tpage = frame->tpage;
			SetSemiTrans ( poly, 0 );
			}
		else
			{
			poly->tpage = (frame->tpage&(-97)) + (sprite->transp_rate*32);
			SetSemiTrans ( poly, 1 );
			}
			
		}


	if ( sprite->display )
		AddPrim ( cdb->ot + sprite->depth, poly );

}


void draw_special_sprite_primitive( Sprite *sprite )
{
DR_AREA *poly;

	poly  = (DR_AREA *)sprite->prim + cdb->buffnum;
	AddPrim ( cdb->ot + sprite->depth, poly );
}






void allocate_sprite_primitive_list( LONG max_sprites )
{
	if ( sizeof ( Sprite ) & 3 )
		display_error ( "Sprite struct not lw aligned");

	spawned_sprite_primitive_list = allocate_mem ( 0, sizeof ( Prim ) * max_sprites * 2 );
}

void allocate_drtp_list( LONG max_sprites )
{
	if ( sizeof ( Sprite ) & 3 )
		display_error ( "Sprite struct not lw aligned");

	drtp_list = allocate_mem ( 0, sizeof ( POLY_FT3 ) * max_sprites * 2 );
}









POLY_FT4 str_dummy[2][3];
SPRT str_sp[2][3];
void update_backdrop();

void *bg_pro, *ingame_bg_pro;

void add_backdropx ( WORD spr_type )
{
SPRT *sprt1, *sprt2, *sprt3;
POLY_FT4 *dummy;
Spr_anim_frame_san *frame;
Spr_anim_header_san *spr_anim_header_san;
BYTE *byte_addr;
LONG offs;
LONG clut;
WORD buffnum;

	bg_pro = add_process( update_backdrop );


// calc frame data for backdrop
	offs = *(sprite_animation_file + spr_type);
	byte_addr = (BYTE *)sprite_animation_file + offs;
	spr_anim_header_san = (Spr_anim_header_san *)byte_addr;
	byte_addr += sizeof ( Spr_anim_header_san );
	clut = spr_anim_header_san->clut;
	


	for ( buffnum=0; buffnum<2; buffnum++ )
		{
		frame = (Spr_anim_frame_san *)byte_addr;


//*** Set sprite 1 ***
		dummy = &str_dummy[ buffnum][0];
		sprt1  = &str_sp   [ buffnum][0];

		SetSprt ( sprt1 );
		sprt1->clut = spr_anim_header_san->clut;
		sprt1->u0   = frame->u;
		sprt1->v0   = frame->v;
		sprt1->w    = frame->w;
		sprt1->h    = frame->h;
		sprt1->x0   = 0;
		sprt1->y0   = 0;
		sprt1->r0   = sprt1->g0 = sprt1->b0 = 128;


		dummy->x0 = 600;
		dummy->y0 = 0;
		dummy->x1 = 601;
		dummy->y1 = 0;
		dummy->x2 = 600;
		dummy->y2 = 0;
		dummy->x3 = 601;
		dummy->y3 = 0;
		SetPolyFT4 ( dummy );

		dummy->tpage = frame->tpage;
		dummy->u0=0;
		dummy->u1=0;
		dummy->u2=0;
		dummy->v0=0;
		dummy->v1=0;
		dummy->v2=0;
		SetSemiTrans ( dummy, 0 );


//*** Set sprite 2 ***
		frame++;

		dummy = &str_dummy[ buffnum ][1];
		sprt2  = &str_sp   [ buffnum ][1];

		SetSprt ( sprt2 );
		sprt2->clut = spr_anim_header_san->clut;
		sprt2->u0   = frame->u;
		sprt2->v0   = frame->v;
		sprt2->w    = frame->w;
		sprt2->h    = frame->h;
		sprt2->x0   = 0 + sprt1->w;
		sprt2->y0   = 0;
		sprt2->r0   = sprt2->g0 = sprt2->b0 = 128;

		dummy->x0 = 600;
		dummy->y0 = 0;
		dummy->x1 = 601;
		dummy->y1 = 0;
		dummy->x2 = 600;
		dummy->y2 = 0;
		dummy->x3 = 601;
		dummy->y3 = 0;
		SetPolyFT4 ( dummy );

		dummy->tpage = frame->tpage;
		dummy->u0=0;
		dummy->u1=0;
		dummy->u2=0;
		dummy->v0=0;
		dummy->v1=0;
		dummy->v2=0;
		SetSemiTrans ( dummy, 0 );


//*** Set sprite 3 ***
		frame++;

		dummy = &str_dummy[ buffnum ][2];
		sprt3  = &str_sp   [ buffnum ][2];

		SetSprt ( sprt3 );
		sprt3->clut = spr_anim_header_san->clut;
		sprt3->u0   = frame->u;
		sprt3->v0   = frame->v;
		sprt3->w    = frame->w;
		sprt3->h    = frame->h;
		sprt3->x0   = 0 + sprt1->w + sprt2->w;
		sprt3->y0   = 0;
		sprt3->r0   = sprt3->g0 = sprt3->b0 = 128;

		dummy->x0 = 600;
		dummy->y0 = 0;
		dummy->x1 = 601;
		dummy->y1 = 0;
		dummy->x2 = 600;
		dummy->y2 = 0;
		dummy->x3 = 601;
		dummy->y3 = 0;
		SetPolyFT4 ( dummy );

		dummy->tpage = frame->tpage;
		dummy->u0=0;
		dummy->u1=0;
		dummy->u2=0;
		dummy->v0=0;
		dummy->v1=0;
		dummy->v2=0;
		SetSemiTrans ( dummy, 0 );

		}



}

void update_backdropx()
{
SPRT *sprt;
POLY_FT4 *dummy;

// dummy and sprite polys into OT
	sprt = &str_sp[ cdb->buffnum][0];
	dummy = &str_dummy[cdb->buffnum][0];
	AddPrim ( cdb->ot + FURTHEST_DEPTH+0, sprt );
	AddPrim ( cdb->ot + FURTHEST_DEPTH + 0, dummy );

	sprt = &str_sp[ cdb->buffnum][1];
	dummy = &str_dummy[cdb->buffnum][1];
	AddPrim ( cdb->ot + FURTHEST_DEPTH+0, sprt );
	AddPrim ( cdb->ot + FURTHEST_DEPTH+0, dummy );

	sprt = &str_sp[ cdb->buffnum][2];
	dummy = &str_dummy[cdb->buffnum][2];
	AddPrim ( cdb->ot + FURTHEST_DEPTH+0, sprt );
	AddPrim ( cdb->ot + FURTHEST_DEPTH+0, dummy );

}




void add_backdrop ( WORD spr_type )
{
SPRT *sprt1, *sprt2, *sprt3;
POLY_FT4 *dummy;
Spr_anim_frame_san *frame;
Spr_anim_header_san *spr_anim_header_san;
BYTE *byte_addr;
LONG offs;
LONG clut;
WORD buffnum;




	spawn_sprite( BK );
	sprite->x.w.hi = -512;
	sprite->y.w.hi = -512;
	sprite->xscale = (256 * 256) / 200;
	sprite->centre_if_scaled = NO;
	sprite->depth = FURTHEST_DEPTH;

	spawn_sprite( BK );
	sprite->frame = 1;
	sprite->x.w.hi = -2;
	sprite->y.w.hi = -512;
	sprite->xscale = (256 * 256) / 200;
	sprite->centre_if_scaled = NO;
	sprite->depth = FURTHEST_DEPTH;

	return;

	bg_pro = add_process( update_backdrop );


// calc frame data for backdrop
	offs = *(sprite_animation_file + spr_type);
	byte_addr = (BYTE *)sprite_animation_file + offs;
	spr_anim_header_san = (Spr_anim_header_san *)byte_addr;
	byte_addr += sizeof ( Spr_anim_header_san );
	clut = spr_anim_header_san->clut;
	


	for ( buffnum=0; buffnum<2; buffnum++ )
		{
		frame = (Spr_anim_frame_san *)byte_addr;

		frame->w = 256;

//*** Set sprite 1 ***
		dummy = &str_dummy[ buffnum][0];
		sprt1  = &str_sp   [ buffnum][0];

		SetSprt ( sprt1 );
		sprt1->clut = spr_anim_header_san->clut;
		sprt1->u0   = frame->u;
		sprt1->v0   = frame->v;
		sprt1->w    = frame->w;
		sprt1->h    = frame->h;
		sprt1->x0   = 0;
		sprt1->y0   = 0;
		sprt1->r0   = sprt1->g0 = sprt1->b0 = 128;


		dummy->x0 = 600;
		dummy->y0 = 0;
		dummy->x1 = 601;
		dummy->y1 = 0;
		dummy->x2 = 600;
		dummy->y2 = 0;
		dummy->x3 = 601;
		dummy->y3 = 0;
		SetPolyFT4 ( dummy );

		dummy->tpage = frame->tpage;
		dummy->u0=0;
		dummy->u1=0;
		dummy->u2=0;
		dummy->v0=0;
		dummy->v1=0;
		dummy->v2=0;
		SetSemiTrans ( dummy, 0 );


//*** Set sprite 2 ***
		frame++;

		frame->w=256;

		dummy = &str_dummy[ buffnum ][1];
		sprt2  = &str_sp   [ buffnum ][1];

		SetSprt ( sprt2 );
		sprt2->clut = spr_anim_header_san->clut;
		sprt2->u0   = frame->u;
		sprt2->v0   = frame->v;
		sprt2->w    = frame->w;
		sprt2->h    = frame->h;
		sprt2->x0   = 0 + sprt1->w;
		sprt2->y0   = 0;
		sprt2->r0   = sprt2->g0 = sprt2->b0 = 128;

		dummy->x0 = 600;
		dummy->y0 = 0;
		dummy->x1 = 601;
		dummy->y1 = 0;
		dummy->x2 = 600;
		dummy->y2 = 0;
		dummy->x3 = 601;
		dummy->y3 = 0;
		SetPolyFT4 ( dummy );

		dummy->tpage = frame->tpage;
		dummy->u0=0;
		dummy->u1=0;
		dummy->u2=0;
		dummy->v0=0;
		dummy->v1=0;
		dummy->v2=0;
		SetSemiTrans ( dummy, 0 );


		}



}


void update_backdrop()
{
SPRT *sprt;
POLY_FT4 *dummy;

// dummy and sprite polys into OT
	sprt = &str_sp[ cdb->buffnum][0];
	dummy = &str_dummy[cdb->buffnum][0];
	AddPrim ( cdb->ot + FURTHEST_DEPTH+0, sprt );
	AddPrim ( cdb->ot + FURTHEST_DEPTH + 0, dummy );

	sprt = &str_sp[ cdb->buffnum][1];
	dummy = &str_dummy[cdb->buffnum][1];
	AddPrim ( cdb->ot + FURTHEST_DEPTH+0, sprt );
	AddPrim ( cdb->ot + FURTHEST_DEPTH+0, dummy );

}


void kill_backdrop()
{
	kill_process( bg_pro );
}

















/**************/

POLY_FT4 poly_sp[2];

void add_ingame_backdrop ( WORD spr_type )
{
POLY_FT4 *sprt1;
Spr_anim_frame_san *frame;
Spr_anim_header_san *spr_anim_header_san;
BYTE *byte_addr;
LONG offs;
LONG clut;
WORD buffnum;

	ingame_bg_pro = add_process( update_ingame_backdrop );


// calc frame data for backdrop
	offs = *(sprite_animation_file + spr_type);
	byte_addr = (BYTE *)sprite_animation_file + offs;
	spr_anim_header_san = (Spr_anim_header_san *)byte_addr;
	byte_addr += sizeof ( Spr_anim_header_san );
	clut = spr_anim_header_san->clut;
	


	for ( buffnum=0; buffnum<2; buffnum++ )
		{
		frame = (Spr_anim_frame_san *)byte_addr;


//*** Set sprite 1 ***
		sprt1  = &poly_sp[ buffnum];

		SetPolyFT4 ( sprt1 );
		SetSemiTrans ( sprt1, 0 );
		sprt1->clut = spr_anim_header_san->clut;
		sprt1->u0   = sprt1->u2   = frame->u;
		sprt1->u1   = sprt1->u3   = frame->u + frame->w-1;


		sprt1->v0   = sprt1->v1 = frame->v;
		sprt1->v2   = sprt1->v3 = frame->v + frame->h;

		sprt1->tpage = frame->tpage;
		sprt1->clut = spr_anim_header_san->clut;

		sprt1->x0   = 0;
		sprt1->x1   = 512;
		sprt1->x2   = 0;
		sprt1->x3   = 512;
		sprt1->y0   = 0;
		sprt1->y1   = 0;
		sprt1->y2   = 255;
		sprt1->y3   = 255;

		sprt1->r0   = sprt1->g0 = sprt1->b0 = 128;

		}



}

void update_ingame_backdrop()
{
POLY_FT4 *sprt;

// dummy and sprite polys into OT
	sprt = &poly_sp[ cdb->buffnum];
	AddPrim ( cdb->ot + FURTHEST_DEPTH+0, sprt );

}




void kill_ingame_backdrop()
{
	kill_process( ingame_bg_pro );
}



void draw_simple_poly( POLY_FT4 *ft, WORD x, WORD y, WORD w, WORD h, WORD u, WORD v, UWORD tp, UWORD clut_id )
{

	SetPolyFT4 ( ft );

	ft->x0 = x;
	ft->x1 = x + (w*2);
	ft->x2 = x;
	ft->x3 = x + (w*2);

	ft->y0 = y;
	ft->y1 = y;
	ft->y2 = y + h;
	ft->y3 = y + h;

	ft->u0 = (( u <<(2-0)) % (64<<(2-0)));
	ft->u1 = ft->u0 + (w-1);
	ft->u2 = ft->u0;
	ft->u3 = ft->u1;

	ft->v0 = v % 256;
	ft->v1 = ft->v0;
	ft->v2 = ft->v0 + h;
	ft->v3 = ft->v2;

	ft->tpage = tp;
	ft->clut = clut_id;

	ft->r0 = 
	ft->g0 = 
	ft->b0 = 128;

	ft -> tpage = GetTPage ( 0, 0, u, v ); 

	SetSemiTrans( ft, 0 );
	AddPrim ( cdb->ot + MIDDLE_DEPTH, ft );

}

//POLY_G4 g4[2];
//POLY_FT4 ft4[2];


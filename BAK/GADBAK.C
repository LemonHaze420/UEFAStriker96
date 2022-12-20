void spawn_rdb_gadgetx( Gadget *gadget )
{
WORD quad_w, quad_h;
WORD box_x1,box_x2,box_y1,box_y2;
Spr_anim_frame_san *fr;	
Sprite *quad;
Gadget *parent = gadget->parent;
Rdb_gadget *rdb = gadget->rdb_gadget;
Textman_struct *ts;
WORD transp_rate;



// box is positioned so that
// 0,0 = centre point in box
// and 0,0 is centre of screen

	gadget->upd_rtn = (void *)&update_rdb_gadget;
	gadget->type = RDB_GADGET;

	transp_rate = 2;

	rdb->tint = 0;			// subtract amount = 0
	gadget->mode = GADGET_INIT;	// start fade up
	rdb->transp_rate = transp_rate;


//** Top left quad **
	spawn_sprite( DBOX );
	rdb->tl = sprite;
	sprite->gadget = gadget;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->mode = GADGET_INIT;
	sprite->type2 = RDB_TL;
	sprite->frame = 0;
	sprite->transp_rate = transp_rate;
	sprite->depth 					= MIDDLE_DEPTH;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 0;


//** Centre sprite **
	spawn_sprite ( -1 );
	rdb->cen = sprite;
	sprite->depth 					= MIDDLE_DEPTH;
	sprite->tint.b = sprite->tint.g = sprite->tint.r = 0;
	sprite->transp_rate = transp_rate;
	sprite->gadget = gadget;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->mode = GADGET_INIT;
	sprite->type2 = RDB_CEN;

	rdb->x1 = rdb->size.x + sprite->xoffs;
	rdb->x2 = rdb->x1 + rdb->size.w;
	rdb->y1 = rdb->size.y + sprite->yoffs;
	rdb->y2 = rdb->y1 + rdb->size.h;


//** Top right quad **
	spawn_sprite( DBOX );
	rdb->tr = sprite;
	sprite->frame = 1;
	sprite->transp_rate = transp_rate;
	sprite->depth 					= MIDDLE_DEPTH;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 0;
	sprite->gadget = gadget;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->mode = GADGET_INIT;
	sprite->type2 = RDB_TR;
	fr = sprite->spr_anim_frame_san+0;
	rdb->quad_w = fr->w*2;
	rdb->quad_h = fr->h*4;

//** Bot left quad **
	spawn_sprite( DBOX );
	rdb->bl = sprite;
	sprite->frame = 2;
	sprite->transp_rate = transp_rate;
	sprite->depth 					= MIDDLE_DEPTH;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 0;
	sprite->gadget = gadget;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->mode = GADGET_INIT;
	sprite->type2 = RDB_BL;

//** Bot right quad **
	spawn_sprite( DBOX );
	rdb->br = sprite;
	sprite->frame = 3;
	sprite->transp_rate = transp_rate;
	sprite->depth 					= MIDDLE_DEPTH;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 0;
	sprite->gadget = gadget;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->mode = GADGET_INIT;
	sprite->type2 = RDB_BR;

//** Left vert sprite **
	spawn_sprite( -1 );
	rdb->lv = sprite;
	sprite->transp_rate = transp_rate;
	sprite->depth 					= MIDDLE_DEPTH;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 0;
	sprite->gadget = gadget;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->mode = GADGET_INIT;
	sprite->type2 = RDB_LV;

//** Right vert sprite **
	spawn_sprite( -1 );
	rdb->rv = sprite;
	sprite->transp_rate = transp_rate;
	sprite->depth 					= MIDDLE_DEPTH;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 0;
	sprite->gadget = gadget;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->mode = GADGET_INIT;
	sprite->type2 = RDB_RV;

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



// box is positioned so that
// 0,0 = centre point in box
// and 0,0 is centre of screen

	gadget->upd_rtn = (void *)&update_rdb_gadget;
	gadget->type = RDB_GADGET;

	transp_rate = 2;

	rdb->tint = 0;			// subtract amount = 0
	gadget->mode = GADGET_INIT;	// start fade up
	rdb->transp_rate = transp_rate;


//*** tl quad ***
	spawn_sprite( BOX );
	sprite->frame = 0;
	fr = tl->spr_anim_frame_san+ sprite->frame;
	rdb->tl = sprite;
	rdb->tl_quad_w = fr->w*2;
	rdb->tl_quad_h = fr->h*4;
	sprite->upd_rtn = (void *)&rdb_sprite_upd2;
 

//** tr quad ***
	spawn_sprite( BOX );
	rdb->tr = sprite;
	sprite->frame = 2;
	sprite->upd_rtn = (void *)&rdb_sprite_upd2;



//** bl quad ***
	spawn_sprite( BOX );
	rdb->bl = sprite;
	sprite->frame = 5;
	sprite->upd_rtn = (void *)&rdb_sprite_upd2;



//** br quad ***
	spawn_sprite( BOX );
	rdb->br = sprite;
	sprite->frame = 7;
	sprite->upd_rtn = (void *)&rdb_sprite_upd2;


//** top centre ***
	spawn_sprite( BOX );
	sprite->frame = 1;
	fr = sprite->spr_anim_frame_san + sprite->frame;
	rdb->top_w = fr->w*2;
	rdb->top = sprite;
	sprite->centre_if_scaled = NO;
	sprite->upd_rtn = (void *)&rdb_sprite_upd2;


//** bot centre ***
	spawn_sprite( BOX );
	rdb->bot = sprite;
	sprite->frame = 6;
	sprite->centre_if_scaled = NO;
	sprite->upd_rtn = (void *)&rdb_sprite_upd2;


//** lt ***
	spawn_sprite( BOX );
	sprite->frame = 3;
	fr = sprite->spr_anim_frame_san + sprite->frame;
	rdb->lv = sprite;
	rdb->lv_h = fr->h*4
	sprite->centre_if_scaled = NO;
	sprite->upd_rtn = (void *)&rdb_sprite_upd2;

//** rt ***
	spawn_sprite( BOX );
	rdb->rv = sprite;
	sprite->frame = 4;
	sprite->centre_if_scaled = NO;
	sprite->upd_rtn = (void *)&rdb_sprite_upd2;

//** centre **
	spawn_sprite( BOX );
	rdb->cen = sprite;
 	sprite->frame = 8;
	fr = sprite->spr_anim_frame_san + sprite->frame;
	sprite->centre_if_scaled=NO;
	sprite->upd_rtn = (void *)&rdb_sprite_upd2;
	rdb->cen_w = fr->w*2;
	rdb->cen_h = fr->h*4;


#if 0
//** Top left quad **
	spawn_sprite( DBOX );
	rdb->tl = sprite;
	sprite->gadget = gadget;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->mode = GADGET_INIT;
	sprite->type2 = RDB_TL;
	sprite->frame = 0;
	sprite->transp_rate = transp_rate;
	sprite->depth 					= MIDDLE_DEPTH;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 0;


//** Centre sprite **
	spawn_sprite ( -1 );
	rdb->cen = sprite;
	sprite->depth 					= MIDDLE_DEPTH;
	sprite->tint.b = sprite->tint.g = sprite->tint.r = 0;
	sprite->transp_rate = transp_rate;
	sprite->gadget = gadget;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->mode = GADGET_INIT;
	sprite->type2 = RDB_CEN;

	rdb->x1 = rdb->size.x + sprite->xoffs;
	rdb->x2 = rdb->x1 + rdb->size.w;
	rdb->y1 = rdb->size.y + sprite->yoffs;
	rdb->y2 = rdb->y1 + rdb->size.h;


//** Top right quad **
	spawn_sprite( DBOX );
	rdb->tr = sprite;
	sprite->frame = 1;
	sprite->transp_rate = transp_rate;
	sprite->depth 					= MIDDLE_DEPTH;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 0;
	sprite->gadget = gadget;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->mode = GADGET_INIT;
	sprite->type2 = RDB_TR;
	fr = sprite->spr_anim_frame_san+0;
	rdb->quad_w = fr->w*2;
	rdb->quad_h = fr->h*4;

//** Bot left quad **
	spawn_sprite( DBOX );
	rdb->bl = sprite;
	sprite->frame = 2;
	sprite->transp_rate = transp_rate;
	sprite->depth 					= MIDDLE_DEPTH;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 0;
	sprite->gadget = gadget;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->mode = GADGET_INIT;
	sprite->type2 = RDB_BL;

//** Bot right quad **
	spawn_sprite( DBOX );
	rdb->br = sprite;
	sprite->frame = 3;
	sprite->transp_rate = transp_rate;
	sprite->depth 					= MIDDLE_DEPTH;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 0;
	sprite->gadget = gadget;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->mode = GADGET_INIT;
	sprite->type2 = RDB_BR;

//** Left vert sprite **
	spawn_sprite( -1 );
	rdb->lv = sprite;
	sprite->transp_rate = transp_rate;
	sprite->depth 					= MIDDLE_DEPTH;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 0;
	sprite->gadget = gadget;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->mode = GADGET_INIT;
	sprite->type2 = RDB_LV;

//** Right vert sprite **
	spawn_sprite( -1 );
	rdb->rv = sprite;
	sprite->transp_rate = transp_rate;
	sprite->depth 					= MIDDLE_DEPTH;
	sprite->tint.r = sprite->tint.g = sprite->tint.b = 0;
	sprite->gadget = gadget;
	sprite->upd_rtn = (void *)&rdb_sprite_upd;
	sprite->mode = GADGET_INIT;
	sprite->type2 = RDB_RV;
#endif

}



void update_rdb_gadget( Gadget *gadget )
{
Rdb_gadget *rdb = gadget->rdb_gadget; //[num];
Textman_struct *ts;
Gadget *new_gadget, *gad_store;
WORD box_x1,box_x2,box_y1,box_y2, type, quad_w, quad_h;
Spr_anim_frame_san *fr;	
Sprite *sprite2, *quad;


	if ( gadget->parent )
		{
		if ( (gadget->parent->type == TEXT_STRING_GADGET) || (gadget->parent->type == BUTTON_GADGET) )
			{
			if ( gadget->parent->selected_num != gadget->selected_num )
				{
				// select num on parent has changed, so update the box again

				if ( gadget->parent->type == TEXT_STRING_GADGET )
					ts = gadget->parent->textstring_gadget->textman_struct;
				else
					ts = gadget->parent->button_gadget->textman_struct;

				gadget->selected_num = gadget->parent->selected_num;
				}
			}
		}



	if ( gadget->mode == GADGET_INIT )
		{
		rdb->tint += 5;
		if ( rdb->tint >= rdb->max_tint )
			{
			rdb->tint = rdb->max_tint;
			gadget->mode = GADGET_UPDATE;
			}
		}

#if 0
	if ( gadget->mode == GADGET_KILL )
		{
//		rdb->tint -= 10;
//		if ( rdb->tint <= 0 )
//			{
//			rdb->tint = 0;
			gadget->mode = GADGET_OFF;
			rdb->tl->mode = GADGET_KILL;
			rdb->tr->mode = GADGET_KILL;
			rdb->bl->mode = GADGET_KILL;
			rdb->br->mode = GADGET_KILL;
			rdb->lv->mode = GADGET_KILL;
			rdb->rv->mode = GADGET_KILL;
			rdb->cen->mode = GADGET_KILL;
			printf("kill rdb sprs\n");
//			}


		}
#endif
//	rdb->size.x = rdb->x1 = rdb->size.x


//	rdb->size.x = 0;
//	rdb->size.y = 0;
//	rdb->size.w = 800;
//	rdb->size.h = 800;

//	rdb->x1 = rdb->size.x - ( rdb->size.w/2 );
//	rdb->x2 = rdb->size.x + ( rdb->size.w/2 );

//	rdb->y1 = rdb->size.y - ( rdb->size.h/2 );
//	rdb->y2 = rdb->size.y + ( rdb->size.h/2 );

	rdb->x1 = rdb->size.x;
	rdb->y1 = rdb->size.y;
	rdb->x2 = rdb->x1 + rdb->size.w;
	rdb->y2 = rdb->y1 + rdb->size.h;

	if ( rdb->update==YES ) 
		rdb->update=NO;

	if ( (rdb->size.x != rdb->old_size.x) || (rdb->size.y != rdb->old_size.y) || (rdb->size.w != rdb->old_size.w) || (rdb->size.h != rdb->old_size.h) )
		{
		rdb->old_size = rdb->size;
		rdb->update = YES;
		}

//	FntPrint("xy1=%d %d  xy2=%d %d\n",rdb->x1, rdb->y1, rdb->x2, rdb->y2 );
	

}


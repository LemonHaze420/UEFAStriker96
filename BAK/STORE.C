#if 0
void spawn_rounded_dialog_box_gadget( WORD gadget_num, BYTE *text, WORD x, WORD y, WORD wd, WORD ht, WORD id )
{
Gadget *gadget = gadget_header.gadget_list + gadget_num;
Rounded_dialog_box *box = &gadget->rounded_dialog_box;
Textman_struct *textman_struct;
WORD quad_w,quad_h;
WORD adj_y;
Sprite *quad;
WORD box_x1,box_x2,box_y1,box_y2;


	box->text_id = id;
	box->text = text;
	box->text_x = x;
	box->text_y = y;
	box->font_type = MILFORD_FONT;
	box->justify_type = FONT_CENTERED;
	box->group = 0;
	box->font_logic = milford_wrap_font_logic;


	textman_struct = add_text_string ( box->text_id, box->text , box->font_type, box->font_logic, box->text_x, box->text_y , box->justify_type, box->group, SCREEN_UNIT_WD, gadget );
	adj_y = -( (textman_struct->box_y1 - textman_struct->box_y0)/2);
	adjust_text_string_pos ( textman_struct->tail_sprite, 0, adj_y );
	textman_struct-> box_y0 += adj_y;
	textman_struct-> box_y1 += adj_y;

	box-> x = textman_struct->box_x0;
	box-> y = textman_struct->box_y0;
	box-> w = textman_struct->box_x1 - textman_struct->box_x0;
	box-> h = textman_struct->box_y1 - textman_struct->box_y0;

	box->tint = 0;

	spawn_rounded_dialog_box( gadget );

//*** spawn a gadget to process this wheel ***

	spawn_gadget ( ROUNDED_DIALOG_BOX_GADGET, gadget );

}
#endif





void spawn_dialog_box_gadget( Gadget *gadget, BYTE *text, WORD x, WORD y, WORD wd, WORD ht )
{

Dialog_box *box = &gadget->dialog_box;

	spawn_sprite ( -1 );
	box->head_sprite = sprite;

	sprite->x.w.hi = x;
	sprite->y.w.hi = y;
	sprite->untextured_frame.w = 0;
	sprite->untextured_frame.h = 0;
	sprite->depth 					= MIDDLE_DEPTH;
	sprite->tint.r = 0;
	sprite->tint.g = 0;
	sprite->tint.b = 0;
	sprite->transp_rate = 1;


}

void process_dialog_box_gadget( Gadget *gadget )
{
Dialog_box *box = &gadget->dialog_box;
Textman_struct *textman_struct;
WORD xs,ys,xdiff,ydiff,xd,yd;
WORD adj_y;
BYTE *text;
WORD upd,cnt;

	
	if ( gadget->mode != GADGET_KILL )
		{
		textman_struct = add_text_string ( box->text_id, box->text , box->font_type, box->font_logic, box->text_x, box->text_y , box->justify_type, box->group, SCREEN_UNIT_WD, gadget );
		}
	else
		textman_struct = 0;

	sprite = box->head_sprite;

	if ( textman_struct )
		{
		// textman_strcut contains nz if add_text_string spawned a string.....

		adj_y = -( (textman_struct->box_y1 - textman_struct->box_y0)/2);

		adjust_text_string_pos ( textman_struct->tail_sprite, 0, adj_y );
		textman_struct-> box_y0 += adj_y;
		textman_struct-> box_y1 += adj_y;

		box-> x = textman_struct->box_x0 - (40/2);
		box-> y = textman_struct->box_y0 - (50/2);

		box-> w = textman_struct->box_x1 - textman_struct->box_x0 + 40;
		box-> h = textman_struct->box_y1 - textman_struct->box_y0 + 50;

		xdiff =  box->w - sprite->untextured_frame.w;
		ydiff =  box->h - sprite->untextured_frame.h;

		// set speed to move box
		sprite->xvel2.l = (xdiff * 65536 ) / 8;
		sprite->yvel2.l = (ydiff * 65536 ) / 8;
		}


	if ( gadget->sequence_num == 0 )
		{
		// check for box opening to full size

		sprite->xvel1.l += sprite->xvel2.l;
		sprite->yvel1.l += sprite->yvel2.l;

		if ( (sprite->xvel1.w.hi >= box->w) && ( sprite->xvel2.l>0 ) )
			{
			sprite->xvel1.w.hi = box->w;
			sprite->yvel1.w.hi = box->h;
			sprite->xvel2.l = 0;
			sprite->yvel2.l = 0;
			}

		if ( (sprite->xvel1.w.hi <= box->w) && ( sprite->xvel2.l<0 ) )
			{
			sprite->xvel1.w.hi = box->w;
			sprite->yvel1.w.hi = box->h;
			sprite->xvel2.l = 0;
			sprite->yvel2.l = 0;
			}
		}

	if ( gadget->sequence_num == 2 )
		{

		}
	

	if ( gadget->mode == GADGET_KILL )
		{
		// make box close

		if ( gadget->sequence_num == 0 )
			{
			box -> xvel1.l = -3025000;	// closing speed
			gadget->sequence_num++;
			}

		if ( box->w > box->h )
			{
			sprite->xvel1.l += box->xvel1.l;
			sprite->yvel1.l += ( box->xvel1.l * box->h ) / box->w;
			}
		else
			{
			sprite->xvel1.l += ( box->xvel1.l * box->w ) / box->h;
			sprite->yvel1.l += box->xvel1.l;
			}

		// check for box closed
		if ( sprite->xvel1.l <= 0 )
			{
			box->xvel1.l=0;
			sprite->xvel1.l= sprite->yvel1.l = 0;
		// box has closed
			sprite->active=NO;
			gadget->mode = GADGET_OFF;
			}
		}



	sprite->x.w.hi = box->x;
	sprite->y.w.hi = box->y;
	sprite->untextured_frame.w = sprite->xvel1.w.hi;
	sprite->untextured_frame.h = sprite->yvel1.w.hi;
	sprite->xoffs = -(sprite->untextured_frame.w/2) + (box->w/2);
	sprite->yoffs = -(sprite->untextured_frame.h/2) + (box->h/2);

}
void italic_font_logic ( Sprite *sprite )
{

// check for character deactivated
	if ( sprite->sequence_num == FONT_DYING )
		{
		sprite->transp_rate = 1;
		if ( sprite->tint.r <= 15 )
			{
			sprite->tint.r = 0;
			sprite->active=NO;
			}
		else
			{
			sprite->tint.r-=25;
			}
		sprite->tint.g = sprite->tint.b = sprite->tint.r;
		}


	if ( sprite->timer1 )
		{
		sprite->transp_rate = 1;
		sprite->timer1--;
		sprite->tint.r = sprite->tint.g = sprite->tint.b = 0;
		return;
		}



// character fading up
	if ( sprite->sequence_num == FONT_INIT )
		{
		sprite->transp_rate = -1;
		sprite->tint.r += 15;
		if ( sprite->tint.r > 90 )
			{
			sprite->tint.r = 90;
			sprite->sequence_num = FONT_STAGE1;
			}
		}


// character selectable
	if ( sprite->sequence_num == FONT_STAGE1 )
		{
		//sprite->transp_rate = -1;
		if ( (gadget_header.current_gadget == sprite->group_num) && (main_wheel.active==NO ) )
			{
			sprite->tint.r = ((timer/4)&1)?60:127;
			}
		else
			{
			sprite->tint.r = 127;
			}
		}

	sprite->tint.g = sprite->tint.b = sprite->tint.r;
}




#if 0


WORD stadium_obj_names[]= { rl_MStad_A, rl_MStad_B, rl_MStad_C, rl_MStad_D, rl_MStad_E, 0 };
Gadget *stadsel_gads[10];




void init_stadium_select()
{
	load_m01_file ( STADIUM_OPEN, STADIUM_OPEN_SLOT );
	load_m01_file ( STADIUM_SHUT, STADIUM_SHUT_SLOT );
}




void stadium_select_gadget_upd ( Gadget *gadget )
{
WORD ob,cnt;
SOD *sod, *island_sod;
//WORD val;

	sod = &sod_header->sod_list[ stadium_obj_names[ gadget->preset_val ] ];
	island_sod = &sod_header->sod_list[ rl_Island ];
	sod->user_depth = -20;	// bring stadium forward of it's 'island'


	if ( joy[0].c & PAD_L1 )
		{
		//sod->rot.vy += 50;
		}
	else
		{
		if ( joy[0].c & PAD_L2 )
			{
			//sod->rot.vy -= 50;
			}
		else
			sod->rot.vy -= 20;

		}

	island_sod->rot.vy = sod->rot.vy;
	

}


void update_stadium_select()
{
WORD upd;
Sprite *sprite;
WORD ob,cnt;
SOD *sod;
BYTE *text;

	if ( main_wheel.sequence_num == 0 )
		{
		main_wheel.sequence_num++;

	//** Stadium name **
		gadget = gadget_header.gadget_list + STADIUM_SELECT_GADGETS + 0;
		gadget_init.transp_rate = 0;
		set_gadget_text_attr ( 0,  0, -350, FONT1, 0, FONT_CENTERED, 0 , stadium_names_text_ptr[ share->lang ], italic_font_logic );
		spawn_gadget ( TEXT_LINE_GADGET,  gadget );
  		gadget->upd_rtn = (void *)stadium_select_gadget_upd;
		stadsel_gads[0] = gadget;


	//** Stadium stats **

		gadget = gadget_header.gadget_list + STADIUM_SELECT_GADGETS + 2;
		set_gadget_text_attr ( 1,  0,50, TINY_FONT, -2, FONT_CENTERED, 0 , stadium_stats_text_ptr[ share->lang ] , italic_font_logic );
		set_gadget_box_attr ( 0, 0, -1, -1,  0,0,0 );
		gadget_init.transp_rate = 0;
		gadget_init.xvel1.l = 4350000;
		spawn_gadget ( DIALOG_BOX_GADGET, gadget );
		stadsel_gads[2] = gadget;



		// start expand stadium object
		gadget = gadget_header.gadget_list + STADIUM_SELECT_GADGETS + 0;
		sod = &sod_header->sod_list[ stadium_obj_names[ gadget->preset_val ] ] ;
		start_obj_m01_anim1 ( sod , STADIUM_OPEN_SLOT , MATRIX_ANIM_SINGLE_SHOT + MATRIX_ANIM );

		// start expand island object
		sod = &sod_header->sod_list[ rl_Island ] ;
		start_obj_m01_anim1 ( sod , STADIUM_OPEN_SLOT , MATRIX_ANIM_SINGLE_SHOT + MATRIX_ANIM );
		}


	sod_header->sod_list[ rl_Island ].active = YES;


	gadget = stadsel_gads[ 0 ];


//** activate only the selected stadium **

	for ( cnt=0;cnt<5;cnt++ )
		{
		ob = stadium_obj_names[cnt];

		if ( cnt == gadget->preset_val )
			{
			sod_header->sod_list[ ob ].active = YES;
			}
		else
			{
			sod_header->sod_list[ ob ].active = NO;
			}
		
		}



	if ( main_wheel.active == NO )
		{
		if ( gadget_header.current_gadget == -1 )
			gadget_header.current_gadget = 0;

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

		// adjust stadium name preset
		upd = mod_gad_preset_lr ( stadsel_gads[0] );
		// adjust stadium stats preset
		upd = mod_gad_preset_lr ( stadsel_gads[2] );
		}




	FntPrint("preset=%d\n",gadget->preset_val);

}

void kill_stadium_select()
{
WORD cnt;
SOD *sod;

	kill_gadget ( stadsel_gads[0] );
	kill_gadget ( stadsel_gads[2] );


	if ( main_wheel.active )
		{
		// kill stadium
		sod = &sod_header->sod_list[ stadium_obj_names[ gadget->preset_val ] ] ;
		start_obj_m01_anim1 ( sod, STADIUM_SHUT_SLOT , MATRIX_ANIM_SINGLE_SHOT + MATRIX_ANIM + MATRIX_ANIM_DEACTIVATE );

		// kill island
		sod = &sod_header->sod_list[ rl_Island ] ;
		start_obj_m01_anim1 ( sod, STADIUM_SHUT_SLOT , MATRIX_ANIM_SINGLE_SHOT + MATRIX_ANIM + MATRIX_ANIM_DEACTIVATE );

	}
}

#endif




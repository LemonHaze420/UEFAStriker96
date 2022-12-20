
/********************************************************************
 *																						  *
 *																						  *
 * Font draw functions															  *
 *																						  *
 *																						  *
 ********************************************************************/


/********************************************************************
 *																						  *
 * Includes																			  *
 *																						  *
 ********************************************************************/

#include "incs.h"

#ifdef PSX
#include "..\..\striker\src\anim.def"
#include	"main.h"
#include "datafile.h"
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include	"shell.h"
#include "hardware.h"
#include	"font.h"
#endif


/********************************************************************
 *																						  *
 * Defines																			  *
 *																						  *
 ********************************************************************/

#define MAX_TEXT_STRINGS_ACTIVE 32	// in a single group
#define TEXT_STRING_GROUPS 8

// special font chars
#define FONT_Y_SPACE	  254
#define FONT_CR		  253
#define FONT_ENDSTRING 252



/********************************************************************
 *																						  *
 * Prototypes																		  *
 *																						  *
 ********************************************************************/

void add_font ( Font_data *font_data, UWORD font_num );
void init_fonts();
void spawn_text_string ( Textman_struct *textman );
void unspawn_text_string( Sprite *sprite );
void unspawn_all_text_strings();
void apply_to_sprite_string( Sprite *sprite );
void get_group_sprite ( WORD group );
void apply_seqnum_to_sprite_string( Sprite *sprite );
void milford_font_logic ( Sprite *sprite );
void start_text_manager( WORD text_strings );
void adjust_text_string_pos ( Sprite *sprite, WORD xamount, WORD yamount );

/********************************************************************
 *																						  *
 * Vars																				  *
 *																						  *
 ********************************************************************/

typedef struct
{
	WORD current_spawned_string;
	Sprite *spawned_strings_list[ MAX_TEXT_STRINGS_ACTIVE ];
}Text_group;

Text_group text_group [ TEXT_STRING_GROUPS ];
Sprite *head_sprite;



WORD tslot_poses[ TEXTMAN_TSLOTS ];

Font *resident_fonts[ MAX_FONTS ];

Font_data milford_font_data = {

	MILFT,// graphic type
	26,		// start of alpha lower
	0,		// start of alpha upper
	52,	// start of numeric
	65,		// comma
	64,		// stop
	62,		// exclamation
	68,		// double quote
	0,			// single quote
	73,		// dollar
	0,		// pound
	0,		// hat
	71,	// ampersand
	74, 	//	asterisk
	66,	// open bracket
	67,	// close bracket
	69,	// plus
	70,	//	minus
	72,	// hash
	63,	// question mark
	134,		// colon
	79,	// return 
	78,	// backspace
	77,	// space
	133,	// dummy space
	2,		// x space between characters was 2
	46,	// y space between characters

//	11,18,// set frame n to width x
	134, 6,
	0,0,
};

Font_data tiny_font_data = {

	TYFNT,// graphic type
	0,		// start of alpha lower
	0,		// start of alpha upper
	41,	// start of numeric
	29,	// comma
	28,	// stop
	26,	// exclamation
	32,	// double quote
	0,		// single quote
	37,	// dollar
	0,		// pound
	0,		// hat
	0,		// ampersand
	38, 	//	asterisk
	30,	// open bracket
	31,	// close bracket
	33,	// plus
	34,	//	minus
	36,	// hash
	27,	// question mark
	0,		// colon
	0,		// return 
	0,		// backspace
	0,		// space
	50,	// dummy space
	2,		// x space between characters
	32,	// y space between characters

//	11,18,// set frame n to width x
//	15,24,
	0,0,
};


Font_data title_font_data = {

	TITLFT,// graphic type
	0,		// start of alpha lower
	0,		// start of alpha upper
	0,	// start of numeric
	0,	// comma
	0,	// stop
	0,	// exclamation
	0,	// double quote
	0,		// single quote
	0,	// dollar
	0,		// pound
	0,		// hat
	0,		// ampersand
	0, 	//	asterisk
	0,	// open bracket
	0,	// close bracket
	0,	// plus
	0,	//	minus
	0,	// hash
	0,	// question mark
	27,		// colon
	0,		// return 
	0,		// backspace
	0,		// space
	26,	// dummy space
	2,		// x space between characters
	32,	// y space between characters

//	0, 20,  //a
//	2, 19,  //c
//	4,17,   //e
//	11, 16, //l
//	18, 21, //s
//	19, 16, //t
//	20, 22, //u
//	22, 20, //w
//	24, 17, //y
//	11,18,// set frame n to width x
//	15,24,
	0,0,
};


Font_data lolly_font_data = {

	LOLFT,// graphic type
	0,		// start of alpha lower
	0,		// start of alpha upper
	0,	// start of numeric
	0,	// comma
	0,	// stop
	0,	// exclamation
	0,	// double quote
	0,		// single quote
	0,	// dollar
	0,		// pound
	0,		// hat
	0,		// ampersand
	0, 	//	asterisk
	0,	// open bracket
	0,	// close bracket
	0,	// plus
	0,	//	minus
	0,	// hash
	0,	// question mark
	27,		// colon
	0,		// return 
	0,		// backspace
	0,		// space
	28,	// dummy space
	1,		// x space between characters
	32,	// y space between characters

	0,0,
};








/********************************************************************
 *																						  *
 * Prototypes																		  *
 *																						  *
 ********************************************************************/

void setup_font_lookup_list ( Font *font );
void init_text_spawner();
void update_text_manager();


void init_fonts()
{
WORD cnt;

	for ( cnt=0;cnt < MAX_FONTS; cnt++ )
		{
		resident_fonts[cnt] = (Font *)0;
		}


	init_text_spawner();

	add_font ( &tiny_font_data, TINY_FONT );
	add_font ( &title_font_data, TITLE_FONT );
	add_font ( &lolly_font_data, LOLLY_FONT );
	add_font ( &milford_font_data, MILFORD_FONT );

}

void add_font ( Font_data *font_data, UWORD font_num )
{
Font *font;

	resident_fonts[font_num] = allocate_mem ( 0, sizeof ( Font ) );

	font = resident_fonts[font_num];
	
	font->font_data = font_data;
	setup_font_lookup_list ( font );

//	printf("fontdata=%x\n",font->font_data);
}


void setup_font_lookup_list ( Font *font )
{
WORD cnt;
Spr_anim_frame_san *frame;

	for ( cnt=0;cnt<255;cnt++ )
		font->lookup_list[cnt] = 255;

	for ( cnt=0;cnt<26;cnt++ )
		{
		font->lookup_list[ 'A'  + cnt ] = font->font_data->alpha_upper + cnt;
		font->lookup_list[ 'a' + cnt ] = font->font_data->alpha_lower + cnt;
		}

	for ( cnt=0;cnt<10;cnt++ )
		{
		font->lookup_list[ '0' + cnt ] = font->font_data->numeric + cnt;
		}

	font->lookup_list[ ',' ] = font->font_data->comma;
	font->lookup_list[ '.' ] = font->font_data->stop;
	font->lookup_list[ '!' ] = font->font_data->exclamation;
	font->lookup_list[ '"' ] = font->font_data->double_quote;
	font->lookup_list[ 39  ] = font->font_data->single_quote;
	font->lookup_list[ '$' ] = font->font_data->dollar;
	font->lookup_list[ 'œ' ] = font->font_data->pound;
	font->lookup_list[ '^' ] = font->font_data->hat;
	font->lookup_list[ '&' ] = font->font_data->ampersand;
	font->lookup_list[ '*' ] = font->font_data->asterisk;
	font->lookup_list[ '(' ] = font->font_data->open_bracket;
	font->lookup_list[ ')' ] = font->font_data->close_bracket;
	font->lookup_list[ '+' ] = font->font_data->plus;
	font->lookup_list[ '-' ] = font->font_data->minus;
	font->lookup_list[ '#' ] = font->font_data->hash;
	font->lookup_list[ '?' ] = font->font_data->question_mark;
	font->lookup_list[ ':' ] = font->font_data->colon;
	font->lookup_list[ '' ] = font->font_data->backspace;	//ascii 27
	font->lookup_list[ 'Ù' ] = font->font_data->ret;			//ascii 217
	font->lookup_list[ 'ð' ] = font->font_data->space;			//ascii 240
	font->lookup_list[ ' ' ] = font->font_data->dummy_space;
	font->lookup_list[ 10  ] = FONT_CR;
	font->lookup_list[ 0   ] = FONT_ENDSTRING;



//adjust widths of any chars

  
	cnt=0;
	while(1)
		{
		if ( font->font_data->adjust[ cnt ].wd )
			{
			frame = get_sprite_frame_addr ( font->font_data->graphic_type );
			frame += font->font_data->adjust[cnt].fr;
			frame->real_w = font->font_data->adjust[cnt].wd;
//			frame->w = 26; //font->font_data->adjust[cnt].wd;
//			printf("fr=%d wd=%d\n",font->font_data->adjust[cnt].fr,frame->real_w);
			cnt++;
			}
		else
			break;
		}
		

} 





//WORD temp_frame_offs[]={ 0,-12,0,-14,-2,-12,0 };
WORD temp_frame_offs[]={ -1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0 };



void init_text_spawner()
{
WORD gr,str;
Text_group *tg;

		tg = text_group;
		for ( gr=0;gr < TEXT_STRING_GROUPS; gr++ )
			{
			tg->current_spawned_string = 0;

			for ( str=0;str < MAX_TEXT_STRINGS_ACTIVE; str++ )
				tg->spawned_strings_list[ str ]  = 0;
			tg++;
			}


	
}


/*************************************
 *												 *
 * Spawn a text string					 *
 *												 *
 * 0,0 = centre of screen				 *
 *												 *
 *************************************/

void spawn_text_string ( Textman_struct *textman )
{
Font *font = resident_fonts[ textman->font_type ];
Spr_anim_frame_san *frame;
UBYTE fr;
WORD first_char_x;
WORD start_x;
WORD total_sprite_cnt;	// total number of sprites spawned
WORD line_sprite_cnt;	// sprites spawned on a line
Sprite *sprite_store;
Text_group *tg = &text_group[ textman->spawn_group];
WORD temp;
WORD head_sprite_xoffs,head_sprite_yoffs;
WORD y_adj;
WORD space_chr;
Sprite *new_sprite;
WORD cnt=0;

WORD x = textman->x;
WORD y = textman->y;
WORD wd;

BYTE *string = &textman->string[0];



//	printf("going to spawn '%s'\n",string );

											 
// tlxy , brxy = boxed area occupied by font

//	x += SCREEN_UNIT_WD/2;
//	y += SCREEN_UNIT_HT/2;

	head_sprite_xoffs=0;
	head_sprite_yoffs=0;

	textman->x1 = 32767;
	textman->x2 = -32767;
	textman->y1 = 32767;
	textman->y2 = -32767;


	first_char_x = x;
	total_sprite_cnt=0;

	line_sprite_cnt=0;

	sprite = 0;
	while ( 1 )
		{

		fr = font->lookup_list [ *string ];

		if ( fr < 242 )
			{

			#if DEBUG
			if ( sprite->frame == 255 )
 				display_error ( "unknown font character\n");
			#endif


			if ( total_sprite_cnt == 0 )
				{
				spawn_sprite ( font->font_data->graphic_type );
				head_sprite = sprite;
				sprite->prev = 0;	// no sprites before this one , therefore this is the head sprite

				frame = sprite->spr_anim_frame_san + fr;
				head_sprite_xoffs = frame->xoffs;
				head_sprite_yoffs = frame->yoffs;
				}
			else
				{
				sprite = spawn_sprite_after ( font->font_data->graphic_type );	// store addr of last sprite spawned in this sprite struct.
				frame = sprite->spr_anim_frame_san + fr;
				}


			
			sprite->frame = fr;

			if ( fr == font->font_data->dummy_space )
				{
				sprite->display=NO;
				}

				

			sprite->gadget = textman->gadget;

			if ( textman->font_logic )
				sprite->upd_rtn = textman->font_logic;

			


			sprite->x.w.hi = x;
			sprite->y.w.hi = y + (frame->yoffs*SCREEN_UNIT_HT)/share->screen_yres;
			sprite->depth = NEAREST_DEPTH;
			sprite->transp_rate = -1;
			sprite->group_num = textman->spawn_group;
			sprite->textman = textman;



			x +=  ((frame->real_w*SCREEN_UNIT_WD)/share->screen_xres ) + font->font_data->x_gap_size;



			temp = sprite->y.w.hi + ((frame->h*SCREEN_UNIT_HT)/share->screen_yres);

			if ( textman->y2 < temp )
				textman->y2 = temp;
			
			if ( textman->y1 > sprite->y.w.hi )
				textman->y1 = sprite->y.w.hi;

			total_sprite_cnt++;
			line_sprite_cnt++;
			//tail_font_sprite = sprite;
			}
		else
			{

			if ( (fr == FONT_CR ) || (fr == FONT_ENDSTRING) )
				{

				if ( textman->justify_type & FONT_XC )
					{

					// adjust every sprite from this one to the beginning

					//start_x = (right_edge_pos-x)/2;
					start_x = (0-x)/2;

					sprite_store = sprite;
													 
					// this is the rightmost sprite in the text string


					while( line_sprite_cnt )
					  	{
						sprite->x.w.hi += start_x;

						frame = sprite->spr_anim_frame_san + sprite->frame;


//						temp = sprite->x.w.hi  + ((frame->w*SCREEN_UNIT_WD)/share->screen_xres );
						temp = sprite->x.w.hi  + ((frame->real_w*SCREEN_UNIT_WD)/share->screen_xres );

						if ( textman->x1 >= sprite->x.w.hi )
							textman->x1 = sprite->x.w.hi;
						if ( temp >= textman->x2 )
							textman->x2 = temp;

						sprite = sprite->prev;
						line_sprite_cnt--;
						}

					// this is the leftmost sprite in the text string

					sprite = sprite_store;
					}

				if ( textman->justify_type & FONT_RJUST )
					{
					wd = start_x-x;
					start_x = first_char_x + ((0-x)/1);
					sprite_store = sprite;
													 
					// this is the rightmost sprite in the text string

					while( line_sprite_cnt )
					  	{
						sprite->x.w.hi += start_x; //(x/2);

						frame = sprite->spr_anim_frame_san + sprite->frame;
						//temp = sprite->x.w.hi  + ((frame->w*SCREEN_UNIT_WD)/share->screen_xres );
						temp = sprite->x.w.hi  + ((frame->real_w*SCREEN_UNIT_WD)/share->screen_xres );

						if ( textman->x1 >= sprite->x.w.hi )
							textman->x1 = sprite->x.w.hi;

						if ( temp >= textman->x2 )
							textman->x2 = temp;


						sprite = sprite->prev;
						line_sprite_cnt--;
						}

					// this is the leftmost sprite in the text string

					sprite = sprite_store;
					}


				if ( textman->justify_type & FONT_NORMAL )
					{
					wd = start_x-x;
					start_x = first_char_x; //first_char_x + ((0-x)/1);
					sprite_store = sprite;
													 
					// this is the rightmost sprite in the text string

					while( line_sprite_cnt )
					  	{
						sprite->x.w.hi += start_x; //(x/2);

						frame = sprite->spr_anim_frame_san + sprite->frame;
						//temp = sprite->x.w.hi  + ((frame->w*SCREEN_UNIT_WD)/share->screen_xres );
						temp = sprite->x.w.hi  + ((frame->real_w*SCREEN_UNIT_WD)/share->screen_xres );

						if ( textman->x1 >= sprite->x.w.hi )
							textman->x1 = sprite->x.w.hi;
						if ( temp >= textman->x2 )
							textman->x2 = temp;

						
						

						sprite = sprite->prev;
						line_sprite_cnt--;
						}

					// this is the leftmost sprite in the text string

					sprite = sprite_store;
					}


				line_sprite_cnt=0;
				x = first_char_x;
				y += font->font_data->y_gap_size;
				}
			}

//		sprite->x.w.hi -= head_sprite_xoffs;
//		sprite->y.w.hi -= head_sprite_yoffs;


		string++;

		if ( fr == FONT_ENDSTRING )
			{

#if 0
			if ( (textman->justify_type & FONT_YC) )
				{
				sprite_store = sprite;

				y_adj = ( textman->y2 - textman->y1 )/2;

				while ( sprite )
					{
					//sprite->y.w.hi -= y_adj;
					sprite = sprite->prev;
					}
				textman->y1 -= y_adj;
				textman->y2 -= y_adj;
				sprite = sprite_store;
				}
#endif





//			temp = y + ((frame->h*SCREEN_UNIT_HT)/share->screen_yres);
//			if ( textman->y2 < temp )
//				textman->y2 = temp;


			break;


			}
		cnt++;
		}

			
}

void unspawn_text_string( Sprite *sprite_ptr )
{
WORD cnt;
POLY_GT4 *poly;
// unspawn all sprites from this sprite to all it's previous sprites

	cnt=0;
	while(1)
		{
		sprite_ptr->active = NO;

		cnt++;
		 if ( sprite_ptr->prev == 0)
		 	break;
		sprite_ptr = sprite_ptr->prev;
		}

}



void unspawn_text_group( WORD spawn_group )
{
Sprite *sprite;
WORD gr;
Text_group *tg = &text_group[spawn_group];

	display_error ( "illegal call to unspawn text group" );
	while ( tg-> current_spawned_string )
		{
		tg->current_spawned_string--;
		unspawn_text_string ( tg->spawned_strings_list[ tg->current_spawned_string ] );
		}	

}

void adjust_text_string_pos ( Sprite *sprite, WORD xamount, WORD yamount )
{
	
	while ( 1 )
		{
		sprite->x.w.hi += xamount;
		sprite->y.w.hi += yamount;
		if ( (sprite = sprite->prev) == 0 )
			break; 
		}

}




void apply_to_sprite_string( Sprite *sprite )
{
Sprite *prev;

	prev = sprite;

	while ( 1 )
		{
		prev->upd_rtn 		 = sprite->upd_rtn;
		prev->xvel1.l 		 = sprite->xvel1.l;
		prev->yvel1.l 		 = sprite->yvel1.l;
		prev->timer1 		 = sprite->timer1;
		prev->timer2 		 = sprite->timer2;
		prev->transp_rate  = sprite->transp_rate;
		prev->tint 			 = sprite->tint;
		prev->sequence_num = sprite->sequence_num;

		prev = sprite->prev;
		if ( prev == 0 )
			break;



		}

}


void apply_seqnum_to_sprite_string( Sprite *sprite )
{
//Sprite *prev;
WORD num;

	num = sprite->sequence_num;

//	printf("sprite=%x\n",sprite );


	while ( 1 )
		{

		sprite->sequence_num = num;

		sprite = sprite->prev;
		if ( sprite == 0 )
			break;
		}
}


void get_group_sprite ( WORD group )
{
// set SPRITE to first sprite in the chosen group
	sprite = text_group [ group ].spawned_strings_list[0];
}






/******************************************************************
 *
 * Text string manager
 *
 ******************************************************************/

Textman_header textman_header;




void start_text_manager( WORD text_strings )
{
WORD cnt;
	add_process ( update_text_manager );

	textman_header.max_text_strings = text_strings;
	textman_header.text_string_list = 
				allocate_mem ( 0, sizeof ( Textman_struct ) * text_strings );

	for ( cnt=0; cnt< text_strings; cnt++ )
		{
		textman_header.text_string_list[ cnt ].active =  0;   
		textman_header.text_string_list[ cnt ].string[0] =  0;   
		textman_header.text_string_list[ cnt ].tail_sprite = 0;
		}

	textman_header.slot_cnt=0;
}

void update_text_manager()
{					 
Textman_struct *text;

WORD cnt = textman_header.max_text_strings;

//	FntPrint("update textman\n");
	text = textman_header.text_string_list;

	while ( cnt ) 
		{
		if ( (text->active) && (text->to_die) )
			{
			text->active = NO;
			sprite = text->tail_sprite;
			sprite->sequence_num = FONT_DYING;
			apply_seqnum_to_sprite_string ( sprite );
			}
		text->to_die = YES;		// set all strings to_die
		cnt--;
		text++;
		}
}



void set_textman_slot( WORD slot_pos, WORD num_slots )
{

	tslot_poses[ slot_pos ] = textman_header.slot_cnt;
	textman_header.slot_cnt += num_slots; // reserve this num slots at [slot_pos]

}





void milford_font_logic ( Sprite *sprite )
{
WORD die_speed,spd;

/******************
 *
 * Additive font
 *
 *******************/

// check for character deactivated

//	FntPrint("seq=%d   ",sprite->sequence_num);

	if ( sprite->sequence_num == FONT_DYING )
		{

		if ( sprite->textman->die_speed == 0 )
			{
			// If die_speed set to 0, font disappears instantly
			sprite->tint.r = 255;
			sprite->display = NO;
			sprite->active=NO;
			return;
			}
		else
			{
			sprite->tint.g-=sprite->textman->die_speed;
			sprite->tint.b-=sprite->textman->die_speed;
			sprite->tint.r-=sprite->textman->die_speed;
			}

		if ( sprite->tint.r > 220 )
			{
			sprite->tint.r = sprite->tint.g = sprite->tint.b = 0;
			sprite->active=NO;
			}
		}


// character drawn for first time
	if ( sprite->sequence_num == FONT_INIT )
		{

		sprite->tint.r = sprite->tint.g = sprite->tint.b = 50; //make_random_number(30);
		sprite->transp_rate = 1;
		sprite->sequence_num = FONT_STAGE1;
		sprite->xscale = 256;

		sprite->timer1=make_random_number(8);
		}


//** Character fading up **
	if ( sprite->sequence_num == FONT_STAGE1 )
		{
		sprite->tint.r += sprite->textman->fade_up_speed;

		if ( sprite->textman->fade_up_speed == 0 )
			{
			// Fade up instantly
			sprite->tint.r = 128;
			}

		if ( sprite->tint.r >= 128 )
			{
			sprite->tint.r = 128;
			sprite->sequence_num = FONT_STAGE2;
			sprite->sequence_num = FONT_STAGE3; // bypass stage 2...

			}


		}



	if ( (sprite->sequence_num==FONT_STAGE1) || (sprite->sequence_num==FONT_STAGE2) || (sprite->sequence_num == FONT_STAGE3) )
		{
		// flash font if selected


		spd = frame_update==1?16:8;
		if ( ( ( ((flash_counter/spd)&1)==0) ) && ( sprite->group_num == shell->current_item ) )
			{
			sprite->tint.r = 40;
			sprite->sequence_num = FONT_STAGE3;
			}
		else
			{
			if (sprite->sequence_num == FONT_STAGE3)
				sprite->tint.r = 120;
			}
		}


	sprite->tint.g = sprite->tint.b = sprite->tint.r;


}

void milford_font_logic_sub ( Sprite *sprite )
{
WORD die_speed,spd;

/******************
 *
 * Additive font
 *
 *******************/

// check for character deactivated

//	FntPrint("seq=%d   ",sprite->sequence_num);

	if ( sprite->sequence_num == FONT_DYING )
		{

		if ( sprite->textman->die_speed == 0 )
			{
			// If die_speed set to 0, font disappears instantly
			sprite->tint.r = 221;
			}
		else
			{
			sprite->tint.g-=sprite->textman->die_speed;
			sprite->tint.b-=sprite->textman->die_speed;
			sprite->tint.r-=sprite->textman->die_speed;
			}

		if ( sprite->tint.r > 220 )
			{
			sprite->tint.r = sprite->tint.g = sprite->tint.b = 0;
			sprite->active=NO;
			}
		}


// character drawn for first time
	if ( sprite->sequence_num == FONT_INIT )
		{

		sprite->tint.r = sprite->tint.g = sprite->tint.b = 16;
		sprite->transp_rate = 2;
		sprite->sequence_num = FONT_STAGE1;
		sprite->xscale = 256;

		sprite->timer1=make_random_number(8);
		}


//** Character fading up **
	if ( sprite->sequence_num == FONT_STAGE1 )
		{
		sprite->tint.r += (6 * frame_update)/2;

		if ( sprite->tint.r > 48 )
			{
			sprite->tint.r = 48;
			sprite->sequence_num = FONT_STAGE2;
			}


		}



	sprite->tint.g = sprite->tint.b = sprite->tint.r;

}



void set_rdb_size_from_tm( Rdb_gadget *rdb, Textman_struct *tm )
{
/** Get dimensions from textmanager string
    and set rdb box to this position and size **/

	rdb->size.x = tm->x1 - (6*4);
	rdb->size.y = tm->y1 - (2*4);
	rdb->size.w = tm->x2 - tm->x1 + (12*4);
	rdb->size.h = tm->y2 - tm->y1 + (6*4);

	rdb->x1 = tm->x1 - (6*4);
	rdb->y1 = tm->y1 - (2*4);
	rdb->x2 = rdb->x1  +  tm->x2 - tm->x1 + (12*4);
	rdb->y2 = rdb->y1  +  tm->y2 - tm->y1 + (6*4);


}




Textman_struct *add_text_string ( WORD slot, UBYTE *string, WORD font_type, void *font_logic, WORD x, WORD y, WORD justify_type, WORD spawn_group, Gadget *gadget, WORD fade_up_speed, WORD die_speed )
{
Textman_struct *text;
Font *font;
WORD spawn,xd,yd;
Sprite *prev;


	if ( string == 0 )
		return 0;

// check all strings with active=YES and to_die=YES
	
	text = textman_header.text_string_list + slot;

	spawn = NO;

	font = resident_fonts[ font_type ];

	if ( text->active == YES )
		{
		text->to_die = NO;

		if ( my_strcmp ( string, text->string ) )
			{
			// string already at this slot doesn't match requested one...

			// so kill this string
			sprite = text->tail_sprite;

			if ( sprite )
				{
				sprite->sequence_num = FONT_DYING;
				apply_seqnum_to_sprite_string ( sprite );
				}

			// and spawn a new one
			spawn = YES;
			}


	//*** check if text string has moved ***

		if ( spawn==NO )
			{
			if ( (text->x != x) || (text->y != y) )
				{
				// get distance text needs to move
				xd = x - text->x;
				yd = y - text->y;

				sprite = text->tail_sprite;

				// move whole string by this amount
				adjust_text_string_pos ( sprite , xd, yd );

				text->x1 += xd;
				text->x2 += xd;
				text->y1 += yd;
				text->y2 += yd;

				text->x = x;
				text->y = y;
				}
			}
		}
	else
		{
		spawn = YES;
		}



	if ( spawn )
		{
		// text string is required to be spawned
		text->active = YES;
		text->to_die = NO;
		text->font_logic = font_logic;
		text->font_type = font_type;
		text->x = x;
		text->y = y;
		text->justify_type = justify_type;
		text->spawn_group = spawn_group;
		text->rt_edge_pos = SCREEN_UNIT_WD;
		text->gadget = gadget;
		text->die_speed = die_speed;
		text->fade_up_speed = fade_up_speed;

		strcpy ( text->string, string );

		spawn_text_string ( text );

		text->tail_sprite = sprite;
		text->head_sprite = head_sprite;

		}





	return text;

}









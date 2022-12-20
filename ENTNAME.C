/****************************************************
 *																	 *
 *	 																 *
 * Enter name													 *
 *	 																 *
 *																	 *
 ****************************************************/


#include "incs.h"

#ifdef PSX
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include	"shell.h"
#include	"pad.h"
#include "anim.def"
#include "font.h"
#include "hardware.h"
#include "text.h"
#endif



/****************************************************
 *																	 *
 *	 Defines														 *
 *																	 *
 ****************************************************/

#define ENT_PIXEL_WD 340
#define ENTNAME_STRING_LEN 64
#define EN_GRID_WD	13
#define EN_GRID_HT	6
#define EN_GRID_XSP	44
#define EN_GRID_YSP	55
#define EN_GRID_X		-264
#define EN_GRID_Y		-300+60

#define EN_NAME_X	0 		// position of name currently being entered
#define EN_NAME_Y	(12*4)+60

#define EN_SPACE  	240
#define EN_RETURN 	217
#define EN_BACKSPACE 27


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

void update_enter_name();
void entername_sprite_logic( Sprite *sprite );
void kill_enter_name();

/****************************************************
 *																	 *
 *  Vars															 *
 *																	 *
 ****************************************************/

//void *entername_process;
WORD en_max_len;

//Textstring_gadget entname_gad;

BYTE entname_namestring[ ENTNAME_STRING_LEN ];
BYTE *entname_namestring_ptr = entname_namestring;
Sprite *en_cursor, *en_name_cursor;
BYTE grid_text[]= { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!$&-+#?.,<>()ðÙ" };

WORD en_cx, en_cy;
WORD en_cur_char;

Rdb_gadget en_box1, en_box2;
WORD en_name_dying;
void (*exit_func_ptr)();
BYTE *src_name_string;
WORD ent_wd;



BYTE *entname_text;



void init_enter_name()
{
/** Set global 'text_buffer' with text **/
Font *font = resident_fonts[ MILFORD_FONT ];

WORD type = font->font_data->graphic_type;
WORD x,y,wd;
UBYTE *ptr;
Spr_anim_frame_san *frame;
WORD cnt;
UBYTE chr;
WORD max_len = 16;

	en_max_len = max_len;
	src_name_string = entname_text;

	joy[0].db = joy[0].r = 
	joy[1].db = joy[1].r = 
	joy[2].db = joy[2].r = 
	joy[3].db = joy[3].r = 0;

	en_cx = en_cy = 0;

	//exit_func_ptr = addr;

//*** Clear name string ***

	for ( cnt=0;cnt< ENTNAME_STRING_LEN; cnt++ )
		{
		entname_namestring[cnt]=0;
		}
	en_cur_char=0;


	// copy name string for edit
	if ( src_name_string )
		{
//		strcpy ( entname_namestring, src_name_string );
		entname_namestring[0]=0;
		en_cur_char = 0; //strlen ( src_name_string );
		}


	y=0;
	ptr = grid_text;
	for ( y=0;y<EN_GRID_HT;y++ )
		{
		for ( x=0;x<EN_GRID_WD;x++ )
			{
			spawn_sprite ( type );
			sprite->upd_rtn = (void *)entername_sprite_logic;

			sprite->frame = font->lookup_list[ *ptr ]; 


			// get width in UNITS
			frame = sprite->spr_anim_frame_san + sprite->frame;

			wd = (frame->real_w*SCREEN_UNIT_WD)/screen_xres;

			sprite->x.w.hi = EN_GRID_X + ( x * EN_GRID_XSP ) - (wd/2);
			sprite->y.w.hi = EN_GRID_Y + ( y * EN_GRID_YSP );
			sprite->use_frame_offs = YES;
			sprite->transp_rate = 1;
			ptr++;
		
			}
		}


//	entername_process = add_process( update_enter_name );


//	set_textstring_gadget( 0, &entname_gad, EN_NAME_X, EN_NAME_Y, &entname_namestring_ptr ,0, MILFORD_FONT,milford_font_logic, FONT_XC, 50, -2 );

	en_name_dying = NO;

	en_cursor = spawn_sprite( -1 );
	sprite->upd_rtn = (void *)&entername_sprite_logic;

	sprite->untextured_frame.w = 38;
	sprite->untextured_frame.h = 60;
	sprite->transp_rate = 1;
	sprite->tint.r = 200;
	sprite->tint.g = 50;
	sprite->tint.b = 50;

	joy[0].r_delay = 2;

//	en_cur_char=0;

	shell->keyboard_active = YES;




	spawn_sprite ( -1 );
	sprite->upd_rtn = (void *)&entername_sprite_logic;
	sprite->untextured_frame.w = 16;
	sprite->untextured_frame.h = 8;
	sprite->tint.r = 
	sprite->tint.g = 
	sprite->tint.b = 200;
	en_name_cursor = sprite;





// box behind grid

	set_rdb_gadget( 0, &en_box1, 0-(600/2),-116+60-(336/2), 600, 336,-1 );

	set_rdb_gadget( 0, &en_box2, 0-(600/2), 96+60-(64/2), 600,64, -1 );


	set_title_lolly( edit_name_txt[config->language], game_types_text[ config->language ][ share->game_type ]  );


}



void update_enter_name()
{
WORD pos,wd;
Textman_struct *ts;
UBYTE chr;



	//FntPrint("text='%s'\n",entname_namestring_ptr );

	if ( cjoy->db & PAD_TRI )
		{
		src_name_string=0;
		shell->module_abort=YES;
		return;
		}

	if ( cjoy->db & PAD_STA )
		{
		
		if ( ((en_cx == (EN_GRID_WD-1) ) && (en_cy == (EN_GRID_HT-1) ))  )
			{
			shell->module_abort=YES;
			return;
			}
		else
			{
			en_cx = EN_GRID_WD-1;
			en_cy = EN_GRID_HT-1;
			}
		}

	if ( cjoy->r & PAD_LT )
		{
		sfxSpot( HIT1, 30 );
		en_cx--;
		if ( en_cx < 0 )
			en_cx = EN_GRID_WD-1;
		}

	if ( cjoy->r & PAD_RT )
		{
		sfxSpot( HIT1, 30 );
		en_cx++;
		if ( en_cx == EN_GRID_WD )
			en_cx = 0;
		}

	if ( cjoy->r & PAD_UP )
		{
		sfxSpot( HIT1, 30 );
		en_cy--;
		if ( en_cy < 0 )
			en_cy = EN_GRID_HT-1;
		}

	if ( cjoy->r & PAD_DN )
		{
		sfxSpot( HIT1, 30 );
		en_cy++;
		if ( en_cy == EN_GRID_HT )
			en_cy = 0;
		}


//	FntPrint("cx=%d\n",en_name_cursor->x.w.hi );

	if ( cjoy->db & (PAD_X|PAD_SQU|PAD_CIR)  )
		{
		pos = ( en_cy * EN_GRID_WD ) + en_cx;

		chr = grid_text[pos];

		if ( (chr == EN_SPACE) && (en_cur_char < en_max_len ) && ( ent_wd < ENT_PIXEL_WD ) )
			{
			entname_namestring[ en_cur_char ] = ' ';
			entname_namestring[ en_cur_char+1 ] = 0;
			en_cur_char++;

			}
		else
			{
			if ( chr == EN_BACKSPACE )
				{
				if ( en_cur_char != 0 )
					en_cur_char--;
				entname_namestring [ en_cur_char ] = 0;

				}
			else
				{
				if ( chr == EN_RETURN)
					{

					if ( en_cur_char != 0 )
						{
						shell->module_abort=YES;
//						kill_enter_name();	// cant exit if no name has been entered
						}
					return;
					}
				else
					{

					if ( ( en_cur_char < en_max_len ) && ( ent_wd<ENT_PIXEL_WD) )
						{
						sfxSpot( BUTVOC, 30 );
						entname_namestring[ en_cur_char ] = grid_text[ pos ];
						entname_namestring[ en_cur_char+1 ] = 0;
						en_cur_char++;
						}
					}
				}
			}

		}


	en_cursor->x.w.hi = EN_GRID_X + (en_cx * EN_GRID_XSP ) - 20;
	en_cursor->y.w.hi = EN_GRID_Y + (en_cy * EN_GRID_YSP ) + (7*4);
	en_cursor->tint.r = ((flash_counter/8)&1)?90:190;

//	ts = entname_gad.textman_struct;
	ts = add_text_string ( tslot_poses[MAIN_TSLOT]+30, entname_namestring_ptr , MILFORD_FONT, milford_font_logic, EN_NAME_X, EN_NAME_Y, FONT_XC, -2, 0, 0,0,0 );
	if ( en_cur_char==0)
		wd=0;
	else
		wd = ts->x2 - ts->x1;

	ent_wd = wd;
	FntPrint("wd=%d\n",wd);

	if ( ts )
		{
		en_name_cursor->x.w.hi = ts->x2 + 4; 
		}
	else
		{
		en_name_cursor->x.w.hi = 4;
		}

	en_name_cursor->y.w.hi = EN_NAME_Y + (17*4); 
	en_name_cursor->display = ((flash_counter/8)&1)?YES:NO;

}



void kill_enter_name()
{

	kill_rdb_gadget( en_box1.gadget );
	kill_rdb_gadget( en_box2.gadget );
//	kill_gadget( entname_gad.gadget );
//	kill_process( entername_process );
	shell->keyboard_active=NO;
	en_name_dying=YES;



	// copy new namestring to src

	if ( src_name_string )
		{
		strcpy ( src_name_string, entname_namestring );
		}

#if 0
	if ( exit_func_ptr )
		{
		exit_func_ptr();	// function to call on exiting entername
		#if DEBUG
		printf("called...exit\n");
		#endif
		}
#endif
	
}



void entername_sprite_logic( Sprite *sprite )
{

	if ( en_name_dying == YES )
		{
		kill_sprite ( sprite );

		}
}


void enter_name()
{
	exec_shell( init_enter_name, update_enter_name, kill_enter_name );
	shell->master_option_active=NO;
}

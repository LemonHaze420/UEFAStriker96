/****************************************************
 *																	 *
 *	 																 *
 * Language select											 *
 *	 																 *
 *																	 *
 ****************************************************/


#include "incs.h"

#ifdef PSX
#include "\global\s01.h"
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include	"pad.h"
#include "anim.def"
#include "font.h"
#include "hardware.h"
#include "render.h"
#include "datafile.h"
#include "skindraw.h"
#include "sod.h"
#include	"scene.h"
#include "anim.h"
#include "..\..\striker\frontend\mod.def"
#include "fmv.h"
#endif



/****************************************************
 *																	 *
 *	 Defines														 *
 *																	 *
 ****************************************************/

#define LS_MOVIES      LANGSEL_MOVIES+0
#define LS_EXIT_MOVIES LANGSEL_MOVIES+12
#define LS_INTRO_MOVIE LANGSEL_MOVIES+18

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

void overlay_text();
void lang_store_bg();

/****************************************************
 *																	 *
 *  Vars															 *
 *																	 *
 ****************************************************/


UBYTE *lang_texts, *lang_texts_pak;
RECT lang_store_rect,text_rect;
UBYTE *lang_store_buffer;
WORD lang_timer;
void init_langsel_once()
{
	config->language=ENG;
}



void init_langsel()
{
WORD text_lens[]={ 80, 94, 98, 90, 86 };

	disable_display();
//	init_game_loop();
	enable_display( 0 );

//	alloc_screen_dbuffs();


	lang_texts_pak = (UBYTE *)read_datafile_alloc( LANG_TEXTS_PAK );

	init_display( 256, 256, 0,0,0, YES, NO, YES );	//screen xy res & bg color

	lang_store_rect.x = 0;
	lang_store_rect.y = 0;
	lang_store_rect.w = (96*3)/2;
	lang_store_rect.h = 32;

	lang_timer=0;

	text_rect.x = 20*3;
	text_rect.y = 55;

//** allocate mem for memory behind text ***
	lang_store_buffer = allocate_mem( 0, lang_store_rect.w * lang_store_rect.h * 2 );


//** allocate mem for unpacked word textures **
	lang_texts = allocate_mem( 0, 45000 );

//** decompress textures to allocated area **
	unpropak( lang_texts_pak,lang_texts  );
//	unpropak( lang_texts , lang_texts_pak );




	store_processes();

	init_gadgets( 1 );
	init_sprites ( FRONTEND_SAN, 2 );
	init_fonts();
	start_text_manager( 4 );	// call before update_frontend





}


UWORD ls_movies_rt[]={LS_MOVIES+0, LS_MOVIES+1,LS_MOVIES+2,LS_MOVIES+3,LS_MOVIES+4,LS_MOVIES+5 };
UWORD ls_movies_lt[]={LS_MOVIES+6,LS_MOVIES+7,LS_MOVIES+8,LS_MOVIES+9,LS_MOVIES+10,LS_MOVIES+11 };

void run_langsel()
{
WORD abort_seq;
WORD text_lens[]={ 80, 94, 98, 90, 86 };


	push(0);

	init_langsel();


	abort_seq = NO;
	action ( LS_INTRO_MOVIE,0 );
	lang_store_bg();


	while ( abort_seq == NO )
		{
		lang_timer++;

		bg_loop();

		overlay_text();
		if ( cjoy->db & PAD_RT )
			{
			lang_timer=0;

			action( ls_movies_rt[config->language], 0 );

			config->language++;

			if ( config->language == MAX_LANGS )
				config->language = 0;



			lang_store_bg();

			}

		if ( cjoy->db & PAD_LT )
			{
			lang_timer=0;


			config->language--;
			if ( config->language < 0 )
				config->language = MAX_LANGS-1;

			action( ls_movies_lt[config->language], 0 );

			lang_store_bg();


			}


		if ( cjoy->c & PAD_X )
			abort_seq = YES;
		}


//** play exit movie

	action ( LS_EXIT_MOVIES + config->language,0 );

	restore_processes();

	config->language=0;
	pop(0);
}





void overlay_text()
{
UBYTE buffer[96*3*33];
UBYTE *src;
WORD x,y;
RECT rect = text_rect;

WORD wd = 80;
UBYTE *bptr = buffer;
UBYTE text_wds[]={80,94,88,90,86};
ULONG srcs[]={ 
	20 + 0, 
	20+(80*32*3) + 20 + 0, 
	20+(94*32*3) + 20+(80*32*3) + 20 + 0, 
	20+(88*32*3) +	20+(94*32*3) + 20+(80*32*3) + 20 + 0, 
	20+(90*32*3) + 20+(88*32*3) +	20+(94*32*3) + 20+(80*32*3) + 20 + 0, 
	20+(86*32*3) + 20+(90*32*3) + 20+(88*32*3) +	20+(94*32*3) + 20+(80*32*3) + 20 + 0, 
};


	wd = text_wds[ config->language ];


	rect.w = (wd*3)/2;
	rect.h = 32;
	rect.y = text_rect.y + (cdb->buffnum?256:0 );

	StoreImage( &rect, (ULONG *)buffer );
	DrawSync(0);


	src = lang_texts + srcs[ config->language ];

	if ( ((lang_timer/1)&15)< 11 )
		{
		for ( y=0; y<32; y++ )
			{
			for ( x=0; x<wd; x++ )
				{
				bptr = buffer + ( y*wd*3 ) + (x*3);
				if ( (*src == 0) && (*(src+1) == 0) && (*(src+2) == 0) )
					{
					}
				else
					{
					*bptr   = *src;
					*(bptr+1) = *(src+1);
					*(bptr+2) = *(src+2);
					}
				src+=3;
				}
			}
		LoadImage( &rect, (ULONG *)buffer );
		}
	else
		{
		lang_store_rect.x = rect.x;
		lang_store_rect.y = rect.y;
		LoadImage( &lang_store_rect, (ULONG *)lang_store_buffer );
		}

	DrawSync(0);

}

void lang_store_bg()
{
//	DrawSync(0);
	lang_store_rect.x = text_rect.x;
	lang_store_rect.y = text_rect.y;
	StoreImage( &lang_store_rect, (ULONG *)lang_store_buffer );
	DrawSync(0);
//	VSync(1);
}


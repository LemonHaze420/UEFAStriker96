/****************************************************
 *																	 *
 *																	 *
 *	 Memory card access										 *
 *  Uses high-level Libmcrd.lib							 *
 *																	 *
 *																	 *
 ****************************************************/
#include "incs.h"

#ifdef PSX
#include "..\..\global\striker.def" // defs for PSX striker
#include "..\..\global\s01.h"	// structs used by scene "s01" format
#include "..\..\striker\frontend\mod.def"
#include "..\..\striker\src\anim.def"

#include	"sod.h"
#include	"scene.h"
#include "datafile.h"
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include	"shell.h"
#include "hardware.h"
#include "polydraw.h"	//psx structs
#include "text.h"
#include	"font.h"
#include	"pad.h"
#include "fmv.h"
#include "utils.h"
#include "teamedit.h"
#include "options.h"
#include "config.h"
#include "teamsel.h"
#include "render.h"
#endif


/****************************************************
 *																	 *
 *	 Defines														 *
 *																	 *
 ****************************************************/

#define BLOCK_MAX 15
#define CARD_BUFF_ADDR ( 0x80500000 )

enum
{
Mc_void,
Mc_ok,
Mc_nocard,
Mc_newcard,
Mc_insertcard,
Mc_exists,
Mc_unformatted,
Mc_read_error,
Mc_read_file_headers_com,
Mc_read_file_headers_ok,
Mc_load_file,
};

BYTE *mc_txts[]={

"Mc_void",
"Mc_ok",
"Mc_nocard",
"Mc_newcard",
"Mc_insertcard",
"Mc_exists",
"Mc_unformatted",
"Mc_read_error",
"Mc_read_file_headers_com",
"Mc_read_file_headers_ok",
"Mc_load_file",
};


BYTE *txt_errors[]={
"McErrNone",
"McErrCardNotExist",
"McErrCardInvalid",
"McErrNewCard",
"McErrNotFormat",
"McErrFileNotExist",
"McErrAlreadyExist",
"McErrBlockFull",
"McErrExtend",
};

/****************************************************
 *																	 *
 *	 Prototypes													 *
 *																	 *
 ****************************************************/

void convert_string_from_jis_to_ascii ( UWORD *src, BYTE *dest );
BYTE convert_jis_to_ascii ( UWORD jis );



/****************************************************
 *																	 *
 *	 Structs														 *
 *																	 *
 ****************************************************/


typedef struct
{
	BYTE magic[2];					/* always 'S', 'C' */
	BYTE type;						/* 0x11, 0x12, 0x13 for 1,2 or 3 icons */
	BYTE slots;						/* num of slots used */
	UWORD jis_text_name[32];	/* jis format file name */
	BYTE pad[28];
	BYTE clut[32];					/* Icon 16 color clut */
	BYTE icon_image[128*3];		/* Animating icon 16x16 16color pixels */
}Mc_header;

typedef struct
{
	struct DIRENTRY directory[2][BLOCK_MAX];
	WORD channel;	// 0 or 16
	WORD card_num; // 0 or 1

	WORD file_headers_read;

	WORD command;
	WORD result;

	LONG total_files[2];

	Mc_header headers[2][ 15 ]; // header for every file on each card

	BYTE ascii_text_name[2][15][32];	// 


	UBYTE file_buffer[ 8192 ]; // Mc_load_file loads selected_file to here
	WORD  selected_card;	// 0 or 1
	WORD  selected_file;	
	BYTE  directory_loaded[2];	// yes or no for each card
	BYTE  card_present[2];
}Mc_control;









/****************************************************
 *																	 *
 *	 Vars															 *
 *																	 *
 ****************************************************/

BYTE filename[32];
LONG filesize;
LONG cmds, rslt;
//WORD mode[2];
WORD card_num,channel;
WORD numfilesoncard[2];
WORD cursor_y;
static struct DIRENTRY fileList[2][BLOCK_MAX];
Mc_control mc;
BYTE *file_filenames[15];

BYTE jis_to_ascii_table[]={

//ascii codes, starting at 0x8140

	32,	//	space == jis 0x8140
	32,		
	32,
	44,	//	,	== jis 0x8143 etc
	46,	//	.
	32,
	58,	//	:
	59,	//	;
	63,	//	?
	33,	//	!
	32,
	32,
	32,
	32, 
	32,
	94,	//	^

//8150
	126,	//	~
	95,	//	_
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	45,	// alone in the dark requires this to be a minus ( 0x815b )
	32,
	32,
	47,	// backslash
	32,

//8160
	32,
	32,
	124,	//	|
	32,
	32,
	96,	//	`
	39,	//	'
	32,
	34,	//	"
	40,	//	(
	41,	//	)
	32,
	32,
	91,	//	[
	93,	//	]
	123,	//	{

//8170
	125,	//	}
	60,	//	<
	62,	//	>
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	43,	//	+
	45,	//	-
	32,
	32,
	32,

	32,
	61,	//	=
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	92,	//	\

	36,	//	$
	32,
	32,
	37,	//	%
	35,	//	#
	38,	//	&
	42,	//	)
	64,	//	@
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,

	//1a0
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//1b0
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//1c0
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//1d0
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//1e0
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//1f0
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//200
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//210
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//220
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	//230
	32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,

	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	48, //0

	49, //1-9
	50,
	51,
	52,
	53,
	54,
	55,
	56,
	57,
	32,
	32,
	32,
	32,
	32,
	32,
	32,

	65,	//'A-Z'
	66,
	67,
	68,
	69,
	70,
	71,
	72,
	73,
	74,
	75,
	76,
	77,
	78,
	79,
	80,

	81,
	82,
	83,
	84,
	85,
	86,
	87,
	88,
	89,
	90,
	32,
	32,
	32,
	32,
	32,
	32,

	32,
	97,		//'a-z'
	98,
	99,
	100,
	101,
	102,
	103,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	111,

	112,
	113,
	114,
	115,
	116,
	117,
	118,
	119,
	120,
	121,
	122,
	32,
	32,
	32,
	32,
	32,

};

void convert_string_from_jis_to_ascii ( UWORD *src, BYTE *dest )
{
	while( 1 )
		{
		*dest = convert_jis_to_ascii( *src );
		if ( *dest == 0 )
			break;
		else
			{
			src++;
			dest++;
			}
		
		}
}

BYTE convert_jis_to_ascii ( UWORD jis )
{
UWORD jis_code,ascii_out;
LONG del;

BYTE text[2];

	jis_code = (jis&0xff)*256;
	jis_code+= jis/256;

	if ( (jis_code <0x8140) || (jis_code >0x829a) )
		{
		return 0; // illegal jis code
		}
	else
		{
		ascii_out = jis_to_ascii_table[jis_code-0x8140];
		if ( ascii_out == 0 )
			{
			printf("jis_code %x can't be converted ,",jis_code);
			printf("substituting a '-'\n");
			ascii_out='-';
			}

		return ascii_out;



		}

} 

















void init_memcard_bootup()
{
	MemCardInit(1);
	VSync(0); // try removing this
	MemCardStart();
	card_num=channel=0;
}

void init_memcard()
{
	cursor_y = 0;
	mc.selected_card = 0;
	mc.command = 0;
	mc.directory_loaded[0] = 
	mc.directory_loaded[1] = NO;

	mc.card_present[0] =
	mc.card_present[1] = NO;
}

void update_memcard()
{

LONG ret;
WORD card;
LONG files[2]; 
WORD cnt;
BYTE *ptr;
WORD res;

BYTE *dest_addr;
UWORD jis_chr;
BYTE chr;
BYTE *dst;
WORD *src;

	switch ( MemCardSync(1,&cmds,&rslt) )
	 	{
		case -1:
			{
			//**** Process user memory card commands ****
			switch( mc.command )
				{
				case Mc_newcard:
					MemCardExist( channel );
					mc.command = Mc_read_file_headers_com;	// automatically load all headers on both memory cards
				break;

				case Mc_read_file_headers_com:
					if ( rslt == McErrNotFormat )
						{
						mc.result = Mc_unformatted;
						mc.command = Mc_void;
						}
					else
						{
						if ( rslt != McErrNone )
							{
							//printf("result=%s\n",txt_errors[ rslt ] );
							mc.result = Mc_read_error;
							mc.command = Mc_void;
							}
						else
							{
							//*** Result == No error, so read file ***
							if ( mc.file_headers_read < mc.total_files[ card_num ] )
								{
								dest_addr = (BYTE *)&mc.headers[ card_num ][ mc.file_headers_read ];
								strcpy( filename, mc.directory[ card_num ][ mc.file_headers_read ].name );
								MemCardReadFile( channel, filename,  (LONG *)dest_addr, 0, 256 );
								}
							else				
								{
								mc.command = Mc_void;
								}
							}
						}
				break;

				default:
					card_num = 1-card_num;
					channel = card_num * 16;
					MemCardExist( channel );
				break;
				}
			}
		break;

		case 0:
		// Busy doin' some shet
		break;

		case 1:
		switch( cmds )
			{
			case McFuncExist:
				{
				switch ( rslt )
					{
					case McErrNone:
					break;

					case McErrNewCard:
						MemCardAccept( channel );	// Get further detailed information

						mc.file_headers_read = 0;
						mc.directory_loaded[ card_num ] = NO;
						mc.card_present[ card_num ] = YES;
						mc.command = Mc_newcard;
						mc.result = Mc_ok;
					break;

					default:
						mc.card_present[ card_num ] = NO;
					break;							
					}
				}
			break;

			case McFuncAccept:
				{
				switch(rslt)
					{
					case McErrNone:
					case McErrNewCard:
						{
						// read the directory information.... all 15 blocks on the cards.
						// With no specific file name.
						MemCardGetDirentry( channel, "*", mc.directory[ card_num ], &mc.total_files[ card_num ], 0, BLOCK_MAX );  

//						mc.total_files[ card_num ] = 
//						for( cnt=0, mc.total_files[ card_num ]=0; cnt<files[ card_num]; cnt++ )
//							mc.total_files[ card_num ] += fileList[card_num][cnt].size/8192 + (fileList[ card_num ][cnt].size%8192 ? 1:0);
						}
					break;
					
					case McErrNotFormat:	// Found unformatted card
						mc.result = Mc_unformatted;
					break;
													
					default:
						mc.result = Mc_read_error;
					break;
					}
				}
			break;


			case McFuncReadFile:
				{
				if( rslt==McErrNone )
					{
					switch( mc.command )
						{
						case Mc_read_file_headers_com:
							{
							//**** A file has been loaded successfully, convert then filename to ASCII ****
							src = &mc.headers[ card_num ][ mc.file_headers_read ].jis_text_name[ 0 ];
							dst = &mc.ascii_text_name[ card_num ][ mc.file_headers_read ][ 0 ];
							
							convert_string_from_jis_to_ascii( src, dst );
							mc.file_headers_read++;

							if ( mc.file_headers_read == mc.total_files[ card_num ] )
								{
								mc.directory_loaded[ card_num ] = YES;
								}
							}
						break;	
						}
					}
				else
					{
					printf("error reading\n");
					mc.result = Mc_read_error;
					mc.command = Mc_void;
					}

				}
			break;

			}

		break;
		}




	FntPrint("command = %s   result=%s\n\n",mc_txts[mc.command],mc_txts[mc.result]  );	


#if 0


	if ( mc.command == Mc_read_file_headers_com )
		{
		FntPrint("Reading file headers on card %d.. %d to read\n", mc.card_num, mc.file_headers_read );
		}


	if ( mc.card_present[ mc.selected_card] )
		{
		for( cnt=0; cnt<mc.total_files[ mc.selected_card ]; cnt++)
			{
			FntPrint("name='%s'\n", &mc.ascii_text_name[ mc.selected_card ][ cnt ][0] );
			}
	  
		}
#endif	
}









void test_memcard()
{
WORD cnt;

		if ( cjoy->db & PAD_TRI )
			{
			if ( mc.card_present[ 1-mc.selected_card ] == YES )
				mc.selected_card = 1-mc.selected_card;
			}

	if ( mc.card_present[ mc.selected_card ] && mc.directory_loaded[ mc.selected_card ] )
		{
		for( cnt=0; cnt< mc.total_files[ mc.selected_card ]; cnt++ )
			{
			if ( (cursor_y == cnt) && ((timer/8)&1) )
				FntPrint("*%s\n",&mc.ascii_text_name[ mc.selected_card ][cnt][0] );
			else
				FntPrint(" %s\n",&mc.ascii_text_name[ mc.selected_card ][cnt][0] );
			}
		if( cjoy->r & PAD_UP )
			cursor_y--;
		if( cjoy->r & PAD_DN )
			cursor_y++;

		if( cjoy->db & PAD_CIR)
			{
			mc.command = Mc_load_file;
			mc.selected_file = cursor_y;
			}
		}





}




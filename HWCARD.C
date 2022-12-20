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
#include	"frontend.h"
#include "hardware.h"
#include "hwcard.h"
#include "Jistable.h"
#include "memcard.h"
#include "mem.h"
#endif


/****************************************************
 *																	 *
 *	 Defines														 *
 *																	 *
 ****************************************************/

#define BLOCK_MAX 15


BYTE *mc_txts[]={

"Mc_cancel",
"Mc_newcard",
"Mc_ok",
"Mc_ready",
"Mc_save_file",
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


BYTE *txt_cmds[]={
0,
"McFuncExist",
"McFuncAccept",
"McFuncReadFile",
"McFuncWriteFile",
"McFuncReadData",	
"McFuncWriteData"
};


//results
#if 0
#define	McErrNone		(0)
#define	McErrCardNotExist	(1)
#define	McErrCardInvalid	(2)
#define	McErrNewCard		(3)
#define	McErrNotFormat		(4)
#define	McErrFileNotExist	(5)
#define	McErrAlreadyExist	(6)
#define	McErrBlockFull		(7)
#define	McErrExtend		(0x8000)
#endif


/****************************************************
 *																	 *
 *	 Prototypes													 *
 *																	 *
 ****************************************************/

void convert_string_from_ascii_to_jis ( BYTE *src, UWORD *dest );
void convert_string_from_jis_to_ascii ( UWORD *src, BYTE *dest );
BYTE convert_jis_to_ascii ( UWORD jis );
UWORD convert_ascii_to_jis ( BYTE ascii );
WORD mc_check_file_exists( BYTE *filename );

void download_mc_icons( WORD card );
void set_savename_digits();
void set_game_name();
void mc_wait_system_free();
void mc_wait_system_done();
void load_header_files();



/****************************************************
 *																	 *
 *	 Structs														 *
 *																	 *
 ****************************************************/

/****************************************************
 *																	 *
 *	 Vars															 *
 *																	 *
 ****************************************************/

BYTE filename[32];
LONG filesize;
LONG cmds, rslt;
Mc_control *mc;


Mc_control card_struct;


void init_memcard_bootup()
{
UWORD *wdest;
Mc_header *head;
ULONG *lsrc;
WORD x;
GsIMAGE	tim;

	//printf("doing memcard init\n");
	MemCardInit(0);
//	VSync(0); // try removing this
	MemCardStart();

	mc = &card_struct;

	mc->card_present[0] =
	mc->card_present[1] = NO;

	mc->perform_dir_read = NO;

	mc->result = Mc_ok; //busy;

	mc->card_num=0;
	mc->selected_card = 0;
	mc->header_bytes_to_read = 128;	// default num bytes to read from 512 byte header
	mc->init_func = 0;
	mc->exit_func = 0;

	mc->new_card[0] =
	mc->new_card[1] = NO;

	mc->card_busy[0] =
	mc->card_busy[1] = YES;

	mc->directory_loaded[0] = 
	mc->directory_loaded[1] = NO;

	mc->command = 0;	//???
	mc->command = Mc_ready;

	strcpy( mc->save_name, MCARD_PRODUCT_CODE );
	strcpy( mc->game_save_name, "UEF-" );



	head = (Mc_header *)&mc->save_file_buffer[0];
	head->magic[0]	= 'S';
	head->magic[1]	= 'C';
	head->type  	= 0x11;


//*** Transfer raw data from tim image ***

//	lsrc = (ULONG *)mc->tim_icon;
//	lsrc++;

   //GsGetTimInfo( lsrc, &tim );
	//memcpy( &head->icon_image[0], tim.pixel, (16*8) );
	//memcpy( &head->clut[0], tim.clut, 16*2 );

	memcpy( &head->icon_image[0], &card_icon_tim+64,16*8 );
	memcpy( &head->clut[0], &card_icon_tim+20 , 16*2);


	//** Convert ascii game name to JIS ***
	convert_string_from_ascii_to_jis( mc->game_save_name, &head->jis_text_name[0] );	// convert it

	//printf("mc init done \n");
}

void init_memcard()
{

	mc->card_busy[0]=mc->card_busy[1]=YES;
	return;




}

//WORD busy[2];

void update_memcard()
{

//LONG ret;
//WORD card;

	if ( mc == 0 )
		return;	// don't check memory cards

/**** Check for cards present, formatted, valid, newly inserted ****/
	switch ( MemCardSync(1,&cmds,&rslt) )
	 	{
		case -1:
			{
			mc->card_num = 1 - mc->card_num;
			MemCardExist( mc->card_num*16 );	// check for existance first
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
//					printf("func exist rslt=%s\n",txt_errors[ rslt ] );

					switch ( rslt )
						{
						case McErrNone:
							mc->card_present[ mc->card_num ] = YES;
							mc->card_busy[ mc->card_num ] = NO;
						break;

						case McErrNewCard:

							mc->new_card[ mc->card_num ] = YES;
							mc->total_game_files[ mc->card_num ] = 0;
							mc->total_specific_game_files[ mc->card_num ] = 0;
							mc->directory_loaded[ mc->card_num ] = NO;
							mc->card_present[ mc->card_num ] = YES;
							mc->card_busy[ mc->card_num ] = YES;
							MemCardAccept( mc->card_num*16 );	// Find out if card formatted
							mc->selected_card = mc->card_num;
						break;

						default:
							//** McErrCardNotExist **
							mc->card_present[ mc->card_num ] = NO;
							mc->directory_loaded[ mc->card_num ] = NO;
							mc->total_files[ mc->card_num ] = 0;
							mc->total_game_files[ mc->card_num ] = 0;
							mc->total_specific_game_files[ mc->card_num ] = 0;
							mc->selected_card = 1-mc->card_num;
							mc->card_busy[ mc->card_num ] = NO;
						break;							
						}
					}
				break;

				case McFuncAccept:
					{
					/**** A card is present and OK,  now check if formatted ****/

//					printf("func accept rslt=%s\n",txt_errors[ rslt ] );

					mc->card_formatted[ mc->card_num ] = YES;

					switch( rslt )
						{
						case McErrNotFormat:
							mc->card_formatted[ mc->card_num ] = NO;
							mc->card_busy[ mc->card_num ] = NO;
						break;

						case McErrNone:
							mc->card_formatted[ mc->card_num ] = YES;
							mc->card_busy[ mc->card_num ] = NO;
						break;
						}
					}
				}			
		break;
		}



/*** Check if header files need to be loaded an a newly inserted card *****/

	load_header_files();


/*** Ensure that user cannot select a port that doesn't contain a card ****/
	if ( mc->card_present[ 0 ] || mc->card_present[ 1 ] )
		{
		if ( mc->card_present[ mc->selected_card ] == NO )
			{
			mc->selected_card = 1-mc->selected_card;
			}
		}
}




#if 1
WORD mc_check_file_exists( BYTE *filename )
{
WORD i;
WORD files = mc->total_files[ mc->selected_card ];
BYTE *dir_name;

	for( i=0; i<files; i++ )
		{
		dir_name = mc->directory[ mc->selected_card ][ i ].name;

		if( !strcmp( dir_name, filename ) )
			return YES;
		}
	return NO;
}
#endif









void load_header_files()
{
WORD file_cnt, cnt, ret, len;
UWORD *src;
BYTE *dirname, *dst;
WORD	card;

WORD sel_store = mc->selected_card;

//	if ( mc->game_type != share->game_type )
//		{
		/** Read card directories, regardless of whether a new card has been inserted **/
//		mc->perform_dir_read = YES;
//		}

	for( card=0; card<2; card++ )
		{
		if ( ((mc->new_card[ card ] || mc->perform_dir_read ))  && mc->card_present[card] && (mc->card_busy[card]==NO) )
			{

			mc->game_type = share->game_type; /* Store game type for use with mc->total_specific_game_files */
			mc->new_card[ card ] = NO;
			mc->selected_card = card;

			mc->directory_loaded[ card ] = NO;
			mc->total_game_files[ card ] = 0;
			mc->total_specific_game_files[ card ] = 0;

			printf("reading dir on %s card for gametype %d\n",card?"right":"left", share->game_type);
			mc_read_directory();

			if ( mc->result != McErrNone )
				{
				mc->directory_loaded[card]=YES;
				mc->total_game_files[ card ] = 0;
				mc->total_specific_game_files[ card ] = 0;
				mc->card_present[card]=0;
				printf("Load header files error\n");
				return;
				}


			for( file_cnt=0; file_cnt< mc->total_files[ card ]; file_cnt++ )
				{
				//Copy appendix name from directory file eg BESLES-00256KNOCK01 , copy KNOCK01
				dst = &mc->ascii_text_name[ card ][ file_cnt ][ 0 ];

				len = strlen( mc->save_name );
				dirname = &mc->directory[ card ][ file_cnt ].name[len];
				strncpy( dst, dirname, 32 );

				dirname = &mc->directory[ card ][ file_cnt ].name[0];

				//** Count num of BESLES00256 saves on this card **
				if ( strncmp( dirname, mc->save_name, len ) == 0 ) 
					mc->total_game_files[ card ]++;

				//** Count num of BESLES00256LEAGUE saves on this card **
			
				sprintf(text_buffer, "%s%s" , mc->save_name, mc->save_appendix );
				len = strlen( text_buffer );
				if ( strncmp( dirname, text_buffer, len ) == 0 ) 
					{
					mc->total_specific_game_files[ card ]++;
					}
				}

			mc->directory_loaded[ card ] = YES;
			}
		}

	mc->selected_card = sel_store;
	mc->perform_dir_read = NO;



}






void download_mc_icons( WORD card )
{

// Transfer 1st icon from each file to vram

WORD cnt;

WORD icons = mc->total_files[ card ];
WORD x,y;
BYTE *src;
RECT rect;

	return;

	rect.w = 4;
	rect.h = 16;
	rect.x = mc->icon_x + (rect.w * 15 * card );
	rect.y = mc->icon_y;


	for( cnt=0; cnt< icons; cnt++ )
		{
		src = &mc->headers[ card ][ cnt ].icon_image[ 0 ];

		LoadImage( &rect, (ULONG *)src );
		DrawSync(0);

		// Tell the Ladies and Gentlemen where the icon texture data is.
		mc->tpage[card][cnt] = GetTPage( 0,0, rect.x, rect.y );
		mc->u[card][cnt]= rect.x;
		mc->v[card][cnt]= rect.y;
		rect.x += rect.w;
		}



	rect.w = 16;
	rect.h = 1;

	for( cnt=0; cnt< icons; cnt++ )
		{
		rect.x = mc->clut_x+ ((cnt&7)*rect.w);
		rect.y = mc->clut_y + (card*2) + (cnt/8);


		src = &mc->headers[ card ][ cnt ].clut[0];

		mc->clut_id[card][cnt] = LoadClut2( (ULONG *)src, rect.x, rect.y);
		DrawSync(0);
//		FntPrint("id=%d\n",mc->clut_id[card][cnt] );
		
//		rect.x += rect.w;
		}



}



void set_savename_digits()
{
BYTE *dirname;
WORD cnt;
BYTE *src;
WORD len;
Mc_header *head;

WORD num, hi_num;
BYTE *full_name = text_buffer;
        
// If our savename exists, 

	hi_num=0;
	num=0;

	strcpy( full_name, mc->save_name );
	strcat( full_name, mc->save_appendix );
	len = strlen( full_name );

	for( cnt=0; cnt< mc->total_files[ mc->selected_card ]; cnt++ )
		{
		dirname =  mc->directory[ mc->selected_card ][ cnt ].name;

		//Compare "savename+appendix" with directory name
		if ( strncmp( dirname, full_name, len ) == 0 )
			{
			//Both strings match...

			src = dirname;
			src += len;

			if ( *src == '0' )
				src++;	// ignore leading zero
			num = atoi( src ); // convert appendix into an (k)int

			if ( num >= hi_num )
				hi_num = num;

			
			}
		}



	//*** Highest num *****


	hi_num++;
	if ( hi_num > 99 )
		hi_num=0;
	sprintf( mc->save_digit_buff, "%02d", hi_num );


	if ( share->game_type == CONFIG )
		{
		mc->save_digit_buff[0]=0;
		}



//***** Create game file name WITHIN file ********

	//*** make filename eg 'Striker-league00' ***
	sprintf( filename, "%s%s%s", mc->game_save_name, mc->save_appendix, mc->save_digit_buff );

	head = (Mc_header *)&mc->save_file_buffer[0];
	convert_string_from_ascii_to_jis( filename, &head->jis_text_name[0] );	// convert it

}








void mc_wait_system_free()
{
WORD ret;

	while( 1 )
		{
		ret= MemCardSync(1,&cmds, &rslt );
		if ( ret == -1 )
			break;
		}
}


void mc_wait_system_done()
{
WORD ret;

	while( 1 )
		{
		ret= MemCardSync(1,&cmds, &rslt );
		if ( ret == 1 )
			break;
		}

	mc->result = rslt;
}

void mc_load_file()
{
	mc_wait_system_free();

	MemCardReadFile( mc->selected_card*16, mc->filename,  (ULONG *)mc->load_file_buffer, 0, mc->filesize );
	printf("reading file '%s'\n",mc->filename );

	mc_wait_system_done();


}


void mc_read_directory()
{
WORD cnt;
WORD card = mc->selected_card;
	
	mc_wait_system_free();

	mc->result = MemCardGetDirentry( card*16, "*", mc->directory[ card ], &mc->total_files[ card ], 0, BLOCK_MAX );  

	if ( mc->result == McErrNone )
		{
		for( cnt=0, mc->total_blocks[ card ]=0; cnt<mc->total_files[ card ]; cnt++ )
			mc->total_blocks[ card ] += mc->directory[ card ][ cnt ].size/8192 + (mc->directory[ card ][ cnt ].size%8192 ? 1:0);
		}
}


void mc_delete_file()
{
WORD card = mc->selected_card;

	mc->result = MemCardDeleteFile( card*16, mc->filename  );

	if ( mc->result == McErrNone )
		mc->new_card[ card ] = YES;	// refresh this directory

}

void mc_format_card()
{
WORD card = mc->selected_card;

	mc->result = MemCardFormat( card * 16 );

	if ( mc->result == McErrNone )
		{
		mc->card_formatted[ card ] = YES;
		mc->new_card[ card ] = YES;

		}


}


void mc_save_file()
{
Mc_header *head;

//	mc->init_func( Mc_save_file ); //removed 9/10/98

//*** Set num slots in file header ***
	head = (Mc_header *)&mc->save_file_buffer[0];
	head->slots	= (mc->filesize/8192)+(mc->filesize%8192)?1:0;

	if ( mc_check_file_exists( mc->filename ) == NO )
		{
		//*** File to create does not exist, so create it ***

		mc_wait_system_free();
		mc->result = MemCardCreateFile( mc->selected_card * 16, mc->filename, mc->filesize/8192 + (mc->filesize%8192 ? 1:0) );

		if ( mc->result == McErrNotFormat )
			{
			mc->card_formatted[ mc->selected_card ] = NO;
			return;
			}


		if ( mc->result != McErrNone )
			return;
		}


	mc_wait_system_free();
	//Write file buffer to memory card

	MemCardWriteFile( mc->selected_card * 16, mc->filename,  (LONG *)mc->save_file_buffer, 0, mc->filesize );

	mc_wait_system_done();

	mc->new_card[ mc->selected_card ] = YES;

//	mc->exit_func( Mc_save_file ); //removed 9/10/98

}


WORD check_lt_card_present()
{
WORD ret;
	MemCardExist( 0 );	// check for existance first

	while( 1 )
		{
		ret= MemCardSync( 0,&cmds, &rslt );
		if ( ret == 1 )
			break;
		}

	if ( (rslt == McErrNewCard) || (rslt == McErrNone ) )
		{
		printf("Card found in LEFT port\n");
		return OK;
		}
	else
		{
		printf("No Card found in LEFT port\n");
		return NO;
		}

}

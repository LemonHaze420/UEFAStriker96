/***************************************************************
 *																					*
 *																					*
 *	FILE:	IO functions														*
 *																					*
 *																					*
 ***************************************************************/


#include	"incs.h"
#ifdef PSX
#include "..\..\global\striker.def" // defs for PSX striker
#include "..\..\striker\frontend\mod.def"
#include "datafile.h"
#include	"frontend.h"
#include "hardware.h"
#include	"mem.h"
#include "utils.h"
#include	"io.h"
#endif


#if CD_LOAD == YES
	#define BIGFILE "\\DATAFILE.DAT"
#else
	#define BIGFILE "\\datafile.dat"
#endif

/***************************************************************
 *																					*
 *	Variables																	*
 *																					*
 ***************************************************************/

FILE_HEADER	file_header;
LONG			hd_fp;
CdlFILE		cd_fp;
LONG			io_status = FCLOSED;
BYTE 			datafile_buffer[2048];	//storage for datafile.dat containing
												//offset to sector for each file




/************************************************************
 *																				*
 *	FUNCTION:	Load a chunk of data from HD/CD to memory.	*
 *																				*
 *	PASSED:		Pointer to filename,									*
 *				which file in big file to read,						*
 *				buffer to load to,										*
 *				max size of buffer,										*
 *				file control. (eg open it:read it:close it)		*
 *																				*
 *	RETURNS:	Size of file. (0=error)									*
 *																				*
 *	NOTES:		With this method we can cut down on file		*
 *				opening times as the file only gets opened		*
 *				once and we seek to each data lump position		*
 *				within it.													*
 *				buffer must be at least 2k								*
 *																				*
 ************************************************************/

ULONG	read_io_data( BYTE *filename, LONG index, BYTE *buffer, ULONG buffsize, BYTE control )
{
ULONG	size = 0;
LONG	n = 0;


#if CD_LOAD == NO
	size = read_hd_data( filename, index, buffer, buffsize, control );
#elif CD_LOAD == YES
	while( !size )
		{
		size = read_cd_data( filename, index, buffer, buffsize, control );
		if( !size )
			{
			for(n=0 ; n<30*5 ; n++)
				VSync(2);
			print_error( E_READSAM, NULL, -1 );
			}
		}
#endif

	return( size );
}





/************************************************************
 *																				*
 *	FUNCTION:	Load a chunk of data from HD to memory.		* 
 *																				*
 *	PASSED:		Pointer to filename,									*
 *				which file in big file to read,						*
 *				buffer to load to,										*
 *				max size of buffer,										*
 *				file control. (eg open it:read it:close it)		*
 *																				*
 *	RETURNS:	Size of file. (0=error)									*
 *																				*
 *	NOTES:		With this method we can cut down on file		*
 *				opening times as the file only gets opened		*
 *				once and we seek to each data lump position		*
 *				within it.													*
 *																				*
 ************************************************************/
ULONG	read_hd_data( BYTE *filename, LONG index, BYTE *buffer, ULONG buffsize, BYTE control )
{
#if CD_LOAD == NO

ULONG			size = 0;
UBYTE			newname[100];
ULONG			n=0;
FILE_HEADER	*ptr;


// Open big file //
//---------------//
	if( control & OPEN_IT )
		{
//		strcpy( newname, "c:\\shell\\front\\src\\object\\" );
		strcpy( newname, filename );
//		strcat( newname, filename );
		hd_fp = PCopen( newname, READ, 0 );
		if( hd_fp == -1 )
			{
			PCclose( hd_fp );
			print_error( E_CANT_FIND, newname, -1 );
			return(0);
			}
		io_status = FOPENED;

// Clear all file header info //
#if 0
	file_header.numfiles = -1;
	for(n=0 ; n<MAX_FILES ; n++)
		{
		file_header.file[n].seek_pos =
		file_header.file[n].size = -1;
		}
#endif

// Read big file header into RAM for later use //
		PClseek( hd_fp, 0, FROM_START );
		PCread( hd_fp, buffer, 2048 );
		ptr = (FILE_HEADER *) buffer;
		file_header.numfiles = ptr->numfiles;
		for(n=0 ; n<file_header.numfiles ; n++)
			{
			file_header.file[n].seek_pos = ptr->file[n].seek_pos;
			file_header.file[n].size = ptr->file[n].size;
			}
		size = 2048;
		}


// Read piece of file into memory //
//--------------------------------//
	if( control & READ_IT )
		{
		if( file_header.file[index].size > buffsize )
			{
			#if DEBUG
			printf( "filenum %d too big; filesize=%d bytes ; buffer=%d bytes",index,file_header.file[index].size,buffsize );
			#endif
			return(0);
			}

		PClseek( hd_fp, file_header.file[index].seek_pos, FROM_START );
		PCread( hd_fp, buffer, file_header.file[index].size );
		size = file_header.file[index].size;
//		printf("loaded %dk\n", size/1024);
		}


// Close big file //
//----------------//
	if( control & CLOSE_IT )
		{
		PCclose( hd_fp );
		io_status = FCLOSED;
		size = file_header.file[index].size;
		}

	return(size);
#elif CD_LOAD == YES
	return(0);
#endif
}





/*---------------------------------------------------------*/
/*																				*/
/*	FUNCTION:	Load a chunk of data from CD to memory.		*/
/*																				*/
/*	PASSED:		Pointer to filename,									*/
/*				which file in big file to read,						*/
/*				buffer to load to,										*/
/*				max size of buffer,										*/
/*				file control. (eg open it:read it:close it)		*/
/*																				*/
/*	RETURNS:	Size of file. (0=error)									*/
/*																				*/
/*	NOTES:		With this method we can cut down on file		*/
/*				opening times as the file only gets opened		*/
/*				once and we seek to each data lump position		*/
/*				within it.													*/
/*				buffer must be at least 2k								*/
/*																				*/
/*---------------------------------------------------------*/
ULONG	read_cd_data( BYTE *filename, LONG index, BYTE *buffer, ULONG buffsize, BYTE control )
{
#if CD_LOAD == YES

CdlFILE		temp;
CdlFILE		*fp = &cd_fp;
ULONG			size = 0;
UBYTE			newname[100];
FILE_HEADER	*ptr;
ULONG			n;


// Open big file //
//---------------//
	if( control & OPEN_IT )
		{
		strcpy( newname, filename );
		strcat( newname, ";1" );


		if( !CdSearchFile(fp, newname) )
			{
			#if DEBUG
			printf( "cant find filenum %d", newname );
			#endif
			return(0);
			}
		io_status = FOPENED;

// Clear all file header info //
#if 0
	file_header.numfiles = -1;
	for(n=0 ; n<MAX_FILES ; n++)
		{
		file_header.file[n].seek_pos =
		file_header.file[n].size = -1;
		}
#endif

// Read big file header into RAM for later use //
		if( !get_cd_data(buffer, fp, 2048) )
			return(0);
		ptr = (FILE_HEADER *) buffer;
		file_header.numfiles = ptr->numfiles;
		for(n=0 ; n<file_header.numfiles ; n++)
			{
			file_header.file[n].seek_pos = ptr->file[n].seek_pos;
			file_header.file[n].size = ptr->file[n].size;
			}
		size = 2048;
		}


// Read piece of file into memory //
//--------------------------------//
	if( control & READ_IT )
		{
		if( io_status == FCLOSED )
			{
			#if DEBUG
			printf( "file not opened\n" );
			#endif
			return(0);
			}
		if( file_header.file[index].size > buffsize )
			{
			n = file_header.file[index].size;
			#if DEBUG
			printf( "filenum %d too big; filesize=%d bytes ; buffer=%d bytes",index,file_header.file[index].size,buffsize );
			#endif
			return(0);
			}

		temp.pos.minute = cd_fp.pos.minute;
		temp.pos.second = cd_fp.pos.second;
		temp.pos.sector = cd_fp.pos.sector;
		temp.pos.track = cd_fp.pos.track;

		n = file_header.file[index].seek_pos / 2048;
		n += CdPosToInt( &cd_fp.pos );
		CdIntToPos( n, (CdlLOC *) &temp ); 
		if( !get_cd_data(buffer, &temp, file_header.file[index].size) )
			size = 0;
		else
			{
			size = file_header.file[index].size;
			}
		}


// Close big file //
//----------------//
	if( control & CLOSE_IT )
		{
		io_status = FCLOSED;
		size = file_header.file[index].size;
		}

	return(size);
#elif CD_LOAD == NO
	return(0);
#endif
}




/*---------------------------------------------------------*/
/*																				*/
/*	FUNCTION:	Read in data from CD.								*/
/*																				*/
/*	PASSED:		buffer to load to,									*/
/*				pointer to file structure with seek pos in,		*/
/*				size of file in bytes.									*/
/*																				*/
/*	RETURNS:	Size of file. (0=error : 1=ok)						*/
/*																				*/
/*---------------------------------------------------------*/
ULONG	get_cd_data( BYTE *buffer, CdlFILE *fp, ULONG size )
{
#if CD_LOAD == YES
LONG	n,i;

	size = (size+2047) / 2048;

	if( !CdControl(CdlSeekL, (UBYTE *) &fp->pos, 0))
		{
		print_error( E_SEEK, NULL, -1 );
		return(0);
		}
	while( (n=CdSync(1,0)) != CdlComplete )
		{
		if( n == CdlDiskError ) return(0);
		for(i=0 ; i<3 ; i++) VSync(2);
//		Vsync(0);
		}
	if( !CdRead(size, (ULONG *) buffer, CdlModeSpeed) )
		{
		print_error( E_READ, NULL, -1 );
		return(0);
		}
	while( (n=CdReadSync(1,0)) > 0 )
		{
		for(i=0 ; i<3 ; i++) VSync(2);
		}
	if( n < 0 ) return(0);

#endif
	return(1);
}


/*
	Please note..


	Elvis
	a
	t	 
	shit
		 
	  

*/

/******************************************
 *														*
 * Functions utilising IO functions above	*
 *														*
 ******************************************/
void open_datafile()
{
//****  Open the large datafile ****

//	CdInit();

//#if CD_LOAD == YES
//	CdInit();
//#else
//	PCinit();
//#endif
	read_io_data ( BIGFILE, NULL, datafile_buffer, 0xffffff, OPEN_IT );

}

void read_datafile ( WORD file_num, BYTE *addr, LONG max_size )
{
LONG size;
//****  Read a file from within the large datafile ****

	size = read_io_data ( BIGFILE, file_num, addr, max_size, READ_IT );


//	printf("loading file %d\n",file_num );

}

void close_datafile()
{
//****  Close the large datafile ****
	read_io_data ( BIGFILE, NULL, datafile_buffer, 0xffffff, CLOSE_IT );
} 


void *read_datafile_alloc ( WORD file_num )
{
//**** Load a file from the datafile into an allocated area of memory ****

BYTE *buffer;
ULONG file_size;

//	printf("loading...\n");
	file_size = file_header.file[file_num].size;

	buffer = allocate_mem ( 0, file_size );

	if ( buffer )
		{
		read_datafile ( file_num, buffer, 1000000 );
		}
	else
		{
		#if DEBUG
		printf("*** Could not allocate %d bytes for filenum %d' ***\n",file_size,file_num );
		#endif
		}
	return buffer;
}







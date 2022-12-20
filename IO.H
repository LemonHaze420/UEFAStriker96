/*---------------------------------------------------------*/
/*															*/
/*	FILE:	Header file for "io.c"							*/
/*															*/
/*	AUTHOR:	Steve Williams									*/
/*															*/
/*	DATE:	22/04/97										*/
/*															*/
/*---------------------------------------------------------*/


#ifndef	_IO_H
#define	_IO_H



/*---------------------------------------------------------*/
/*					CONSTANTS								*/
/*---------------------------------------------------------*/
// PC file control flags
#define	READ			0
#define	WRITE			1
#define	READ_WRITE		2

#define	FROM_START		0
#define	FROM_CURRENT	1
#define	FROM_END		2

// Control bits to say what read data function should do
#define	OPEN_IT		0x01
#define	READ_IT		0x02
#define	CLOSE_IT	0x04

#define	FCLOSED		0
#define	FOPENED		1

#define	MAX_FILES	128				// max number of files in big file


/*---------------------------------------------------------*/
/*					TYPEDEFS								*/
/*---------------------------------------------------------*/
//
// Big file header
//
typedef	struct
	{
	ULONG	seek_pos;
	ULONG	size;
	} READ_DETAILS;
typedef	struct
	{
	ULONG			numfiles;
	READ_DETAILS	file[MAX_FILES];
	} FILE_HEADER;


/*---------------------------------------------------------*/
/*					PROTOTYPES								*/
/*---------------------------------------------------------*/
ULONG	load_file( BYTE *filename, BYTE *buffer, ULONG max_size );
ULONG	load_hd_file( BYTE *filename, BYTE *buffer, ULONG max_size );
ULONG	load_cd_file( BYTE *name, BYTE *buffer );

ULONG	read_io_data( BYTE *filename, LONG index, BYTE *buffer, ULONG buffsize, BYTE control );
ULONG	read_hd_data( BYTE *filename, LONG index, BYTE *buffer, ULONG buffsize, BYTE control );
ULONG	read_cd_data( BYTE *filename, LONG index, BYTE *buffer, ULONG buffsize, BYTE control );
ULONG	get_cd_data( BYTE *buffer, CdlFILE *fp, ULONG size );


void open_datafile();
void read_datafile ( WORD file_num, BYTE *addr, LONG max_size );
void *read_datafile_alloc ( WORD file_num );
void close_datafile();


/*---------------------------------------------------------*/
/*					EXTERNALS								*/
/*---------------------------------------------------------*/
extern	FILE_HEADER	file_header;
extern	LONG		hd_fp;
extern	LONG		io_status;


#endif


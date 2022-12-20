extern	ULONG		ceiling;
extern	void init_debug_file( void );
extern	void *divulge_heap_end( void );
extern	WORD preloader( BYTE *filename, BYTE *buffer );
extern	WORD match_string( UBYTE *s, UBYTE *d );
extern	void clear_struct( UBYTE *s, LONG many );
extern	BYTE		*free_mem_ptr;
extern	void copy_struct( UBYTE *s, UBYTE *d, LONG many );
extern	WORD save_file( BYTE *filename, BYTE *buffer, LONG buffer_size, WORD dir );
extern	WORD load_file( BYTE *filename, BYTE *buffer, LONG buffer_size, WORD dir );
extern	void show_file_error( LONG error );
extern	void copy_string( BYTE *s, BYTE *d );
extern	void append_string( BYTE *tag, BYTE *word );
extern	void quit_msg( BYTE *fmt, ...);
extern	void initialise_cd( void );
extern	WORD is_file_there( UBYTE *filename, WORD dir );


extern	BYTE		*file_messages[];
extern	WORD		file_load_retry,cd_initialised;
extern	LONG		last_filesize;
extern	void debug_print( BYTE *fmt, ...);
extern	UBYTE	debug_string[];


extern	void *allocate_mem( ULONG size );
extern	void zero( UBYTE *s, LONG many );

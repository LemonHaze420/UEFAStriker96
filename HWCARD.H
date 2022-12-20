


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
	Mc_header headers[2][ 15 ]; // header for every file on each card
	struct DIRENTRY directory[2][15];
	WORD card_num; // 0 or 1

	WORD command;							 // Pass command 
	WORD result;							 // Get result
	WORD total_blocks[2];				 // Total blocks used on each card
	LONG total_files[2];					 // Files on each card
	WORD total_game_files[2];			 // Total number of our game files on card (mc->save_name)
	WORD total_specific_game_files[2];// Total number of our game files on card (mc->save_name) including appendix name eg BESLES00256LEAGUE
	BYTE ascii_text_name[2][15][32];	 // game name converted from JIS stored here

	BYTE filename[32];					// name of file to load , delete or overwrite
	BYTE game_save_name[32];			// name of file within file
	BYTE save_name[32];					// BESLES name given to slot ****** MAX 20 CHARS! ******
	BYTE save_appendix[16];				// appendix added to save_name and game_save_name
	BYTE save_digit_buff[4];			// 
	ULONG filesize;						// total size of memcard save file( including 512 byte header )
	BYTE *tim_icon;						// Address of icon (16x16x4bit) in TIM format to be saved in 512 byte header
	UWORD header_bytes_to_read;			// Num of bytes to read from header 128 to 512

	WORD card_busy[2];
	ULONG load_file_buffer[(8192+512)/4]; 	// THESE MUST BE LWORD ALIGNED Mc_load_file loads selected_file to here
	ULONG save_file_buffer[(8192+512)/4]; 	// THESE MUST BE LWORD ALIGNED Mc_save_file saves selected_file from here
	WORD  selected_card;					// 0,1 LEFT/RIGHT
	BYTE  directory_loaded[2];			// yes or no for each card
	BYTE  card_present[2];				// yes or no for each card
	BYTE	card_formatted[2];			// yes or no
	BYTE 	new_card[2];
	WORD  game_type;

	WORD  perform_dir_read;
	WORD	game_loaded;
	WORD	game_saved;
	UWORD tpage[2][15];					// poly info for each icon of each card
	UWORD u[2][15];
	UWORD v[2][15];
	UWORD clut_id[2][15];

	WORD icon_x;							// Download the card icons to VRAM xy, growing across
	WORD icon_y;	
	WORD clut_x; 							// Download the card cluts to VRAM xy, growing across
	WORD clut_y; 

	void (*init_func)(WORD);		// This func (if NZ ), called at start of a process passing process type
	void (*exit_func)(WORD);		// This func (if NZ ), called at end of a process and passing process type

	WORD mode;
}Mc_control;

extern Mc_control *mc;

void init_memcard_bootup();
void init_memcard();
void update_memcard();

void update_test_memcard();
void init_test_memcard();

void mc_delete_file();
void mc_overwrite_file();
void mc_save_file();
void mc_load_file();
void mc_read_directory();



enum
{
Mc_cancel,					 
Mc_newcard,					 
Mc_ok,						 
Mc_ready,					 
Mc_save_file,				 
Mc_busy,
Mc_format,
};


WORD check_lt_card_present();


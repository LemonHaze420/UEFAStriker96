extern	BYTE	time_and_date[];
extern	ULONG  end_of_ram;
extern	BYTE	sysfont_pal;
extern	BYTE  sysfont_spr;
extern   BYTE  card_icon_tim;
extern	BYTE  ability_file_addr;

#define HEAP0_SIZE	(1100*1024)
#define HEAP1_SIZE	(200*1024)
#define HEAP2_SIZE	0
#define HEAP3_SIZE	0


extern void compact_otr( LONG *a, WORD n );


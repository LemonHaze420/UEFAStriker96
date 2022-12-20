extern	BYTE	time_and_date[];
extern	BYTE  end_of_ram;
extern	BYTE	sysfont_pal;
extern	BYTE  sysfont_spr;
extern   BYTE  card_icon_tim;
#define HEAP0_SIZE	(1300*1024)
#define HEAP1_SIZE	500000
#define HEAP2_SIZE	400000
#define HEAP3_SIZE	400000

extern void compact_otr( LONG *a, WORD n );


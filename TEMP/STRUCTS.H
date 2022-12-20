typedef struct
{
	WORD  audio_bank,channel;
	LONG	start,end;
 	WORD	volume;
}Audio_track;

typedef	struct
	{
	WORD	lo;
	WORD	hi;
	} Word_access;

typedef	struct
	{
	UWORD	lo;
	UWORD	hi;
	} Uword_access;

typedef union
	{
	LONG			l;
	Word_access	w;
	} Pos;

typedef union
	{
	ULONG				l;
	Uword_access	w;
	} Upos;

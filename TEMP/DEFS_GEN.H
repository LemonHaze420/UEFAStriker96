

#define 	FLAGTRUE	 1
#define	TRUE 		 1
#define	YES		 1
#define	OK 		 1
#define	ON			 1
#define	SET		 1
#define	OPEN		 1

#define 	FLAGFALSE 0
#define	FALSE		 0
#define	NO			 0
#define	DONE		 0
#define	NOT_OK	 0
#define	OFF		 0
#define	CLR		 0
#define	SHUT		 0
#define	CANCEL	 0
#define	SLOT_ACTIVE	 0

#define	NEXT		 2
#define 	ESCAPE	 2
#define	INIT		 2
#define	ERROR		 2

#define ASCENDING 	0
#define DESCENDING 	1

#define	FILE_NOT_FOUND	5



typedef signed char BYTE;
typedef unsigned char UBYTE;
typedef signed short WORD;
typedef unsigned short UWORD;
typedef signed long LONG;
typedef unsigned long ULONG;
typedef signed long INT32;
typedef unsigned long UINT32;
typedef long long LONG64;

#define BIT(n)	(1<<n)

#define BIT_SET( of, bit) ((of) |=  (bit))
#define BIT_CLR( of, bit) ((of) &= ~(bit))
#define BIT_TST( of, bit) ((of) & (bit))


#define	E_READSAM	0
#define	E_EXCEEDS	1
#define	E_READ		2
#define	E_CANT_FIND	3	
#define	E_SEEK		4
#define	E_UNOPENED	5
#define KILL_SEQUENCE 256	// num passed to sequence_num to signal that this logic can terminate





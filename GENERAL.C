/********************************************************************
 *																						  *
 *																						  *
 * General functions , generic to any machine							  *
 *																						  *
 *																						  *
 ********************************************************************/

#include	"incs.h"

#ifdef PSX
#include "..\..\global\striker.def" // defs for PSX striker
#include "..\..\striker\frontend\mod.def"
#include "..\..\striker\src\anim.def"
//#include "..\..\striker\movies\fruit\mod.def"
//#include "..\..\striker\movies\hotdog\mod.def"
#include "..\..\global\s01.h"	// structs used by scene "s01" format
#include	"sod.h"
#include	"scene.h"
#include "datafile.h"
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include "hardware.h"
#include "polydraw.h"	//psx structs
#include	"font.h"
#include "text.h"
#include "plasma.h"
#include	"render.h"
#include "anim.h"	
//#include "test.h"
#include	"mem.h"
#include	"fvars.h"
#include	"pad.h"
#include	"camera.h"
#include	"light.h"
#include "utils.h"
#include	"io.h"
//#include	"sound.h"
//#include "teamsel.h"
#include "options.h"
//#include "..\..\global\image.h"

#endif





/********************************************************************
 *																						  *
 * Address list executor														  *
 *																						  *
 ********************************************************************/

#define MAX_PROCESS_NESTS 5
void ( *process_list[MAX_PROCESSES])(void);
void ( *store_process_list[ MAX_PROCESS_NESTS ][MAX_PROCESSES])(void);
WORD current_process_nest;
//UWORD current_process;


void init_process_executor()
{
WORD cnt;
//	process_list[0]=0;
//	current_process=0;

	for ( cnt=0;cnt< MAX_PROCESSES; cnt++ )
		{
		process_list[cnt]=0;
		}

	current_process_nest=0;

}

void *add_process( void *addr )
{
WORD cnt;
	for ( cnt=0;cnt<MAX_PROCESSES;cnt++ )
		{
		if ( process_list[cnt] == 0 )
			{
			process_list[ cnt ] = addr;
			return &process_list[ cnt ];
			}
		}


	display_error ( "add_process\nAll processes active");
	return 0;

}

void store_processes()
{
WORD cnt;

	//printf("storing processes to nest %d\n",current_process_nest);

// make a copy of all processes currently used
// *** NOTE - now, can be nested upto 5 levels ***

	for ( cnt=0;cnt<MAX_PROCESSES;cnt++ )
		{
		store_process_list[ current_process_nest ][cnt] = process_list[cnt];
		}
	current_process_nest++;
	if ( current_process_nest >= MAX_PROCESS_NESTS )
		display_error( "Too many process nests" );
}

void restore_processes()
{
WORD cnt;

	current_process_nest--;

	for ( cnt=0;cnt<MAX_PROCESSES;cnt++ )
		{
		process_list[cnt] = store_process_list[ current_process_nest ][cnt];
		}
}



void execute_processes()
{
void (*ptr)();
WORD cnt;

/* 
	Execute a list of addresses
*/


	for( cnt=0;cnt<MAX_PROCESSES;cnt++ )
		{
		ptr = (void *)process_list[cnt];
		if ( ptr  )
			(*ptr)();

//		if ( ptr )
//			{
//			(*ptr)();
//			cnt++;
//			}
//		else
//			break;
		}
}

void kill_process( ULONG *addr )
{
	*addr = (ULONG)0;
}

/********************************************************************/





/********************************************************************
 *																						  *
 * Random number generator														  *
 *																						  *
 ********************************************************************/

Pos random_number;

ULONG make_random_number( ULONG max_rand_num_size )
{
/* pass max size of number to generate */

ULONG b;

	b = rand ();
	random_number.l=b;

	if ( max_rand_num_size )
		b %= (max_rand_num_size+1);

	return b;
}
/********************************************************************/



/********************************************************************
 *																						  *
 * Memory 																			  *
 * allocation/deallocation														  *
 *																						  *
 ********************************************************************/

static UBYTE *mh_ptr[4];			// heap pointer
static UBYTE *mh_ptr_save[4][128];			// saved heap pointer
static LONG mh_amount_used_save[4][128];	// Save amount used
static LONG mh_amount_avail[4];			// Amount allocated
static LONG mh_ptr_current[4];

LONG mh_amount_used[4];					// Amount currently allocated
LONG mh_max_used[4];						// Maximum amount allocated
BYTE *heap_area[4]; 

/* Initialise and clear heap to zero */
/* --------------------------------- */
/*
 Returns:
	0 - O.K.
	1 - Error
*/





LONG my_heap_init(WORD heap_num, WORD init_ram )
{
BYTE *addr;
LONG heap_sizes[]={ HEAP0_SIZE, HEAP1_SIZE, HEAP2_SIZE, HEAP3_SIZE };
LONG amount;

	amount = heap_sizes[ heap_num ];

/* Round up to DWORD */
	amount = (amount+3)>>2;
	amount = amount << 2;


	mh_ptr_current[ heap_num ]=0;

	mh_ptr[ heap_num ] = heap_area[ heap_num ];	// set to area reserved for heap in mem.s

	mh_ptr_save[ heap_num ][ mh_ptr_current[ heap_num ] ] = NULL;

	// clear ram?
	if ( init_ram )
		memset(mh_ptr[ heap_num ],0,amount);
	mh_amount_avail[ heap_num ]=amount;
	mh_amount_used[ heap_num ]=0;
	mh_max_used[ heap_num ]=0;
	return OK;
}


// save the current heap pointer
void my_heap_pushpointer( WORD heap_num )
{
	mh_ptr_current[ heap_num ]++;


	mh_ptr_save[ heap_num ][ mh_ptr_current[ heap_num ] ] = mh_ptr[ heap_num ];
	mh_amount_used_save[ heap_num ][mh_ptr_current[ heap_num ] ]=mh_amount_used[ heap_num ];
	//printf("pushed heap at %x\n",mh_ptr[ heap_num ] );

//	printf("push hp=%d\n",mh_ptr_current[ heap_num ]);

}


// restore the heap pointer
// - a sort of very primitive free()
void my_heap_poppointer( WORD heap_num )
{

	if ( mh_ptr_current[ heap_num ] <= 0 )
		{
		#if DEBUG
		printf("Invalid save pointer '%d'\n",mh_ptr_current[ heap_num ] );
		exit(1);
		#endif
		}

	mh_ptr[ heap_num ] = mh_ptr_save[ heap_num ][ mh_ptr_current[ heap_num ] ];	// recover heap pointer
	mh_ptr_save[ heap_num ][ mh_ptr_current[ heap_num ] ] = NULL;		// clear saved heap pointer
	mh_amount_used[ heap_num ]=mh_amount_used_save[ heap_num ][mh_ptr_current[ heap_num ] ];		// Restore amount used
//	printf("pop hp=%d\n",mh_ptr_current[ heap_num ]);

	mh_ptr_current[ heap_num ]--;

}


void zero( UBYTE *s, LONG many )
{
LONG	*ls, lmany;

	while( (LONG)s & 3 )
		{
		*s++ = 0;
 		many--;
		}

	ls = ( LONG *)s;
	lmany = many>>2;
	many -= ( lmany<<2);
	while( lmany-- )
		{
		*ls++ = 0;
		}

	s = (UBYTE *)ls;
	while( many-- )
		{
		*s++ = 0;
		}


}

// ask how much you want in bytes
// it'll round it so it's divisible by four
// and return a pointer to it
void *my_malloc(WORD heap_num, LONG howmuch)
{
UBYTE *retp;

	//printf("mem used=%d mem avail=%d\n",mh_amount_used,mh_amount_avail);

/* Round up to DWORD */
	howmuch = (howmuch+3)>>2;
	howmuch = howmuch << 2;

/* Update amount used */
	mh_amount_used[ heap_num ] += howmuch;

/* Too much used ? */
	if (mh_amount_used[ heap_num ] > mh_amount_avail[ heap_num ] )
		{
		mh_amount_used[ heap_num ] -= howmuch;
		return NULL;
		}

/* Keep track of max used */
	if (mh_amount_used[ heap_num ] > mh_max_used[ heap_num ] )
		mh_max_used[ heap_num ]=mh_amount_used[ heap_num ];

	retp = mh_ptr[ heap_num ];			// return pointer to start of allocated lump
	mh_ptr[ heap_num ] += howmuch;	// advance heap pointer
	zero( retp, howmuch );

//	printf("amount used=%d\n",mh_amount_used);


	return(retp);
}













/*********************************************************************
 *
 * This memory allocation/deallocation reserves a large chunk
 * after the heap and allocates and deallocates within that
 * area. Allocate mem starts at the bottom of the reserved chunk
 * and further allocate_mems() allocate just after that.
 * Use push() and pop() to store/restore the current memory pointer.
 *
 *********************************************************************/


void init_memory_alloc()
{

	heap_area[0] = (BYTE *)&end_of_ram;
	heap_area[1] = heap_area[0] + HEAP0_SIZE;
	heap_area[2] = heap_area[1] + HEAP1_SIZE;
	heap_area[3] = heap_area[2] + HEAP2_SIZE;


	my_heap_init ( 0,YES );	// reset heap 0
	my_heap_init ( 1,YES );	// reset heap 1

}


void *allocate_mem ( WORD heap_num, ULONG amount )
{
BYTE *addr;
ULONG addr_k, addr_rem, laddr;

	addr=my_malloc ( heap_num, amount );

#if DEBUG
	if ( amount==0 )
		{
		printf("ERROR trying to alloc ZERO bytes\n");
		debug_print_mess( "error trying to alloc zero bytes", 0 );
		}
#endif

#if DEBUG

	if ( heap_num==1)
		{
	laddr = addr;
	addr_k = ( laddr - 0x80000000 ) / 1024; 
	addr_rem = (laddr-0x80000000)%10;

	printf("trying to allocate %d.%dk to addr %d.%dk\n",amount/1024,amount%10, addr_k, addr_rem );
		}
#endif

#if DEBUG
	if ( addr == 0 )
		{
		sprintf ( text_buffer, "*** Error , could not allocate %d bytes in heap %d ***\n",amount, heap_num);
		debug_print_mess( text_buffer,0 );
		}
	else			
		{
//		if ( amount > 1000 )
//			printf("reserved 0x%x to 0x%x\n",addr, addr+amount );
		}
#endif

	return addr;
}


void push( WORD heap_num )
{
//Save the current memory pointer
	my_heap_pushpointer( heap_num );
}

void pop( WORD heap_num )
{
//Restore the current memory pointer
	my_heap_poppointer( heap_num );
}

ULONG read_free_addr( WORD heap_num )
{

	return (ULONG)mh_ptr[ heap_num ];
}











WORD my_strcmp ( BYTE *s1, BYTE *s2 )
{

	while ( *s1 && *s2 )
		{
		if ( *s1 != *s2 )
			{
			break;
			}
		s1++;
		s2++;


		}

	if ( (*s1 == 0) && (*s2 == 0) )
		{
		return 0;
		}
	else
		{
		return 1;
		}


}

ULONG get_heap_size( WORD heap_num )
{
	return ( mh_amount_used[heap_num] );
}

ULONG get_heap_start( WORD heap_num )
{
	return ( (ULONG )&heap_area[heap_num] );
}

BYTE *get_heap_end( WORD heap_num )
{
BYTE *addr = heap_area[heap_num];

	return ( addr +  mh_amount_used[heap_num] );
}




/*************************************************************
 *																				 *
 * Bubble number sorter (up to 256 numbers)						 *
 * For 64 bit numbers													 *
 *																				 *
 *    Pass  - list of numbers to sort (long64 )					 *
 *          - size of list												 *
 *          - Ascending or Descending								 *
 *																				 *
 * Returns  - WORD * indexed sort list								 *
 *																				 *
 *************************************************************/
#define BUBBLE_LIST_SIZE 256
WORD bubble_index_list[ BUBBLE_LIST_SIZE ];
LONG64 bubble_sort_list64[ BUBBLE_LIST_SIZE ];

WORD *bubble_sort64( LONG64 *list_to_sort, WORD size, WORD order )
{
LONG64 *num_ptr1, *num_ptr2, store;
WORD *index_ptr1, *index_ptr2, swap,cnt;

//*** Order = Ascending or descending ****


/** Set up index list **/
	for( cnt=0; cnt<BUBBLE_LIST_SIZE; cnt++)
		{
		bubble_index_list[cnt] = cnt;
		}


	for( cnt=0; cnt<size; cnt++)
		{
		bubble_sort_list64[cnt] = list_to_sort[cnt];
		}
	bubble_sort_list64[size]=0;


	swap=YES;


	if ( order == DESCENDING )
		{
		//*** Sort by descending order ****
		while( swap )
			{
			swap = NO;
			num_ptr1 = &bubble_sort_list64[0];
			num_ptr2 = &bubble_sort_list64[1];
			index_ptr1 = &bubble_index_list[0];
			index_ptr2 = &bubble_index_list[1];

			for( cnt=size-1; cnt>0; cnt-- )
				{
				if ( *num_ptr1 < *num_ptr2 )
					{
					store = *num_ptr1;
					*num_ptr1 = *num_ptr2;
					*num_ptr2 = store;

					store = *index_ptr1;
					*index_ptr1 = *index_ptr2;
					*index_ptr2 = store;

					swap = YES;
					}
				else
					{

					}
				num_ptr1++;
				num_ptr2++;
				index_ptr1++;
				index_ptr2++;
				}

			}
		}
	else
		{
		//*** Sort by ascending order ****
		while( swap )
			{
			swap = NO;
			num_ptr1 = &bubble_sort_list64[0];
			num_ptr2 = &bubble_sort_list64[1];
			index_ptr1 = &bubble_index_list[0];
			index_ptr2 = &bubble_index_list[1];

			for( cnt=0; cnt<size-1; cnt++ )
				{
				if ( *num_ptr1 > *num_ptr2 )
					{
					store = *num_ptr1;
					*num_ptr1 = *num_ptr2;
					*num_ptr2 = store;

					store = *index_ptr1;
					*index_ptr1 = *index_ptr2;
					*index_ptr2 = store;

					swap = YES;
					}
				else
					{

					}
				num_ptr1++;
				num_ptr2++;
				index_ptr1++;
				index_ptr2++;
				}

			}
		}

	return bubble_index_list;
}



/*************************************************************
 *																				 *
 * Bubble number sorter (up to 256 numbers)						 *
 * For 16 bit numbers													 *
 *																				 *
 *    Pass  - list of numbers to sort (word )					 *
 *          - size of list												 *
 *          - Ascending or Descending								 *
 *																				 *
 * Returns  - WORD * indexed sort list								 *
 *																				 *
 *************************************************************/

WORD *bubble_sort16( WORD *list_to_sort, WORD size, WORD order )
{
LONG64 *num_ptr1, *num_ptr2, store;
WORD *index_ptr1, *index_ptr2, swap,cnt;

//*** Order = Ascending or descending ****


/** Set up index list **/
	for( cnt=0; cnt<BUBBLE_LIST_SIZE; cnt++)
		{
		bubble_index_list[cnt] = cnt;
		}


	for( cnt=0; cnt<size; cnt++)
		{
		bubble_sort_list64[cnt] = list_to_sort[cnt];
		}
	bubble_sort_list64[size]=0;


	swap=YES;


	if ( order == DESCENDING )
		{
		//*** Sort by descending order ****
		while( swap )
			{
			swap = NO;
			num_ptr1 = &bubble_sort_list64[0];
			num_ptr2 = &bubble_sort_list64[1];
			index_ptr1 = &bubble_index_list[0];
			index_ptr2 = &bubble_index_list[1];

			for( cnt=size-1; cnt>0; cnt-- )
				{
				if ( *num_ptr1 < *num_ptr2 )
					{
					store = *num_ptr1;
					*num_ptr1 = *num_ptr2;
					*num_ptr2 = store;

					store = *index_ptr1;
					*index_ptr1 = *index_ptr2;
					*index_ptr2 = store;

					swap = YES;
					}
				else
					{

					}
				num_ptr1++;
				num_ptr2++;
				index_ptr1++;
				index_ptr2++;
				}

			}
		}
	else
		{
		//*** Sort by ascending order ****
		while( swap )
			{
			swap = NO;
			num_ptr1 = &bubble_sort_list64[0];
			num_ptr2 = &bubble_sort_list64[1];
			index_ptr1 = &bubble_index_list[0];
			index_ptr2 = &bubble_index_list[1];

			for( cnt=0; cnt<size-1; cnt++ )
				{
				if ( *num_ptr1 > *num_ptr2 )
					{
					store = *num_ptr1;
					*num_ptr1 = *num_ptr2;
					*num_ptr2 = store;

					store = *index_ptr1;
					*index_ptr1 = *index_ptr2;
					*index_ptr2 = store;

					swap = YES;
					}
				else
					{

					}
				num_ptr1++;
				num_ptr2++;
				index_ptr1++;
				index_ptr2++;
				}

			}
		}

	return bubble_index_list;
}











#define MAX_MEM_POINTERS 512
#define MAX_PROCESSES 64
void init_process_executor();
void *add_process( void *addr );
void execute_processes();
void kill_process( ULONG *addr );
void store_processes();
void restore_processes();

void push(WORD heap_num );
void pop( WORD heap_num );
void *allocate_mem( WORD heap_num, ULONG size );
extern ULONG read_free_addr( WORD heap_num );

ULONG make_random_number( ULONG max_rand_num_size );
extern Pos random_number;

extern WORD my_strcmp ( BYTE *s1, BYTE *s2 );

extern LONG mh_amount_used[];
							
extern UBYTE *mh_ptr[4];
extern BYTE *heap_area[];

extern LONG my_heap_init(WORD heap_num, WORD init_ram );
ULONG get_heap_size( WORD heap_num );
ULONG get_heap_start( WORD heap_num );
BYTE *get_heap_end( WORD heap_num );


WORD *bubble_sort64( LONG64 *list_to_sort, WORD size, WORD order );
WORD *bubble_sort16( WORD *list_to_sort, WORD size, WORD order );


void init_heaps();

extern BYTE *temp_alloc_from_end_of_stack( ULONG size );


extern ULONG largest_alloc_size;
extern ULONG largest_stack_size;


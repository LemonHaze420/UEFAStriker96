#if DEBUG == YES
 #define DISPLAY_SYSTEM_FONT \
			if ( (print_debug_info == 0) || (print_debug_info == 3)  )	\
				FntFlush(-1);
#else
 #define DISPLAY_SYSTEM_FONT 
#endif


#if REAL_PLAYSTATION == NO
 #define	POLLHOST pollhost(); 
#else
 #define POLLHOST 
#endif


#if DEBUG == YES
 #define	DRAW_CPU_BAR	draw_cpu_bar(); 			//display processor timing bar
 #define	DRAW_GPU_BAR	draw_gpu_bar(); 			//display processor timing bar
 #define	SWITCH_DEBUG_INFO	switch_debug_info();
 #define	SINGLE_STEP_PAUSE single_step_pause();
#else
 #define DRAW_CPU_BAR
 #define DRAW_GPU_BAR
 #define SWITCH_DEBUG_INFO
 #define SINGLE_STEP_PAUSE
#endif


#define SET_STACK( addr , ret_addr )	\
			ret_addr = SetSp ( addr );


#define POINT_STACK_AT_DECACHE oldstack = SetSp( 0x1f800400 );
#define RESTORE_STACK SetSp ( oldstack );





extern	void init_time( void );
extern	void start_cpu_timer( void );
extern	void stop_cpu_timer( void );
extern	void calibrate_frame( void );
extern	void wait_vblank( void );
extern	void log_bar_segment( LONG v, Timebar *tb, CVECTOR *col );
extern	void init_timebar( Timebar *t, LONG max );
extern	void draw_timebar( Timebar *tb );
extern	void draw_cpu_timebar( void );
extern	void start_gpu_timer( void );



extern	Timebar	cpu_timebar, gpu_timebar;


Time_pocket	time;


#define	TBAR_CPU_BACK_R	0
#define	TBAR_CPU_BACK_G	64
#define	TBAR_CPU_BACK_B	0

#define	TBAR_CPU_R 0
#define	TBAR_CPU_G 200
#define	TBAR_CPU_B 0

#define	TBAR_GPU_BACK_R	64
#define	TBAR_GPU_BACK_G	64
#define	TBAR_GPU_BACK_B	0

#define	TBAR_GPU_R 200
#define	TBAR_GPU_G 200
#define	TBAR_GPU_B 0

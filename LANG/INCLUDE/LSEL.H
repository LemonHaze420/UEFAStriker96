extern	void setup_screen_640X512( Db *db );
extern	void language_select_exec( void );
extern	void init_language_select( void );

extern	volatile LONG	frame_counter, vsync_counter,frame_update;
extern	Wave	*wave;
extern	Pos	seed,random_number;
extern	void make_random_number( void );

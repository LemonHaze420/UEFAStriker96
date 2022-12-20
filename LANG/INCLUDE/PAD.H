extern	Pad	*get_controller( WORD no );
extern	void dis_the_pads( void );
extern void debounce_pads( u_short *key, u_short *last,u_short *new );
extern	void initialise_control_pads( void );
extern	void update_pads( void );
extern	Player_input	controller;


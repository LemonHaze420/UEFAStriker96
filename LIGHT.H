void enable_light ( WORD light_num, WORD light_type, WORD x,WORD y,WORD z, WORD r, WORD g, WORD b, WORD intensity );
extern MATRIX sw_color_matrix;
void kill_light ( WORD light_num );
void reset_lights();
void set_dir_light_matrix();

extern MATRIX dir_light_matrix; //directional light matrix


void move_lights( MATRIX *cam );

typedef struct
{
	LIGHT_DATA 	light_data[MAX_LIGHTS];	// data for each light from x01
	MATRIX     	light_matrix;				// light anim data plugged here
	MATRIX	  	color_matrix;				// color of each light
	MATRIX	  	output_light_matrix;		// light matrix that goes to GTE for directional lights
	WORD			any_hw_light_active;		// yes if any light passed to GTE 
	CVECTOR		ambient_color;
	WORD			num_lights;
}LIGHT;



extern LIGHT light;

extern WORD selected_light;

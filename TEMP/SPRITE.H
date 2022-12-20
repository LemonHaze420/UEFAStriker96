/** Data for each sprite animation frame **/
typedef	struct
{
	UWORD	tpage;			// sprite frame tpage 
	UWORD	w,h;				// sprite wd,ht
	UBYTE	u,v;				// sprite texture UV
	WORD xoffs,yoffs;		// x,y offsets to draw this sprite frame
	WORD real_w,real_h;	// 
}Spr_anim_frame_san;





/** Header for sprite animation **/
typedef	struct
{
	UWORD	frames;			// number of frames in animation
	UWORD	clut;				// clut for sprite
}Spr_anim_header_san;


typedef struct Sprite
{
	void  *prim;
	POLY_FT3	*dummy;
	Spr_anim_frame_san *spr_anim_frame_san;	// addr of 1st frame of animation within .SAN file
	Spr_anim_frame_san untextured_frame;		// dummy frame for untextured sprite
	Pos	x,y;
	Pos	x2,y2;
	Pos	xvel1,yvel1; //,xvel2,yvel2;

	BYTE	xflip;
	BYTE	centre_if_scaled;	// yes=sprite will be centered if scaled in x or y
	BYTE  transp_rate,old_transp_rate;	// 0,1,2,3, -1
	BYTE	active,initialised;
	BYTE	use_frame_offs;	// yes = add frame xy offs to sprite pos

	WORD	display;					// yes = sprite on
	WORD	old_xscale,xscale;	// 256 = normal size , 128 = half size etc
	WORD	old_yscale,yscale;
	WORD	xoffs,yoffs;	// added to xy
	WORD	type;
	UWORD	frame,old_frame;
	UWORD clut,num_frames;
	WORD	depth;
	WORD	w,h;
	WORD	update_cnt;	//	
	WORD	group_num;
	WORD	sequence_num;
	CVECTOR tint,old_tint;		// rgb tint for this poly
	CVECTOR dest_tint;
	WORD	brightness;		// brightness level 128=default
	WORD	mode;
	WORD	type2;
	WORD	temp;	
	WORD	timer1,timer2;
	void	(*upd_rtn)( void * );	// Pointer to update routine of object
  	void	*prev;						// sprite spawned before this one
  	void	*next;						// sprite spawned after this one

	UBYTE r0,g0,b0,pad0;
	UBYTE r1,g1,b1,pad1;
	UBYTE r2,g2,b2,pad2;
	UBYTE r3,g3,b3,pad3;


	WORD	gouraud; // yes uses r0 -> b3 above

	WORD	special;								// added to prim list for clipping
	struct Gadget *gadget;					// gadget this sprite belongs to
	struct Textman_struct *textman;		// textmanager struct for this sprite

	WORD	die_speed;
}Sprite;


void draw_sprites();
Sprite *spawn_sprite( WORD spr_type );

Sprite *spawn_sprite_after ( WORD spr_type );
Spr_anim_frame_san *get_sprite_frame_addr ( WORD type );
extern Sprite *sprite;
void kill_sprite( Sprite *sprite );


extern LONG *sprite_animation_file;	// .SAN file addr



void kill_sprite_group( WORD group_num );

Sprite *spawn_clip_sprite( RECT *rect, WORD depth );
Sprite *spawn_null_clip_sprite( WORD depth );


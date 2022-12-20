#if 0
void add_plasma( WORD min_rgb, WORD max_rgb, ULONG base_vel, ULONG rand_vel, WORD wd, WORD ht, WORD block_wd, WORD block_ht, WORD rgb_link, WORD transp_rate, WORD depth  );
void init_plasma();



typedef struct
{
	Pos r;
	Pos g;
	Pos b;
	Pos rv;
	Pos gv;
	Pos bv;
	Pos val;
	Pos val_vel;
}Plasma_vertex;

typedef struct
{
	Plasma_vertex *plasma_vertex;		// addr of plasma vertex data
	WORD array_wd,array_ht;				// wd,ht of array
	WORD min_rgb,max_rgb;				// min/max rgb level
	WORD min_val,max_val;
	ULONG base_vel;						// base velocity	
	UWORD rand_vel;						// random velocity added to base
	WORD  block_wd,block_ht;			// wd,ht of a plasma block
	WORD  poly_wd,poly_ht;				// wd,ht of object in polys
	WORD  rgb_link;						// yes = rgb = same value
	WORD	active;							// yes or no
	POLY_G4 *poly[2];						// addr of lists of allocated polys
	WORD  transp_rate;
	WORD  depth;
}Plasma;
#endif

extern void add_backdrop( WORD gfx_type );
extern void kill_backdrop();
void draw_special_sprite_primitive( Sprite *sprite );


void add_ingame_backdrop ( WORD spr_type );
void kill_ingame_backdrop();

void draw_simple_poly( POLY_FT4 *ft, WORD x, WORD y, WORD w, WORD h, WORD u, WORD v, UWORD tp, UWORD clut_id );

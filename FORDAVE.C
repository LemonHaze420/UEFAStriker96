#define	MAX_ANIMMAN_FRAMES	43
#define	ANIMMAN_TEX_X 		(512 << 1)
#define	ANIMMAN_TEX_Y 		0
#define	ANIMMAN_CLUT_X		768
#define	ANIMMAN_CLUT_Y		192
#define	ANIMMAN_BPP			_8BIT
#define	ANIMMAN_W	 		64
#define	ANIMMAN_H	 		64
#define	ANIMMAN_POS_X		(512-ANIMMAN_W-8)
#define	ANIMMAN_POS_Y		(256-ANIMMAN_H-8-YOFFS)
#define	ANIMMAN_SAVE_X		960
#define	ANIMMAN_SAVE_Y		128

short		animating_man_active = 0;			// 0=no : 1=preserve initial slice : 2=do him
short		animating_man_frame;


void	draw_bloke( void )
{
int		x, y, w;
POLY_FT4	*ft4;

	if( animating_man_active )
		{

/* Preserve initial background behind bloke. */
/*-------------------------------------------*/
		if( animating_man_active == 1 )
			{
			setRECT( &rect, ANIMMAN_POS_X,ANIMMAN_POS_Y,ANIMMAN_W,ANIMMAN_H );
			MoveImage( &rect, ANIMMAN_SAVE_X,ANIMMAN_SAVE_Y );
			VSync(0);
			animating_man_active = 2;
			}

		else
			{
/* Restore backdrop behind bloke. */
/*--------------------------------*/
			setRECT( &rect, ANIMMAN_SAVE_X,ANIMMAN_SAVE_Y,ANIMMAN_W,ANIMMAN_H );
			MoveImage( &rect, ANIMMAN_POS_X,ANIMMAN_POS_Y );

/* Draw next frame of bloke to screen. */
/*-------------------------------------*/
/* Get next frame VRAM coords */
			x = ANIMMAN_TEX_X;
			x += ((animating_man_frame & 0xf) * ANIMMAN_W);
			y = ANIMMAN_TEX_Y;
			y += (animating_man_frame / 16) * ANIMMAN_H;

/* Setup prim */
			ft4 = &genprim.ft4;
			SetPolyFT4( ft4 );
			SetSemiTrans( ft4, SEMITRANSOFF );
			SetShadeTex( ft4, SHADETEXON );
			setRGB0( ft4, 128,128,128 );
			setXYWH( ft4, ANIMMAN_POS_X, ANIMMAN_POS_Y, ANIMMAN_W, ANIMMAN_H );
			setTPage( ft4, ANIMMAN_BPP, 0, x>>(2-ANIMMAN_BPP), y );
			setClut( ft4, ANIMMAN_CLUT_X, ANIMMAN_CLUT_Y );
			x &= ((256 >> (2-ANIMMAN_BPP)) - 1);
			setUVWH( ft4, x, y, ANIMMAN_W, ANIMMAN_H );
			DrawPrim( ft4 );
			}
		}
}

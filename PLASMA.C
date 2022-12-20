#if 0
/****************************************************
 *																	 *
 *	 Plasma draw												 *
 *  inlcFunctions used speifically by PSX to render		 *
 *  a scene														 *
 *																	 *
 ****************************************************/

#include "incs.h"
#include "..\..\global\s01.h"	// structs used by scene "s01" format
#include "hardware.h"
#include "sod.h"
#include "scene.h"
#include "plasma.h"
#include "anim.h"

void init_plasma_polys();
void draw_plasma_to_polys();


void update_plasma();
//plas

Plasma plasma_header;
Plasma *plasma = &plasma_header;

void init_plasma()
{
WORD cnt;


	plasma->active = NO;

	add_process( update_plasma );

}

void add_plasma( WORD min_rgb, WORD max_rgb, ULONG base_vel, ULONG rand_vel, WORD wd, WORD ht, WORD block_wd, WORD block_ht, WORD rgb_link, WORD transp_rate, WORD depth  )
{
WORD cnt,x,y;


//ULONG vals[]= { 0x200000, 0x500000, 0x900000, 0xc00000, 0x900000, 0x500000, 0x200000, 0x200000 };
ULONG vals[]= { 0x200000, 0x200000, 0x300000, 0x400000, 0x600000, 0x800000,0xa00000,0xb00000 };

	plasma->depth = depth;
	plasma->transp_rate = transp_rate;
	plasma->active = YES;
	plasma->rgb_link = rgb_link;
	plasma->array_wd = wd+1;
	plasma->array_ht = ht+1;
	plasma->block_wd = block_wd;	// width in pixels of a single plasma block
	plasma->block_ht = block_ht;	// height in pixels of a single plasma block
	plasma->poly_wd = wd;
	plasma->poly_ht = ht;
	plasma->min_rgb = min_rgb;
	plasma->max_rgb = max_rgb;
	plasma->base_vel = base_vel <<0;
	plasma->rand_vel = rand_vel;

	plasma->plasma_vertex = allocate_mem ( 0,sizeof(Plasma_vertex) * plasma->array_wd * plasma->array_ht );

	for ( cnt=0; cnt< plasma->array_wd * plasma->array_ht; cnt++ )
		{
		plasma->plasma_vertex[ cnt ].r.l = 65570;
		plasma->plasma_vertex[ cnt ].g.l = 65570;
		plasma->plasma_vertex[ cnt ].b.l = 65570;
		plasma->plasma_vertex[ cnt ].rv.l = 0x200000;
		plasma->plasma_vertex[ cnt ].gv.l = 0x200000;
		plasma->plasma_vertex[ cnt ].bv.l = 0x200000;
		plasma->plasma_vertex[ cnt ].val.l = 0;
		plasma->plasma_vertex[ cnt ].val_vel.l = 0;
		}

	cnt=0;			
	for ( y=0; y< plasma->array_ht;y++ )
		{
		for ( x=0; x< plasma->array_wd;x++ )
			{
//			plasma->plasma_vertex[ cnt ].val.l = vals[y];
//			plasma->plasma_vertex[ cnt ].val_vel.l = 100000;
//			plasma->min_val = 0;
//			plasma->max_val = 50;

			cnt++;
			}
		}


	init_plasma_polys();



	for(cnt=0;cnt<50;cnt++)
		{
		update_plasma();
		}

}

void update_plasma()
{

WORD pv; //plasma vertex cnt

WORD x,y,wd,ht;
Plasma_vertex *plasma_vertex;

//*** Process plasma array ****

	if ( plasma->active )
		{

		wd = plasma->poly_wd;
		ht = plasma->poly_ht;

		for ( pv=0; pv< (wd+1)*(ht+1); pv++ )
			{
			plasma->plasma_vertex[pv].r.l += plasma->plasma_vertex[pv].rv.l;
			if ( plasma->plasma_vertex[pv].r.w.hi > plasma->max_rgb )
				{
				plasma->plasma_vertex[pv].r.w.hi = plasma->max_rgb;
				plasma->plasma_vertex[pv].rv.l = -plasma->plasma_vertex[pv].rv.l;
				}

			if ( plasma->plasma_vertex[pv].r.w.hi < plasma->min_rgb )
				{
				plasma->plasma_vertex[pv].r.w.hi = plasma->min_rgb;
				plasma->plasma_vertex[pv].rv.l = (make_random_number(plasma->rand_vel)<<0) + plasma->base_vel;
				}

			if ( plasma->rgb_link == NO )
				{
				plasma->plasma_vertex[pv].g.l += plasma->plasma_vertex[pv].gv.l;
				if ( plasma->plasma_vertex[pv].g.w.hi > plasma->max_rgb )
					{
					plasma->plasma_vertex[pv].g.w.hi = plasma->max_rgb;
					plasma->plasma_vertex[pv].gv.l = -plasma->plasma_vertex[pv].gv.l;
					}

				if ( plasma->plasma_vertex[pv].g.w.hi < plasma->min_rgb )
					{
					plasma->plasma_vertex[pv].g.w.hi = plasma->min_rgb;
					plasma->plasma_vertex[pv].gv.l = (make_random_number(plasma->rand_vel)<<0) + plasma->base_vel;
					}

				plasma->plasma_vertex[pv].b.l += plasma->plasma_vertex[pv].bv.l;
				if ( plasma->plasma_vertex[pv].b.w.hi > plasma->max_rgb )
					{
					plasma->plasma_vertex[pv].b.w.hi = plasma->max_rgb;
					plasma->plasma_vertex[pv].bv.l = -plasma->plasma_vertex[pv].bv.l;
					}

				if ( plasma->plasma_vertex[pv].b.w.hi < plasma->min_rgb )
					{
					plasma->plasma_vertex[pv].b.w.hi = plasma->min_rgb;
					plasma->plasma_vertex[pv].bv.l = (make_random_number(plasma->rand_vel)<<0) + plasma->base_vel;
					}
				}
			else
				{
				plasma->plasma_vertex[pv].g.w.hi =
				plasma->plasma_vertex[pv].b.w.hi = plasma->plasma_vertex[pv].r.w.hi;
				}






			plasma->plasma_vertex[pv].val.l += plasma->plasma_vertex[pv].val_vel.l;
			if ( plasma->plasma_vertex[pv].val.w.hi > plasma->max_val )
				{
				plasma->plasma_vertex[pv].val.w.hi = plasma->max_val;
				plasma->plasma_vertex[pv].val_vel.l = -plasma->plasma_vertex[pv].val_vel.l;
				}

			if ( plasma->plasma_vertex[pv].val.w.hi < plasma->min_val )
				{
				plasma->plasma_vertex[pv].val.w.hi = plasma->min_val;
				plasma->plasma_vertex[pv].val_vel.l = -plasma->plasma_vertex[pv].val_vel.l;
				}
			}


		draw_plasma_to_polys();
		}





}





POLY_GT4 pd[2];

void init_plasma_polys()
{
POLY_G4 *poly;
WORD wd,ht,x,y,buff;
WORD cnt;

	wd = plasma->poly_wd;
	ht = plasma->poly_ht;
	plasma->poly[0] = allocate_mem( 0,sizeof ( POLY_G4 ) * wd * ht );	
	plasma->poly[1] = allocate_mem( 0,sizeof ( POLY_G4 ) * wd * ht );

	for( buff=0; buff<2; buff++ )
		{
		poly = plasma->poly[ buff ];

		for( y=0;y<ht;y++ )
			{
			for( x=0;x<wd;x++ )
				{
				SetPolyG4( poly );

				SetSemiTrans ( poly,plasma->transp_rate==-1?0:1 );
#if 0
				poly-> tpage = GetTPage ( DUMMY_TEXTURE_BPP, plasma->transp_rate, DUMMY_TEXTURE_X,DUMMY_TEXTURE_Y ); 
				poly-> u0 = 
				poly-> u1 = 
				poly-> u2 =
				poly-> u3 = ( DUMMY_TEXTURE_X <<(2-DUMMY_TEXTURE_BPP)) % (64<<(2-DUMMY_TEXTURE_BPP));
				poly-> v0 = 
				poly-> v1 = 
				poly-> v2 =
				poly-> v3 = DUMMY_TEXTURE_Y % 256;
#endif
				poly++;
				}
			}
		}


	for(cnt=0;cnt<2;cnt++)
		{
		pd[cnt].x0 = 0;
		pd[cnt].x1 = 1;
		pd[cnt].x2 = 0;
		pd[cnt].x3 = 1;
		pd[cnt].y0 = 0;
		pd[cnt].y1 = 1;
		pd[cnt].y2 = 0;
		pd[cnt].y3 = 1;

		pd[cnt].u0 = 
		pd[cnt].u1 = 
		pd[cnt].u2 =
		pd[cnt].u3 = ( DUMMY_TEXTURE_X <<(2-DUMMY_TEXTURE_BPP)) % (64<<(2-DUMMY_TEXTURE_BPP));
		pd[cnt].v0 = 
		pd[cnt].v1 = 
		pd[cnt].v2 =
		pd[cnt].v3 = DUMMY_TEXTURE_Y % 256;

		pd[cnt].r0 = pd[cnt].r1 = pd[cnt].r2 = pd[cnt].r3 = 255;
		pd[cnt].g0 = pd[cnt].g1 = pd[cnt].g2 = pd[cnt].g3 = 255;

		SetPolyGT4( &pd[cnt] );
		pd[cnt].tpage = GetTPage ( DUMMY_TEXTURE_BPP, plasma->transp_rate, DUMMY_TEXTURE_X,DUMMY_TEXTURE_Y ); 
		SetSemiTrans( &pd[cnt], 1 );
		}

}

void draw_plasma_to_polys()
{
WORD pv,x,y,wd,ht;
POLY_G4 *face;
LONG *ot = cdb->ot;
WORD xp = 0;
WORD yp = 0;



	wd = plasma->poly_wd;
	ht = plasma->poly_ht;

	pv=0;
	for ( y=0; y<ht; y++ )
		{
		for ( x=0; x<wd; x++ )
			{
			face = plasma->poly[ cdb->buffnum];
			face += x + (y*(wd+0));


			face->r0 = plasma->plasma_vertex[ pv ].r.w.hi + plasma->plasma_vertex[ pv ].val.w.hi;
			face->g0 = plasma->plasma_vertex[ pv ].g.w.hi + plasma->plasma_vertex[ pv ].val.w.hi;
			face->b0 = plasma->plasma_vertex[ pv ].b.w.hi + plasma->plasma_vertex[ pv ].val.w.hi;

			face->r1 = plasma->plasma_vertex[ pv+1 ].r.w.hi + plasma->plasma_vertex[ pv+1 ].val.w.hi;
			face->g1 = plasma->plasma_vertex[ pv+1 ].g.w.hi + plasma->plasma_vertex[ pv+1 ].val.w.hi;
			face->b1 = plasma->plasma_vertex[ pv+1 ].b.w.hi + plasma->plasma_vertex[ pv+1 ].val.w.hi;

			face->r2 = plasma->plasma_vertex[ pv+0 + wd+1 ].r.w.hi  + plasma->plasma_vertex[ pv + wd+1 ].val.w.hi;
			face->g2 = plasma->plasma_vertex[ pv+0 + wd+1  ].g.w.hi + plasma->plasma_vertex[ pv + wd+1 ].val.w.hi;
			face->b2 = plasma->plasma_vertex[ pv+0 + wd+1  ].b.w.hi + plasma->plasma_vertex[ pv + wd+1 ].val.w.hi;

			face->r3 = plasma->plasma_vertex[ pv+1 + wd+1 ].r.w.hi + plasma->plasma_vertex[ pv+1+ wd+1 ].val.w.hi;
			face->g3 = plasma->plasma_vertex[ pv+1 + wd+1 ].g.w.hi + plasma->plasma_vertex[ pv+1+ wd+1 ].val.w.hi;
			face->b3 = plasma->plasma_vertex[ pv+1 + wd+1 ].b.w.hi + plasma->plasma_vertex[ pv+1+ wd+1 ].val.w.hi;


			face->x0 = xp + (x*plasma->block_wd );
			face->x1 = xp + (x*plasma->block_wd ) + plasma->block_wd;
			face->x2 = xp + (x*plasma->block_wd );
			face->x3 = xp + (x*plasma->block_wd ) + plasma->block_wd;

			face->y0 = yp + (y*plasma->block_ht );
			face->y1 = yp + (y*plasma->block_ht );
			face->y2 = yp + (y*plasma->block_ht )+ plasma->block_ht;
			face->y3 = yp + (y*plasma->block_ht )+ plasma->block_ht;


			AddPrim( ot + plasma->depth, face );
//			AddPrim( ot + MIDDLE_DEPTH, face );
			pv++;
			}
		pv++;
		}

	AddPrim( ot + PLASMA_DEPTH, &pd[cdb->buffnum] );

}
#endif

/****************************************************
 *																	 *
 *	 Polygon draw												 *
 *																	 *
 ****************************************************/
#if 0
#include	"incs.h"

#ifdef PSX
#include "..\..\global\striker.def" // defs for PSX striker
#include "..\..\striker\frontend\mod.def"
#include "..\..\striker\src\anim.def"
//#include "..\..\striker\movies\fruit\mod.def"
//#include "..\..\striker\movies\hotdog\mod.def"
#include "..\..\global\s01.h"	// structs used by scene "s01" format
#include	"sod.h"
#include	"scene.h"
#include "datafile.h"
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include "hardware.h"
#include "polydraw.h"	//psx structs
#include	"font.h"
#include "text.h"
#include "plasma.h"
#include	"render.h"
#include "anim.h"	
#include	"mem.h"
#include	"fvars.h"
#include	"pad.h"
#include	"camera.h"
#include	"light.h"
#include "utils.h"
#include	"io.h"
#include "options.h"
//#include "..\..\global\image.h"
#endif

/****************************************************
 *																	 *
 *	 Defines														 *
 *																	 *
 ****************************************************/

#define LIGHTS_ACTIVE YES

#define PHONG_POLY_BRIGHTNESS 128

#define M_IDENTITY {{{4096,0,0},{0,4096,0},{0,0,4096}},{0,0,0}}

#define cs_gte_MulRotMatrix0( inmat, outmat )	\
	{											\
	gte_ldclmv( inmat );						\
	gte_rtir( );								\
	gte_stclmv( outmat );						\
	gte_ldclmv( (BYTE *) inmat+2 );				\
	gte_rtir( );								\
	gte_stclmv( (BYTE *) outmat+2);				\
	gte_ldclmv( (BYTE *) inmat+4 );				\
	gte_rtir( );								\
	gte_stclmv( (BYTE *) outmat+4 );			\
	}

/****************************************************
 *																	 *
 *	 Structs														 *
 *																	 *
 ****************************************************/


/****************************************************
 *																	 *
 *	 Vars															 *
 *																	 *
 ****************************************************/


/****************************************************
 *																	 *
 *	 Prototypes													 *
 *																	 *
 ****************************************************/


#define SCENE_ENVMAP_SHIFT 7

// (4096/envmap_wd)*2 should equal 2^ scene_envmap_shift


void getuv3( SVECTOR *n, POLY_GT3 *gt3 )
{
VECTOR	in;
	ApplyRotMatrix( n++, &in );


	gt3 -> u0 = ( in.vx >> SCENE_ENVMAP_SHIFT ) + scene_envmap_u0;
	gt3 -> v0 = ( in.vy >> (SCENE_ENVMAP_SHIFT*1) ) + scene_envmap_v0;

	ApplyRotMatrix( n++, &in );
	gt3 -> u1 = ( in.vx >> SCENE_ENVMAP_SHIFT ) + scene_envmap_u0;
	gt3 -> v1 = ( in.vy >> (SCENE_ENVMAP_SHIFT*1) ) + scene_envmap_v0;

	ApplyRotMatrix( n++, &in );
	gt3 -> u2 = ( in.vx >> SCENE_ENVMAP_SHIFT ) + scene_envmap_u0;
	gt3 -> v2 = ( in.vy >> (SCENE_ENVMAP_SHIFT*1) ) + scene_envmap_v0;


}


void getuv4( SVECTOR *n, POLY_GT4 *gt4 )
{
VECTOR	in;

	ApplyRotMatrix( n++, &in );

	gt4 -> u0 = ( in.vx >> SCENE_ENVMAP_SHIFT ) + scene_envmap_u0;
	gt4 -> v0 = ( in.vy >> (SCENE_ENVMAP_SHIFT*1) ) + scene_envmap_v0;

	ApplyRotMatrix( n++, &in );
	gt4 -> u1 = ( in.vx >> SCENE_ENVMAP_SHIFT ) + scene_envmap_u0;
	gt4 -> v1 = ( in.vy >> (SCENE_ENVMAP_SHIFT*1) ) + scene_envmap_v0;

	ApplyRotMatrix( n++, &in );
	gt4 -> u2 = ( in.vx >> SCENE_ENVMAP_SHIFT ) + scene_envmap_u0;
	gt4 -> v2 = ( in.vy >> (SCENE_ENVMAP_SHIFT*1) ) + scene_envmap_v0;
														 
	ApplyRotMatrix( n++, &in );
	gt4 -> u3 = ( in.vx >> SCENE_ENVMAP_SHIFT ) + scene_envmap_u0;
	gt4 -> v3 = ( in.vy >> (SCENE_ENVMAP_SHIFT*1) ) + scene_envmap_v0;

//	FntPrint("uv0=%d %d uv3=%d %d\n",gt4->u0, gt4->v0, gt4->u3, gt4->v3);
}




#if 0
void do_poly_mipmap3( POLY_GT3 *poly_ptr, GT3_face *face, Scene_obj *scene_obj )
{
	if ( scene_obj->do_mipmap_swap )
		{
		switch ( scene_obj->mip_level )
			{
			case 0:
				poly_ptr->u0 = face->t0.x;
				poly_ptr->v0 = face->t0.y;
				poly_ptr->u1 = face->t1.x;
				poly_ptr->v1 = face->t1.y;
				poly_ptr->u2 = face->t2.x;
				poly_ptr->v2 = face->t2.y;
				poly_ptr->tpage = face->tpage;
			break;

			case 1:
				poly_ptr->u0 = face->t0_mip1.x;
				poly_ptr->v0 = face->t0_mip1.y;
				poly_ptr->u1 = face->t1_mip1.x;
				poly_ptr->v1 = face->t1_mip1.y;
				poly_ptr->u2 = face->t2_mip1.x;
				poly_ptr->v2 = face->t2_mip1.y;
				poly_ptr->tpage = face->tpage_mip1;
			break;

			case 2:
				poly_ptr->u0 = face->t0_mip2.x;
				poly_ptr->v0 = face->t0_mip2.y;
				poly_ptr->u1 = face->t1_mip2.x;
				poly_ptr->v1 = face->t1_mip2.y;
				poly_ptr->u2 = face->t2_mip2.x;
				poly_ptr->v2 = face->t2_mip2.y;
				poly_ptr->tpage = face->tpage_mip2;
			break;
			}
		}
}

void do_poly_mipmap4( POLY_GT4 *poly_ptr, GT4_face *face, Scene_obj *scene_obj )
{

	if ( scene_obj->do_mipmap_swap )
		{
		switch ( scene_obj->mip_level )
			{
			case 0:
				poly_ptr->u0 = face->t0.x;
				poly_ptr->v0 = face->t0.y;
				poly_ptr->u1 = face->t1.x;
				poly_ptr->v1 = face->t1.y;
				poly_ptr->u2 = face->t2.x;
				poly_ptr->v2 = face->t2.y;
				poly_ptr->u3 = face->t3.x;
				poly_ptr->v3 = face->t3.y;
				poly_ptr->tpage = face->tpage;
			break;

			case 1:
				poly_ptr->u0 = face->t0_mip1.x;
				poly_ptr->v0 = face->t0_mip1.y;
				poly_ptr->u1 = face->t1_mip1.x;
				poly_ptr->v1 = face->t1_mip1.y;
				poly_ptr->u2 = face->t2_mip1.x;
				poly_ptr->v2 = face->t2_mip1.y;
				poly_ptr->u3 = face->t3_mip1.x;
				poly_ptr->v3 = face->t3_mip1.y;
				poly_ptr->tpage = face->tpage_mip1;
			break;

			case 2:
				poly_ptr->u0 = face->t0_mip2.x;
				poly_ptr->v0 = face->t0_mip2.y;
				poly_ptr->u1 = face->t1_mip2.x;
				poly_ptr->v1 = face->t1_mip2.y;
				poly_ptr->u2 = face->t2_mip2.x;
				poly_ptr->v2 = face->t2_mip2.y;
				poly_ptr->u3 = face->t3_mip2.x;
				poly_ptr->v3 = face->t3_mip2.y;
				poly_ptr->tpage = face->tpage_mip2;
			break;


			}
		}



}
#endif


#if 0
void rottrans_centre_vertex( Scene_obj *obj )
{
ULONG otz;
WORD mip_level;
	gte_ldv3( &obj->centre_vertex,&obj->centre_vertex,&obj->centre_vertex );//load vertices to GTE
	gte_rtpt();				//rottrans them with local matrix
	gte_avsz3();
	gte_stotz( &otz );	// setup depth sort flag

// otz holds the screen z for this scene, so 
//	it can be used to calculate the necessary mipmap level


//**** mipmap processing ****
	if ( obj->do_mipmap_swap )	obj->do_mipmap_swap--;

	if ( otz < 2500 )
		mip_level = 0;
	else
		{
		if ( otz < 4600 ) 
			mip_level = 1;
		else
			mip_level = 2;
		}


	if ( obj->mip_level != mip_level )
		{
		obj->mip_level = mip_level;
		obj->do_mipmap_swap = 2;	//perform swap over 2 frames
		}
}
#endif





void setup_scene_g3s( Scene_obj *scene_obj, Scene_obj_src *scene_obj_src )
{		
POLY_GT3 *poly_ptr[2];
POLY_GT3 *e_poly_ptr[2];
G3_face_src *face_source;
G3_face *face;
BYTE *src;
WORD count,cnt;	
//Scene_obj *scene_obj;
	
//	scene_poly_data = scene_header->scene_poly_data + scene_obj->scene_obj_num;

//** Allocate area for polygon data **
	scene_obj->g3_face = (G3_face *)allocate_mem ( 0, sizeof(G3_face) * scene_obj_src->g3_cnt );
	face = scene_obj->g3_face;

//** Allocate area for primitives **
	scene_obj->g3_ptr[0] = (POLY_GT3 *)allocate_mem ( 0, sizeof( POLY_GT3 ) * scene_obj_src->g3_cnt );
	scene_obj->g3_ptr[1] = (POLY_GT3 *)allocate_mem ( 0, sizeof( POLY_GT3 ) * scene_obj_src->g3_cnt );

	if ( scene_obj->environment_mapping == YES )
		{
		scene_obj->eg3_ptr[0] = (POLY_GT3 *)allocate_mem ( 0, sizeof( POLY_GT3 ) * scene_obj_src->g3_cnt );
		scene_obj->eg3_ptr[1] = (POLY_GT3 *)allocate_mem ( 0, sizeof( POLY_GT3 ) * scene_obj_src->g3_cnt );
		e_poly_ptr[0] = scene_obj->eg3_ptr[0];
		e_poly_ptr[1] = scene_obj->eg3_ptr[1];
		}

	poly_ptr[0] = scene_obj->g3_ptr[0];
	poly_ptr[1] = scene_obj->g3_ptr[1];

	scene_obj->g3_cnt = scene_obj_src->g3_cnt;
	src = (BYTE *)scene_header_src;
	src += scene_obj_src->g3_face_src_offs;
	face_source = (G3_face_src *)src;

//	printf("texture map xy=%d %d  bpp=%d\n",scene_header_src->envmap_tx, scene_header_src->envmap_ty, scene_header_src->envmap_bpp );

	count = scene_obj_src->g3_cnt;

	#if DEBUG
	printf("g3s=%d\n",count);
	#endif

	while ( count-- )
		{
		face->v0 = face_source->v0;
		face->v1 = face_source->v1;
		face->v2 = face_source->v2;
		face->n0 = face_source->n0;
		face->n1 = face_source->n1;
		face->n2 = face_source->n2;

		face->c0 = face_source->c0;
		face->c1 = face_source->c1;
		face->c2 = face_source->c2;

		for(cnt=0;cnt<2;cnt++)
			{
			SetPolyGT3 ( poly_ptr[cnt]);
			SetSemiTrans ( poly_ptr[cnt],face_source->transp_rate==-1?0:1 );

			poly_ptr[cnt] -> r0  = face->c0.r;
			poly_ptr[cnt] -> g0  = face->c0.g;
			poly_ptr[cnt] -> b0  = face->c0.b;
			poly_ptr[cnt] -> r1  = face->c1.r;
			poly_ptr[cnt] -> g1  = face->c1.g;
			poly_ptr[cnt] -> b1  = face->c1.b;
			poly_ptr[cnt] -> r2  = face->c2.r;
			poly_ptr[cnt] -> g2  = face->c2.g;
			poly_ptr[cnt] -> b2  = face->c2.b;


			poly_ptr[cnt] -> tpage = GetTPage ( DUMMY_TEXTURE_BPP, face_source->transp_rate, DUMMY_TEXTURE_X,DUMMY_TEXTURE_Y ); 
			poly_ptr[cnt] -> u0 = 
			poly_ptr[cnt] -> u1 = 
			poly_ptr[cnt] -> u2 = ( DUMMY_TEXTURE_X <<(2-DUMMY_TEXTURE_BPP)) % (64<<(2-DUMMY_TEXTURE_BPP));
			poly_ptr[cnt] -> v0 = 
			poly_ptr[cnt] -> v1 = 
			poly_ptr[cnt] -> v2 = DUMMY_TEXTURE_Y % 256;


			if ( scene_obj->environment_mapping == YES )
				{
				SetPolyGT3 ( e_poly_ptr[cnt]);
				SetSemiTrans ( e_poly_ptr[cnt],1 );	// env polys always transparent
				e_poly_ptr[cnt] -> tpage = GetTPage ( scene_header_src->envmap_bpp, 1, scene_header_src->envmap_tx, scene_header_src->envmap_ty ); 
				e_poly_ptr[cnt] -> clut = scene_header_src->envmap_clut;
				}

			}

		face++;							  
		face_source++;

		poly_ptr[0]++;
		poly_ptr[1]++;

		e_poly_ptr[0]++;
		e_poly_ptr[1]++;

		}

}


void setup_scene_g4s( Scene_obj *scene_obj, Scene_obj_src *scene_obj_src )
{
POLY_GT4 *poly_ptr[2];
POLY_GT4 *e_poly_ptr[2];
G4_face_src *face_source;
G4_face *face;
BYTE *src;
WORD count,cnt;	
WORD bpp,u0,u1,u2,u3,v0,v1,v2,v3;
//Generic_texture *texture;	
LONG tp1,tp2;
//Scene_poly_data *scene_poly_data;
	
//	scene_poly_data = scene_header->scene_poly_data + scene_obj->scene_obj_num;

//	texture = texture_name_list [ 0 ].datalist_ptr;

	e_poly_ptr[0]=
	e_poly_ptr[1]=0;

//** Allocate area for polygon source data **
	scene_obj->g4_face = (G4_face *)allocate_mem ( 0, sizeof(G4_face) * scene_obj_src->g4_cnt );
	face = scene_obj->g4_face;

//** Allocate area for primitives **
	scene_obj->g4_ptr[0] = (POLY_GT4 *)allocate_mem ( 0, sizeof( POLY_GT4 ) * scene_obj_src->g4_cnt );
	scene_obj->g4_ptr[1] = (POLY_GT4 *)allocate_mem ( 0, sizeof( POLY_GT4 ) * scene_obj_src->g4_cnt );
	poly_ptr[0] = scene_obj->g4_ptr[0];
	poly_ptr[1] = scene_obj->g4_ptr[1];


	if ( scene_obj->environment_mapping == YES )
		{
		scene_obj->eg4_ptr[0] = (POLY_GT4 *)allocate_mem ( 0, sizeof( POLY_GT4 ) * scene_obj_src->g4_cnt );
		scene_obj->eg4_ptr[1] = (POLY_GT4 *)allocate_mem ( 0, sizeof( POLY_GT4 ) * scene_obj_src->g4_cnt );
		e_poly_ptr[0] = scene_obj->eg4_ptr[0];
		e_poly_ptr[1] = scene_obj->eg4_ptr[1];
		}

	scene_obj->g4_cnt = scene_obj_src->g4_cnt;
	src = (BYTE *)scene_header_src;
	src += scene_obj_src->g4_face_src_offs;
	face_source = (G4_face_src *)src;

	count = scene_obj_src->g4_cnt;

	#if DEBUG
	printf("g4s=%d\n",count);
	#endif

	while ( count-- )
		{
		face->v0 = face_source->v0;
		face->v1 = face_source->v1;
		face->v2 = face_source->v2;
		face->v3 = face_source->v3;
		face->n0 = face_source->n0;
		face->n1 = face_source->n1;
		face->n2 = face_source->n2;
		face->n3 = face_source->n3;

		face->c0 = face_source->c0;
		face->c1 = face_source->c1;
		face->c2 = face_source->c2;
		face->c3 = face_source->c3;

//		if ( strcmp ( "rl_Fan_Blades", scene_obj_src->name ) == 0 )
//			printf("rgb0=%d %d %d rgb3=%d %d %d\n",face->c0.r,face->c0.g,face->c0.b, face->c3.r,face->c3.g,face->c3.b );

		for(cnt=0;cnt<2;cnt++)
			{

			poly_ptr[cnt] -> r0  = face->c0.r;
			poly_ptr[cnt] -> g0  = face->c0.g;
			poly_ptr[cnt] -> b0  = face->c0.b;

			poly_ptr[cnt] -> r1  = face->c1.r;
			poly_ptr[cnt] -> g1  = face->c1.g;
			poly_ptr[cnt] -> b1  = face->c1.b;

			poly_ptr[cnt] -> r2  = face->c2.r;
			poly_ptr[cnt] -> g2  = face->c2.g;
			poly_ptr[cnt] -> b2  = face->c2.b;

			poly_ptr[cnt] -> r3  = face->c3.r;
			poly_ptr[cnt] -> g3  = face->c3.g;
			poly_ptr[cnt] -> b3  = face->c3.b;


			if ( scene_obj->environment_mapping == YES )
				{
				SetPolyGT4 ( e_poly_ptr[cnt]);
				SetSemiTrans ( e_poly_ptr[cnt],1 );	// env polys always transparent
				e_poly_ptr[cnt] -> tpage = GetTPage ( scene_header_src->envmap_bpp, 1, scene_header_src->envmap_tx, scene_header_src->envmap_ty ); 
				e_poly_ptr[cnt] -> clut = scene_header_src->envmap_clut;
				}


			poly_ptr[cnt] -> tpage = GetTPage ( DUMMY_TEXTURE_BPP, face_source->transp_rate, DUMMY_TEXTURE_X,DUMMY_TEXTURE_Y ); 
			poly_ptr[cnt] -> u0 = 
			poly_ptr[cnt] -> u1 = 
			poly_ptr[cnt] -> u2 =
			poly_ptr[cnt] -> u3 = ( DUMMY_TEXTURE_X <<(2-DUMMY_TEXTURE_BPP)) % (64<<(2-DUMMY_TEXTURE_BPP));
			poly_ptr[cnt] -> v0 = 
			poly_ptr[cnt] -> v1 = 
			poly_ptr[cnt] -> v2 =
			poly_ptr[cnt] -> v3 = DUMMY_TEXTURE_Y % 256;

			SetPolyGT4 ( poly_ptr[cnt]);
			SetSemiTrans ( poly_ptr[cnt],face_source->transp_rate==-1?0:1 );
			}

		face++;							  
		face_source++;

		poly_ptr[0]++;
		poly_ptr[1]++;

		e_poly_ptr[0]++;
		e_poly_ptr[1]++;

		}


}



void setup_scene_gt3s( Scene_obj *scene_obj, Scene_obj_src *scene_obj_src )
{
POLY_GT3 *poly_ptr[2];
POLY_GT3 *e_poly_ptr[2];
GT3_face_src *face_source;
GT3_face *face;
BYTE *src;
WORD count,cnt;	
ULONG offs;
//Scene_poly_data *scene_poly_data;
	
//	scene_poly_data = scene_header->scene_poly_data + scene_obj->scene_obj_num;

	e_poly_ptr[0]=
	e_poly_ptr[1]=0;

	scene_obj->gt3_face = (GT3_face *)allocate_mem ( 0, sizeof(GT3_face) * scene_obj_src->gt3_cnt );
	scene_obj->gt3_ptr[0] = (POLY_GT3 *)allocate_mem ( 0, sizeof( POLY_GT3 ) * scene_obj_src->gt3_cnt );
	scene_obj->gt3_ptr[1] = (POLY_GT3 *)allocate_mem ( 0, sizeof( POLY_GT3 ) * scene_obj_src->gt3_cnt );

	if ( scene_obj->environment_mapping == YES )
		{
		scene_obj->egt3_ptr[0] = (POLY_GT3 *)allocate_mem ( 0, sizeof( POLY_GT3 ) * scene_obj_src->gt3_cnt );
		scene_obj->egt3_ptr[1] = (POLY_GT3 *)allocate_mem ( 0, sizeof( POLY_GT3 ) * scene_obj_src->gt3_cnt );
		e_poly_ptr[0] = scene_obj->egt3_ptr[0];
		e_poly_ptr[1] = scene_obj->egt3_ptr[1];
		}

	poly_ptr[0] = scene_obj->gt3_ptr[0];
	poly_ptr[1] = scene_obj->gt3_ptr[1];

	scene_obj->gt3_cnt = scene_obj_src->gt3_cnt;
	src = (BYTE *)scene_header_src;

	src += scene_obj_src->gt3_face_src_offs;
	face_source = (GT3_face_src *)src;

	face = scene_obj->gt3_face;

	count = scene_obj_src->gt3_cnt;

	while ( count-- )
		{
		face->v0 = face_source->v0;
		face->v1 = face_source->v1;
		face->v2 = face_source->v2;
		face->n0 = face_source->n0;
		face->n1 = face_source->n1;
		face->n2 = face_source->n2;
		face->t0 = face_source->t0;
		face->t1 = face_source->t1;
		face->t2 = face_source->t2;
//		face->t0_mip1 = face_source->t0_mip1;
//		face->t1_mip1 = face_source->t1_mip1;
//		face->t2_mip1 = face_source->t2_mip1;
//		face->t0_mip2 = face_source->t0_mip2;
//		face->t1_mip2 = face_source->t1_mip2;
//		face->t2_mip2 = face_source->t2_mip2;

		face->c0 = face_source->c0;
		face->c1 = face_source->c1;
		face->c2 = face_source->c2;

		face->tpage = face_source->tpage;
//		face->tpage_mip1 = face_source->tpage_mip1;
//		face->tpage_mip2 = face_source->tpage_mip2;

//		face->tpage_mip1 = face_source->tpage_mip1;
//		face->tpage_mip2 = face_source->tpage_mip2;

		//material = material_data + face_source->material_num;

		for(cnt=0;cnt<2;cnt++)
			{
			SetPolyGT3 ( poly_ptr[cnt]);
			SetSemiTrans ( poly_ptr[cnt],face_source->transp_rate==-1?0:1 );
			//SetSemiTrans ( poly_ptr[cnt],0 );
			poly_ptr[cnt] -> u0 	  = face_source->t0.x;
			poly_ptr[cnt] -> v0 	  = face_source->t0.y;
			poly_ptr[cnt] -> u1 	  = face_source->t1.x;
			poly_ptr[cnt] -> v1 	  = face_source->t1.y;
			poly_ptr[cnt] -> u2 	  = face_source->t2.x;
			poly_ptr[cnt] -> v2 	  = face_source->t2.y;
			poly_ptr[cnt] -> tpage = face->tpage;
			poly_ptr[cnt] -> clut  = face_source->clut;

			poly_ptr[cnt] -> r0  = face->c0.r;
			poly_ptr[cnt] -> g0  = face->c0.g;
			poly_ptr[cnt] -> b0  = face->c0.b;
			poly_ptr[cnt] -> r1  = face->c1.r;
			poly_ptr[cnt] -> g1  = face->c1.g;
			poly_ptr[cnt] -> b1  = face->c1.b;
			poly_ptr[cnt] -> r2  = face->c2.r;
			poly_ptr[cnt] -> g2  = face->c2.g;
			poly_ptr[cnt] -> b2  = face->c2.b;

			if ( scene_obj->environment_mapping == YES )
				{
				SetPolyGT3 ( e_poly_ptr[cnt]);
				SetSemiTrans ( e_poly_ptr[cnt],1 );	// env polys always transparent
				e_poly_ptr[cnt] -> tpage = GetTPage ( scene_header_src->envmap_bpp, 1, scene_header_src->envmap_tx, scene_header_src->envmap_ty ); 
				e_poly_ptr[cnt] -> clut = scene_header_src->envmap_clut;
				}

			}

		face++;							  
		face_source++;

		poly_ptr[0]++;
		poly_ptr[1]++;

		e_poly_ptr[0]++;
		e_poly_ptr[1]++;


		}


}







void setup_scene_gt4s( Scene_obj *scene_obj, Scene_obj_src *scene_obj_src )
{
POLY_GT4 *poly_ptr[2];
POLY_GT4 *e_poly_ptr[2];
GT4_face_src *face_source;
GT4_face *face;
BYTE *src;
WORD count,cnt;	
WORD bpp,u0,u1,u2,u3,v0,v1,v2,v3;
LONG tp,clut;
ULONG *ul_addr;
LONG tp1,tp2;

	e_poly_ptr[0]=
	e_poly_ptr[1]=0;

	scene_obj->gt4_face = (GT4_face *)allocate_mem ( 0, sizeof(GT4_face) * scene_obj_src->gt4_cnt );
	scene_obj->gt4_ptr[0] = (POLY_GT4 *)allocate_mem ( 0, sizeof( POLY_GT4 ) * scene_obj_src->gt4_cnt );
	scene_obj->gt4_ptr[1] = (POLY_GT4 *)allocate_mem ( 0, sizeof( POLY_GT4 ) * scene_obj_src->gt4_cnt );


	if ( scene_obj->environment_mapping == YES )
		{
		scene_obj->egt4_ptr[0] = (POLY_GT4 *)allocate_mem ( 0, sizeof( POLY_GT4 ) * scene_obj_src->gt4_cnt );
		scene_obj->egt4_ptr[1] = (POLY_GT4 *)allocate_mem ( 0, sizeof( POLY_GT4 ) * scene_obj_src->gt4_cnt );
		e_poly_ptr[0] = scene_obj->egt4_ptr[0];
		e_poly_ptr[1] = scene_obj->egt4_ptr[1];
		}

	poly_ptr[0] = scene_obj->gt4_ptr[0];
	poly_ptr[1] = scene_obj->gt4_ptr[1];

	scene_obj->gt4_cnt = scene_obj_src->gt4_cnt;
	src = (BYTE *)scene_header_src;
	src += scene_obj_src->gt4_face_src_offs;
	face_source = (GT4_face_src *)src;

	face = scene_obj->gt4_face;

	count = scene_obj_src->gt4_cnt;

	while ( count-- )
		{
		face->v0 	  = face_source->v0;
		face->v1 	  = face_source->v1;
		face->v2 	  = face_source->v2;
		face->v3 	  = face_source->v3;
		face->n0 	  = face_source->n0;
		face->n1 	  = face_source->n1;
		face->n2 	  = face_source->n2;
		face->n3 	  = face_source->n3;
		face->t0 	  = face_source->t0;
		face->t1 	  = face_source->t1;
		face->t2 	  = face_source->t2;
		face->t3 	  = face_source->t3;
//		face->t0_mip1 = face_source->t0_mip1;
//		face->t1_mip1 = face_source->t1_mip1;
//		face->t2_mip1 = face_source->t2_mip1;
//		face->t3_mip1 = face_source->t3_mip1;
//		face->t0_mip2 = face_source->t0_mip2;
//		face->t1_mip2 = face_source->t1_mip2;
//		face->t2_mip2 = face_source->t2_mip2;
//		face->t3_mip2 = face_source->t3_mip2;


		face->c0 = face_source->c0;
		face->c1 = face_source->c1;
		face->c2 = face_source->c2;
		face->c3 = face_source->c3;

		face->tpage = face_source->tpage;
//		face->tpage_mip1 = face_source->tpage_mip1;
//		face->tpage_mip2 = face_source->tpage_mip2;
	  

		for(cnt=0;cnt<2;cnt++)
			{
			SetPolyGT4 ( poly_ptr[cnt]);
			SetSemiTrans ( poly_ptr[cnt],face_source->transp_rate==-1?0:1 );
			poly_ptr[cnt] -> u0 	= face_source->t0.x;
			poly_ptr[cnt] -> v0 	= face_source->t0.y;
			poly_ptr[cnt] -> u1 	= face_source->t1.x; 
			poly_ptr[cnt] -> v1 	= face_source->t1.y; 
			poly_ptr[cnt] -> u2 	= face_source->t2.x; 
			poly_ptr[cnt] -> v2 	= face_source->t2.y;
			poly_ptr[cnt] -> u3 	= face_source->t3.x;
			poly_ptr[cnt] -> v3 	= face_source->t3.y;
			poly_ptr[cnt] -> tpage = face->tpage;
			poly_ptr[cnt] -> clut  = face_source->clut; 

			poly_ptr[cnt] -> r0  = face->c0.r;
			poly_ptr[cnt] -> g0  = face->c0.g;
			poly_ptr[cnt] -> b0  = face->c0.b;
			poly_ptr[cnt] -> r1  = face->c1.r;
			poly_ptr[cnt] -> g1  = face->c1.g;
			poly_ptr[cnt] -> b1  = face->c1.b;
			poly_ptr[cnt] -> r2  = face->c2.r;
			poly_ptr[cnt] -> g2  = face->c2.g;
			poly_ptr[cnt] -> b2  = face->c2.b;
			poly_ptr[cnt] -> r3  = face->c3.r;
			poly_ptr[cnt] -> g3  = face->c3.g;
			poly_ptr[cnt] -> b3  = face->c3.b;


			if ( scene_obj->environment_mapping == YES )
				{
				SetPolyGT4 ( e_poly_ptr[cnt]);
				SetSemiTrans ( e_poly_ptr[cnt],1 );	// env polys always transparent
				e_poly_ptr[cnt] -> tpage = GetTPage ( scene_header_src->envmap_bpp, 1 , scene_header_src->envmap_tx, scene_header_src->envmap_ty );
				e_poly_ptr[cnt] -> clut = scene_header_src->envmap_clut;
				}

			}

		face++;							  
		face_source++;

		poly_ptr[0]++;
		poly_ptr[1]++;

		e_poly_ptr[0]++;
		e_poly_ptr[1]++;

		}


}











/**********************************************************************
 **********************************************************************
 **																						**
 **																						**
 **    Prelit polygon draw functions											**
 **																						**
 **																						**
 **********************************************************************
 **********************************************************************/

void pl_draw_scene_g3s( Scene_obj *scene_obj )
{
G3_face  *face_source;
POLY_GT3  *poly_ptr;
ULONG otz;
LONG nclip;
LONG *ot = cdb->ot + scene_obj->sod->user_depth;
WORD count;

	face_source = scene_obj->g3_face; //** scene vertices **
	poly_ptr = scene_obj->g3_ptr[cdb->buffnum]; 
	count = scene_obj->g3_cnt; //** num faces on this scene **

	while ( count )
		{
		gte_ldv3c( &face_source->v0 );	//load vertices to GTE
		gte_rtpt();								//rottrans them with local matrix
		count--;
		face_source++;

		gte_nclip();							//set flag for backface clipping
		gte_stopz( &nclip );

		//FntPrint("g3 xy=%d %d\n",poly_ptr->x0,poly_ptr->y0);
	
		if ( nclip > 0)
			{
			gte_stsxy3_gt3( ( ULONG *)poly_ptr );	//store 2d coords in polygon
		  	gte_avsz3();
			gte_stotz( &otz );							// setup depth sort flag

			if ( otz > 0 && otz < 16384 )
				{ 
				otz >>= OT_SHIFT;	 						// place poly in ordering tab


				addPrim ( ot + otz + NEAREST_DEPTH , poly_ptr );

				}

			}

		poly_ptr++;
		}

}

void pl_draw_scene_g4s( Scene_obj *scene_obj )
{
G4_face  *face_source;
POLY_GT4  *poly_ptr;
ULONG otz;
LONG nclip;
LONG *ot = cdb->ot + scene_obj->sod->user_depth;
WORD count;


WORD color_override;
CVECTOR tint_c0;

	face_source = scene_obj->g4_face; //** scene vertices **
	poly_ptr = scene_obj->g4_ptr[cdb->buffnum]; 
	count = scene_obj->g4_cnt; //** num faces on this scene **

	if ( (color_override = scene_obj->sod->color_override) )
		{
//		if ( color_override == TINT_OVERRIDE )
			tint_c0 = scene_obj->sod->tint_c0;
		
		}


	while ( count )
		{
		gte_ldv3c( &face_source->v0 );	//load vertices to GTE
		gte_rtpt();								//rottrans them with local matrix
		count--;

		gte_nclip();							//set flag for backface clipping
		gte_stopz( &nclip );

//		FntPrint("g4 xy=%d %d\n",poly_ptr->x0,poly_ptr->y0);
	
		if ( nclip > 0)
			{
			gte_stsxy3_gt4( ( ULONG *)poly_ptr );	//store 2d coords in polygon
		  	gte_avsz3();
			gte_stotz( &otz );							// setup depth sort flag

			if ( color_override )
				{
				if ( color_override == TINT_OVERRIDE )
					{
					poly_ptr->r0 = poly_ptr->r1 = poly_ptr->r2 = poly_ptr->r3 = tint_c0.r;
					poly_ptr->g0 = poly_ptr->g1 = poly_ptr->g2 = poly_ptr->g3 = tint_c0.g;
					poly_ptr->b0 = poly_ptr->b1 = poly_ptr->b2 = poly_ptr->b3 = tint_c0.b;
					}

				if ( color_override == COLOR_OVERRIDE )
					{
					poly_ptr->r0 = face_source->c0.r;
					poly_ptr->r1 = face_source->c1.r;
					poly_ptr->r2 = face_source->c2.r;
					poly_ptr->r3 = face_source->c3.r;

					poly_ptr->g0 = face_source->c0.g;
					poly_ptr->g1 = face_source->c1.g;
					poly_ptr->g2 = face_source->c2.g;
					poly_ptr->g3 = face_source->c3.g;

					poly_ptr->b0 = face_source->c0.b;
					poly_ptr->b1 = face_source->c1.b;
					poly_ptr->b2 = face_source->c2.b;
					poly_ptr->b3 = face_source->c3.b;

					}
				}


			if ( otz >0 && otz < 16384 )
				{ 
				otz >>= OT_SHIFT;	 						// place poly in ordering tab

				// 4th vertex
				gte_ldv0( &face_source->v3 );
				gte_rtps();
				addPrim ( ot + otz + NEAREST_DEPTH, poly_ptr );
				gte_stsxy( ( ULONG *)&poly_ptr->x3 );
				}
			}

		face_source++;
		poly_ptr++;
		}

}






void pl_draw_scene_gt3s( Scene_obj *scene_obj )
{
GT3_face  *face_source;
POLY_GT3  *poly_ptr;
ULONG otz;
LONG nclip;
LONG *ot = cdb->ot + scene_obj->sod->user_depth;
WORD count;
ULONG big=0;

WORD color_override;
CVECTOR tint_c0;

	face_source = scene_obj->gt3_face; //** scene vertices **
	poly_ptr = scene_obj->gt3_ptr[cdb->buffnum]; 
	count = scene_obj->gt3_cnt; //** num faces on this scene **


	if ( (color_override = scene_obj->sod->color_override) )
		{
		tint_c0 = scene_obj->sod->tint_c0;
		}

	while ( count )
		{
		gte_ldv3c( &face_source->v0 );	//load vertices to GTE
		gte_rtpt();								//rottrans them with local matrix
		count--;

		gte_nclip();							//set flag for backface clipping
		gte_stopz( &nclip );

	
//		do_poly_mipmap3( poly_ptr, face_source, scene_obj );

		if ( nclip > 0)
			{
			if ( color_override )
				{
				if ( color_override == TINT_OVERRIDE )
					{
					poly_ptr->r0 = poly_ptr->r1 = poly_ptr->r2 = tint_c0.r;
					poly_ptr->g0 = poly_ptr->g1 = poly_ptr->g2 = tint_c0.g;
					poly_ptr->b0 = poly_ptr->b1 = poly_ptr->b2 = tint_c0.b;
					}

				if ( color_override == COLOR_OVERRIDE )
					{
					poly_ptr->r0 = face_source->c0.r;
					poly_ptr->r1 = face_source->c1.r;
					poly_ptr->r2 = face_source->c2.r;

					poly_ptr->g0 = face_source->c0.g;
					poly_ptr->g1 = face_source->c1.g;
					poly_ptr->g2 = face_source->c2.g;

					poly_ptr->b0 = face_source->c0.b;
					poly_ptr->b1 = face_source->c1.b;
					poly_ptr->b2 = face_source->c2.b;
					}
				}

			gte_stsxy3_gt3( ( ULONG *)poly_ptr );	//store 2d coords in polygon
		  	gte_avsz3();
			gte_stotz( &otz );							// setup depth sort flag

//			FntPrint("ot=%d\n",otz);


			if ( otz > 0 && otz < 16384 )
				{ 
				otz >>= OT_SHIFT;	 						// place poly in ordering tab
				addPrim ( ot + otz + NEAREST_DEPTH , poly_ptr );
				}

			}

		face_source++;
		poly_ptr++;
		}
}






void pl_draw_scene_gt4s( Scene_obj *scene_obj )
{
GT4_face  *face_source;
POLY_GT4  *poly_ptr;
ULONG otz;
LONG nclip;
LONG *ot = cdb->ot + scene_obj->sod->user_depth;
WORD count;

WORD color_override;
CVECTOR tint_c0;


	face_source = scene_obj->gt4_face; //** scene vertices **
	poly_ptr = scene_obj->gt4_ptr[cdb->buffnum]; 
	count = scene_obj->gt4_cnt; //** num faces on this scene **

	if ( (color_override = scene_obj->sod->color_override) )
		{
//		if ( color_override == TINT_OVERRIDE )
			tint_c0 = scene_obj->sod->tint_c0;
		
		}


	while ( count )
		{
		gte_ldv3c( &face_source->v0 );	//load vertices to GTE
		gte_rtpt();								//rottrans them with local matrix
		count--;

		gte_nclip();							//set flag for backface clipping
		gte_stopz( &nclip );

	
		// mipmap must be called even if polygon offscn, cuz it may miss
		// trigger flag
//		do_poly_mipmap4( poly_ptr, face_source, scene_obj );

		if ( nclip > 0)
			{

			if ( color_override )
				{
				if ( color_override == TINT_OVERRIDE )
					{
					poly_ptr->r0 = poly_ptr->r1 = poly_ptr->r2 = poly_ptr->r3 = tint_c0.r;
					poly_ptr->g0 = poly_ptr->g1 = poly_ptr->g2 = poly_ptr->g3 = tint_c0.g;
					poly_ptr->b0 = poly_ptr->b1 = poly_ptr->b2 = poly_ptr->b3 = tint_c0.b;
					//FntPrint("c=%d\n",poly_ptr->r0);
					}

				if ( color_override == COLOR_OVERRIDE )
					{
					poly_ptr->r0 = face_source->c0.r;
					poly_ptr->r1 = face_source->c1.r;
					poly_ptr->r2 = face_source->c2.r;
					poly_ptr->r3 = face_source->c3.r;

					poly_ptr->g0 = face_source->c0.g;
					poly_ptr->g1 = face_source->c1.g;
					poly_ptr->g2 = face_source->c2.g;
					poly_ptr->g3 = face_source->c3.g;

					poly_ptr->b0 = face_source->c0.b;
					poly_ptr->b1 = face_source->c1.b;
					poly_ptr->b2 = face_source->c2.b;
					poly_ptr->b3 = face_source->c3.b;

					}
				}

			gte_stsxy3_gt4( ( ULONG *)poly_ptr );	//store 2d coords in polygon
		  	gte_avsz3();
			gte_stotz( &otz );							// setup depth sort flag

			if ( otz > 0 && otz < 16384 )
				{ 
				otz >>= OT_SHIFT;	 						// place poly in ordering tab

				// 4th vertex
				gte_ldv0( &face_source->v3 );
				gte_rtps();
				addPrim ( ot + otz + NEAREST_DEPTH, poly_ptr );

				gte_stsxy( ( ULONG *)&poly_ptr->x3 );
				}
			}
		face_source++;
		poly_ptr++;
		}
}











/**********************************************************************
 **********************************************************************
 **																						**
 **																						**
 **    Phong lit polygon draw functions										**
 **																						**
 **																						**
 **********************************************************************
 **********************************************************************/

void rl_phong_draw_scene_g3s( Scene_obj *scene_obj, MATRIX *local_mat )
{
G3_face  *face_source;
POLY_GT3  *poly_ptr,*e_poly_ptr;
ULONG otz;
LONG nclip;
LONG *ot = cdb->ot + scene_obj->sod->user_depth;
UBYTE code,e_code;
WORD count;

WORD u0,u1,u2,u3,v0,v1,v2,v3;


	face_source = scene_obj->g3_face; //** scene vertices **
	poly_ptr = scene_obj->g3_ptr[cdb->buffnum]; 


	e_poly_ptr = scene_obj->eg3_ptr[cdb->buffnum]; 

	count = scene_obj->g3_cnt; //** num faces on this scene **


	//return;


	while ( count )
		{
		gte_ldv3c( &face_source->v0 );	//load vertices to GTE
		gte_rtpt();								//rottrans them with local matrix
		count--;

		gte_nclip();							//set flag for backface clipping
		gte_stopz( &nclip );

	
		if ( nclip > 0)
			{
			gte_stsxy3_gt3( poly_ptr );	//store 2d coords in polygon
			gte_stsxy3_gt3( e_poly_ptr );	//store 2d coords in polygon
		  	gte_avsz3();
			gte_stotz( &otz );							// setup depth sort flag

			if ( otz >0 && otz < 16384 )
				{ 
				PushMatrix();
				gte_SetRotMatrix( local_mat );
				gte_SetTransMatrix( local_mat );
				getuv3(  &face_source -> n0, e_poly_ptr );
				PopMatrix();


//				gte_SetRotMatrix( &storemat );
//				gte_SetTransMatrix( &storemat );



				gte_ldv0 ( &face_source->n0 );
				gte_ldrgb( &face_source->c0 );
				gte_nccs();
				code = poly_ptr->code;
				e_code = e_poly_ptr->code;
				gte_strgb((CVECTOR *)&poly_ptr->r0);
				gte_strgb((CVECTOR *)&e_poly_ptr->r0);


				gte_ldv0 ( &face_source->n1 );
				gte_ldrgb( &face_source->c1 );
				gte_nccs();
				otz >>= OT_SHIFT;	 						// place poly in ordering tab
				gte_strgb((CVECTOR *)&poly_ptr->r1);
				gte_strgb((CVECTOR *)&e_poly_ptr->r1);

				gte_ldv0 ( &face_source->n2 );
				gte_ldrgb( &face_source->c2 );
				gte_nccs();
				gte_strgb((CVECTOR *)&poly_ptr->r2);
				gte_strgb((CVECTOR *)&e_poly_ptr->r2);
				poly_ptr->code = code;
				e_poly_ptr->code = e_code;


				addPrim ( ot + otz + NEAREST_DEPTH, e_poly_ptr );
				addPrim ( ot + otz + NEAREST_DEPTH, poly_ptr );

				}

			}

		face_source++;
		poly_ptr++;
		e_poly_ptr++;
		}

}






void rl_phong_draw_scene_g4s( Scene_obj *scene_obj, MATRIX *local_mat )
{
G4_face  *face_source;
POLY_GT4  *poly_ptr,*e_poly_ptr;
ULONG otz;
LONG nclip;
LONG *ot = cdb->ot + scene_obj->sod->user_depth;
UBYTE code,e_code;
WORD count;
WORD u0,u1,u2,u3,v0,v1,v2,v3;

	face_source = scene_obj->g4_face; //** scene vertices **
	poly_ptr = scene_obj->g4_ptr[cdb->buffnum]; 

	e_poly_ptr = scene_obj->eg4_ptr[cdb->buffnum]; 

	count = scene_obj->g4_cnt; //** num faces on this scene **

	//gte_ReadMatrix( &storemat );


	while ( count )
		{
		gte_ldv3c( &face_source->v0 );	//load vertices to GTE
		gte_rtpt();								//rottrans them with local matrix
		count--;

		gte_nclip();							//set flag for backface clipping
		gte_stopz( &nclip );

	
		if ( nclip > 0)
			{
			gte_stsxy3_gt4( poly_ptr );	//store 2d coords in polygon
			gte_stsxy3_gt4( e_poly_ptr );	//store 2d coords in polygon
		  	gte_avsz3();
			gte_stotz( &otz );							// setup depth sort flag

			if ( otz >0 && otz < 16384 )
				{ 
				PushMatrix();
				gte_SetRotMatrix( local_mat );
				gte_SetTransMatrix( local_mat );
				getuv4( &face_source -> n0, e_poly_ptr );
				PopMatrix();

				gte_ldv0 ( &face_source->n0 );
				gte_ldrgb( &face_source->c0 );
				gte_nccs();
				code = poly_ptr->code;
				e_code = e_poly_ptr->code;
				gte_strgb((CVECTOR *)&poly_ptr->r0);
				gte_strgb((CVECTOR *)&e_poly_ptr->r0);


				gte_ldv0 ( &face_source->n1 );
				gte_ldrgb( &face_source->c1 );
				gte_nccs();
				otz >>= OT_SHIFT;	 						// place poly in ordering tab
				gte_strgb((CVECTOR *)&poly_ptr->r1);
				gte_strgb((CVECTOR *)&e_poly_ptr->r1);

				gte_ldv0 ( &face_source->n2 );
				gte_ldrgb( &face_source->c2 );
				gte_nccs();
				gte_strgb((CVECTOR *)&poly_ptr->r2);
				gte_strgb((CVECTOR *)&e_poly_ptr->r2);


				gte_ldv0 ( &face_source->n3 );
				gte_ldrgb( &face_source->c3 );
				gte_nccs();
				gte_strgb((CVECTOR *)&poly_ptr->r3);
				gte_strgb((CVECTOR *)&e_poly_ptr->r3);
				poly_ptr->code = code;
				e_poly_ptr->code = e_code;


				// 4th vertex
				gte_ldv0( &face_source->v3 );
				gte_rtps();
				addPrim ( ot + otz + NEAREST_DEPTH, e_poly_ptr );
				gte_stsxy( ( ULONG *)&poly_ptr->x3 );
				gte_stsxy( ( ULONG *)&e_poly_ptr->x3 );
				addPrim ( ot + otz + NEAREST_DEPTH, poly_ptr );

				}

			}

		face_source++;
		poly_ptr++;
		e_poly_ptr++;
		}

}










void rl_phong_draw_scene_gt3s( Scene_obj *scene_obj, MATRIX *local_mat )
{
GT3_face  *face_source;
POLY_GT3  *poly_ptr,*e_poly_ptr;
ULONG otz;
LONG nclip;
LONG *ot = cdb->ot + scene_obj->sod->user_depth;
UBYTE code,e_code;
WORD count;

WORD u0,u1,u2,u3,v0,v1,v2,v3;


	face_source = scene_obj->gt3_face; //** scene vertices **
	poly_ptr = scene_obj->gt3_ptr[cdb->buffnum]; 


	e_poly_ptr = scene_obj->egt3_ptr[cdb->buffnum]; 

	count = scene_obj->gt3_cnt; //** num faces on this scene **



	while ( count )
		{
		gte_ldv3c( &face_source->v0 );	//load vertices to GTE
		gte_rtpt();								//rottrans them with local matrix
		count--;

		gte_nclip();							//set flag for backface clipping
		gte_stopz( &nclip );

	
		if ( nclip > 0)
			{
			gte_stsxy3_gt3( poly_ptr );	//store 2d coords in polygon
			gte_stsxy3_gt3( e_poly_ptr );	//store 2d coords in polygon
		  	gte_avsz3();
			gte_stotz( &otz );							// setup depth sort flag

			if ( otz >0 && otz < 16384 )
				{ 
				PushMatrix();
				gte_SetRotMatrix( local_mat );
				gte_SetTransMatrix( local_mat );
				getuv3( &face_source -> n0, e_poly_ptr );
				PopMatrix();


				gte_ldv0 ( &face_source->n0 );
				gte_ldrgb( &face_source->c0 );
				gte_nccs();
				code = poly_ptr->code;
				e_code = e_poly_ptr->code;
				gte_strgb((CVECTOR *)&poly_ptr->r0);
				gte_strgb((CVECTOR *)&e_poly_ptr->r0);


				gte_ldv0 ( &face_source->n1 );
				gte_ldrgb( &face_source->c1 );
				gte_nccs();
				otz >>= OT_SHIFT;	 						// place poly in ordering tab
				gte_strgb((CVECTOR *)&poly_ptr->r1);
				gte_strgb((CVECTOR *)&e_poly_ptr->r1);

				gte_ldv0 ( &face_source->n2 );
				gte_ldrgb( &face_source->c2 );
				gte_nccs();
				gte_strgb((CVECTOR *)&poly_ptr->r2);
				gte_strgb((CVECTOR *)&e_poly_ptr->r2);
				poly_ptr->code = code;
				e_poly_ptr->code = e_code;


				addPrim ( ot + otz + NEAREST_DEPTH, e_poly_ptr );
				addPrim ( ot + otz + NEAREST_DEPTH, poly_ptr );

				}

			}

		face_source++;
		poly_ptr++;
		e_poly_ptr++;
		}

}



void rl_phong_draw_scene_gt4s( Scene_obj *scene_obj, MATRIX *local_mat )
{
GT4_face  *face_source;
POLY_GT4  *poly_ptr,*e_poly_ptr;
ULONG otz;
LONG nclip;
LONG *ot = cdb->ot + scene_obj->sod->user_depth;
UBYTE code,e_code;
WORD count;
WORD u0,u1,u2,u3,v0,v1,v2,v3;


	face_source = scene_obj->gt4_face; //** scene vertices **
	poly_ptr = scene_obj->gt4_ptr[cdb->buffnum]; 

	e_poly_ptr = scene_obj->egt4_ptr[cdb->buffnum]; 

	count = scene_obj->gt4_cnt; //** num faces on this scene **


	while ( count )
		{
		gte_ldv3c( &face_source->v0 );	//load vertices to GTE
		gte_rtpt();								//rottrans them with local matrix
		count--;

		gte_nclip();							//set flag for backface clipping
		gte_stopz( &nclip );

	
		if ( nclip > 0)
			{
			gte_stsxy3_gt4( poly_ptr );	//store 2d coords in polygon
			gte_stsxy3_gt4( e_poly_ptr );	//store 2d coords in polygon
		  	gte_avsz3();
			gte_stotz( &otz );							// setup depth sort flag

			if ( otz >0 && otz < 16384 )
				{ 
				PushMatrix();
				gte_SetRotMatrix( local_mat );
				gte_SetTransMatrix( local_mat );
				getuv4( &face_source -> n0, e_poly_ptr );
				PopMatrix();


				gte_ldv0 ( &face_source->n0 );
				gte_ldrgb( &face_source->c0 );
				gte_nccs();
				code = poly_ptr->code;
				e_code = e_poly_ptr->code;
				gte_strgb((CVECTOR *)&poly_ptr->r0);
				gte_strgb((CVECTOR *)&e_poly_ptr->r0);


				gte_ldv0 ( &face_source->n1 );
				gte_ldrgb( &face_source->c1 );
				gte_nccs();
				otz >>= OT_SHIFT;	 						// place poly in ordering tab
				gte_strgb((CVECTOR *)&poly_ptr->r1);
				gte_strgb((CVECTOR *)&e_poly_ptr->r1);

				gte_ldv0 ( &face_source->n2 );
				gte_ldrgb( &face_source->c2 );
				gte_nccs();
				gte_strgb((CVECTOR *)&poly_ptr->r2);
				gte_strgb((CVECTOR *)&e_poly_ptr->r2);


				gte_ldv0 ( &face_source->n3 );
				gte_ldrgb( &face_source->c3 );
				gte_nccs();
				gte_strgb((CVECTOR *)&poly_ptr->r3);
				gte_strgb((CVECTOR *)&e_poly_ptr->r3);
				poly_ptr->code = code;
				e_poly_ptr->code = e_code;


				// 4th vertex
				gte_ldv0( &face_source->v3 );
				gte_rtps();
				addPrim ( ot + otz + NEAREST_DEPTH, e_poly_ptr );
				gte_stsxy( ( ULONG *)&poly_ptr->x3 );
				gte_stsxy( ( ULONG *)&e_poly_ptr->x3 );
				addPrim ( ot + otz + NEAREST_DEPTH, poly_ptr );
				}

			}

		face_source++;
		poly_ptr++;
		e_poly_ptr++;
		}

}






/**********************************************************************
 **********************************************************************
 **																						**
 **																						**
 **    Real lit polygon draw functions											**
 **	 ( for Point and directional lights )									**
 **																						**
 **********************************************************************
 **********************************************************************/


void rl_draw_scene_g3s( Scene_obj *scene_obj, SVECTOR *light_transposed, WORD light0_type )
{
G3_face  *face_source;
POLY_GT3  *poly_ptr;

UBYTE code;
ULONG otz;
LONG nclip;
LONG *ot = cdb->ot + scene_obj->sod->user_depth;
WORD count;
VECTOR pos_a,pos_b,pos_c;
MATRIX lmat;

	face_source = scene_obj->g3_face; //** scene vertices **
	poly_ptr = scene_obj->g3_ptr[cdb->buffnum]; 
	count = scene_obj->g3_cnt; //** num faces on this scene **

		
	while ( count )
		{
		gte_ldv3c( &face_source->v0 );	//load vertices to GTE
		gte_rtpt();								//rottrans them with local matrix
		count--;

		gte_nclip();							//set flag for backface clipping
		gte_stopz( &nclip );


		if ( nclip > 0)
			{
			gte_stsxy3_gt3( ( ULONG *)poly_ptr );	//store 2d coords in polygon

		  	gte_avsz3();
			gte_stotz( &otz );							// setup depth sort flag

			if ( otz > 0 && otz < 16384 )
				{ 
				otz >>= OT_SHIFT;	 						// place poly in ordering tab

				code = poly_ptr->code;


				if ( light.any_hw_light_active == YES )
					{
					if ( light0_type == POINT_LIGHT ) 
						{
						pos_a.vx = light_transposed->vx - face_source->v0.vx;
						pos_a.vy = light_transposed->vy - face_source->v0.vy;
						pos_a.vz = light_transposed->vz - face_source->v0.vz;
						pos_b.vx = light_transposed->vx - face_source->v1.vx;
						pos_b.vy = light_transposed->vy - face_source->v1.vy;
						pos_b.vz = light_transposed->vz - face_source->v1.vz;
						pos_c.vx = light_transposed->vx - face_source->v2.vx;
						pos_c.vy = light_transposed->vy - face_source->v2.vy;
						pos_c.vz = light_transposed->vz - face_source->v2.vz;


					//*** Vertex a ***
						VectorNormalS ( &pos_a, (SVECTOR *)&lmat.m[0][0] ); //normalize and output straight to matrix
						gte_SetLightMatrix(&lmat);
						gte_ldv0 ( &face_source->n0 );
						gte_ldrgb( &face_source->c0 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r0);



					//*** Vertex b ***
						VectorNormalS ( &pos_b, (SVECTOR *)&lmat.m[0][0] ); //normalize and output straight to matrix
						gte_SetLightMatrix(&lmat);
						gte_ldv0 ( &face_source->n1 );
						gte_ldrgb( &face_source->c1 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r1);

					//*** Vertex c ***
						VectorNormalS ( &pos_c, (SVECTOR *)&lmat.m[0][0] ); //normalize and output straight to matrix
						gte_SetLightMatrix(&lmat);
						gte_ldv0 ( &face_source->n2 );
						gte_ldrgb( &face_source->c2 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r2);

						}

					else 
						{
					//*** Vertex a ***
						gte_ldv0 ( &face_source->n0 );
						gte_ldrgb( &face_source->c0 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r0);

					//*** Vertex b ***
						gte_ldv0 ( &face_source->n1 );
						gte_ldrgb( &face_source->c1 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r1);

					//*** Vertex c ***
						gte_ldv0 ( &face_source->n2 );
						gte_ldrgb( &face_source->c2 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r2);
						}
					}



				addPrim ( ot + otz + NEAREST_DEPTH, poly_ptr );




				poly_ptr->code = code;

				}

			}


		face_source++;
		poly_ptr++;
		}


}



void rl_draw_scene_g4s( Scene_obj *scene_obj, SVECTOR *light_transposed, WORD light0_type )
{
G4_face  *face_source;
POLY_GT4  *poly_ptr;
UBYTE code;
ULONG otz;
LONG nclip;
LONG *ot = cdb->ot + scene_obj->sod->user_depth;
WORD count;
VECTOR pos_a,pos_b,pos_c,pos_d;
MATRIX lmat;


	face_source = scene_obj->g4_face; //** scene vertices **
	poly_ptr = scene_obj->g4_ptr[cdb->buffnum]; 
	count = scene_obj->g4_cnt; //** num faces on this scene **

	while ( count )
		{
		gte_ldv3c( &face_source->v0 );	//load vertices to GTE
		gte_rtpt();								//rottrans them with local matrix
		count--;

		gte_nclip();							//set flag for backface clipping
		gte_stopz( &nclip );


		if ( nclip > 0)
			{
			gte_stsxy3_gt4( ( ULONG *)poly_ptr );	//store 2d coords in polygon

		  	gte_avsz3();
			gte_stotz( &otz );							// setup depth sort flag

			if ( otz > 0 && otz < 16384 )
				{ 
				otz >>= OT_SHIFT;	 						// place poly in ordering tab

				code = poly_ptr->code;

				if ( light.any_hw_light_active == YES )
					{
					if ( light0_type == POINT_LIGHT ) 
						{
						pos_a.vx = light_transposed->vx - face_source->v0.vx;
						pos_a.vy = light_transposed->vy - face_source->v0.vy;
						pos_a.vz = light_transposed->vz - face_source->v0.vz;
						pos_b.vx = light_transposed->vx - face_source->v1.vx;
						pos_b.vy = light_transposed->vy - face_source->v1.vy;
						pos_b.vz = light_transposed->vz - face_source->v1.vz;
						pos_c.vx = light_transposed->vx - face_source->v2.vx;
						pos_c.vy = light_transposed->vy - face_source->v2.vy;
						pos_c.vz = light_transposed->vz - face_source->v2.vz;
						pos_d.vx = light_transposed->vx - face_source->v3.vx;
						pos_d.vy = light_transposed->vy - face_source->v3.vy;
						pos_d.vz = light_transposed->vz - face_source->v3.vz;


					//*** Vertex a ***
						VectorNormalS ( &pos_a, (SVECTOR *)&lmat.m[0][0] ); //normalize and output straight to matrix
						gte_SetLightMatrix(&lmat);
						gte_ldv0 ( &face_source->n0 );
						gte_ldrgb( &face_source->c0 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r0);



					//*** Vertex b ***
						VectorNormalS ( &pos_b, (SVECTOR *)&lmat.m[0][0] ); //normalize and output straight to matrix
						gte_SetLightMatrix(&lmat);
						gte_ldv0 ( &face_source->n1 );
						gte_ldrgb( &face_source->c1 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r1);

					//*** Vertex c ***
						VectorNormalS ( &pos_c, (SVECTOR *)&lmat.m[0][0] ); //normalize and output straight to matrix
						gte_SetLightMatrix(&lmat);
						gte_ldv0 ( &face_source->n2 );
						gte_ldrgb( &face_source->c2 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r2);

					//*** Vertex d ***
						VectorNormalS ( &pos_d, (SVECTOR *)&lmat.m[0][0] ); //normalize and output straight to matrix
						gte_SetLightMatrix(&lmat);
						gte_ldv0 ( &face_source->n3 );
						gte_ldrgb( &face_source->c3 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r3);

						}

					else 
						{
					//*** Vertex a ***
						gte_ldv0 ( &face_source->n0 );
						gte_ldrgb( &face_source->c0 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r0);

					//*** Vertex b ***
						gte_ldv0 ( &face_source->n1 );
						gte_ldrgb( &face_source->c1 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r1);

					//*** Vertex c ***
						gte_ldv0 ( &face_source->n2 );
						gte_ldrgb( &face_source->c2 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r2);

					//*** Vertex d ***
						gte_ldv0 ( &face_source->n3 );
						gte_ldrgb( &face_source->c3 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r3);

						}
					}



				// 4th vertex
				gte_ldv0( &face_source->v3 );
				gte_rtps();
				addPrim ( ot + otz + NEAREST_DEPTH, poly_ptr );
				gte_stsxy( ( ULONG *)&poly_ptr->x3 );

				poly_ptr->code = code;
				}

			}


		face_source++;
		poly_ptr++;
		}


}




void rl_draw_scene_gt3s( Scene_obj *scene_obj, SVECTOR *light_transposed, WORD light0_type )
{
GT3_face  *face_source;
POLY_GT3  *poly_ptr;
UBYTE code;
ULONG otz;
LONG nclip;
LONG *ot = cdb->ot + scene_obj->sod->user_depth;
WORD count;
MATRIX lmat;
VECTOR pos_a,pos_b,pos_c;


	face_source = scene_obj->gt3_face; //** scene vertices **
	poly_ptr = scene_obj->gt3_ptr[cdb->buffnum]; 
	count = scene_obj->gt3_cnt; //** num faces on this scene **

	while ( count )
		{
		gte_ldv3c( &face_source->v0 );	//load vertices to GTE
		gte_rtpt();								//rottrans them with local matrix
		count--;

		gte_nclip();							//set flag for backface clipping
		gte_stopz( &nclip );

//		do_poly_mipmap3( poly_ptr, face_source, scene_obj );

		if ( nclip > 0)
			{
			gte_stsxy3_gt3( ( ULONG *)poly_ptr );	//store 2d coords in polygon

		  	gte_avsz3();
			gte_stotz( &otz );							// setup depth sort flag

			if ( otz > 0 && otz < 16384 )
				{ 
				otz >>= OT_SHIFT;	 						// place poly in ordering tab

				code = poly_ptr->code;


				if ( light.any_hw_light_active == YES )
					{
					if ( light0_type == POINT_LIGHT ) 
						{
						pos_a.vx = light_transposed->vx - face_source->v0.vx;
						pos_a.vy = light_transposed->vy - face_source->v0.vy;
						pos_a.vz = light_transposed->vz - face_source->v0.vz;
						pos_b.vx = light_transposed->vx - face_source->v1.vx;
						pos_b.vy = light_transposed->vy - face_source->v1.vy;
						pos_b.vz = light_transposed->vz - face_source->v1.vz;
						pos_c.vx = light_transposed->vx - face_source->v2.vx;
						pos_c.vy = light_transposed->vy - face_source->v2.vy;
						pos_c.vz = light_transposed->vz - face_source->v2.vz;


					//*** Vertex a ***
						VectorNormalS ( &pos_a, (SVECTOR *)&lmat.m[0][0] ); //normalize and output straight to matrix
						gte_SetLightMatrix(&lmat);
						gte_ldv0 ( &face_source->n0 );
						gte_ldrgb( &face_source->c0 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r0);



					//*** Vertex b ***
						VectorNormalS ( &pos_b, (SVECTOR *)&lmat.m[0][0] ); //normalize and output straight to matrix
						gte_SetLightMatrix(&lmat);
						gte_ldv0 ( &face_source->n1 );
						gte_ldrgb( &face_source->c1 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r1);

					//*** Vertex c ***
						VectorNormalS ( &pos_c, (SVECTOR *)&lmat.m[0][0] ); //normalize and output straight to matrix
						gte_SetLightMatrix(&lmat);
						gte_ldv0 ( &face_source->n2 );
						gte_ldrgb( &face_source->c2 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r2);

						}

					else 
						{
					//*** Vertex a ***
						gte_ldv0 ( &face_source->n0 );
						gte_ldrgb( &face_source->c0 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r0);

					//*** Vertex b ***
						gte_ldv0 ( &face_source->n1 );
						gte_ldrgb( &face_source->c1 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r1);

					//*** Vertex c ***
						gte_ldv0 ( &face_source->n2 );
						gte_ldrgb( &face_source->c2 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r2);
						}
					}



				addPrim ( ot + otz + NEAREST_DEPTH, poly_ptr );




				poly_ptr->code = code;

				}

			}


		face_source++;
		poly_ptr++;
		}


}



void rl_draw_scene_gt4s( Scene_obj *scene_obj, SVECTOR *light_transposed, WORD light0_type )
{
GT4_face  *face_source;
POLY_GT4  *poly_ptr;
UBYTE code;
ULONG otz;
LONG nclip;
LONG *ot = cdb->ot + scene_obj->sod->user_depth;
WORD count;
MATRIX lmat;
VECTOR pos_a,pos_b,pos_c,pos_d;


	face_source = scene_obj->gt4_face; //** scene vertices **
	poly_ptr = scene_obj->gt4_ptr[cdb->buffnum]; 
	count = scene_obj->gt4_cnt; //** num faces on this scene **

	while ( count )
		{
		gte_ldv3c( &face_source->v0 );	//load vertices to GTE
		gte_rtpt();								//rottrans them with local matrix
		count--;

		gte_nclip();							//set flag for backface clipping
		gte_stopz( &nclip );


		if ( nclip > 0)
			{
			gte_stsxy3_gt4( ( ULONG *)poly_ptr );	//store 2d coords in polygon

		  	gte_avsz3();
			gte_stotz( &otz );							// setup depth sort flag

//			do_poly_mipmap4( poly_ptr, face_source, scene_obj );

			if ( otz > 0 && otz < 16384 )
				{ 
				otz >>= OT_SHIFT;	 						// place poly in ordering tab

				code = poly_ptr->code;


				if ( light.any_hw_light_active == YES )
					{
					if ( light0_type == POINT_LIGHT ) 
						{
						pos_a.vx = light_transposed->vx - face_source->v0.vx;
						pos_a.vy = light_transposed->vy - face_source->v0.vy;
						pos_a.vz = light_transposed->vz - face_source->v0.vz;
						pos_b.vx = light_transposed->vx - face_source->v1.vx;
						pos_b.vy = light_transposed->vy - face_source->v1.vy;
						pos_b.vz = light_transposed->vz - face_source->v1.vz;
						pos_c.vx = light_transposed->vx - face_source->v2.vx;
						pos_c.vy = light_transposed->vy - face_source->v2.vy;
						pos_c.vz = light_transposed->vz - face_source->v2.vz;
						pos_d.vx = light_transposed->vx - face_source->v3.vx;
						pos_d.vy = light_transposed->vy - face_source->v3.vy;
						pos_d.vz = light_transposed->vz - face_source->v3.vz;


					//*** Vertex a ***
						VectorNormalS ( &pos_a, (SVECTOR *)&lmat.m[0][0] ); //normalize and output straight to matrix
						gte_SetLightMatrix(&lmat);
						gte_ldv0 ( &face_source->n0 );
						gte_ldrgb( &face_source->c0 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r0);



					//*** Vertex b ***
						VectorNormalS ( &pos_b, (SVECTOR *)&lmat.m[0][0] ); //normalize and output straight to matrix
						gte_SetLightMatrix(&lmat);
						gte_ldv0 ( &face_source->n1 );
						gte_ldrgb( &face_source->c1 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r1);

					//*** Vertex c ***
						VectorNormalS ( &pos_c, (SVECTOR *)&lmat.m[0][0] ); //normalize and output straight to matrix
						gte_SetLightMatrix(&lmat);
						gte_ldv0 ( &face_source->n2 );
						gte_ldrgb( &face_source->c2 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r2);

					//*** Vertex d ***
						VectorNormalS ( &pos_d, (SVECTOR *)&lmat.m[0][0] ); //normalize and output straight to matrix
						gte_SetLightMatrix(&lmat);
						gte_ldv0 ( &face_source->n3 );
						gte_ldrgb( &face_source->c3 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r3);
						}

					else 
						{
					//*** Vertex a ***
						gte_ldv0 ( &face_source->n0 );
						gte_ldrgb( &face_source->c0 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r0);

					//*** Vertex b ***
						gte_ldv0 ( &face_source->n1 );
						gte_ldrgb( &face_source->c1 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r1);

					//*** Vertex c ***
						gte_ldv0 ( &face_source->n2 );
						gte_ldrgb( &face_source->c2 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r2);

					//*** Vertex d ***
						gte_ldv0 ( &face_source->n3 );
						gte_ldrgb( &face_source->c3 );
						gte_nccs();
						gte_strgb((CVECTOR *)&poly_ptr->r3);

						}
					}



				// 4th vertex
				gte_ldv0( &face_source->v3 );
				gte_rtps();
				addPrim ( ot + otz + NEAREST_DEPTH, poly_ptr );
				gte_stsxy( ( ULONG *)&poly_ptr->x3 );


				poly_ptr->code = code;

				}

			}


		face_source++;
		poly_ptr++;
		}


}



#endif


























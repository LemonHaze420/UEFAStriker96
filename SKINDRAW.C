
/****************************************************
 *																	 *
 *	 Skin object draw											 *
 *																	 *
 ****************************************************/



#include "incs.h"

#ifdef PSX
//#else
//#include "defs_gen.h"
//#include "lib_psx.h"	// set to libn64.h , libpc.h or libpsx.h
//#include "defs_psx.h"
//#include "..\..\global\striker.h"	 // share structure for all formats
#include "..\..\global\s01.h"	// structs used by scene "s01" format
#include "skindraw.h"
#include	"sod.h"
#include	"scene.h"
//#include	"structs.h"
//#include	"main.h"
#include	"general.h"
#include	"frontend.h"
#include "hardware.h"
#include	"camera.h"
#include	"io.h"
#endif

#define M_IDENTITY {{{4096,0,0},{0,4096,0},{0,0,4096}},{0,0,0}}


void setup_skin_g3s( Skin_obj *skin_obj, Skin_obj_src *skin_obj_src );
void setup_skin_g4s( Skin_obj *skin_obj, Skin_obj_src *skin_obj_src );
void setup_skin_gt3s( Skin_obj *skin_obj, Skin_obj_src *skin_obj_src );
void setup_skin_gt4s( Skin_obj *skin_obj, Skin_obj_src *skin_obj_src );

void pl_draw_skin_g3s( Skin_obj *obj );
void pl_draw_skin_g4s( Skin_obj *obj );
void pl_draw_skin_gt3s( Skin_obj *obj );
void pl_draw_skin_gt4s( Skin_obj *obj );




volatile SVECTOR sv;
SVECTOR srot={0,0,0,0};
//MATRIX spin=M_IDENTITY;

WORD temp_anim_offs=0;

void setup_skinned_object( Skin_header_src *header_src, Skin_obj *obj )
{
BYTE *addr = (BYTE *)header_src;
Skin_obj_src *obj_src;
WORD total_matrices,cnt;		
MATRIX *mat;



//	main_wheel.keyboard_active = YES;

	addr += sizeof ( Skin_header_src );
	obj_src = (Skin_obj_src *)addr;


	obj->g3_cnt  = obj_src->g3_cnt;
	obj->g4_cnt  = obj_src->g4_cnt;
	obj->gt3_cnt = obj_src->gt3_cnt;
	obj->gt4_cnt = obj_src->gt4_cnt;

  	obj->real_lit = obj_src->light_active;
	obj->current_anim_frame = 0;
	obj->last_anim_frame = -1;
	obj->anim_initialised = NO;
	obj->anim_timer = NO;
	obj->active = YES;
	obj->depth = MIDDLE_DEPTH;
	obj->anim_frames = obj_src->skin_anim_frames;
	obj->anim_matrices = obj_src->skin_anim_matrices;

	obj->pos.vx = obj->pos.vy = obj->pos.vz = 0;
	obj->pos2.vx = obj->pos2.vy = obj->pos2.vz = 0;
	obj->rot.vx = obj->rot.vy = obj->rot.vz = 0;
	obj->rot2.vx = obj->rot2.vy = obj->rot2.vz = 0;

	addr = (BYTE *)obj_src;
	addr += obj_src->matrix_offs;
	mat = (MATRIX *)addr;
	total_matrices = obj->anim_frames * obj->anim_matrices;
	#if DEBUG
	printf("total mats=%d\n",total_matrices);
	#endif

	obj->matrix_list = (MATRIX *)allocate_mem( 0, sizeof ( MATRIX ) * total_matrices );


//	printf("objmatlist=%x\n",obj->matrix_list);

	for(cnt=0;cnt<total_matrices;cnt++ )
		{
		obj->matrix_list[cnt] = mat[cnt];

//		camera.aspect_correction=YES;
//		fix_aspect_ratio( &mat[cnt] );
		}

	if ( obj->g3_cnt )
		setup_skin_g3s( obj, obj_src );

	if ( obj->g4_cnt )
		setup_skin_g4s( obj, obj_src );


	if ( obj->gt3_cnt )
		setup_skin_gt3s( obj, obj_src );

	if ( obj->gt4_cnt )
		setup_skin_gt4s( obj, obj_src );
}







void setup_skin_g3s( Skin_obj *skin_obj, Skin_obj_src *skin_obj_src )
{		
POLY_GT3 *poly_ptr[2];
Skin_g3_face_src *face_source;
Skin_g3_face *face;
BYTE *src;
WORD count,cnt;	

//** Allocate area for polygon data **
	skin_obj->g3_face = (Skin_g3_face *)allocate_mem ( 0, sizeof(Skin_g3_face) * skin_obj_src->g3_cnt );
	face = skin_obj->g3_face;

//** Allocate area for primitives **
	skin_obj->g3_ptr[0] = (POLY_GT3 *)allocate_mem ( 0, sizeof( POLY_GT3 ) * skin_obj_src->g3_cnt );
	skin_obj->g3_ptr[1] = (POLY_GT3 *)allocate_mem ( 0, sizeof( POLY_GT3 ) * skin_obj_src->g3_cnt );


	poly_ptr[0] = skin_obj->g3_ptr[0];
	poly_ptr[1] = skin_obj->g3_ptr[1];

	src = (BYTE *)skin_obj_src;
	src += skin_obj_src->g3_face_src_offs;
	face_source = (Skin_g3_face_src *)src;



	count = skin_obj_src->g3_cnt;

	while ( count-- )
		{
		face->v0 = face_source->v0;
		face->v1 = face_source->v1;
		face->v2 = face_source->v2;

//		printf("xyzm=%d %d %d %d\n",face->v0.vx,face->v0.vy,face->v0.vz,face->v0.matrix_num);

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

			//printf("rgb=%d %d %d\n",face->c0.r,face->c0.g,face->c0.b );

			poly_ptr[cnt] -> tpage = GetTPage ( DUMMY_TEXTURE_BPP, face_source->transp_rate, DUMMY_TEXTURE_X,DUMMY_TEXTURE_Y ); 
			poly_ptr[cnt] -> u0 = 
			poly_ptr[cnt] -> u1 = 
			poly_ptr[cnt] -> u2 = ( DUMMY_TEXTURE_X <<(2-DUMMY_TEXTURE_BPP)) % (64<<(2-DUMMY_TEXTURE_BPP));
			poly_ptr[cnt] -> v0 = 
			poly_ptr[cnt] -> v1 = 
			poly_ptr[cnt] -> v2 = DUMMY_TEXTURE_Y % 256;
			}

		face++;							  
		face_source++;

		poly_ptr[0]++;
		poly_ptr[1]++;


		}
}

void setup_skin_g4s( Skin_obj *skin_obj, Skin_obj_src *skin_obj_src )
{		
POLY_GT4 *poly_ptr[2];
Skin_g4_face_src *face_source;
Skin_g4_face *face;
BYTE *src;
WORD count,cnt;	

//** Allocate area for polygon data **
	skin_obj->g4_face = (Skin_g4_face *)allocate_mem ( 0, sizeof(Skin_g4_face) * skin_obj_src->g4_cnt );
	face = skin_obj->g4_face;

//** Allocate area for primitives **
	skin_obj->g4_ptr[0] = (POLY_GT4 *)allocate_mem ( 0, sizeof( POLY_GT4 ) * skin_obj_src->g4_cnt );
	skin_obj->g4_ptr[1] = (POLY_GT4 *)allocate_mem ( 0, sizeof( POLY_GT4 ) * skin_obj_src->g4_cnt );


	poly_ptr[0] = skin_obj->g4_ptr[0];
	poly_ptr[1] = skin_obj->g4_ptr[1];

	src = (BYTE *)skin_obj_src;
	src += skin_obj_src->g4_face_src_offs;
	face_source = (Skin_g4_face_src *)src;



	count = skin_obj_src->g4_cnt;

//	printf("setting up %d g4s\n",count );

	while ( count-- )
		{
		face->v0 = face_source->v0;
		face->v1 = face_source->v1;
		face->v2 = face_source->v2;
		face->v3 = face_source->v3;

//		printf("xyzm=%d %d %d %d\n",face->v0.vx,face->v0.vy,face->v0.vz,face->v0.matrix_num);

		face->c0 = face_source->c0;
		face->c1 = face_source->c1;
		face->c2 = face_source->c2;
		face->c3 = face_source->c3;

		for(cnt=0;cnt<2;cnt++)
			{
			SetPolyGT4 ( poly_ptr[cnt]);
			SetSemiTrans ( poly_ptr[cnt],face_source->transp_rate==-1?0:1 );

			poly_ptr[cnt] -> r0  = 255; //face->c0.r;
			poly_ptr[cnt] -> g0  = 255; //face->c0.g;
			poly_ptr[cnt] -> b0  = 255; //face->c0.b;
			poly_ptr[cnt] -> r1  = 255; //face->c1.r;
			poly_ptr[cnt] -> g1  = 255; //face->c1.g;
			poly_ptr[cnt] -> b1  = 255; //face->c1.b;
			poly_ptr[cnt] -> r2  = 255; //face->c2.r;
			poly_ptr[cnt] -> g2  = 255; //face->c2.g;
			poly_ptr[cnt] -> b2  = 255; //face->c2.b;
			poly_ptr[cnt] -> r3  = 255; //face->c3.r;
			poly_ptr[cnt] -> g3  = 255; //face->c3.g;
			poly_ptr[cnt] -> b3  = 255; //face->c3.b;

			//printf("rgb=%d %d %d\n",face->c0.r,face->c0.g,face->c0.b );

			poly_ptr[cnt] -> tpage = GetTPage ( DUMMY_TEXTURE_BPP, face_source->transp_rate, DUMMY_TEXTURE_X,DUMMY_TEXTURE_Y ); 
			poly_ptr[cnt] -> u0 = 
			poly_ptr[cnt] -> u1 = 
			poly_ptr[cnt] -> u2 =
			poly_ptr[cnt] -> u3 = ( DUMMY_TEXTURE_X <<(2-DUMMY_TEXTURE_BPP)) % (64<<(2-DUMMY_TEXTURE_BPP));
			poly_ptr[cnt] -> v0 = 
			poly_ptr[cnt] -> v1 = 
			poly_ptr[cnt] -> v2 =
			poly_ptr[cnt] -> v3 = DUMMY_TEXTURE_Y % 256;

			}

		face++;							  
		face_source++;

		poly_ptr[0]++;
		poly_ptr[1]++;


		}
}

void setup_skin_gt3s( Skin_obj *skin_obj, Skin_obj_src *skin_obj_src )
{		
POLY_GT3 *poly_ptr[2];
Skin_gt3_face_src *face_source;
Skin_gt3_face *face;
BYTE *src;
WORD count,cnt;	

//** Allocate area for polygon data **
	skin_obj->gt3_face = (Skin_gt3_face *)allocate_mem ( 0, sizeof(Skin_gt3_face) * skin_obj_src->gt3_cnt );
	face = skin_obj->gt3_face;

//** Allocate area for primitives **
	skin_obj->gt3_ptr[0] = (POLY_GT3 *)allocate_mem ( 0, sizeof( POLY_GT3 ) * skin_obj_src->gt3_cnt );
	skin_obj->gt3_ptr[1] = (POLY_GT3 *)allocate_mem ( 0, sizeof( POLY_GT3 ) * skin_obj_src->gt3_cnt );


	poly_ptr[0] = skin_obj->gt3_ptr[0];
	poly_ptr[1] = skin_obj->gt3_ptr[1];

	src = (BYTE *)skin_obj_src;
	src += skin_obj_src->gt3_face_src_offs;
	face_source = (Skin_gt3_face_src *)src;



	count = skin_obj_src->gt3_cnt;

	while ( count-- )
		{
		face->v0 = face_source->v0;
		face->v1 = face_source->v1;
		face->v2 = face_source->v2;

//		printf("xyzm=%d %d %d %d\n",face->v0.vx,face->v0.vy,face->v0.vz,face->v0.matrix_num);

		face->c0 = face_source->c0;
		face->c1 = face_source->c1;
		face->c2 = face_source->c2;

		face->t0 = face_source->t0;
		face->t1 = face_source->t1;
		face->t2 = face_source->t2;

		face->tpage = face_source->tpage;

		for(cnt=0;cnt<2;cnt++)
			{
			SetPolyGT3 ( poly_ptr[cnt]);
			SetSemiTrans ( poly_ptr[cnt],face_source->transp_rate==-1?0:1 );

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

			}

		face++;							  
		face_source++;

		poly_ptr[0]++;
		poly_ptr[1]++;


		}
}

void setup_skin_gt4s( Skin_obj *skin_obj, Skin_obj_src *skin_obj_src )
{		
POLY_GT4 *poly_ptr[2];
Skin_gt4_face_src *face_source;
Skin_gt4_face *face;
BYTE *src;
WORD count,cnt;	

//** Allocate area for polygon data **
	skin_obj->gt4_face = (Skin_gt4_face *)allocate_mem ( 0, sizeof(Skin_gt4_face) * skin_obj_src->gt4_cnt );
	face = skin_obj->gt4_face;

//** Allocate area for primitives **
	skin_obj->gt4_ptr[0] = (POLY_GT4 *)allocate_mem ( 0, sizeof( POLY_GT4 ) * skin_obj_src->gt4_cnt );
	skin_obj->gt4_ptr[1] = (POLY_GT4 *)allocate_mem ( 0, sizeof( POLY_GT4 ) * skin_obj_src->gt4_cnt );


	poly_ptr[0] = skin_obj->gt4_ptr[0];
	poly_ptr[1] = skin_obj->gt4_ptr[1];

	src = (BYTE *)skin_obj_src;
	src += skin_obj_src->gt4_face_src_offs;
	face_source = (Skin_gt4_face_src *)src;



	count = skin_obj_src->gt4_cnt;

//	printf("setting up %d gt4s\n",count );

	while ( count-- )
		{
		face->v0 = face_source->v0;
		face->v1 = face_source->v1;
		face->v2 = face_source->v2;
		face->v3 = face_source->v3;

		face->c0 = face_source->c0;
		face->c1 = face_source->c1;
		face->c2 = face_source->c2;
		face->c3 = face_source->c3;

		face->t0 = face_source->t0;
		face->t1 = face_source->t1;
		face->t2 = face_source->t2;
		face->t3 = face_source->t3;

		face->tpage = face_source->tpage;


		for(cnt=0;cnt<2;cnt++)
			{
			SetPolyGT4 ( poly_ptr[cnt]);
			SetSemiTrans ( poly_ptr[cnt],face_source->transp_rate==-1?0:1 );

			poly_ptr[cnt] -> u0 	  = face_source->t0.x;
			poly_ptr[cnt] -> v0 	  = face_source->t0.y;
			poly_ptr[cnt] -> u1 	  = face_source->t1.x;
			poly_ptr[cnt] -> v1 	  = face_source->t1.y;
			poly_ptr[cnt] -> u2 	  = face_source->t2.x;
			poly_ptr[cnt] -> v2 	  = face_source->t2.y;
			poly_ptr[cnt] -> u3 	  = face_source->t3.x;
			poly_ptr[cnt] -> v3 	  = face_source->t3.y;
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


			}

		face++;							  
		face_source++;

		poly_ptr[0]++;
		poly_ptr[1]++;


		}
}









void draw_skin_objs(Skin_obj *obj )
{
MATRIX temp_mat=M_IDENTITY;
MATRIX temp_mat2=M_IDENTITY;



	if ( obj->active )
		{

	//	adjust_svector ( &srot, 20 );

		RotMatrix ( &obj->rot2, &temp_mat );
		RotMatrix ( &obj->rot, &obj->user_matrix );



		TransMatrix ( &temp_mat2, &obj->pos2 );

		
		CompMatrix ( &obj->user_matrix,&temp_mat,   &obj->user_matrix );

		TransMatrix ( &obj->user_matrix, &obj->pos );

		CompMatrix ( &obj->user_matrix, &temp_mat2, &obj->user_matrix );

//		fntprint_mat( "mat=", &temp_mat );


		if ( obj->g3_cnt )
			pl_draw_skin_g3s( obj );

		if ( obj->g4_cnt )
			pl_draw_skin_g4s( obj );

		if ( obj->gt3_cnt )
			pl_draw_skin_gt3s( obj );

		if ( obj->gt4_cnt )
			pl_draw_skin_gt4s( obj );
		}

}




void pl_draw_skin_g3s( Skin_obj *obj )
{
Skin_g3_face  *face_source;
POLY_GT3  *poly_ptr;
ULONG otz;
LONG nclip;
LONG *ot = cdb->ot + obj->depth;
WORD count;
WORD mat_num;
MATRIX id=M_IDENTITY;
MATRIX vertex_matrix;
MATRIX vertex_matrix2;
MATRIX *matrix_list;
MATRIX mat1;



	face_source = obj->g3_face; //** scene vertices **
	poly_ptr = obj->g3_ptr[cdb->buffnum]; 
	count = obj->g3_cnt; //** num faces on this scene **

	matrix_list = obj->matrix_list;


//	FntPrint("mats=%d\n",obj->anim_matrices);
	


//	CompMatrix( &spin, &camera.output_mat, &mat1 );
	mat1 = obj->user_matrix;


	while ( count )
		{

		//** Vert 0 **
		vertex_matrix2 = *(matrix_list + (face_source->v0.matrix_num*obj->anim_matrices)+obj->current_anim_frame);

//		FntPrint("n=%d\n",face_source->v0.matrix_num*obj->anim_matrices );

		CompMatrix ( &mat1, &vertex_matrix2, &vertex_matrix );
		sv.vx = face_source->v0.vx;
		sv.vy = face_source->v0.vy;
		sv.vz = face_source->v0.vz;

//		FntPrint("xyz=%d %d %d\n",sv.vx,sv.vy,sv.vz);

		sv.pad=0;
		gte_SetRotMatrix  ( &vertex_matrix );
		gte_SetTransMatrix( &vertex_matrix );
		gte_ldv0( &sv );
		gte_rtps();
		gte_stsxy( ( ULONG *)&poly_ptr->x0 );

		//** Vert 1 **
		vertex_matrix2 = *(matrix_list + (face_source->v1.matrix_num*obj->anim_matrices)+obj->current_anim_frame);
		CompMatrix ( &mat1, &vertex_matrix2, &vertex_matrix );
		sv.vx = face_source->v1.vx;
		sv.vy = face_source->v1.vy;
		sv.vz = face_source->v1.vz;
		gte_SetRotMatrix  ( &vertex_matrix );
		gte_SetTransMatrix( &vertex_matrix );
		gte_ldv0( &sv );
		gte_rtps();
		gte_stsxy( ( ULONG *)&poly_ptr->x1 );

		//** Vert 2 **
		vertex_matrix2 = *(matrix_list + (face_source->v2.matrix_num*obj->anim_matrices)+obj->current_anim_frame);
		CompMatrix ( &mat1, &vertex_matrix2, &vertex_matrix );
		sv.vx = face_source->v2.vx;
		sv.vy = face_source->v2.vy;
		sv.vz = face_source->v2.vz;
		gte_SetRotMatrix  ( &vertex_matrix );
		gte_SetTransMatrix( &vertex_matrix );
		gte_ldv0( &sv );
		gte_rtps();
		gte_stsxy( ( ULONG *)&poly_ptr->x2 );

		

//		FntPrint("g3 xy1=%d %d xy2=%d %d xy3=%d %d\n",poly_ptr->x0,poly_ptr->y0,poly_ptr->x1,poly_ptr->y1,poly_ptr->x2,poly_ptr->y2);

		count--;
		face_source++;

//		gte_nclip();							//set flag for backface clipping
//		gte_stopz( &nclip );



		otz=0;
		nclip=1;	
//		if ( nclip > 0)
//			{
			//gte_stsxy3_gt3( ( ULONG *)poly_ptr );	//store 2d coords in polygon
//		  	gte_avsz3();
//			gte_stotz( &otz );							// setup depth sort flag

//			if ( otz > 0 && otz < 16384 )
//				{ 
//				otz >>= OT_SHIFT;	 						// place poly in ordering tab

//				poly_ptr->r0 = 255;
//				poly_ptr->r1 = 255;
//				poly_ptr->r2 = 255;

				addPrim ( ot + otz + NEAREST_DEPTH , poly_ptr );

//				}

//			}

		poly_ptr++;
		}

}


void pl_draw_skin_g4s( Skin_obj *obj )
{
Skin_g4_face  *face_source;
POLY_GT4  *poly_ptr;
ULONG otz;
LONG nclip;
LONG *ot = cdb->ot + obj->depth;
WORD count;
WORD mat_num;
MATRIX id=M_IDENTITY;
MATRIX vertex_matrix;
MATRIX vertex_matrix2;
MATRIX *matrix_list;
MATRIX mat1;


	face_source = obj->g4_face; //** scene vertices **
	poly_ptr = obj->g4_ptr[cdb->buffnum]; 
	count = obj->g4_cnt; //** num faces on this scene **

	matrix_list = obj->matrix_list;

//	CompMatrix( &spin, &camera.output_mat, &mat1 );

	mat1 = obj->user_matrix;


	while ( count )
		{

		//** Vert 0 **
		vertex_matrix2 = *(matrix_list + (face_source->v0.matrix_num*obj->anim_matrices)+obj->current_anim_frame);

		CompMatrix ( &mat1, &vertex_matrix2, &vertex_matrix );
		sv.vx = face_source->v0.vx;
		sv.vy = face_source->v0.vy;
		sv.vz = face_source->v0.vz;
		sv.pad=0;
		gte_SetRotMatrix  ( &vertex_matrix );
		gte_SetTransMatrix( &vertex_matrix );
		gte_ldv0( &sv );
		gte_rtps();
		gte_stsxy( ( ULONG *)&poly_ptr->x0 );

		//** Vert 1 **
		vertex_matrix2 = *(matrix_list + (face_source->v1.matrix_num*obj->anim_matrices)+obj->current_anim_frame);
		CompMatrix ( &mat1, &vertex_matrix2, &vertex_matrix );
		sv.vx = face_source->v1.vx;
		sv.vy = face_source->v1.vy;
		sv.vz = face_source->v1.vz;
		gte_SetRotMatrix  ( &vertex_matrix );
		gte_SetTransMatrix( &vertex_matrix );
		gte_ldv0( &sv );
		gte_rtps();
		gte_stsxy( ( ULONG *)&poly_ptr->x1 );

		//** Vert 2 **
		vertex_matrix2 = *(matrix_list + (face_source->v2.matrix_num*obj->anim_matrices)+obj->current_anim_frame);
		CompMatrix ( &mat1, &vertex_matrix2, &vertex_matrix );
		sv.vx = face_source->v2.vx;
		sv.vy = face_source->v2.vy;
		sv.vz = face_source->v2.vz;
		gte_SetRotMatrix  ( &vertex_matrix );
		gte_SetTransMatrix( &vertex_matrix );
		gte_ldv0( &sv );
		gte_rtps();
		gte_stsxy( ( ULONG *)&poly_ptr->x2 );

		//** Vert 3 **
		vertex_matrix2 = *(matrix_list + (face_source->v3.matrix_num*obj->anim_matrices)+obj->current_anim_frame);
		CompMatrix ( &mat1, &vertex_matrix2, &vertex_matrix );
		sv.vx = face_source->v3.vx;
		sv.vy = face_source->v3.vy;
		sv.vz = face_source->v3.vz;
		gte_SetRotMatrix  ( &vertex_matrix );
		gte_SetTransMatrix( &vertex_matrix );
		gte_ldv0( &sv );
		gte_rtps();
		gte_stsxy( ( ULONG *)&poly_ptr->x3 );

		

//		FntPrint("g3 xy1=%d %d xy2=%d %d xy3=%d %d\n",poly_ptr->x0,poly_ptr->y0,poly_ptr->x1,poly_ptr->y1,poly_ptr->x2,poly_ptr->y2);

		count--;
		face_source++;

//		gte_nclip();							//set flag for backface clipping
//		gte_stopz( &nclip );



		nclip=1;	
		otz=0;
//		if ( nclip > 0)
//			{
			//gte_stsxy3_gt3( ( ULONG *)poly_ptr );	//store 2d coords in polygon
//		  	gte_avsz3();
//			gte_stotz( &otz );							// setup depth sort flag

//				otz >>= OT_SHIFT;	 						// place poly in ordering tab

				addPrim ( ot + otz + NEAREST_DEPTH , poly_ptr );

//				}
//
//			}

		poly_ptr++;
		}

}





WORD upcnt=0;



void pl_draw_skin_gt3s( Skin_obj *obj )
{
Skin_gt3_face  *face_source;
POLY_GT3  *poly_ptr;
ULONG otz;
LONG nclip;
LONG *ot = cdb->ot + obj->depth;
WORD count;
MATRIX id=M_IDENTITY;
MATRIX vertex_matrix,vertex_matrix2, *matrix_list,mat1;
ULONG otz1, otz2, otz3;
WORD upcnt;

//	return;
	face_source = obj->gt3_face; //** scene vertices **
	poly_ptr = obj->gt3_ptr[cdb->buffnum]; 
	count = obj->gt3_cnt; //** num faces on this scene **

	matrix_list = obj->matrix_list;


//	CompMatrix( &spin, &camera.output_mat, &mat1 );
	mat1 = obj->user_matrix; //spin;					
	fix_aspect_ratio( &mat1 );

	upcnt=0;
	while ( count )
		{


		//** Vert 0 **
		vertex_matrix2 = *(matrix_list + (face_source->v0.matrix_num*obj->anim_matrices)+obj->current_anim_frame);

		CompMatrix ( &mat1, &vertex_matrix2, &vertex_matrix );
		sv.vx = face_source->v0.vx;
		sv.vy = face_source->v0.vy;
		sv.vz = face_source->v0.vz;
		sv.pad=0;
		gte_SetRotMatrix  ( &vertex_matrix );
		gte_SetTransMatrix( &vertex_matrix );
		gte_ldv0( &sv );
		gte_rtps();
		gte_stsxy( ( ULONG *)&poly_ptr->x0 );
		gte_stsz( &otz1 );

		//** Vert 1 **
		vertex_matrix2 = *(matrix_list + (face_source->v1.matrix_num*obj->anim_matrices)+obj->current_anim_frame);
		CompMatrix ( &mat1, &vertex_matrix2, &vertex_matrix );
		sv.vx = face_source->v1.vx;
		sv.vy = face_source->v1.vy;
		sv.vz = face_source->v1.vz;
		gte_SetRotMatrix  ( &vertex_matrix );
		gte_SetTransMatrix( &vertex_matrix );
		gte_ldv0( &sv );
		gte_rtps();
		gte_stsxy( ( ULONG *)&poly_ptr->x1 );
		gte_stsz( &otz2 );

		//** Vert 2 **
		vertex_matrix2 = *(matrix_list + (face_source->v2.matrix_num*obj->anim_matrices)+obj->current_anim_frame);
		CompMatrix ( &mat1, &vertex_matrix2, &vertex_matrix );
		sv.vx = face_source->v2.vx;
		sv.vy = face_source->v2.vy;
		sv.vz = face_source->v2.vz;
		gte_SetRotMatrix  ( &vertex_matrix );
		gte_SetTransMatrix( &vertex_matrix );
		gte_ldv0( &sv );
		gte_rtps();
		gte_stsxy( ( ULONG *)&poly_ptr->x2 );
		gte_stsz( &otz3 );


		gte_ldsxy3( *( ULONG *)&poly_ptr->x0, *( ULONG *)&poly_ptr->x1, *( ULONG *)&poly_ptr->x2 );
		gte_nclip();
		gte_stopz( &nclip ); 
		count--;
		face_source++;

		if ( nclip > 0 )
			{

		  #if 0
			gte_ldsz3( otz1, otz2, otz3 );
		  	gte_avsz3();
			gte_stszotz( &otz );							// setup depth sort flag

		  #else
			otz = (otz1 + otz2 + otz3)/12;
		  #endif


			if ( otz > 0 && otz < 16384 )
				{ 
				otz >>= OT_SHIFT;	 						// place poly in ordering tab
				//FntPrint("%d  ",otz+NEAREST_DEPTH);

				//otz=0;
				addPrim ( ot + otz + NEAREST_DEPTH , poly_ptr );
				}
			}
		poly_ptr++;
		upcnt++;
		}


}


void pl_draw_skin_gt4s( Skin_obj *obj )
{
Skin_gt4_face  *face_source;
POLY_GT4  *poly_ptr;
ULONG otz;
LONG nclip;
LONG *ot = cdb->ot + obj->depth;
WORD count;
MATRIX id=M_IDENTITY;
MATRIX vertex_matrix,vertex_matrix2, *matrix_list,mat1;
ULONG otz1, otz2, otz3;
WORD upcnt=0;

//	return;

	face_source = obj->gt4_face; //** scene vertices **
	poly_ptr = obj->gt4_ptr[cdb->buffnum]; 
	count = obj->gt4_cnt; //** num faces on this scene **

	matrix_list = obj->matrix_list;


//	CompMatrix( &spin, &camera.output_mat, &mat1 );
	mat1 = obj->user_matrix; //spin;					
	fix_aspect_ratio( &mat1 );

	while ( count )
		{


		//** Vert 1 **
		vertex_matrix2 = *(matrix_list + (face_source->v0.matrix_num*obj->anim_matrices)+obj->current_anim_frame);
		CompMatrix ( &mat1, &vertex_matrix2, &vertex_matrix );
		sv.vx = face_source->v0.vx;
		sv.vy = face_source->v0.vy;
		sv.vz = face_source->v0.vz;
		sv.pad=0;
		gte_SetRotMatrix  ( &vertex_matrix );
		gte_SetTransMatrix( &vertex_matrix );
		gte_ldv0( &sv );
		gte_rtps();
		gte_stsxy( ( ULONG *)&poly_ptr->x0 );
		gte_stsz( &otz1 );

		//** Vert 2 **
		vertex_matrix2 = *(matrix_list + (face_source->v1.matrix_num*obj->anim_matrices)+obj->current_anim_frame);
		CompMatrix ( &mat1, &vertex_matrix2, &vertex_matrix );
		sv.vx = face_source->v1.vx;
		sv.vy = face_source->v1.vy;
		sv.vz = face_source->v1.vz;
		gte_SetRotMatrix  ( &vertex_matrix );
		gte_SetTransMatrix( &vertex_matrix );
		gte_ldv0( &sv );
		gte_rtps();
		gte_stsxy( ( ULONG *)&poly_ptr->x1 );
		gte_stsz( &otz2 );

		//** Vert 3 **
		vertex_matrix2 = *(matrix_list + (face_source->v2.matrix_num*obj->anim_matrices)+obj->current_anim_frame);
		CompMatrix ( &mat1, &vertex_matrix2, &vertex_matrix );
		sv.vx = face_source->v2.vx;
		sv.vy = face_source->v2.vy;
		sv.vz = face_source->v2.vz;
		gte_SetRotMatrix  ( &vertex_matrix );
		gte_SetTransMatrix( &vertex_matrix );
		gte_ldv0( &sv );
		gte_rtps();
		gte_stsxy( ( ULONG *)&poly_ptr->x2 );
		gte_stsz( &otz3 );

		//** Vert 4 **
		vertex_matrix2 = *(matrix_list + (face_source->v3.matrix_num*obj->anim_matrices)+obj->current_anim_frame);
		CompMatrix ( &mat1, &vertex_matrix2, &vertex_matrix );
		sv.vx = face_source->v3.vx;
		sv.vy = face_source->v3.vy;
		sv.vz = face_source->v3.vz;
		gte_SetRotMatrix  ( &vertex_matrix );
		gte_SetTransMatrix( &vertex_matrix );
		gte_ldv0( &sv );
		gte_rtps();
		gte_stsxy( ( ULONG *)&poly_ptr->x3 );

		gte_ldsxy3( *( ULONG *)&poly_ptr->x0, *( ULONG *)&poly_ptr->x1, *( ULONG *)&poly_ptr->x2 );
		gte_nclip();
		gte_stopz( &nclip ); 
		count--;
		face_source++;


		if ( nclip > 0 )
			{
		  #if 1
			otz = (otz1 + otz2 + otz3)/12;
		  #else
			gte_ldsz3( otz1, otz2, otz3 );
		  	gte_avsz3();
			gte_stszotz( &otz );							// setup depth sort flag
		  #endif

			if ( otz > 0 && otz < 16384 )
				{ 
				otz >>= OT_SHIFT;	 						// place poly in ordering tab
				//otz=0;
				addPrim ( ot + otz + NEAREST_DEPTH , poly_ptr );
				}
			}
		poly_ptr++;
		upcnt++;
		}


}



void load_skin_file( WORD file_num, Skin_obj *obj )
{
Skin_header_src *sk;

	push(1);
	sk = allocate_mem( 1, MAX_SKINFILE_SIZE );

	if ( file_header.file[ file_num ].size >= MAX_SKINFILE_SIZE )
		{
		#if DEBUG
		debug_print_mess( "Not enough memory allocated for skin file", 0 );
		#endif
		}

	read_datafile( file_num, (BYTE *)sk, MAX_SKINFILE_SIZE );
	
	setup_skinned_object( sk, obj );

	pop(1);
}

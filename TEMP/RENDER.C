
/****************************************************
 *																	 *
 *	 Scene render												 *
 *  Functions used speifically by PSX to render		 *
 *  a scene														 *
 *																	 *
 ****************************************************/



#include	"incs.h"


//#include "plasma.c"


#ifdef PSX
#include "..\..\global\striker.def" // defs for PSX striker
#include "..\..\striker\frontend\mod.def"
#include "..\..\striker\src\anim.def"
#include "..\..\striker\movies\fruit\mod.def"
#include "..\..\striker\movies\hotdog\mod.def"
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
#include "..\..\global\image.h"
#endif




#define TEXTURE_AREA_SIZE (600*1024)
#define M_IDENTITY {{{4096,0,0},{0,4096,0},{0,0,4096}},{0,0,0}}



/****************************************************
 *																	 *
 *	 Structs														 *
 *																	 *
 ****************************************************/


/****************************************************
 *																	 *
 *	 Prototypes													 *
 *																	 *
 ****************************************************/


void load_scene_materials( WORD texture_file_num );
void setup_dummy_texture();

Scene_header sc_head;
Scene_header *scene_header=&sc_head;

WORD check_if_object_to_be_excluded( WORD current_obj );
WORD get_u0 ( WORD tex_x, WORD bpp );
void build_strip ( WORD strip_src, WORD strip_dest );
void get_texture_xy_from_tpage_uv( UWORD tpage, UWORD u0, UWORD v0, UWORD *dest_x, UWORD *dest_y );
BYTE *read_ted_data( BYTE *ted_addr, BYTE *fname, WORD slot );

//void read_all_ted_data( BYTE *ted_addr );
//BYTE *get_eol( BYTE *addr );


MATRIX test_mat;


Ted_block ted_block[256];

#if 0


void render_scene()
{
Scene_obj *scene_obj;
MATRIX scene_rot_matrix=M_IDENTITY;
MATRIX m1=M_IDENTITY;
MATRIX m2=M_IDENTITY;
MATRIX m3=M_IDENTITY;

SVECTOR cam_trans_sv;
WORD cnt,num_objects;
SVECTOR pos;

//**********
MATRIX feckboy;
MATRIX cam=M_IDENTITY;
MATRIX ob_mat;
LONG flg;
SVECTOR temp;
MATRIX cam_trans_mat;
MATRIX phong_matrix = M_IDENTITY;
MATRIX id = M_IDENTITY;
//**********

//MATRIX inv={ -4096, 0, 0, 0, -4096,0 ,0,0, -4096 };
SVECTOR sv;

MATRIX environment_adjust_rotmat;
//Scene_poly_data *scene_poly_data;

LONG poly_time=0;
WORD light0_type;
VECTOR lv;
SOD *sod;
WORD obj_num;

//MATRIX tmpmat;

	if ( light.light_data[0].active )
		light0_type = light.light_data[0].type;
	else
		light0_type = 0;


	scene_obj = scene_header->scene_obj_list;  


	num_objects = sod_header->num_objects;


//	if ( scene_obj->upd_rtn )
//		(*scene_obj->upd_rtn)( scene_obj );	//Run the update routine




	do_camera_animation();	// outputs new matrix into camera.matrix
	do_light_animation();

	cam = camera.matrix;

//*** create a angle and position to rotate the scene camera by ***
	RotMatrix( &camera.data[camera.current].angle, &m3 );
	TransMatrix( &m3, &camera.data[camera.current].pos );
	CompMatrix (  &cam,&m3, &cam );



	fix_aspect_ratio( &cam );

	camera.output_mat = cam;

	//fntprint_mat( "mat\n", &cam );

	set_dir_light_matrix();



	SetRotMatrix ( &cam );	//added
	SetTransMatrix ( &cam );	//added

 //* Make rotation matrix for this scene
	RotMatrix ( &sod_header->rot, &scene_rot_matrix );

	feckboy = cam;


	cam_trans_sv.vx = (WORD)cam.t[0]; 
	cam_trans_sv.vy = (WORD)cam.t[1]; 
	cam_trans_sv.vz = (WORD)cam.t[2]; 


//** rotate scene pos by camera angle **

	ApplyMatrix ( &cam, &sod_header->pos, (VECTOR *)&cam_trans_mat.t[0] );

	RotMatrix ( &sod_header->envrot_vec, &environment_adjust_rotmat );


	for ( obj_num=0; obj_num< num_objects; obj_num++ )
		{
		if ( (void *)scene_obj > (void *)0 )
			{
			sod = scene_obj->sod;


			if ( scene_obj->upd_rtn )
				(*scene_obj->upd_rtn)( scene_obj );	//Run the update routine


			if ( sod->active == YES )
				{


				do_object_animation1 ( sod );
				do_object_animation2 ( sod );

				RotMatrix ( &sod->rot, &m1 );
				m1.t[0]=0;
				m1.t[1]=0;
				m1.t[2]=0;

				//** sod->anim_matrix is matrix output by anim.c **

				CompMatrix ( &sod->user_matrix_a,&sod->user_matrix_b,&m3 );
				CompMatrix ( &m3, &sod->anim_matrix,  &m2 );
				CompMatrix ( &m2, &m1, &m2 );


//				m2 = m3;




				CompMatrix ( &scene_rot_matrix, &m2, &ob_mat );


	  			gte_SetRotMatrix(&cam);
	  			gte_SetTransMatrix(&cam_trans_mat);

				gte_RotTrans(&cam_trans_sv, (VECTOR *)&feckboy.t, &flg);
				gte_SetTransMatrix(&feckboy);

		//* 	Load transfer vector to back color vector * 
				gte_ldbkdir(feckboy.t[0],feckboy.t[1],feckboy.t[2]);
				
				temp.vx = (WORD)ob_mat.t[0];
				temp.vy = (WORD)ob_mat.t[1];
				temp.vz = (WORD)ob_mat.t[2];

//				gte_ReadRotMatrix ( &tmpmat );
//				fntprint_mat ( "tmpmat1=",&tmpmat );

				gte_ldsv(&temp);		// load local transfer to [sv]
				gte_rtirbk();			// [lv]=[bk]+[rt]*[sv]
				gte_mvlvtr();			// move [lv] to [tr]

//				gte_ReadRotMatrix ( &tmpmat );
//				fntprint_mat ( "tmpmat2=",&tmpmat );


   	//* 	Create rotation *

				gte_MulMatrix0( &cam, &ob_mat, &m2 );


		//*	* Create light matrix **

				MulMatrix0 (  &light.light_matrix, &ob_mat, &m1 );
				light.output_light_matrix = m1;
				gte_SetLightMatrix(&m1);

		//*	** Calculation for point light ***
				if ( light0_type == POINT_LIGHT )
					{
					ob_mat.t[0] = sod_header->pos.vx + sod->anim_matrix.t[0];
					ob_mat.t[1] = sod_header->pos.vy + sod->anim_matrix.t[1];
					ob_mat.t[2] = sod_header->pos.vz + sod->anim_matrix.t[2];

				//Transpose object matrix into M1
					TransposeMatrix ( &ob_mat, &m1 );

					ApplyMatrixLV ( &m1, (VECTOR *)&ob_mat.t[0], &lv );

				//Multiply the light_vector by the transposed object matrix
					ApplyMatrixSV ( &m1, &light.light_data[0].angpos, &sv );
									
					sv.vx -= lv.vx;
					sv.vy -= lv.vy;
					sv.vz -= lv.vz;
					}




		//*	* make phong shade matrix ***

				gte_MulMatrix0 ( &ob_mat, &environment_adjust_rotmat, &phong_matrix );

				SetRotMatrix(&m2);

		//* 	Reset backcolor, as it's corrupted above
				SetBackColor(light.ambient_color.r,light.ambient_color.g,light.ambient_color.b );	

		//* 	Draw polygons *

		// 	gte light matrix holds light_matrix * object rotation

				ReadRotMatrix ( &scene_obj->matrix );

//				CompMatrix ( &scene_obj->sod->store,&scene_obj->matrix,  &m3 );
//				SetRotMatrix ( &m3 );
//				SetTransMatrix ( &m3 );
//				ReadRotMatrix ( &scene_obj->matrix );
//				ReadRotMatrix ( &scene_obj->sod->current_matrix );

				//fntprint_mat("smat=",&scene_obj->sod->matrix);

				//ReadRotMatrix( &arse );
				//fntprint_mat("currmat=",&arse );


				if ( scene_obj->sod->name[0] != 'L' )		// Don't draw laser objects ( drawn seperately )
					{
					if ( scene_obj->environment_mapping )
						{
						if ( scene_obj->g3_cnt  )	rl_phong_draw_scene_g3s  ( scene_obj, &phong_matrix );
						if ( scene_obj->g4_cnt  )	rl_phong_draw_scene_g4s  ( scene_obj, &phong_matrix );
						if ( scene_obj->gt3_cnt )	rl_phong_draw_scene_gt3s ( scene_obj, &phong_matrix );
						if ( scene_obj->gt4_cnt )	rl_phong_draw_scene_gt4s ( scene_obj, &phong_matrix );
						}
					else
						{
						if ( scene_obj->real_lit )
							{
							if ( scene_obj->g3_cnt  )	rl_draw_scene_g3s  ( scene_obj, &sv, light0_type );
							if ( scene_obj->g4_cnt  )	rl_draw_scene_g4s  ( scene_obj, &sv, light0_type );
							if ( scene_obj->gt3_cnt )	rl_draw_scene_gt3s ( scene_obj, &sv, light0_type );
			  				if ( scene_obj->gt4_cnt )	rl_draw_scene_gt4s ( scene_obj, &sv, light0_type );
							}
						else
							{
							if ( scene_obj->g3_cnt  )	pl_draw_scene_g3s  ( scene_obj );
							if ( scene_obj->g4_cnt  )	pl_draw_scene_g4s  ( scene_obj );
							if ( scene_obj->gt3_cnt )	pl_draw_scene_gt3s ( scene_obj );
							if ( scene_obj->gt4_cnt )	pl_draw_scene_gt4s ( scene_obj );
							}
						}
					}
				}

			scene_obj++;
			}
		}

}











void setup_scene_objects()
{
BYTE *addr;
Scene_obj *scene_obj;
Scene_obj_src *scene_obj_src;
WORD current_obj;
SOD *sod;
MATRIX id=M_IDENTITY;
WORD result;

//**** Allocate memory for this scene structure ****
	scene_header->scene_obj_list = (Scene_obj *)allocate_mem ( 0, sizeof ( Scene_obj ) * sod_header->num_objects );

	scene_obj = scene_header->scene_obj_list;
	addr = (BYTE *)scene_header_src;
	addr += scene_header_src->scene_data_offs;	// point to start of source object data
	scene_obj_src = (Scene_obj_src *)addr;
 
	for ( current_obj=0; current_obj< sod_header->num_objects; current_obj++ )
		{

		sod = sod_header->sod_list + current_obj;

		//set scene pos and rotation to 0
			scene_obj->upd_rtn=0;
			scene_obj->do_mipmap_swap = 3;
			scene_obj->mip_level = 0;
			scene_obj->centre_vertex = scene_obj_src->centre_vertex;
			scene_obj->mip_mapping = scene_obj_src->mip_mapping;

			scene_obj->g3_cnt = scene_obj->g4_cnt = scene_obj->gt3_cnt = scene_obj->gt4_cnt = 0;

	  		scene_obj->real_lit = scene_obj_src->light_active;
			scene_obj->environment_mapping = scene_obj_src->environment_mapping;

			scene_envmap_u0 = get_u0 ( scene_header_src->envmap_tx, scene_header_src->envmap_bpp );
			scene_envmap_v0 = scene_header_src->envmap_ty;

			scene_envmap_u0 += ( scene_header_src->envmap_tw/2 );
			scene_envmap_v0 += ( scene_header_src->envmap_th/2 );

//			scene_envmap_vshift = (4096/scene_header_src->envmap_tw)*2;


			// set addr of this object's SOD
			scene_obj->sod = sod_header->sod_list + current_obj;

		if ( sod->exclude_from_scene == NO )
			{

			if ( scene_obj_src->g3_cnt )
				setup_scene_g3s( scene_obj, scene_obj_src );

			if ( scene_obj_src->g4_cnt )
				setup_scene_g4s( scene_obj, scene_obj_src );

			if ( scene_obj_src->gt3_cnt )
				setup_scene_gt3s( scene_obj, scene_obj_src );

			if ( scene_obj_src->gt4_cnt )
				setup_scene_gt4s( scene_obj, scene_obj_src );
			}
		scene_obj_src++;

		scene_obj++;
		}
}


void init_renderer()
{

//**** set GTE Ambience level ****
	SetBackColor ( light.ambient_color.r, light.ambient_color.g, light.ambient_color.b );

//***  squirt font onto VRAM as scene may overwrite this ***
//	setup_system_font();	



	if ( (sizeof ( G3_face ) & 3) )
		display_error ( "g3_face not longword aligned" );
	if ( (sizeof ( G4_face ) & 3) )
		display_error ( "g4_face not longword aligned" );
	if ( (sizeof ( GT3_face ) & 3) )
		display_error ( "gt3_face not longword aligned" );
	if ( (sizeof ( GT4_face ) & 3) )
		display_error ( "gt4_face not longword aligned" );

	if ( (sizeof ( G3_face_src ) & 3) )
		display_error ( "g3_face_src not longword aligned" );
	if ( (sizeof ( G4_face_src ) & 3) )
		display_error ( "g4_face_src not longword aligned" );
	if ( (sizeof ( GT3_face_src ) & 3) )
		display_error ( "gt3_face_src not longword aligned" );
	if ( (sizeof ( GT4_face_src ) & 3) )
		display_error ( "gt4_face_src not longword aligned" );


	if ( (sizeof ( Scene_obj ) & 3) )
		display_error ( "scene_obj not longword aligned" );

	if ( (sizeof ( Scene_header ) & 3) )
		display_error ( "scene_header not longword aligned" );

	if ( (sizeof ( Scene_header_src ) & 3) )
		display_error ( "scene_header_src not longword aligned" );

	if ( (sizeof ( LIGHT_DATA ) & 3) )
		display_error ( "LIGHT_DATA struct not longword aligned" );



}




/**********************************************************************************
 *																										    *
 * Laser beam functions																				 *
 *																											 *
 **********************************************************************************/

#if 0
POLY_GT3 *laser_gt3_list[ MAX_LASERS ][2];
UWORD polynum,face_type;

void init_all_lasers()
{
Laser_data *laser_data;
WORD current_laser;
	for ( current_laser=0; current_laser< MAX_LASERS; current_laser++ )
		{
		laser_data = &sod_header->laser_data[ current_laser ];
		laser_data->active = NO;
		}
	sod_header->lasers_active=0;
	polynum = face_type = 0;
}
void add_laser( Laser_data *laser_data, WORD laser_num )
{
Scene_obj *laser_scene_obj;
WORD polynum, buff;
POLY_GT3 *poly;

	// set up laser positions and colors

	laser_scene_obj = scene_header->scene_obj_list + laser_data->laser_name;
	laser_data->active = YES;

	laser_gt3_list[laser_num][0] = allocate_mem ( 0, sizeof (POLY_GT3) * laser_data-> max_laser_beams);
	laser_gt3_list[laser_num][1] = allocate_mem ( 0, sizeof (POLY_GT3) * laser_data-> max_laser_beams);

	for ( polynum=0; polynum<laser_data->max_laser_beams; polynum++ )
		{
		for ( buff=0; buff<2; buff++ )
			{
			poly = laser_gt3_list [ laser_num ][ buff ];
			poly += polynum;

			SetPolyGT3 ( poly );
			SetSemiTrans ( poly, 1 );
			
			poly-> tpage = GetTPage ( DUMMY_TEXTURE_BPP, laser_data->transp_rate, DUMMY_TEXTURE_X,DUMMY_TEXTURE_Y ); 
			poly-> u0 = 
			poly-> u1 = 
			poly-> u2 = ( DUMMY_TEXTURE_X <<(2-DUMMY_TEXTURE_BPP)) % (64<<(2-DUMMY_TEXTURE_BPP));
			poly-> v0 = 
			poly-> v1 = 
			poly-> v2 = DUMMY_TEXTURE_Y % 256;

			poly->r0 = poly->g0 = poly-> b0 = 14;
			poly->r1 = poly->g1 = poly-> b1 = 14;

			poly->r2 = laser_data->r;
			poly->g2 = laser_data->g;
			poly->b2 = laser_data->b;
			}
		}


// copy whole laser_data struct into sod header....
	sod_header->laser_data[ laser_num ] = *laser_data;		// global laser struct
	sod_header->lasers_active++;
}



void draw_lasers()
{
WORD laser_screen_pos[2];
G3_face *target_face_g3;
G4_face *target_face_g4;
GT3_face *target_face_gt3;
GT4_face *target_face_gt4;

Scene_obj *laser_scene_obj;
Scene_obj *target_scene_obj;
POLY_GT3 *poly;
SVECTOR *v0,*v1;
SVECTOR dummy_v0;

WORD polycnt,obcnt,lasers_to_draw,obj_to_process,current_laser;
WORD target_polycnt=0;

Laser_data *laser_data=0;

#if DEBUG
WORD total_lasers_per_obj=0;
WORD total_lasers_drawn=0;
#endif

LONG *ot = cdb->ot;

	#if DEBUG
	start_vc();
	#endif

	v0 = &dummy_v0;
	v1 = &dummy_v0;

	for ( current_laser=0; current_laser< MAX_LASERS; current_laser++ )
		{
		laser_data = &sod_header->laser_data [ current_laser ]; 

		if ( laser_data-> active )
			{
			laser_scene_obj = scene_header->scene_obj_list + laser_data->laser_name;

			poly = laser_gt3_list[ current_laser ] [ cdb->buffnum ];

			SetRotMatrix( &laser_scene_obj->matrix );
			SetTransMatrix( &laser_scene_obj->matrix );

			gte_ldv0( &laser_data->pos );					//load vertices to GTE
			gte_rtps();											//rottrans them with local matrix
			gte_stsxy( ( ULONG *)&laser_screen_pos[0] );	//store 2d coords in polygon


			obcnt=0;
			while( 1 )
				{

				if ( (obj_to_process = laser_data->object_list[ obcnt ]) == -1) 
					break;
				else
					{
					obcnt++;

					target_scene_obj = scene_header->scene_obj_list + obj_to_process;

					SetRotMatrix( &target_scene_obj->matrix );
					SetTransMatrix( &target_scene_obj->matrix );

					while(1)
						{

						target_scene_obj->current_laser_face++;
						face_type = target_scene_obj->current_laser_face & 3;

						if ( face_type == 0 )
							if ( target_scene_obj->g3_cnt ) 		break;
						if ( face_type == 1 )
							if ( target_scene_obj->g4_cnt )	 	break;
						if ( face_type == 2 )
							if ( target_scene_obj->gt3_cnt ) 	break;
						if ( face_type == 3 )
							if ( target_scene_obj->gt4_cnt ) 	break;
						}


					if ( face_type == 0 )
						{
						target_polycnt = target_scene_obj->g3_cnt;
						}

					if ( face_type == 1 )
						{
						target_polycnt = target_scene_obj->g4_cnt;
						}
					if ( face_type == 2 )
						{
						target_polycnt = target_scene_obj->gt3_cnt;
						}
					if ( face_type == 3 )
						{
						target_polycnt = target_scene_obj->gt4_cnt;
						}



					lasers_to_draw = laser_data->polys_per_object;
					if ( target_polycnt < lasers_to_draw )
						lasers_to_draw = target_polycnt;
		

					for ( polycnt=0; polycnt< lasers_to_draw; polycnt++ )
						{

						if ( face_type == 0 )
							{
							target_face_g3  = target_scene_obj->g3_face + (polynum % target_polycnt);
							v0 = &target_face_g3->v0;
							v1 = &target_face_g3->v1;
							}

						if ( face_type == 1 )
							{
							target_face_g4  = target_scene_obj->g4_face + (polynum % target_polycnt);
							v0 = &target_face_g4->v0;
							v1 = &target_face_g4->v1;
							}
						if ( face_type == 2 )
							{
							target_face_gt3  = target_scene_obj->gt3_face + (polynum % target_polycnt);
							v0 = &target_face_gt3->v0;
							v1 = &target_face_gt3->v1;
							}
						if ( face_type == 3 )
							{
							target_face_gt4  = target_scene_obj->gt4_face + (polynum % target_polycnt);
							v0 = &target_face_gt4->v0;
							v1 = &target_face_gt4->v1;
							}




						//** Point 'a' of laser = point on target polygon **

						gte_ldv0 ( v0 );
						gte_rtps();
						gte_stsxy( ( ULONG *)&poly->x0 );

						//** Point 'b' of laser = point on target polygon **
						gte_ldv0 ( v1 );
						gte_rtps();
						gte_stsxy( ( ULONG *)&poly->x1 );

						//** Point 'c' of laser = laser fixed point **
						poly->x2 = laser_screen_pos[0];
						poly->y2 = laser_screen_pos[1];


						addPrim ( ot + NEAREST_DEPTH, poly );
						poly++;
						polynum++;


						#if DEBUG

						if ( total_lasers_per_obj > laser_data->max_laser_beams )
							{
							display_error( "not enough laser polys allocated" );
							}
 						total_lasers_per_obj++;
						total_lasers_drawn++;

						#endif



						}
					}
				}
			}
		}

#if DEBUG
	FntPrint( "total laser beams=%d\n",total_lasers_drawn);
	show_vc( "laser cpu scanlines=" );
#endif
}





/**** object polygon must be as follows... ***

 89...
 4567
 0123..	poly nums

 poly vertices must be arranged as

 a b
 c d
******/

void draw_plasma_to_model_object ( Plasma *plasma )
{
Scene_obj *scene_obj;
WORD pv,x,y,wd,ht;
GT4_face *face;
	

//	FntPrint("plasma is %d x %d\n",plasma->poly_wd,plasma->poly_ht);


	scene_obj = scene_header->scene_obj_list + plasma->obj_num;

	scene_obj->sod->color_override = COLOR_OVERRIDE;

// put vertex colors into polygon vertices

	wd = plasma->poly_wd;
	ht = plasma->poly_ht;

	pv=0;
	for ( y=ht-1; y>=0; y-- )
		{
		for ( x=0; x<wd; x++ )
			{
			face = scene_obj-> gt4_face + x + (y*(wd+0));
			face->c0.r = plasma->plasma_vertex[ pv ].r.w.hi + plasma->plasma_vertex[ pv ].val.w.hi;
			face->c0.g = plasma->plasma_vertex[ pv ].g.w.hi + plasma->plasma_vertex[ pv ].val.w.hi;
			face->c0.b = plasma->plasma_vertex[ pv ].b.w.hi + plasma->plasma_vertex[ pv ].val.w.hi;

			face->c1.r = plasma->plasma_vertex[ pv+1 ].r.w.hi + plasma->plasma_vertex[ pv+1 ].val.w.hi;
			face->c1.g = plasma->plasma_vertex[ pv+1 ].g.w.hi + plasma->plasma_vertex[ pv+1 ].val.w.hi;
			face->c1.b = plasma->plasma_vertex[ pv+1 ].b.w.hi + plasma->plasma_vertex[ pv+1 ].val.w.hi;

			face->c2.r = plasma->plasma_vertex[ pv+0 + wd+1 ].r.w.hi  + plasma->plasma_vertex[ pv + wd+1 ].val.w.hi;
			face->c2.g = plasma->plasma_vertex[ pv+0 + wd+1  ].g.w.hi + plasma->plasma_vertex[ pv + wd+1 ].val.w.hi;
			face->c2.b = plasma->plasma_vertex[ pv+0 + wd+1  ].b.w.hi + plasma->plasma_vertex[ pv + wd+1 ].val.w.hi;

			face->c3.r = plasma->plasma_vertex[ pv+1 + wd+1 ].r.w.hi + plasma->plasma_vertex[ pv+1+ wd+1 ].val.w.hi;
			face->c3.g = plasma->plasma_vertex[ pv+1 + wd+1 ].g.w.hi + plasma->plasma_vertex[ pv+1+ wd+1 ].val.w.hi;
			face->c3.b = plasma->plasma_vertex[ pv+1 + wd+1 ].b.w.hi + plasma->plasma_vertex[ pv+1+ wd+1 ].val.w.hi;



			//FntPrint("c0=%d\n",face->c0.r);
			face++;
			pv++;
			}
		pv++;
		}
}
#endif

WORD get_u0 ( WORD tex_x, WORD bpp )
{
WORD t,u0;

	t = (tex_x << (2-bpp)) % (64 << (2-bpp));
	u0 = (t>255) ? 255 : t;

	return u0;

}











#if 0
void read_all_ted_data( BYTE *ted_addr )
{
UBYTE *addr;
UWORD clut;
WORD slot=0;

	addr = ted_addr;

	while(1)
		{
	
		// bpp
		addr = strchr ( addr, ',' );
		addr++;
		ted_block[slot].bpp = atoi ( addr );

		// texture x
		addr = strchr ( addr, ',' );
		addr++;
		ted_block[slot].x = atoi ( addr );


		// texture y
		addr = strchr ( addr, ',' );
		addr++;
		ted_block[slot].y = atoi ( addr );

		// texture w
		addr = strchr ( addr, ',' );
		addr++;
		ted_block[slot].w = atoi ( addr );

		// texture ht
		addr = strchr ( addr, ',' );
		addr++;
		ted_block[slot].h = atoi ( addr );

		// clut
		addr = strchr ( addr, ',' );
		addr++;
		clut = atoi ( addr );

		ted_block[slot].clut_x = (clut&0x3f)<<4;
		ted_block[slot].clut_y = clut>>6;

		slot++;

		addr = get_eol( addr );
		if ( addr == 0 )
			break;
		}

//	printf("texture=%d %d %d %d\n",ted_block[slot].x, ted_block[slot].y, ted_block[slot].w, ted_block[slot].h );
}


BYTE *get_eol( BYTE *addr )
{

	while(1)
		{
		if ( (*addr == 0x0d) && (*(addr+1) == 0x0a) )
			break;
		else
			{
			addr++;
			if ( *addr == 0 )
				{
				printf("end of file\n");
				return 0;
				}
			}
		}

}
#endif



/*******************************************************
 *																		 *
 * Get texture xy													 *
 *																		 *
 * Pass tpage, u,v, output xaddr, output yaddr			 *
 * Gets VRAM co-ord of texture from tpage, u0 and v0	 *
 *																		 *
 *******************************************************/

void get_texture_xy_from_tpage_uv( UWORD tpage, UWORD u0, UWORD v0, UWORD *dest_x, UWORD *dest_y )
{
WORD tx,ty,bpp;

// outputs the vram xy position of a texture when given the tpage, u0 and v0

	tx = (tpage<<6) & 0x7c0;
	ty = (tpage<<4) & 0x100;

	bpp = (tpage>>7) & 3;

	tx += u0 >> (2-bpp);
	ty += v0;

	*dest_x = tx;
	*dest_y = ty;

}




#if 0
UBYTE *get_cr( UBYTE *script )
{
	script = strchr( script, 0x0d );
	return script;
}

void setup_ted_list()
{
UBYTE *addr, *end_addr;
//LONG size;
WORD slot=0;
UWORD clut;

	push(0);
	addr = read_datafile_alloc( FRONTEND_TED );
	end_addr = addr + file_header.file[ FRONTEND_TED ].size;

	pop(0);

	while( 1 )
		{
		if ( *addr  )
			{
			// bpp
			addr = strchr ( addr, ',' );
			addr++;
			ted_block[slot].bpp = atoi ( addr );

			// texture x
			addr = strchr ( addr, ',' );
			addr++;
			ted_block[slot].x = atoi ( addr );


			// texture y
			addr = strchr ( addr, ',' );
			addr++;
			ted_block[slot].y = atoi ( addr );

			// texture w
			addr = strchr ( addr, ',' );
			addr++;
			ted_block[slot].w = atoi ( addr );

			// texture ht
			addr = strchr ( addr, ',' );
			addr++;
			ted_block[slot].h = atoi ( addr );

			// clut
			addr = strchr ( addr, ',' );
			addr++;
			clut = atoi ( addr );


			addr = get_cr( addr );
			addr += 2;


			ted_block[slot].clut_x = (clut&0x3f)<<4;
			ted_block[slot].clut_y = clut>>6;
			}
		else
			{
			break;
			}
		slot++;
		}
}
#endif





#endif



void download_texture( BYTE *addr, WORD ted_slot )
{
/****************************************
 *								  					 *
 * Pass addr of texture in RAM,         *
 * and TED_SLOT to download to in vram  *
 *													 *
 ****************************************/
RECT rect;

//	printf("bpp=%d\n",ted_block[ted_slot].bpp );
	rect.x = ted_block[ ted_slot ].x;
	rect.y = ted_block[ ted_slot ].y;
	rect.w = ted_block[ ted_slot ].w >> (2-ted_block[ ted_slot ].bpp);
	rect.h = ted_block[ ted_slot ].h;

//	printf("rect=%d %d %d %d\n",rect.x, rect.y, rect.w, rect.h );

//	printf("loading image from %x to xywh %d %d %d %d\n",addr, rect.x, rect.y, rect.w, rect.h );

	LoadImage( &rect, (ULONG *)addr ); 
	DrawSync(0);
}

void download_clut( BYTE *addr, WORD ted_slot, WORD src_offset, WORD dest_offset, WORD len )
{
/*****************************************
 *								  					  *
 * Pass addr of source CLUT in RAM,      *
 * src_pal_num = pal within palette bank *
 * TED_SLOT = pos to download to in vram *
 *													  *
 *****************************************/
RECT rect;

	addr += src_offset * 2;

	rect.x = ted_block[ ted_slot ].clut_x + dest_offset;
	rect.y = ted_block[ ted_slot ].clut_y;

	rect.w = len;
	rect.h = 1;

	LoadImage( &rect, (ULONG *)addr ); 
	DrawSync(0);
}


void overlay_textures( UBYTE *src1, UBYTE *src2, UBYTE *src3, WORD ted_slot )
{
WORD x,y,w,h;
UBYTE *buffer,*buffer_base;
LONG pixels;
RECT rect;

/*****************************************
 *								      			  *
 * src1 src2 and src3 point to source	  *
 * texture in ram.                       *
 * They must be same size and 8bit       *
 *											        *
 *****************************************/


 	push(0);

	w = ted_block[ ted_slot ].w;
	h = ted_block[ ted_slot ].h;

	buffer_base = allocate_mem( 0, w*h );

	buffer = buffer_base;


	pixels = w*h;

	while( pixels-- )
		{
		*buffer = *src1;
		src1++;

		if ( src2 )
			{
			if ( *src2 )
				*buffer = *src2;
			src2++;
			}					

		if ( src3 )
			{
			if ( *src3 )
				*buffer = *src3;
			src3++;
			}

		buffer++;
		}

	rect.x = ted_block[ ted_slot ].x;
	rect.y = ted_block[ ted_slot ].y;
	rect.w = ted_block[ ted_slot ].w / 2;
	rect.h = ted_block[ ted_slot ].h;

	LoadImage( &rect, (ULONG *)buffer_base ); 
	DrawSync(0);



	pop(0);


//	printf("xywh=%d %d %d %d\n",rect.x, rect.y, rect.w, rect.h );
}






void download_texture2( BYTE *src_addr, WORD src_wd, WORD src_ht, WORD dest_x, WORD dest_y )
{
/****************************************
 *								  					 *
 * Pass addr of texture in RAM,         *
 * and TED_SLOT to download to in vram  *
 *													 *
 ****************************************/
RECT rect;

	rect.x = dest_x;
	rect.y = dest_y;
	rect.w = src_wd >> 1;	// get width in words
	rect.h = src_ht;

	LoadImage( &rect, (ULONG *)src_addr ); 
	DrawSync(0);
}





#if 0
void overlay_texture2( UBYTE *src1, WORD x1, WORD y1, WORD w1, WORD h1, UBYTE *src2, WORD x2, WORD y2, WORD w2, WORD h2, UBYTE *src3, WORD x3, WORD y3, WORD w3, WORD h3, WORD dest_x, WORD dest_y )
{
WORD x,y;
RECT rect;
UBYTE *buffer;

 	push(0);

	rect.x = dest_x;
	rect.y = dest_y;

// find biggest width
	rect.w = w1;
	if ( w2 > rect.wd ) rect.w = w2;
	if ( w3 > rect.wd ) rect.w = w3;

// find biggest height
	rect.h = h1;
	if ( h2 > rect.ht ) rect.h = h2;
	if ( h3 > rect.ht ) rect.h = h3;

// rect = vram destination

// make a buffer of biggest width and height
	buffer = allocate_mem( 0, rect.w*rect.h );



//copy first texture to buffer
	dest = buffer + (y1*rect.w) + x1;
	src = src1;
	for ( y=0;y<h1;y++ )
		{
		for ( x=0;x<w1;x++ )
			{
			//if ( *src )
			*dest = *src;
			src++;
			dest++;
			}
		dest += rect.w - w1;
		}

// download buffer to vram
	rect.w >> 1;
	LoadImage( &rect, (ULONG *)buffer ); 
	DrawSync(0);

	pop(0);
}

#endif














#if 1
void move_texture( WORD ted_slot1, WORD ted_slot2 )
{
/****************************************
 *								  					 *
 * move texture from one vram addr
 * to another		
 *													 *
 ****************************************/
RECT rect1;

	rect1.x = ted_block[ ted_slot1 ].x;
	rect1.y = ted_block[ ted_slot1 ].y;
	rect1.w = ted_block[ ted_slot1 ].w >> (2-ted_block[ ted_slot1 ].bpp);
	rect1.h = ted_block[ ted_slot1 ].h;

	MoveImage( &rect1, ted_block[ ted_slot2 ].x, ted_block[ ted_slot2 ].y); 
	DrawSync(0);

//	printf("moving texture from %d %d to %d %d\n",rect1.x,rect1.y, ted_block[ ted_slot2 ].x, ted_block[ ted_slot2 ].y );
}


void move_clut( WORD ted_slot1, WORD ted_slot2 )
{
/****************************************
 *								  					 *
 * move clut from one vram addr
 * to another		
 *													 *
 ****************************************/
RECT rect1;

	rect1.x = ted_block[ ted_slot1 ].clut_x;
	rect1.y = ted_block[ ted_slot1 ].clut_y;

	if ( ted_block[ ted_slot1 ].bpp == 1 )
		rect1.w = 256;
	else
		rect1.w = 16;

	rect1.h = 1;

	MoveImage2( &rect1, ted_block[ ted_slot2 ].clut_x, ted_block[ ted_slot2 ].clut_y); 
	DrawSync(0);
}

#endif


void load_scene_materials( WORD filenum )
{

// Loads the TEX file for the scene's SDT file
// and transfers data to vram


RECT rect;
UWORD *uw_addr;
ULONG *ul_addr;
UBYTE *addr,*clut_addr;
BYTE loadname[16];
BYTE *buffer;
	
BYTE *texture_temp;

	clear_vram( 0,0,1023,511, 0,0,0 );

	texture_temp = (UBYTE *)0x801ff000 - TEXTURE_AREA_SIZE;

	addr = texture_temp;

	read_datafile ( filenum, addr, TEXTURE_AREA_SIZE );	// load the .TEX file

//	printf("loading materials...\n");


//transfer .TEX file BODY
	uw_addr = (UWORD *)addr;
	ul_addr = (ULONG *)addr;
	clut_addr = addr + *ul_addr; 

	uw_addr+=2;

 	rect.x=*uw_addr++;
 	rect.y=*uw_addr++;
 	rect.w=*uw_addr++;
 	rect.h=*uw_addr++;


	LoadImage ( &rect, (ULONG *)uw_addr );

//transfer .TEX file CLUT


	uw_addr = (UWORD *)clut_addr;
	uw_addr+=2;

 	rect.x=*uw_addr++;
 	rect.y=*uw_addr++;
 	rect.w=*uw_addr++;
 	rect.h=*uw_addr++;

	LoadImage ( &rect, (ULONG *)uw_addr );

	VSync(0);	// hang cpu 'till loadimage done


	setup_dummy_texture(); // for g3 and g4 polys

}








void setup_dummy_texture()
{
RECT rect={DUMMY_TEXTURE_X,DUMMY_TEXTURE_Y,1,1};
ULONG data= 15 + (15<<5) + (15<<10 ) + 32768; //texture color + SEMITRANS bit

// G3s and G4s now use a textured polygon where the texture color
// is 15,15,15. This emulates an untextured polygon, however I can
// apply a semi-transparent rate to each polygon...

	LoadImage ( &rect, &data );	//write pixel to vram
	VSync(0);

}





void load_ted_info()
{

UBYTE name[64];

BYTE *temp_area = get_heap_end( 0 );
WORD cnt;
BYTE *addr;


//	printf("heap0 start=%x end=%x\n",get_heap_start(0), get_heap_end(0) );

//*** load ted ascii file ***
	read_datafile ( FRONTEND_TED, temp_area, 128000 );

//	read_all_ted_data( temp_area );

//*** extract texture info for these specific textures into specified ted block structures... ****
//	read_ted_data ( temp_area, "ktype01.", 0 );
//	read_ted_data ( temp_area, "tstrip01.", 0 );
//	read_ted_data ( temp_area, "tstrip02.", 1 );
//	read_ted_data ( temp_area, "tstrip03.", 2 );
//	read_ted_data ( temp_area, "tstrip04.", 3 );


	read_ted_data ( temp_area, "face.", TED_PLAYER_FACE );
	read_ted_data ( temp_area, "arms.", TED_PLAYER_ARMS );
	read_ted_data ( temp_area, "thighs.", TED_PLAYER_LEGS );

	read_ted_data ( temp_area, "shirtb.", TED_SHIRT_BACK );
	read_ted_data ( temp_area, "shirtf.", TED_SHIRT_FRONT );
	read_ted_data ( temp_area, "sleeve.", TED_SLEEVE );
	read_ted_data ( temp_area, "shorts.", TED_SHORT );
	read_ted_data ( temp_area, "socks.", TED_SOCK );
	read_ted_data ( temp_area, "boots.", TED_BOOT );


#if 0
	read_ted_data ( temp_area, "tvkshia.", TED_TVK_SHIA );
	read_ted_data ( temp_area, "tvkshib.", TED_TVK_SHIB );
	read_ted_data ( temp_area, "tvkshoa.", TED_TVK_SHOA );
	read_ted_data ( temp_area, "tvkshob.", TED_TVK_SHOB );
	read_ted_data ( temp_area, "tvkslea.", TED_TVK_SLEA );
	read_ted_data ( temp_area, "tvksleb.", TED_TVK_SLEB );


	read_ted_data ( temp_area, "tvback0.", TED_TVBACK1 );
	read_ted_data ( temp_area, "tvback1.", TED_TVBACK2 );
#endif

	addr = temp_area;

	sprintf(name,"sprtims\\tes_xx.tim" );

	for(cnt=0;cnt<TOTAL_FLAGS;cnt++)
		{
		name[12]=(cnt/26)+'a';
		name[13]=(cnt%26)+'a';
		addr = read_ted_data ( addr, name, TED_FLAG1+cnt );
//		printf("name=%s\n",name);
		}





}



/***************************************************
 *																	*
 * Read ascii ted file, outputting info into			*
 * chosen ted_block structure								*
 * 																*
 ***************************************************/

BYTE *read_ted_data( BYTE *ted_addr, BYTE *fname, WORD slot )
{
UBYTE *addr;
UWORD clut;

	addr = strstr ( ted_addr, fname );

	
	// bpp
	addr = strchr ( addr, ',' );
	addr++;
	ted_block[slot].bpp = atoi ( addr );

	// texture x
	addr = strchr ( addr, ',' );
	addr++;
	ted_block[slot].x = atoi ( addr );


	// texture y
	addr = strchr ( addr, ',' );
	addr++;
	ted_block[slot].y = atoi ( addr );

	// texture w
	addr = strchr ( addr, ',' );
	addr++;
	ted_block[slot].w = atoi ( addr );

	// texture ht
	addr = strchr ( addr, ',' );
	addr++;
	ted_block[slot].h = atoi ( addr );

	// clut
	addr = strchr ( addr, ',' );
	addr++;
	clut = atoi ( addr );

	ted_block[slot].clut_x = (clut&0x3f)<<4;
	ted_block[slot].clut_y = clut>>6;


//	printf("texture(%d)=%d %d %d %d\n",slot,ted_block[slot].x, ted_block[slot].y, ted_block[slot].w, ted_block[slot].h );
	return addr;

}


#if 0
WORD ted_pal_store;
WORD get_ted_pal_color( WORD ted_slot )
{
RECT rect;
	rect.x = ted_block[ ted_slot ].clut_x + dest_offset;
	rect.y = ted_block[ ted_slot ].clut_y;

	rect.w = 1;
	rect.h = 1;


	StoreImage( &rect, &ted_pal_store );

	return ted_pal_store;
}
#endif


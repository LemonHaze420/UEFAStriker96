/****************************************************
 *																	 *
 *																	 *
 *	 Object/Camera/Light animation						 *
 *																	 *
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
 *	 Structs														 *
 *																	 *
 ****************************************************/


/****************************************************
 *																	 *
 *	 Prototypes													 *
 *																	 *
 ****************************************************/




/****************************************************
 *																	 *
 *	 Defines														 *
 *																	 *
 ****************************************************/



/****************************************************
 *																	 *
 *	 Vars															 *
 *																	 *
 ****************************************************/



M01_header *resident_m01_files[ MAX_RESIDENT_M01_FILES ];
L01_header *resident_l01_files[ MAX_RESIDENT_L01_FILES ];
WORD all_animation_paused;


void reset_animation()
{
WORD cnt;

	for ( cnt=0;cnt< MAX_RESIDENT_M01_FILES; cnt++ )
		{
		resident_m01_files[cnt] = 0;
		}

	for ( cnt=0;cnt< MAX_RESIDENT_L01_FILES; cnt++ )
		{
		resident_l01_files[cnt] = 0;
		}

	all_animation_paused=NO;

	sod_header->anim_frame_counter=0;

}

void start_scene_anim ( WORD anim_seq, WORD slot, WORD matrix_control ) 
{
M01_obj_anim_src *m01_obj_anim,*m01_data;
BYTE *byte_addr;
WORD cnt;
M01_anim *m01_anim;
//Scene_obj *scene_obj;
WORD num_m01_objects,num_s01_objects;
M01_header *m01_header;
SOD *sod;


M01_anim  *mdt_indexlist;
M01_rot   *mdt_rot;		 
M01_trans *mdt_trans;	 
WORD mdt_blocks;			 

//WORD name_match;

//*** Check if mdt file has been loaded to this slot ***

	

	if (!( m01_header = resident_m01_files[slot]) )
		{
		display_error ("Trying to play anim with no MDT ( m01 ) file present\n");
		}

	sod_header->anim_frame_counter=0;
	sod_header->total_frames = m01_header->total_frames;


	byte_addr = (BYTE *)m01_header;
	byte_addr += m01_header->obj_anim_data;
	m01_data = (M01_obj_anim_src *)byte_addr;

	sod = sod_header->sod_list;

	sod->anim_seq = anim_seq;

	num_s01_objects = sod_header->num_objects; 


	while ( num_s01_objects-- )
		{
		num_m01_objects = m01_header->obj_cnt;
		m01_obj_anim = m01_data;

//		if ( sod -> anim_initialised == NO )
//			{
			while ( num_m01_objects-- )
				{
				//sod = sod_header->sod_list; //scene_obj->sod;


				if ( strcmp ( sod->name, m01_obj_anim->name ) == 0 )
					{

//					printf("starting on '%s\n",sod->name );


					byte_addr = (BYTE *)m01_header;
					byte_addr += m01_obj_anim->animlist_offs;
					mdt_indexlist = (M01_anim *)byte_addr;

					byte_addr = (BYTE *)m01_header;
					byte_addr += m01_header->rot_data;
					mdt_rot = (M01_rot *)byte_addr;

					byte_addr = (BYTE *)m01_header;
					byte_addr += m01_header->trans_data;
					mdt_trans = (M01_trans *)byte_addr;

					mdt_blocks = m01_obj_anim -> num_blocks;


					if ( anim_seq == 0 )
						{
						sod -> matrix_control1 = MATRIX_ANIM_ACTIVE + matrix_control;
						sod->anim_initialised1 = YES;
						sod->mdt_indexlist1 = mdt_indexlist;
						sod->mdt_rot1 = mdt_rot;
						sod->mdt_trans1 = mdt_trans;
						sod->mdt_blocks1 = mdt_blocks;
						sod->current_anim_frame1 = 0;
						sod->last_anim_frame1 = -1;
						sod->anim_timer1=0;
						}


					if ( anim_seq == 1 )
						{
						sod -> matrix_control2 = MATRIX_ANIM_ACTIVE + matrix_control;
						sod->anim_initialised2 = YES;
						sod->mdt_indexlist2 = mdt_indexlist;
						sod->mdt_rot2 = mdt_rot;
						sod->mdt_trans2 = mdt_trans;
						sod->mdt_blocks2 = mdt_blocks;
						sod->current_anim_frame2 = 0;
						sod->last_anim_frame2 = -1;
						sod->anim_timer2=0;
						}



			 		break;
					}
				else
					{
					m01_obj_anim++;
					}
				}
//			}

//		else
//			{
			//sod->current_anim_frame=0;
			//sod->last_anim_frame = -1;
			//sod->anim_timer=0;
//			}

		//scene_obj++;
		sod++;
		}
}



#if 0
void stop_scene_anim ( WORD slot ) 
{
M01_obj_anim_src *m01_obj_anim;
BYTE *byte_addr;
WORD cnt;
M01_anim *m01_anim;
//Scene_obj *scene_obj;
WORD num_m01_objects,num_s01_objects;
M01_header *m01_header;
SOD *sod;

	// scan through S01 (sdt) and set anim_initialised and matrix_animation = NO 
	// on all objects present in M01 (mdt)

//*** Check if mdt file has been loaded to this slot ***

	if ( (m01_header = resident_m01_files[slot]) )
		{
		byte_addr = (BYTE *)m01_header;
		byte_addr += m01_header->obj_anim_data;
		m01_obj_anim = (M01_obj_anim_src *)byte_addr;

		sod = sod_header->sod_list;
		num_s01_objects = sod_header->num_objects; 
		while ( num_s01_objects-- )
			{
			num_m01_objects = m01_header->obj_cnt;
			//sod = scene_obj->sod;

			if ( sod -> anim_initialised == YES )
				{

				while ( num_m01_objects-- )
					{

					if ( strcmp ( sod->name, m01_obj_anim->name ) == 0 )
						{
						sod -> anim_initialised = NO;
						sod -> matrix_anim_control &= ~MATRIX_ANIM_ACTIVE;

						FntPrint("halting '%s'\n",sod->name );

						sod->current_anim_frame=0;
						sod->last_anim_frame = -1;
						sod->anim_timer=0;
			 			break;
						}
					else
						{
						m01_obj_anim++;
						}
					}
				}

			sod++;
			}
		}
	else
		{
		#if DEBUG
		printf("Error! Trying to halt anim with no MDT ( m01 ) file present\n");
		#endif
		}
}
#endif



void do_object_animation1 ( SOD *sod )
{
M01_rot *rot;
M01_trans *trans;
M01_anim *m01_anim;

		
	if ( all_animation_paused == YES )
		return;



	if ( sod-> matrix_control1 & MATRIX_ANIM_ACTIVE )
		{

		m01_anim = &sod->mdt_indexlist1[ sod->current_anim_frame1 ];
		rot     = &sod->mdt_rot1   [ m01_anim->rmat_index ];
		trans   = &sod->mdt_trans1 [ m01_anim->tvec_index ];

		if ( sod->current_anim_frame1 != sod->last_anim_frame1 )
			{
	  	//** store old frame number to check for change **

			sod->last_anim_frame1 = sod->current_anim_frame1;

	  	//** set object matrix **

			if ( sod->matrix_control1 & MATRIX_ANIM )
				{
				sod->anim_matrix.m[0][0] =  rot->m[0];
				sod->anim_matrix.m[2][0] =  rot->m[1];
				sod->anim_matrix.m[1][0] =  rot->m[2];
				sod->anim_matrix.m[0][2] =  rot->m[3];
				sod->anim_matrix.m[2][2] =  rot->m[4];
				sod->anim_matrix.m[1][2] =  rot->m[5];
				sod->anim_matrix.m[0][1] =  rot->m[6];
				sod->anim_matrix.m[2][1] =  rot->m[7];
				sod->anim_matrix.m[1][1] =  rot->m[8];
				sod->anim_matrix.t[0]    =  trans->t[0];
				sod->anim_matrix.t[2]    =  trans->t[1];
				sod->anim_matrix.t[1]    =  trans->t[2];
				}

			if ( sod->matrix_control1 & MATRIX_A )
				{
				sod->user_matrix_a.m[0][0] =  rot->m[0];
				sod->user_matrix_a.m[2][0] =  rot->m[1];
				sod->user_matrix_a.m[1][0] =  rot->m[2];
				sod->user_matrix_a.m[0][2] =  rot->m[3];
				sod->user_matrix_a.m[2][2] =  rot->m[4];
				sod->user_matrix_a.m[1][2] =  rot->m[5];
				sod->user_matrix_a.m[0][1] =  rot->m[6];
				sod->user_matrix_a.m[2][1] =  rot->m[7];
				sod->user_matrix_a.m[1][1] =  rot->m[8];
				sod->user_matrix_a.t[0]    =  trans->t[0];
				sod->user_matrix_a.t[2]    =  trans->t[1];
				sod->user_matrix_a.t[1]    =  trans->t[2];
				}
			}


		if ( all_animation_paused == NO )
			sod->anim_timer1++;

		if ( sod->anim_timer1 == (m01_anim->time*1) )	//overall speed
			{
			sod->anim_timer1 = 0;

			sod->current_anim_frame1++;

			if ( sod->current_anim_frame1 == sod->mdt_blocks1 )
				{
				if ( sod->matrix_control1 & MATRIX_ANIM_SINGLE_SHOT )
					{
					sod -> anim_initialised1 = NO;
					sod-> current_anim_frame1--;	// hang on this frame...
					}
				else
					{
					sod->current_anim_frame1 = 0;
					}

				if ( sod->matrix_control1 & MATRIX_ANIM_DEACTIVATE )
					sod->active = NO;
				}
			}
		}
	else
		{
		sod->anim_matrix = sod->s01_matrix;
		}

}

void do_object_animation2 ( SOD *sod )
{
M01_rot *rot;
M01_trans *trans;
M01_anim *m01_anim;

		
	if ( all_animation_paused == YES )
		return;



	if ( sod-> matrix_control2 & MATRIX_ANIM_ACTIVE )
		{

		m01_anim = &sod->mdt_indexlist2[ sod->current_anim_frame2 ];
		rot     = &sod->mdt_rot2   [ m01_anim->rmat_index ];
		trans   = &sod->mdt_trans2 [ m01_anim->tvec_index ];

		if ( sod->current_anim_frame2 != sod->last_anim_frame2 )
			{
	  	//** store old frame number to check for change **

			sod->last_anim_frame2 = sod->current_anim_frame2;

	  	//** set object matrix **

			if ( sod->matrix_control2 & MATRIX_ANIM )
				{
				sod->anim_matrix.m[0][0] =  rot->m[0];
				sod->anim_matrix.m[2][0] =  rot->m[1];
				sod->anim_matrix.m[1][0] =  rot->m[2];
				sod->anim_matrix.m[0][2] =  rot->m[3];
				sod->anim_matrix.m[2][2] =  rot->m[4];
				sod->anim_matrix.m[1][2] =  rot->m[5];
				sod->anim_matrix.m[0][1] =  rot->m[6];
				sod->anim_matrix.m[2][1] =  rot->m[7];
				sod->anim_matrix.m[1][1] =  rot->m[8];
				sod->anim_matrix.t[0]    =  trans->t[0];
				sod->anim_matrix.t[2]    =  trans->t[1];
				sod->anim_matrix.t[1]    =  trans->t[2];
				}

			if ( sod->matrix_control2 & MATRIX_A )
				{
				sod->user_matrix_a.m[0][0] =  rot->m[0];
				sod->user_matrix_a.m[2][0] =  rot->m[1];
				sod->user_matrix_a.m[1][0] =  rot->m[2];
				sod->user_matrix_a.m[0][2] =  rot->m[3];
				sod->user_matrix_a.m[2][2] =  rot->m[4];
				sod->user_matrix_a.m[1][2] =  rot->m[5];
				sod->user_matrix_a.m[0][1] =  rot->m[6];
				sod->user_matrix_a.m[2][1] =  rot->m[7];
				sod->user_matrix_a.m[1][1] =  rot->m[8];
				sod->user_matrix_a.t[0]    =  trans->t[0];
				sod->user_matrix_a.t[2]    =  trans->t[1];
				sod->user_matrix_a.t[1]    =  trans->t[2];
				}
			}


		if ( all_animation_paused == NO )
			sod->anim_timer2++;

		if ( sod->anim_timer2 == (m01_anim->time*1) )	//overall speed
			{
			sod->anim_timer2 = 0;

			sod->current_anim_frame2++;

			if ( sod->current_anim_frame2 == sod->mdt_blocks2 )
				{
				if ( sod->matrix_control2 & MATRIX_ANIM_SINGLE_SHOT )
					{
					sod -> anim_initialised2 = NO;
					sod-> current_anim_frame2--;	// hang on this frame...
					}
				else
					{
					sod->current_anim_frame2 = 0;
					}

				if ( sod->matrix_control2 & MATRIX_ANIM_DEACTIVATE )
					sod->active = NO;
				}
			}
		}
//	else
//		{
//		sod->anim_matrix = sod->s01_matrix;
//		}

}





void load_m01_file ( WORD filenum, WORD slot )
{

	resident_m01_files[slot] = (M01_header *)read_datafile_alloc ( filenum );

}

void load_l01_file ( WORD filenum, WORD slot )
{

	resident_l01_files[slot] = (L01_header *)read_datafile_alloc ( filenum );

}








//***************************************************************************

void start_camera_anim ( WORD slot ) 
{
M01_obj_anim_src *m01_obj_anim;
BYTE *byte_addr;
WORD cnt;
M01_anim *m01_anim;

WORD num_m01_objects,num_cameras;
M01_header *m01_header;
Camera_data *cam = &camera.data[0];

	m01_header = resident_m01_files[slot];

	byte_addr = (BYTE *)m01_header;
	byte_addr += m01_header->obj_anim_data;
	m01_obj_anim = (M01_obj_anim_src *)byte_addr;


	num_m01_objects = m01_header->obj_cnt;

	while ( num_m01_objects-- )
		{
		cam = &camera.data[0];
		num_cameras = camera.num_cameras;

		while ( num_cameras-- )
			{
			if ( cam -> animation_active == NO )
				{

				//printf("doing match of '%s' and '%s'\n",cam-> name, m01_obj_anim->name );

				if ( strcmp ( cam -> name, m01_obj_anim->name ) == 0 )
					{
					cam -> animation_active = YES;
					FntPrint("match of '%s' and '%s'\n",cam-> name, m01_obj_anim->name );

					byte_addr = (BYTE *)m01_header;
					byte_addr += m01_obj_anim->animlist_offs;
					cam -> mdt_indexlist = (M01_anim *)byte_addr;

					byte_addr = (BYTE *)m01_header;
					byte_addr += m01_header->rot_data;
					cam -> mdt_rot = (M01_rot *)byte_addr;

					byte_addr = (BYTE *)m01_header;
					byte_addr += m01_header->trans_data;
					cam -> mdt_trans = (M01_trans *)byte_addr;

					cam -> mdt_blocks = m01_obj_anim -> num_blocks;
					cam -> current_anim_frame=0;
					cam -> last_anim_frame = -1;
					cam -> anim_timer=0;
			 		break;
					}
				}
			cam++;
			}
		m01_obj_anim++;
		}

}









void do_camera_animation()
{
M01_rot *rot;
M01_trans *trans;
M01_anim *m01_anim;
Camera_data *cam;
WORD cnt;
WORD cam_matrix_set = NO;

//   FntPrint("frame counter=%d\n",sod_header->anim_frame_counter);


//*** update global frame counter, and set flag to signal Animation has reset ***
	if ( all_animation_paused == NO )
		sod_header->anim_frame_counter++;

	if ( sod_header->anim_frame_counter > sod_header->total_frames )
		{
		sod_header->anim_frame_counter = 0;
		share->animation_reset_flag = YES;
		}


	cam = camera.data;
	for ( cnt=0; cnt< MAX_CAMERAS; cnt++ )
		{
		if ( cam -> animation_active == YES )
			{
 			m01_anim = &cam->mdt_indexlist[ cam->current_anim_frame ];
			rot     = &cam->mdt_rot   [ m01_anim->rmat_index ];
			trans   = &cam->mdt_trans [ m01_anim->tvec_index ];

			//** set object matrix **

			if ( camera.current == cnt )
				{
				camera.matrix.m[0][0] =  rot->m[0];
				camera.matrix.m[0][1] =  rot->m[2];
				camera.matrix.m[0][2] =  rot->m[1];

				camera.matrix.m[1][0] =  -rot->m[3];
				camera.matrix.m[1][1] =  -rot->m[5];
				camera.matrix.m[1][2] =  -rot->m[4];

				camera.matrix.m[2][0] =  -rot->m[6];
				camera.matrix.m[2][1] =  -rot->m[8];
				camera.matrix.m[2][2] =  -rot->m[7];

				camera.matrix.t[0]    =  -trans->t[0];
				camera.matrix.t[1]    =  -trans->t[2];
				camera.matrix.t[2]    =  -trans->t[1];

				cam_matrix_set = YES;
				}

			if ( cam -> current_anim_frame != cam -> last_anim_frame )
				{
			//** store old frame number to check for change **
				cam-> last_anim_frame  = cam -> current_anim_frame;
				}


			cam-> anim_timer++;

			if ( cam -> anim_timer == (m01_anim->time*1) )	//overall speed
				{
				cam -> anim_timer = 0;

				if ( all_animation_paused == NO )
					cam -> current_anim_frame++;
				if ( cam-> current_anim_frame == cam -> mdt_blocks )
					cam -> current_anim_frame = 0;
				}
			}
		cam++;
		}


//** If camera_matrix wasn't set, using any animation, 
//** Use scene camera matrix
	if ( cam_matrix_set == NO )
		camera.matrix = camera.data[ camera.current ].scene_matrix;
		

}

/*************************************************************************/



void start_light_anim ( WORD slot ) 
{
L01_obj_anim_src *l01_obj_anim;
BYTE *byte_addr;
WORD cnt;
L01_block *l01_blocklist;

WORD num_l01_objects,num_lights;
L01_header *l01_header;
LIGHT_DATA *ld;



	l01_header = resident_l01_files[slot];

	byte_addr = (BYTE *)l01_header;
	byte_addr += l01_header->object_data;
	l01_obj_anim = (L01_obj_anim_src *)byte_addr;


	num_l01_objects = l01_header->object_cnt;

	while ( num_l01_objects-- )
		{
		ld = &light.light_data[0];
		num_lights = light.num_lights;

		while ( num_lights-- )
			{
			if ( ld -> animation_active == NO )
				{

				if ( strcmp ( ld -> name, l01_obj_anim->name ) == 0 )
					{
					ld -> animation_active = YES;

					#if DEBUG
					printf("match of '%s' and '%s'\n",ld-> name, l01_obj_anim->name );
					#endif
					byte_addr = (BYTE *)l01_header;
					byte_addr += l01_obj_anim->blocklist_offs;
					ld -> ldt_blocklist = (L01_block *)byte_addr;

					byte_addr = (BYTE *)l01_header;
					byte_addr += l01_header->angle_data;
					ld -> ldt_anglelist = (L01_angle *)byte_addr;

					ld -> ldt_blocks = l01_obj_anim -> num_blocks;
					ld -> current_anim_frame=0;
					ld -> last_anim_frame = -1;
					ld -> anim_timer=0;
			 		break;
					}
				}
			ld++;
			}
		l01_obj_anim++;
		}

}




void do_light_animation()
{
L01_angle *ang_data;
L01_block *l01_block;
WORD allocated_slot;
LIGHT_DATA *ld;
WORD num_lights;

	if ( all_animation_paused == YES )
		return;

	ld = &light.light_data[0];

	num_lights = light.num_lights;

	while ( num_lights-- )
		{
		if ( ld->animation_active )
			{

 			l01_block = &ld->ldt_blocklist[ ld->current_anim_frame ];

			ang_data = &ld->ldt_anglelist [ l01_block->index ];

			if ( ld->current_anim_frame != ld->last_anim_frame )
				{
	  		//** store old frame number to check for change **

				ld->last_anim_frame  = ld->current_anim_frame;

	  		//** set light matrix **


				if ( (allocated_slot = ld->matrix_slot_num) != -1 )
					{
					ld->anim_ang.vx = ang_data->ang[0];
					ld->anim_ang.vy = ang_data->ang[1];
					ld->anim_ang.vz = ang_data->ang[2];
					}

				}


			if ( all_animation_paused == NO )
				ld->anim_timer++;

			if ( ld->anim_timer == (l01_block->time*1) )	//overall speed
				{
				ld->anim_timer = 0;

				ld->current_anim_frame++;
				if ( ld->current_anim_frame == ld->ldt_blocks )
					ld->current_anim_frame = 0;
				}

			}
		ld++;
		}
}





#if 0
/********************************************************
 *																		  *		
 * Play a single frame of animation to an object		  *
 *																		  *
 * Pass - object_num   = object to play animation on	  *
 *        block_num    = position in list of blocks	  *
 *			 mdt_slot_num = slot that mdt was loaded into  *
 *			 matrix_type  = user_matrix_a / user_matrix_b  *
 * Outputs matrix into sod+object_num->user_matrix_a/b  *
 *																	  	  *
 ********************************************************/

void play_obj_m01 ( WORD object_num, WORD block_num, WORD mdt_slot_num, WORD matrix_control )
{
M01_rot *rot;
M01_trans *trans;
M01_anim *m01_anim;
SOD *sod;
M01_header *m01_header = resident_m01_files[mdt_slot_num];

BYTE *byte_addr;
M01_obj_anim_src *m01_obj_anim,*m01_data;

	byte_addr = (BYTE *)m01_header;
	byte_addr += m01_header->obj_anim_data;
	m01_data = (M01_obj_anim_src *)byte_addr;

	m01_obj_anim = m01_data;

	byte_addr = (BYTE *)m01_header;
	byte_addr += m01_obj_anim->animlist_offs;
	m01_anim = (M01_anim *)byte_addr;

	byte_addr = (BYTE *)m01_header;
	byte_addr += m01_header->rot_data;
	rot = (M01_rot *)byte_addr;

	byte_addr = (BYTE *)m01_header;
	byte_addr += m01_header->trans_data;
	trans = (M01_trans *)byte_addr;


	sod = &sod_header->sod_list[ object_num ];

	m01_anim += block_num;


	rot     += m01_anim->rmat_index;
	trans   += m01_anim->tvec_index;

	//** set object matrix **

	if ( matrix_control & MATRIX_B )
		{
		sod->user_matrix_b.m[0][0] =  rot->m[0];
		sod->user_matrix_b.m[2][0] =  rot->m[1];
		sod->user_matrix_b.m[1][0] =  rot->m[2];
		sod->user_matrix_b.m[0][2] =  rot->m[3];
		sod->user_matrix_b.m[2][2] =  rot->m[4];
		sod->user_matrix_b.m[1][2] =  rot->m[5];
		sod->user_matrix_b.m[0][1] =  rot->m[6];
		sod->user_matrix_b.m[2][1] =  rot->m[7];
		sod->user_matrix_b.m[1][1] =  rot->m[8];
		sod->user_matrix_b.t[0]    =  trans->t[0];
		sod->user_matrix_b.t[2]    =  trans->t[1];
		sod->user_matrix_b.t[1]    =  trans->t[2];
		}
	if ( matrix_control & MATRIX_A )
		{
		sod->user_matrix_a.m[0][0] =  rot->m[0];
		sod->user_matrix_a.m[2][0] =  rot->m[1];
		sod->user_matrix_a.m[1][0] =  rot->m[2];
		sod->user_matrix_a.m[0][2] =  rot->m[3];
		sod->user_matrix_a.m[2][2] =  rot->m[4];
		sod->user_matrix_a.m[1][2] =  rot->m[5];
		sod->user_matrix_a.m[0][1] =  rot->m[6];
		sod->user_matrix_a.m[2][1] =  rot->m[7];
		sod->user_matrix_a.m[1][1] =  rot->m[8];
		sod->user_matrix_a.t[0]    =  trans->t[0];
		sod->user_matrix_a.t[2]    =  trans->t[1];
		sod->user_matrix_a.t[1]    =  trans->t[2];


		}

	if ( matrix_control & MATRIX_ANIM )
		{
		sod->anim_matrix.m[0][0] =  rot->m[0];
		sod->anim_matrix.m[2][0] =  rot->m[1];
		sod->anim_matrix.m[1][0] =  rot->m[2];
		sod->anim_matrix.m[0][2] =  rot->m[3];
		sod->anim_matrix.m[2][2] =  rot->m[4];
		sod->anim_matrix.m[1][2] =  rot->m[5];
		sod->anim_matrix.m[0][1] =  rot->m[6];
		sod->anim_matrix.m[2][1] =  rot->m[7];
		sod->anim_matrix.m[1][1] =  rot->m[8];
		sod->anim_matrix.t[0]    =  trans->t[0];
		sod->anim_matrix.t[2]    =  trans->t[1];
		sod->anim_matrix.t[1]    =  trans->t[2];
		}




}

#endif


void start_obj_m01_anim1 ( SOD *sod, WORD mdt_slot_num, WORD matrix_control )
{
M01_header *m01_header = resident_m01_files[mdt_slot_num];
BYTE *byte_addr;
M01_obj_anim_src *m01_data,*m01_obj_anim;


	byte_addr = (BYTE *)m01_header;
	byte_addr += m01_header->obj_anim_data;
	m01_data = (M01_obj_anim_src *)byte_addr;

	m01_obj_anim = m01_data;

	byte_addr = (BYTE *)m01_header;
	byte_addr += m01_obj_anim->animlist_offs;
	sod->mdt_indexlist1 = (M01_anim *)byte_addr;

	byte_addr = (BYTE *)m01_header;
	byte_addr += m01_header->rot_data;
	sod->mdt_rot1 = (M01_rot *)byte_addr;

	byte_addr = (BYTE *)m01_header;
	byte_addr += m01_header->trans_data;
	sod->mdt_trans1 = (M01_trans *)byte_addr;


	sod->mdt_blocks1 = m01_obj_anim -> num_blocks;
	sod->current_anim_frame1 = 0;
	sod->last_anim_frame1 = -1;
	sod->anim_timer1 = 0;
	sod->anim_initialised1 = NO;

	sod->matrix_control1 = matrix_control | MATRIX_ANIM_ACTIVE;

}

#if 0


void update_obj_m01_anim ( SOD *sod)
{
M01_rot *rot;
M01_trans *trans;
M01_anim *m01_anim;

BYTE *byte_addr;
M01_obj_anim_src *m01_obj_anim,*m01_data;
MATRIX id = M_IDENTITY;


	m01_anim = &sod->mdt_indexlist[ sod->current_anim_frame ];

	rot   = &sod->mdt_rot	 [ m01_anim->rmat_index ];
	trans = &sod->mdt_trans[ m01_anim->tvec_index ];


	if ( sod->current_anim_frame != sod->last_anim_frame )
		{
	//** store old frame number to check for change **

		sod->last_anim_frame  = sod->current_anim_frame;

	//** set object matrix **

		if ( sod->matrix_anim_control & MATRIX_B )
			{
			// Output to MATRIX_B
			sod->user_matrix_b.m[0][0] =  rot->m[0];
			sod->user_matrix_b.m[2][0] =  rot->m[1];
			sod->user_matrix_b.m[1][0] =  rot->m[2];
			sod->user_matrix_b.m[0][2] =  rot->m[3];
			sod->user_matrix_b.m[2][2] =  rot->m[4];
			sod->user_matrix_b.m[1][2] =  rot->m[5];
			sod->user_matrix_b.m[0][1] =  rot->m[6];
			sod->user_matrix_b.m[2][1] =  rot->m[7];
			sod->user_matrix_b.m[1][1] =  rot->m[8];
			sod->user_matrix_b.t[0]    =  trans->t[0];
			sod->user_matrix_b.t[2]    =  trans->t[1];
			sod->user_matrix_b.t[1]    =  trans->t[2];
			}
		if ( sod->matrix_anim_control & MATRIX_A )
			{
			// Output to MATRIX_A
			sod->user_matrix_a.m[0][0] =  rot->m[0];
			sod->user_matrix_a.m[2][0] =  rot->m[1];
			sod->user_matrix_a.m[1][0] =  rot->m[2];
			sod->user_matrix_a.m[0][2] =  rot->m[3];
			sod->user_matrix_a.m[2][2] =  rot->m[4];
			sod->user_matrix_a.m[1][2] =  rot->m[5];
			sod->user_matrix_a.m[0][1] =  rot->m[6];
			sod->user_matrix_a.m[2][1] =  rot->m[7];
			sod->user_matrix_a.m[1][1] =  rot->m[8];
			sod->user_matrix_a.t[0]    =  trans->t[0];
			sod->user_matrix_a.t[2]    =  trans->t[1];
			sod->user_matrix_a.t[1]    =  trans->t[2];
			}
		if ( sod->matrix_anim_control & MATRIX_ANIM )
			{
			// Output to anim_matrix
			sod->anim_matrix.m[0][0] =  rot->m[0];
			sod->anim_matrix.m[2][0] =  rot->m[1];
			sod->anim_matrix.m[1][0] =  rot->m[2];
			sod->anim_matrix.m[0][2] =  rot->m[3];
			sod->anim_matrix.m[2][2] =  rot->m[4];
			sod->anim_matrix.m[1][2] =  rot->m[5];
			sod->anim_matrix.m[0][1] =  rot->m[6];
			sod->anim_matrix.m[2][1] =  rot->m[7];
			sod->anim_matrix.m[1][1] =  rot->m[8];
			sod->anim_matrix.t[0]    =  trans->t[0];
			sod->anim_matrix.t[2]    =  trans->t[1];
			sod->anim_matrix.t[1]    =  trans->t[2];
			}
			
		 }


	if ( all_animation_paused == NO )
		sod->anim_timer++;

	if ( sod->anim_timer == (m01_anim->time*1) )	//overall speed
		{
		sod->anim_timer = 0;

		sod->current_anim_frame++;

		if ( sod->current_anim_frame == sod->mdt_blocks )
			sod->current_anim_frame = 0;
		}

	//** set object matrix **



	sod->anim_timer++;

	FntPrint("tets\n");

	if ( sod->anim_timer == (m01_anim->time*1) )	//overall speed
		{
		sod->anim_timer = 0;

		sod->current_anim_frame++;

		if ( sod->current_anim_frame == sod->mdt_blocks )
			{
			if ( sod->matrix_anim_control & MATRIX_ANIM_SINGLE_SHOT )
				{
				sod->current_anim_frame = 0;
				}

			if ( sod->matrix_anim_control & MATRIX_ANIM_DEACTIVATE )
				sod->active = NO;

			}
		}

}

#endif




#endif







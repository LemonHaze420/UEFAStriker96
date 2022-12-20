/************************************************************************/
/*																							  	*/
/*																							  	*/
/*	Module: Light control															  	*/
/*																							  	*/
/*																							 	*/
/************************************************************************/

/************************************************************************/
/*																							  	*/
/*	include default headers															  	*/
/*																							  	*/
/************************************************************************/

#if 0

#include "incs.h"

#ifdef PSX
//#include "machine.h"	// contains PSX, PC, N64
//#include "defs_gen.h"
//#include "lib_psx.h"	// set to libn64.h , libpc.h or libpsx.h
//#include "defs_psx.h"
#include "..\..\global\striker.def" // defs for PSX striker
//#include "..\..\global\striker.h"	 // share structure for all formats
#include "..\..\global\s01.h"	// structs used by scene "s01" format
#include	"sod.h"
#include "polydraw.h"	//psx structs
#include	"light.h"
#include "..\..\global\image.h"
#endif


MATRIX sw_color_matrix = 
{
// each COLUMN specifies light color and intensity

//lgt1 color	  lgt2 color	       lgt3 color
	ONE*4/4, 		ONE*4/4,					ONE*4/4,
	ONE*4/4,			ONE*4/4,					ONE*4/4,
	ONE*4/4, 		ONE*4/4,					ONE*4/4,
};


MATRIX color_matrix;
void draw_unit ( MATRIX *my_mat );


LIGHT light;

POLY_F4 light_pols[3][2];
WORD selected_light=0;


// set up a light from the list of scene lights
// and place into one of the three hw light slots



void kill_light ( WORD light_num )
{

	light.light_data[light_num].active = NO;
}


void reset_lights()
{
WORD cnt;

	for ( cnt=0;cnt< MAX_LIGHTS; cnt++ )
		{
		light.light_data[cnt].type=0;
		light.light_data[cnt].active=NO;
		light.light_data[cnt].animation_active=NO;
		light.light_data[cnt].debug_ang.vx=
		light.light_data[cnt].debug_ang.vy=
		light.light_data[cnt].debug_ang.vz=0;
		}
}







#if 0
LINE_F2 line_pols[3][2];

void draw_unit ( MATRIX *this_mat )
{

//** Draw unit vector **

SVECTOR vec[3];

BYTE rs[]={ 255, 30,  30  };
BYTE gs[]={ 30,  255, 30  };
BYTE bs[]={ 30,  30,  255 };

WORD cnt;

#define LX 256
#define LY 120

	vec[0].vx = 24;
	vec[0].vy = 0;
	vec[0].vz = 0;

	vec[1].vx = 0;
	vec[1].vy = 24;
	vec[1].vz = 0;

	vec[2].vx = 0;
	vec[2].vy = 0;
	vec[2].vz = 24;


	SetRotMatrix  ( this_mat );
 	SetTransMatrix( this_mat );

	for ( cnt=0; cnt<3; cnt++ )
		{

		line_pols[cnt][cdb->buffnum].r0=rs[cnt];
		line_pols[cnt][cdb->buffnum].g0=gs[cnt];
		line_pols[cnt][cdb->buffnum].b0=bs[cnt];

		SetLineF2 ( &line_pols[cnt][cdb->buffnum] );
		line_pols[cnt][cdb->buffnum].x1 = LX;	// centre of screen
		line_pols[cnt][cdb->buffnum].y1 = LY;

		gte_ldv0( &vec[cnt] );
		gte_rtps();
		gte_stsxy( ( ULONG *)&line_pols[cnt][cdb->buffnum].x0 );
		addPrim ( cdb->ot + OVERLAY_DEPTH, &line_pols[cnt][cdb->buffnum] );
		}


}

#endif








void set_dir_light_matrix()
{
WORD cnt;
MATRIX *lm;
WORD light_num=0;

//SVECTOR v0 = { 0,0,4096 };
SVECTOR v1;
MATRIX rotmat=M_IDENTITY;
LIGHT_DATA *ld;
	lm = &light.light_matrix;
	lm->m[0][0]=lm->m[0][1]=lm->m[0][2]=
	lm->m[1][0]=lm->m[1][1]=lm->m[1][2]=
	lm->m[2][0]=lm->m[2][1]=lm->m[2][2]=0;

	lm = &light.color_matrix;
	lm->m[0][0]=lm->m[0][1]=lm->m[0][2]=
	lm->m[1][0]=lm->m[1][1]=lm->m[1][2]=
	lm->m[2][0]=lm->m[2][1]=lm->m[2][2]=0;

//setup lights using scene light data....

	light.any_hw_light_active = NO;

	for( cnt=0; cnt<MAX_LIGHTS; cnt++ )
		{
		ld = &light.light_data[cnt];

		ld->matrix_slot_num = -1;

		if ( light.light_data[cnt].active==YES )
			{
			if ( light.light_data[cnt].type == DIRECTIONAL_LIGHT )
				{
				// the xyz pos when used for directional lights is actually
				// a vector that should be plugged directly into the light matrix

				// rotate vector v0 by input angle. output to v1
				// and put in light matrix


				RotMatrix ( &ld->debug_ang , &rotmat );

				if ( ld->animation_active == YES )
					ApplyMatrixSV ( &rotmat, &ld->anim_ang, &v1 );
				else
					ApplyMatrixSV ( &rotmat, &ld->scene_ang, &v1 );

				light.light_matrix.m[ light_num ][0] = v1.vx;
				light.light_matrix.m[ light_num ][1] = v1.vy;
				light.light_matrix.m[ light_num ][2] = v1.vz;

				light.color_matrix.m[0][ light_num ] = light.light_data[cnt].color[0]*16;
				light.color_matrix.m[1][ light_num ] = light.light_data[cnt].color[1]*16;
				light.color_matrix.m[2][ light_num ] = light.light_data[cnt].color[2]*16;

				ld->matrix_slot_num = light_num;

				//FntPrint("col %d= %d %d %d\n",light_num,light.light_data[cnt].color[0],light.light_data[cnt].color[1],light.light_data[cnt].color[2] );

				light.any_hw_light_active = YES;

				light_num++;

				if ( light_num == 3 )
					break;	// no more hw lights
				}


			if ( light.light_data[cnt].type == POINT_LIGHT )
				{
				if ( ld->animation_active == YES )
					ld->angpos = ld->anim_ang;
				else
					ld->angpos = ld->scene_ang;

				//matrix for point light set in polydraw.c

				light.color_matrix.m[0][ light_num ] = light.light_data[cnt].color[0]*16;
				light.color_matrix.m[1][ light_num ] = light.light_data[cnt].color[1]*16;
				light.color_matrix.m[2][ light_num ] = light.light_data[cnt].color[2]*16;

				ld->matrix_slot_num = light_num;

				light.any_hw_light_active = YES;

				light_num++;

				if ( light_num == 3 )
					break;	// no more hw lights
				}
			}					

		}


	SetColorMatrix(&light.color_matrix);	  
}
#endif

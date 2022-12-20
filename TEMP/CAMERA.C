/************************************************************************/
/*																							  	*/
/*																							  	*/
/*	Module: Camera control															  	*/
/*																							  	*/
/*																							 	*/
/************************************************************************/

/************************************************************************/
/*																							  	*/
/*	include default headers															  	*/
/*																							  	*/
/************************************************************************/

/************************************************************************/
/*																							  	*/
/*	game includes																		  	*/
/*																							  	*/
/************************************************************************/

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
//#include "test.h"
#include	"mem.h"
#include	"fvars.h"
#include	"pad.h"
#include	"light.h"
#include	"camera.h"
#include "utils.h"
#include	"io.h"
//#include "teamsel.h"
#include "options.h"
//#include "..\..\global\image.h"
#endif


/************************************************************************/
/*																							  	*/
/*	prototypes																			  	*/
/*																							  	*/
/************************************************************************/

//void move_camera( void );
//void perp_vectors(register SVECTOR *i, register SVECTOR *j, register SVECTOR *k);
//void point_camera_by_vector(void);
//void point_camera_by_vector(void);
//void move_matrix_camera( void );
//void perp_vectors(register SVECTOR *a, register SVECTOR *b, register SVECTOR *c);


/************************************************************************/
/*																							  	*/
/*	defs																					  	*/
/*																							  	*/
/************************************************************************/

/************************************************************************/
/*																							  	*/
/*	vars																					  	*/
/*																							  	*/
/************************************************************************/

Camera camera;


#if 0

void reset_cameras()
{
MATRIX ident=M_IDENTITY;
WORD cnt;

	for ( cnt=0;cnt< MAX_CAMERAS; cnt++ )
		{
		camera.data[cnt].pos.vx=0;
		camera.data[cnt].pos.vy=1;
		camera.data[cnt].pos.vz=0;
		camera.data[cnt].angle.vx=
		camera.data[cnt].angle.vy=
		camera.data[cnt].angle.vz=0;
		camera.data[cnt].type = NORMAL_CAM;
		camera.data[cnt].scene_matrix = ident;

		camera.data[cnt].animation_active = NO;
		camera.data[cnt].anim_timer = 0;
		camera.data[cnt].current_anim_frame = 0;
		camera.data[cnt].last_anim_frame = -1;
		camera.data[cnt].anim_suspended=NO;
		}

	camera.aspect_correction = ON;
	camera.current=0;
}









#if 0
void move_matrix_camera( void )
{ 
Camera_data *cam=&camera.data[camera.current];


	if ( item_num == MOVE_CAMERA_ANGLE )
		{


		if ( joy[0].c & PAD_X )
			{
			adjust_vector ( &cam->pos, 10 );
			}

		if ( joy[0].c & PAD_TRI )
			{
			adjust_svector ( &cam->dest, 10 );
			}

		FntPrint("cam src=%d %d %d\ncam dest=%d %d %d\n",cam->pos.vx,cam->pos.vy,cam->pos.vz,cam->dest.vx,cam->dest.vy,cam->dest.vz);
		}
	


//	cam->xa.vx = 0;
//	cam->xa.vy = 0;
//	cam->xa.vz = 0;

	cam->za.vx = (cam->dest.vx - cam->pos.vx );
	cam->za.vy = (cam->dest.vy - cam->pos.vy );
	cam->za.vz = (cam->dest.vz - cam->pos.vz );

	cam->ya.vx = 0;
	cam->ya.vy = 4096;
	cam->ya.vz = 0;


	perp_vectors(&cam->za, &cam->ya, &cam->xa);
	point_camera_by_vector();


}


void perp_vectors(register SVECTOR *a, register SVECTOR *b, register SVECTOR *c)
{
	VECTOR va, vb, vc;

	copyVector(&va, a);
	copyVector(&vb, b);

	VectorNormal(&va, &va);

	if((va.vx == 0) && (va.vy == 0))
		copyVector(&vc, c);
	else
		OuterProduct12(&vb, &va, &vc);

	OuterProduct12(&va, &vc, &vb);

	VectorNormal(&vb, &vb);
	VectorNormal(&vc, &vc);

	copyVector(a, &va);
	copyVector(b, &vb);
	copyVector(c, &vc);
}


void point_camera_by_vector(void)
{
Camera_data *cam = &camera.data[camera.current];
MATRIX real=M_IDENTITY;

VECTOR v1;
MATRIX m1 = M_IDENTITY, m2 = M_IDENTITY;

	m1.m[0][0] = cam->xa.vx;
	m1.m[0][1] = cam->xa.vy;
	m1.m[0][2] = cam->xa.vz;

	m1.m[1][0] = cam->ya.vx;
	m1.m[1][1] = cam->ya.vy;
	m1.m[1][2] = cam->ya.vz;

	m1.m[2][0] = cam->za.vx;
	m1.m[2][1] = cam->za.vy;
	m1.m[2][2] = cam->za.vz;

	v1.vx = -cam->pos.vx;
	v1.vy = -cam->pos.vy;
	v1.vz = -cam->pos.vz;

	TransMatrix(&m2, &v1);
	CompMatrix(&m1, &m2, &real);


	camera.matrix = real;

  	SetRotMatrix(&real);
	SetTransMatrix(&real);


	FntPrint("outmat=\n%d %d %d\n%d %d %d\n%d %d %d\n%d %d %d\n",real.m[0][0],real.m[0][1],real.m[0][2],real.m[1][0],real.m[1][1],real.m[1][2],real.m[2][0],real.m[2][1],real.m[2][2],real.t[0],real.t[1],real.t[2]);
	



}
#endif
#endif





void fix_aspect_ratio(MATRIX *mat)
{					  
//	if ( camera.aspect_correction == YES )
//		{
		mat->m[0][0] = mat->m[0][0] * config->screen_xres / camera.x_aspect;
		mat->m[0][1] = mat->m[0][1] * config->screen_xres / camera.x_aspect;
		mat->m[0][2] = mat->m[0][2] * config->screen_xres / camera.x_aspect;

//		mat->m[1][0] = mat->m[1][0] * 2; //config->screen_xres / camera.x_aspect;
//		mat->m[1][1] = mat->m[1][1] * 2; //config->screen_xres / camera.x_aspect;
//		mat->m[1][2] = mat->m[1][2] * 2; //config->screen_xres / camera.x_aspect;

//		}
}


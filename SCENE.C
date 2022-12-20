
/****************************************************
 *																	 *
 *	 Scene draw													 *
 *	 19.3.97														 *
 *																	 *
 ****************************************************/


 //team
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
#include	"camera.h"
#include	"light.h"
//#include "utils.h"
#include	"io.h"
//#include	"sound.h"
//#include "teamsel.h"
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

void setup_scene_cameras();
void setup_scene_lights();
void setup_sod_header( Scene_startup *scene_startup );
void display_models( WORD *list );

/****************************************************
 *																	 *
 *	 Defines														 *
 *																	 *
 ****************************************************/

#define M_IDENTITY {{{4096,0,0},{0,4096,0},{0,0,4096}},{0,0,0}}


/****************************************************
 *																	 *
 *	 Vars															 *
 *																	 *
 ****************************************************/

Scene_header_src *scene_header_src;
SOD_header sod_header_area;
SOD_header *sod_header= &sod_header_area;


// store for texture bpp, x, y, w, h, clut x, clut y
// index into this by 


#if 0
void init_scene_draw( Scene_startup *ss )
{
//	sod_header = sh;

	reset_lights();
	reset_cameras();
	reset_animation();


// load sdt file to memory
//	load_scene_sdt ( ss );	// load sdt to permanent addr


}





void setup_scene_cameras()
{
WORD cnt;
Camera_src *camera_src;
BYTE *addr;

	camera.num_cameras = scene_header_src->num_cameras;

	if ( camera.num_cameras == 0 )
		{
		display_error ( "no camera found in scene" );
		return;	// no cameras present in this scene
		}

	addr = (BYTE *)scene_header_src;
	addr += scene_header_src-> camera_data_offs;
	camera_src = (Camera_src *)addr;

	for ( cnt=0; cnt<camera.num_cameras; cnt++ )
		{
		strcpy ( &camera.data[cnt].name[0], camera_src->name );
		camera.data[cnt].scene_matrix = camera_src->mat;

		camera_src++;
		}
}


void setup_scene_lights()
{
WORD cnt,current_light;
LIGHT_DATA *light_src,*light_dest,*light_ptr;
BYTE *addr;

	light.num_lights = scene_header_src->num_lights;

	if ( light.num_lights == 0 )
		{
		display_error ( "no light found in scene" );
		return;	// no lights present in this scene
		}

	addr = (BYTE *)scene_header_src;
	addr += scene_header_src-> light_data_offs;
	light_src = (LIGHT_DATA *)addr;

	light_dest = &light.light_data[0];



//copy light data from scene src to allocated area

	for ( cnt=0; cnt<light.num_lights; cnt++ )
		{
		light_dest->type = light_src->type;
		light_dest->color[0] = light_src->color[0];
		light_dest->color[1] = light_src->color[1];
		light_dest->color[2] = light_src->color[2];
		light_dest->scene_ang = light_src->scene_ang;
		light_dest->active = YES;

		strcpy ( light_dest->name, light_src->name );

		light_dest++;
		light_src++;
		}

	light_ptr = light.light_data;

	for ( current_light=0; current_light< light.num_lights; current_light++ )
		{
		if ( light_ptr->type )
			light_ptr->active=YES;
		else
			light_ptr->active=NO;

		light_ptr++;
		}
}













void set_scene_ambience ( BYTE r, BYTE g, BYTE b )
{

	light.ambient_color.r = r;
	light.ambient_color.g = g;
	light.ambient_color.b = b;
	  

}





#if 0
void load_scene_sdt( Scene_startup *scene_startup ) 
{
BYTE *addr;
	addr = read_datafile_alloc ( scene_startup->sdt_filenum );

	scene_header_src = (Scene_header_src *)addr;

//** load scene texture data to VRAM **
	if ( scene_startup->tex_filenum >= 0 )
		load_scene_materials ( scene_startup->tex_filenum );



	scene_header_src = (Scene_header_src *)addr;

	printf("scene '%s' contains....\n",scene_header_src->scene_filename );
	printf("objects... %d\n",sod_header->num_objects);
	printf("cameras....%d\n",camera.num_cameras);
	printf("lights.....%d\n",light.num_lights);
//	printf("scene uses=%d bytes\n",read_free_addr() - mh1);


}
#endif

void start_scene( Scene_startup *scene_startup )
{
BYTE *scene_sdt_temp_area;

//LONG mh1;
//LONG long_addr;


// load texture data
	if ( scene_startup->tex_filenum >= 0 )
		load_scsene_materials ( scene_startup->tex_filenum );


// load sdt file to temp area of memory
	scene_sdt_temp_area = (BYTE *)(0x801ff000 - MAX_SCENE_SDT_SIZE );


//** load sdt file to temp area ***

	read_datafile ( scene_startup->sdt_filenum,scene_sdt_temp_area, MAX_SCENE_SDT_SIZE );
	scene_header_src = (Scene_header_src *)scene_sdt_temp_area;

	setup_sod_header( scene_startup );	// scene object descriptors

	set_scene_ambience ( scene_header_src->ambient_r, scene_header_src->ambient_g, scene_header_src->ambient_b );

	setup_scene_objects();

	setup_scene_cameras();
	setup_scene_lights();



	if ( scene_startup->mdt_filenum != -1 )
 		{
		load_m01_file ( scene_startup->mdt_filenum, 0 );	// load to slot 0
		start_scene_anim ( 0, 0, MATRIX_ANIM_SINGLE_SHOT + MATRIX_ANIM );	//play anim from slot 0
		start_camera_anim( 0 );
		}

	if ( scene_startup->ldt_filenum != -1 )
 		{
		load_l01_file ( scene_startup->ldt_filenum, 0 );	// load to slot 0
		start_light_anim ( 0 );	//play anim from slot 0
		}

	sod_header->envrot_vec = scene_startup->envrot_vec;


	init_renderer();



}

void setup_sod_header( Scene_startup *scene_startup )
{
WORD cnt;
SOD *sod;
BYTE *addr;
Scene_obj_src *scene_obj_src;
MATRIX id=M_IDENTITY;

	sod_header->num_objects	= scene_header_src->num_objects;
	sod_header->sod_list = allocate_mem ( 0, sizeof (SOD) * sod_header->num_objects );

	sod_header->pos.vx = sod_header->pos.vy = sod_header->pos.vz = 0;
	sod_header->rot.vx = sod_header->rot.vy = sod_header->rot.vz = 0;

	addr = (BYTE *)scene_header_src;
	addr += scene_header_src->scene_data_offs;
	scene_obj_src = (Scene_obj_src *)addr;

	sod = sod_header->sod_list;

	for ( cnt=0;cnt<sod_header->num_objects;cnt++)
		{
		sod->exclude_from_scene = NO;

		if ( scene_startup->model_list )
			sod->active = NO;
		else
			sod->active = YES;
		sod->rot.vx = sod->rot.vy = sod->rot.vz = 0;
	  	sod->matrix_control1 = 0;
	  	sod->matrix_control2 = 0;

		sod->anim_initialised1=NO;
		sod->anim_initialised2=NO;
		sod->s01_matrix = scene_obj_src->ob_matrix;
		sod->user_matrix_a = 
		sod->user_matrix_b = id;
		sod->color_override = NO_OVERRIDE;
		sod->user_depth = 0;
		sod->anim_seq = 0;
		strcpy ( sod->name, scene_obj_src->name );

		sod++;
		scene_obj_src++;

		}

// Display only the models indicated in this list
	display_models( scene_startup->model_list );

}



void display_models( WORD *list )
{
	if ( list )
		{
		while ( *list != -1 )
			{
//			sod_header->sod_list[ *list ].exclude_from_scene = NO;
			sod_header->sod_list[ *list ].active = YES;
			list++;
			}
		}
}











































#endif

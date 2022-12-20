/********************************************************************
 *																						  *
 *																						  *
 * PSX debugging utils															  *
 *	This module can be ommitted in Master version						  *
 *																						  *
 *																						  *
 ********************************************************************/



/********************************************************************
 *																						  *
 * includes																			  *
 *																						  *
 ********************************************************************/

//#include	"incs.h"


#include "defs_gen.h"
#include "lib_psx.h"	// set to libn64.h , libpc.h or libpsx.h
#include "defs_psx.h"


#include "..\..\global\striker.def" // defs for PSX striker
#include "..\..\global\striker.h"	 // share structure for all formats
#include "..\..\global\s01.h"	// structs used by scene "s01" format
#include	"sod.h"
#include	"scene.h"
#include	"structs.h"
#include	"main.h"
#include	"general.h"
#include "datafile.h"
#include "hardware.h"
#include "anim.h"	
#include	"mem.h"
#include	"pad.h"
#include	"camera.h"
#include	"light.h"
#include "utils.h"
#include "frontend.h"
#include	"scngrab.c"		//screen grabber
#include "vrampeek.c"	//vram peeker
#include "sprite.h"
#include "gadget.h"
#include "font.h"
#include "shell.h"
/********************************************************************
 *																						  *
 * prototypes																		  *
 *																						  *
 ********************************************************************/

void set_tbar( WORD x, WORD y, WORD height, POLY_F4 *prim1,POLY_F4 *prim2, UBYTE r, UBYTE g, UBYTE b );
void adjust_vector( VECTOR *sv, WORD amount );
void adjust_svector( SVECTOR *sv, WORD amount );

void adjust_scene();
void pause_animation();
void debug_lights_on_off();
void debug_adjust_textstring();
void move_camera();
void debug_sfx();
void debug_adjust_rdb();
void debug_crosshair();

/********************************************************************
 *																						  *
 * vars	 																			  *
 *																						  *
 ********************************************************************/

WORD gpu_bar_stop,gpu_bar_start;
WORD cpu_speed,cpu_bar_stop,cpu_bar_start;
WORD draw_counter;
POLY_F4 cpu_bar[3];
POLY_F4 gpu_bar[2];
Gadget *debug_rdb;
Rdb_gadget debug_rdb_gadget;


/**********
  ] = square
  \ = tri
  ] = circle
  ^ = X
***********/

void setup_system_font()
{
#if DEBUG == YES

#define FONT_VRAM_X 512+(5*64)
#define FONT_VRAM_Y 1*256 // 0 or 1

RECT font_rect={FONT_VRAM_X,FONT_VRAM_Y,128/4,32};
RECT pal_rect={FONT_VRAM_X,FONT_VRAM_Y+128,32,1};


// load normal system font
	FntLoad( FONT_VRAM_X, FONT_VRAM_Y );
	FntOpen( 16, NTSC?9:6, config->screen_xres-16, config->screen_yres, 0, 1600);

//	return;
//copy nicer font over top of standard one
	LoadImage( &font_rect,(ULONG *)&sysfont_spr);

	DrawSync(0);
	LoadImage( &pal_rect,(ULONG *)&sysfont_pal);
	DrawSync(0);
	
#endif
}




#define	TBAR_X		14
#define	TBAR_Y		(NTSC?212:247)
#define	TBAR_HEIGHT	2

ULONG cpu_time,gpu_time;
ULONG peak_cpu_time=0;
ULONG peak_gpu_time=0;
ULONG wait_peak_check;

void init_timer_bars( void )
{
 	GsClearVcount(); 

	cpu_bar_start = cpu_bar_stop = 0;
	gpu_bar_start = gpu_bar_stop = 0;

	set_tbar( TBAR_X,TBAR_Y,TBAR_HEIGHT,&cpu_bar[0],&cpu_bar[1],70/2,70/2,255/2 );
	set_tbar( TBAR_X,TBAR_Y+TBAR_HEIGHT+1,TBAR_HEIGHT,&cpu_bar[2],&cpu_bar[2],235/2,234/2,10/2 );
	set_tbar( TBAR_X,TBAR_Y+((TBAR_HEIGHT)*2)+2,TBAR_HEIGHT,&gpu_bar[0],&gpu_bar[1],220/2,80/2,80/2 );

	peak_cpu_time = peak_gpu_time= cpu_time = gpu_time= 0;
	wait_peak_check = 5;	// wait n frames before check
}



void set_tbar( WORD x, WORD y, WORD height, POLY_F4 *prim1,POLY_F4 *prim2, UBYTE r, UBYTE g, UBYTE b )
{
WORD cnt=2;
POLY_F4 *prim[2];

	prim[0]=prim1;
	prim[1]=prim2;

	while(cnt-- )
		{
		prim[cnt] -> r0 = r;
		prim[cnt] -> g0 = g;
		prim[cnt] -> b0 = b;
	
		prim[cnt] -> x0 =
		prim[cnt] -> x2 = x;
		prim[cnt] -> x1 = 
		prim[cnt] -> x3 = x+(250>>1);

		prim[cnt] -> y0 =
		prim[cnt] -> y1 = y;
		prim[cnt] -> y2 =
		prim[cnt] -> y3 = y+height;
	
		SetPolyF4( prim[cnt] );
		SetSemiTrans( prim[cnt],0 );
		}


}


void draw_cpu_bar( void )
{
WORD	cwidth,gwidth;


  	cpu_bar_stop =GsGetVcount();
	cwidth = (WORD)(cpu_bar_stop - cpu_bar_start);

	cpu_bar[cdb->buffnum].x1 =
	cpu_bar[cdb->buffnum].x3 = (cwidth>>1)+cpu_bar[ cdb->buffnum ].x0;

	cpu_bar[2].x1 =
	cpu_bar[2].x3 = 125 * frame_update + cpu_bar[ 2 ].x0;

	if ( print_debug_info <3 )
		{
		addPrim(cdb->ot+OVERLAY_DEPTH, &cpu_bar[cdb->buffnum] );
		addPrim(cdb->ot+OVERLAY_DEPTH, &cpu_bar[2] );
		}

	//cpu_speed = ((cpu_bar_stop-cpu_bar_start) * 1000)/256;

	cpu_time = ((cwidth * 1000)/256)/frame_update;


}

void show_cpugpu_time()
{
ULONG addr,cnt,cnt2;
BYTE text_str[128];
ULONG bytes_used;
UWORD hun,ht,mil;

//	addr = (ULONG)(read_free_addr(0) - 0x80000000);

	bytes_used = mh_amount_used[0] + mh_amount_used[1] + mh_amount_used[2] + mh_amount_used[3];

	sprintf(  &text_str[0],  "H1=%d H2=%d                                                                               ",mh_amount_used[0] >>10 ,mh_amount_used[1]>>10 );

	sprintf ( &text_str[14+5], "cpu %d.%d       ",cpu_time/10,cpu_time%10 );
	sprintf ( &text_str[24+5], "/%d.%d     \n ",peak_cpu_time/10,peak_cpu_time%10 );

	sprintf ( &text_str[32+5], "gpu %d.%d       ",gpu_time/10,gpu_time%10 );
	sprintf ( &text_str[42+5], "/%d.%d     \n",peak_gpu_time/10,peak_gpu_time%10 );

	FntPrint( text_str );


// dont start calculating max cpu/gpu time for a few frames till they settle down.....
	if ( wait_peak_check )
		wait_peak_check--;
	else
		{
		if ( peak_cpu_time < cpu_time )
			peak_cpu_time = cpu_time;

		if ( peak_gpu_time < gpu_time )
			peak_gpu_time = gpu_time;
		}


}



void draw_gpu_bar( void )
{
WORD	cwidth,gwidth;
 
	gwidth = gpu_bar_stop - gpu_bar_start;

//	FntPrint("gw=%d\n",gwidth);
	gpu_bar[cdb->buffnum].x1 =
	gpu_bar[cdb->buffnum].x3 = (gwidth>>0) + gpu_bar[ cdb->buffnum ].x0;

	if ( print_debug_info <3 )
		{
		AddPrim(cdb->ot+OVERLAY_DEPTH, &gpu_bar[cdb->buffnum] );
		}

	gpu_time = gwidth << ((frame_update==1)?0:1);

}



void single_step_pause( void )
{

	while ( (joy[0].c & PAD_R1) == PAD_R1)
		{
		wait_vblank();
		read_pads();

		if ( (joy[0].r & PAD_L1) )
			break;
		}

}






BYTE *debug_menu_names[]=
{ 
"rdb box pos",
"rdb box size",
"textstring pos",
"abort game",
"abort frontend",
"camera angle",
"camera pos",
"object angle",
"object pos",
"play sfx",
"play audio track",
};



WORD item_num=256;
WORD item_select=0;
WORD item_timer=0;

void init_debug_menu()
{
	item_num=256;
	item_select=0;
	item_timer=0;
	debug_rdb=0;

	add_process ( scene_debug );
	add_process( debug_menu );		// debug menu

}

void debug_menu()
{
WORD r2,cnt;

	r2=joy[0].c & PAD_R2;

	if ( (r2 == 0) && ( item_select==0 ) )
		{
		item_timer=0;
		}


	if ( r2 && (item_select == 0) )
		item_select++;

	if ( item_select == 1 )
		{
		if ( r2 )
			{
			item_timer=20;

			item_num &=~256;

			//show items to select
			for ( cnt=0;cnt<ITEMS;cnt++ )
				{
				if ( item_num == cnt )
					FntPrint("* %s *\n",debug_menu_names[cnt] );
				else
					FntPrint("  %s\n",debug_menu_names[cnt] );
				}


			if ( joy[0].r & PAD_UP )
				{
				item_num--;
				if ( item_num < 0 )
					item_num=ITEMS-1;
				}

			if ( joy[0].r & PAD_DN )
				{
				item_num++;
				if ( item_num >= ITEMS )
					item_num=0;
				}
			item_num |= 256;
			}
		else
			{
			item_select=2;
			}
			
		}
	

	if ( item_select == 2 )
		{
		if ( r2 == 0 )
			{
			item_timer--;
			if ( item_timer<0)
				item_select=0;
			}
		else
			{
			item_select=3;
			}
		}

	if ( item_select == 3 )
		{
		item_num&=~256;

		if ( r2 )
			FntPrint("%s\n",debug_menu_names[item_num]);
		else
			{
			item_select=0;
			item_num|=256;
			}

		}


	//debug_crosshair();


}




void adjust_vector( VECTOR *sv, WORD amount )
{

	if ( joy[0].c & PAD_LT )
		sv->vx-=amount;
	if ( joy[0].c & PAD_RT )
		sv->vx+=amount;

	if ( joy[0].c & PAD_UP )
		sv->vy-=amount;
	if ( joy[0].c & PAD_DN )
		sv->vy+=amount;

	if ( joy[0].c & PAD_L1 )
		sv->vz-=amount;
	if ( joy[0].c & PAD_L2 )
		sv->vz+=amount;
}

void adjust_svector( SVECTOR *sv, WORD amount )
{

	if ( joy[0].c & PAD_LT )
		sv->vx-=amount;
	if ( joy[0].c & PAD_RT )
		sv->vx+=amount;

	if ( joy[0].c & PAD_UP )
		sv->vy-=amount;
	if ( joy[0].c & PAD_DN )
		sv->vy+=amount;

	if ( joy[0].c & PAD_L1 )
		sv->vz-=amount;
	if ( joy[0].c & PAD_L2 )
		sv->vz+=amount;
}

void fntprint_mat ( BYTE *name, MATRIX *mat )
{
	FntPrint("%s%d %d %d\n%d %d %d\n%d %d %d\n%d %d %d\n\n",name,mat->m[0][0],mat->m[0][1],mat->m[0][2],mat->m[1][0],mat->m[1][1],mat->m[1][2],mat->m[2][0],mat->m[2][1],mat->m[2][2],mat->t[0],mat->t[1],mat->t[2]);
}


LONG time[256];
WORD time_pos=0;

void start_vc()
{
	time[time_pos]=GsGetVcount();
	time_pos++;
}

LONG show_vc( BYTE *string )
{
LONG time2;
	time_pos--;
	time2=GsGetVcount()-time[time_pos];

	if ( *string )
		FntPrint("%s%d\n",string,time2 );

	return time2;

}


void aborts()
{
	if ( item_num == ABORT_GAME )
		abort_game = YES;

	if ( item_num == ABORT_FRONTEND )
		{
		abort_frontend = YES;
		abort_game = YES;
		}

}


void scene_debug()
{
	move_camera();
	debug_lights_on_off();
	pause_animation();
	adjust_scene();
	debug_adjust_rdb();
	debug_adjust_textstring();
	debug_sfx();
	aborts();
}

void move_camera()
{
Camera_data *cam=&camera.data[camera.current];


//adjust camera position 
	if ( item_num == MOVE_CAMERA_POS )
		{
		adjust_vector ( &cam->pos, 100 );
		FntPrint("%d %d %d\n",cam->pos.vx,cam->pos.vy,cam->pos.vz );
		}

//adjust camera position 
	if ( item_num == MOVE_CAMERA_ANGLE )
		{
		adjust_svector ( &cam->angle, 25 );

		FntPrint("%d %d %d\n",cam->angle.vx,cam->angle.vy,cam->angle.vz );

		}


#if 0
// Adjust distance to viewing screen
	if ( item_num == MOVE_SCREEN_Z )
		{
		if ( joy[4].c & PAD_UP )
			screen_z--;
		if ( joy[4].c & PAD_DN )
			screen_z++;

		if ( joy[0].c & (PAD_LT+PAD_RT) )
			{
			//camera.data[camera.current].pos.vz = screen_z*8;
			}

		SetGeomScreen(screen_z);	 
		FntPrint("%d\n",screen_z);
		}
#endif

}




void select_a_scenecam()
{
#if 0
MATRIX *mat;
Camera_data *cam=&camera.data[camera.current];
	
//	mat = &cam->scene_matrix;
	mat = &camera.matrix;

	if ( item_num == SELECT_A_SCENECAM )
		{
		if ( joy[0].db & PAD_LT )
			if ( camera.current )	camera.current--;
		if ( joy[0].db & PAD_RT )
			if ( camera.current != camera.num_cameras-1 )	camera.current++;

		if ( joy[0].db & PAD_SQU )
			{
			cam->anim_suspended=cam->anim_suspended?NO:YES;
			}


		FntPrint("Scenecam '%s' camera num %d\n",camera.data[camera.current].name, camera.current);
		FntPrint("[ to halt/resume anim\n");


		}

//	set_camera_matrix ( camera.current );
#endif

}





#if 0
void select_a_freecam()
{
	if ( item_num == SELECT_A_FREECAM )
		FntPrint("doesnt work... feck off\n");
#if 0
	if ( item_num == SELECT_A_FREECAM )
		{
		if ( joy[0].db & PAD_LT )
			if ( camera.current )	camera.current--;
		if ( joy[0].db & PAD_RT )
			if ( camera.current != MAX_SCENE_CAMERAS-1 )	camera.current++;

		FntPrint("Freecam %d\n",camera.current);
		}

#endif
}
#endif








void debug_lights_on_off()
{
#if 0

WORD cnt;
LIGHT_DATA *ld;	

//SVECTOR v0 = { 0,0,4096 };
//VECTOR v1;
MATRIX rotmat=M_IDENTITY;
BYTE *types[]={"None","Omni","Dir","Point"};

	if ( item_num == ADJUST_LIGHT )	
		{
		if ( joy[0].db & PAD_SQU )
			{
			selected_light--;
			if ( selected_light < 0 ) 
				selected_light = light.num_lights-1;
			}

		if ( joy[0].db & PAD_CIR )
			{
			selected_light++;
			if ( selected_light == light.num_lights )
				selected_light = 0;
			}

		ld = &light.light_data[selected_light];

			// change light type
		if ( joy[0].db & PAD_TRI )
			{
			ld->type++;
			if ( ld->type > POINT_LIGHT ) ld->type = 0;
			}



		FntPrint("light([/] '%s' is %s(^)\n",ld->name,ld->active?"On":"Off" );
		FntPrint("type=%s(\\)\n",types[ld->type]);
		if ( joy[0].db & PAD_X )
			ld->active = ld->active?NO:YES;	// toggle light on/off


		if ( ld->type == POINT_LIGHT )
			{
			if ( joy[0].c & PAD_LT )
				ld->scene_ang.vx -= 50;
			if ( joy[0].c & PAD_RT )
				ld->scene_ang.vx += 50;
			if ( joy[0].c & PAD_UP )
				ld->scene_ang.vy -= 50;
			if ( joy[0].c & PAD_DN )
				ld->scene_ang.vy += 50;
			if ( joy[0].c & PAD_L1 )
				ld->scene_ang.vz -= 50;
			if ( joy[0].c & PAD_L2 )
				ld->scene_ang.vz += 50;
			FntPrint("pos=%d %d %d\n",ld->scene_ang.vx,ld->scene_ang.vy,ld->scene_ang.vz);
			}

		if ( ld->type == DIRECTIONAL_LIGHT )
			{
			if ( joy[0].c & PAD_LT )
				ld->debug_ang.vx -= 50;
			if ( joy[0].c & PAD_RT )
				ld->debug_ang.vx += 50;
			if ( joy[0].c & PAD_UP )
				ld->debug_ang.vy -= 50;
			if ( joy[0].c & PAD_DN )
				ld->debug_ang.vy += 50;
			if ( joy[0].c & PAD_L1 )
				ld->debug_ang.vz -= 50;
			if ( joy[0].c & PAD_L2 )
				ld->debug_ang.vz += 50;

			FntPrint("ang=%d %d %d\n",ld->debug_ang.vx&4095,ld->debug_ang.vy&4095,ld->debug_ang.vz&4095);

//			RotMatrix ( &ld->debug_ang , &rotmat );

//			draw_unit ( &rotmat );


			}




		}

#endif
}






WORD step_cnt=0;

void pause_animation()
{
#if 0
	if ( item_num == PAUSE_ANIMATION )
		{
		FntPrint( "[ to single step\n");
		if ( joy[0].db & PAD_X )
			all_animation_paused = 1-all_animation_paused;


		if ( joy[0].db & PAD_SQU )
			{
			step_cnt=2;
			}

		FntPrint("%s\n",all_animation_paused?"yes":"no" );
			
		}

	if ( step_cnt )
		{
		all_animation_paused = YES;
		if ( step_cnt == 2 )
			all_animation_paused = NO;
		else
			all_animation_paused = YES;
										 
		step_cnt--;
		}
#endif
}


#if 0
void show_message( BYTE *string )
{
WORD cnt;

	cdb = (cdb==db[0])? db[1]: db[0]; 
	ClearOTagR( cdb->ot,  OT_SIZE);	/* clear ordering table */

//	swap_display_buffers();
	update_display();
	FntPrint( "\n\n\n%s\n",string );
	FntFlush(-1);

	cdb = (cdb==db[0])? db[1]: db[0]; 
	ClearOTagR( cdb->ot,  OT_SIZE);	/* clear ordering table */

//	swap_display_buffers();
	update_display();

//	for(cnt=0;cnt<30;cnt++)
//		wait_vblank();

}
#endif


WORD current_object=0; //debug select scene object
void adjust_scene()
{
SOD *sod;

#if 0
	if ( item_num == SELECT_OBJECT )
		{
		if ( joy[0].r & PAD_LT )
			if ( current_object != 0 )	current_object--;

		if ( joy[0].r & PAD_RT )
			if ( current_object < (sod_header->num_objects-1) )	current_object++;


		sod = sod_header->sod_list + current_object;
		FntPrint("object '%s'\n",sod->name );

		}


	if ( item_num == MOVE_SCENE_POS )
		{
		adjust_svector ( &sod_header->pos, 100 );
		FntPrint("%d %d %d\n",sod_header->pos.vx,sod_header->pos.vy,sod_header->pos.vz);
		}


	if ( item_num == MOVE_SCENE_ANG )
		{
		adjust_svector ( &sod_header->rot, 25 );
		FntPrint("%d %d %d\n",sod_header->rot.vx,sod_header->rot.vy,sod_header->rot.vz);
		}
#endif


#if 1

	if ( item_num == MOVE_OBJECT_ANGLE )
		{

		sod = sod_header->sod_list + current_object;
		

		FntPrint("'%s' %d %d %d\n",sod->name,sod->rot.vx,sod->rot.vy,sod->rot.vz );

		adjust_svector ( &sod->rot, 10 );
		}


//	if ( item_num == ADJUST_ENVROT_VEC )
//		{
//		FntPrint("%d %d %d\n",sod_header->envrot_vec.vx, sod_header->envrot_vec.vy, sod_header->envrot_vec.vz );
//		adjust_svector ( &sod_header->envrot_vec, 10 );
//
//		}

#endif
}





/*******************************************
 *														 *
 * Display error messgae to screen			 *
 *														 *
 *******************************************/

void error_loop();
BYTE *error_string;
void em()
{
	FntPrint("\n\n\n\nError\n\n%s\n",error_string );
}

void display_error( BYTE *error_string_addr )
{
	SetSp( 0x801ff000 );


	disable_display();
	init_game_loop();
	init_display( 512,256, 25,31,125, YES,YES,NO );	//screen xy res & bg color
	setup_system_font();

	add_process( em );

 	enable_display( 2 );
	error_string = error_string_addr;

	while(1)
		error_loop();
}


void error_loop()
{
	cdb = (cdb==db[0])? db[1]: db[0]; 
	ClearOTagR( cdb->ot,  OT_SIZE);	/* clear ordering table */

//	swap_display_buffers();
	execute_processes();
	wait_vblank();

	if ( enable_display_timer == 0 )
		{
		SetDispMask(1);	//switch screen on
		enable_display_timer = -1;
		}
	else
		{
		if ( enable_display_timer > 0 )
			enable_display_timer--;
		}

	update_display();
	display_system_font();		// display system font to screen
	pollhost();						// poll for debug
	read_pads();
}


LINE_F2 line_prims[2][50];
WORD current_line;
void init_lines()
{
	current_line=0;
}

void draw_line( WORD x0, WORD y0, WORD x1, WORD y1, UBYTE r, UBYTE g, UBYTE b )
{
LINE_F2 *poly = &line_prims[cdb->buffnum][current_line];

	SetLineF2( poly );
	SetSemiTrans( poly, 0 );
	poly->r0 = r;
	poly->g0 = g;
	poly->b0 = b;

	poly->x0 = (x0+(SCREEN_UNIT_WD/2)) >> 1;
	poly->y0 = (y0+(SCREEN_UNIT_HT/2)) >> 2;

	poly->x1 = (x1+(SCREEN_UNIT_WD/2)) >> 1;
	poly->y1 = (y1+(SCREEN_UNIT_HT/2)) >> 2;

	addPrim ( cdb->ot + MIDDLE_DEPTH-50, poly );


	current_line++;
}




RECT debug_rdb_rect={ 0,0, 60,60 };

void debug_adjust_rdb()
{
Rdb_gadget *rdb;

	if ( (item_num == ADJUST_RDB_POS) || (item_num == ADJUST_RDB_SIZE) )
		{
		if ( debug_rdb == 0 )
			{
			debug_rdb = set_rdb_gadget( 0, &debug_rdb_gadget, 0,0,25*4,25*4, -1 );
			debug_rdb->rdb_gadget->size = debug_rdb_rect;

			}
		}
	else
		{
		if ( debug_rdb )
			{
			debug_rdb_rect = debug_rdb->rdb_gadget->size;
			kill_rdb_gadget( debug_rdb );
			debug_rdb=0;
			return;
			}
		}

	if ( debug_rdb )
		{
		rdb = debug_rdb->rdb_gadget;

		if ( item_num == ADJUST_RDB_POS )
			{
			FntPrint("Adjust rdb pos\n");
			if ( cjoy->c & PAD_LT )
				rdb->size.x-=4;
			if ( cjoy->c & PAD_RT )
				rdb->size.x+=4;
			if ( cjoy->c & PAD_UP )
				rdb->size.y-=4;
			if ( cjoy->c & PAD_DN )
				rdb->size.y+=4;
			}

		if ( item_num == ADJUST_RDB_SIZE )
			{
			FntPrint("Adjust rdb size\n");

			if ( cjoy->c & PAD_LT )
				rdb->size.w-=4;
			if ( cjoy->c & PAD_RT )
				rdb->size.w+=4;
			if ( cjoy->c & PAD_UP )
				rdb->size.h-=4;
			if ( cjoy->c & PAD_DN )
				rdb->size.h+=4;
			}

		FntPrint("debug rdb box=%d %d %d %d\n",rdb->size.x, rdb->size.y,rdb->size.w, rdb->size.h  );
		}
}




WORD debug_tsx=0;
WORD debug_tsy=0;
//BYTE *debug_ts="Debug textstring";
BYTE *debug_ts="WWWWWWWWWWWWW";
WORD debug_ts_mode=0;

void debug_adjust_textstring()
{
WORD modes[]={ FONT_XC, FONT_NORMAL, FONT_RJUST };
BYTE *txts[]={ "xcentre","normal","rjust" };

	if ( item_num == ADJUST_TS_POS )
		{
		add_text_string ( tslot_poses[SHELL_TSLOT]+9 , debug_ts, MILFORD_FONT, milford_font_logic, debug_tsx, debug_tsy, modes[debug_ts_mode], 0, 0, 0, 0 );

		if ( cjoy->db & (PAD_TRI|PAD_CIR) )
			{
			debug_ts_mode++;
			debug_ts_mode %= 3;
			}


		if ( cjoy->c & PAD_LT )
			debug_tsx-=4;
		if ( cjoy->c & PAD_RT )
			debug_tsx+=4;
		if ( cjoy->c & PAD_UP )
			debug_tsy-=4;
		if ( cjoy->c & PAD_DN )
			debug_tsy+=4;

		FntPrint("\n\\,] To change justify (%s) xy=%d %d\n\n",txts[debug_ts_mode],debug_tsx, debug_tsy );
		}


}




void debug_sfx()
{
static WORD sfx=0;
static WORD track=0;

	if ( item_num == PLAY_SFX )
		{
		if ( cjoy->r & PAD_LT )
			{
			if ( sfx != 0 )
				sfx--;
			}
		if ( cjoy->r & PAD_RT )
			{
			sfx++;
			}


		if ( cjoy->db & (PAD_X|PAD_TRI|PAD_CIR|PAD_SQU|PAD_R1|PAD_R2|PAD_L1|PAD_L2) )
			{
			sfxSpot( sfx, 30 );
			}

		FntPrint("X to play SFX (%d) , lt,rt to select\n\n",sfx );
		}

	if ( item_num == PLAY_XA )
		{
		if ( cjoy->r & PAD_LT )
			{
			if ( track != 0 )
				track--;
			}
		if ( cjoy->r & PAD_RT )
			{
			track++;
			}


		if ( cjoy->db & (PAD_X|PAD_TRI|PAD_CIR|PAD_SQU|PAD_R1|PAD_R2|PAD_L1|PAD_L2) )
			{
			play_xa_audio_track( track );
			}

		FntPrint("],\\,],^ to play track (%d) , lt,rt to select\n\n",track );
		}



}



WORD dbx=0;
WORD dby=0;
WORD dx, dy, ax, ay;
WORD dx, dy;

void debug_crosshair()
{
WORD tint;

	ax = joy[0].alx;
	ay = joy[0].aly;
	dx = dy = 0;

	if ( ax <= 110 )
		{
		dx = (ax-110)/4;
		}
	else
		{
		if ( ax >= 180 )
			dx = (ax-180)/4;
		}

	if ( ay < 110 )
		{
		dy = (ay-110)/4;
		}
	else
		{
		if ( ay > 180 )
			dy = (ay-180)/4;
		}



	if ( joy[0].r & PAD_LT ) dx=-2;
	if ( joy[0].r & PAD_RT ) dx=2;
	if ( joy[0].r & PAD_UP ) dy=-4;
	if ( joy[0].r & PAD_DN ) dy=4;


	dbx += dx;
	dby += dy;
	if ( dbx < -511 ) dbx = -511;
	if ( dby < -511 ) dby = -511;
	if ( dbx > 511 ) dbx = 511;
	if ( dby > 511 ) dby = 511;





	FntPrint("\n\nCrosshair xy=%d %d\n\n",dbx+512, dby+512);
	FntPrint("dx=%d dy=%d\n",dx, dy );

	tint = ((timer/8)&1)?0:255;

	draw_line( dbx - 30 , dby, dbx - 2 , dby, tint, tint, tint );
	draw_line( dbx + 30 , dby, dbx + 2 , dby, tint, tint, tint  );

	draw_line( dbx, dby - 30, dbx, dby-4, tint, tint, tint  );
	draw_line( dbx, dby + 30, dbx, dby+4, tint, tint, tint  );

}


void show_border_area()
{
WORD xb = 16;	// x border size in pixels
WORD yb = 12;	// y border size in pixels

WORD lx,ty,rx,by;



	if ( print_debug_info == 0 )
		{
		lx = -512 + (xb*2);
		ty = -512+(yb*4);
		rx = -512 + 1024-(xb*2);
		by = -512 + 1024-(yb*4);
		draw_line( lx, ty, rx,ty, 128,128,128 );
		draw_line( lx, by, rx,by, 128,128,128 );
		draw_line( lx, ty, lx,by, 128,128,128 );
		draw_line( rx, ty, rx,by, 128,128,128 );
		}

}


void debug_choose_winner()
{
WORD abort=NO;
WORD pts1=share->team1_pts;
WORD pts2=share->team2_pts;
WORD pts_hi,pts_lo;
BYTE *stats[]={ "Game init", "Game in progress", "Game over", "Round over", "Quit game", "Quit competition" };
BYTE *txt;
WORD cnt;
Rdb_gadget bug_rdb;


	if ( pts1 > pts2 )
		{
		pts_hi = pts1;
		pts_lo = pts2;
		}
	else
		{
		pts_hi = pts2;
		pts_lo = pts1;
		}

	if ( share->team1_cpu )
		{
		pts1 = pts_lo;
		pts2 = pts_hi;
		}
	else
		{
		pts1 = pts_hi;
		pts2 = pts_lo;
		}

	


	set_rdb_gadget( 0, &bug_rdb, -340,-284,680,552, 0 );

	shell->current_item=0;
	while(abort==NO)
		{
		cnt=0;
		txt="Choose Winning Team\n\nLT and RT to adjust points\nor game stat\nX......Exit\n\n";
		add_text_string ( tslot_poses[MAIN_TSLOT]+cnt , txt, MILFORD_FONT, milford_font_logic, 0, -280+(60*cnt), FONT_XC, -2, 0, 0,0 );
		cnt+=5;


		sprintf( text_buffer, "%s(%s) %d pts\n", get_team_name( share->team1), share->team1_cpu?"cpu":"human",pts1 );
		add_text_string ( tslot_poses[MAIN_TSLOT]+cnt , text_buffer, MILFORD_FONT, milford_font_logic, 0, -280+(60*cnt), FONT_XC, 0, 0, 0,0 );
		cnt++;

		sprintf( text_buffer," %s(%s) %d pts\n", get_team_name( share->team2), share->team2_cpu?"cpu":"human", pts2 );
		add_text_string ( tslot_poses[MAIN_TSLOT]+cnt , text_buffer, MILFORD_FONT, milford_font_logic, 0, -280+(60*cnt), FONT_XC, 1, 0, 0,0 );
		cnt++;

		sprintf( text_buffer, "Game stat = '%s'",stats[ share->game_stat ]);
		add_text_string ( tslot_poses[MAIN_TSLOT]+cnt , text_buffer, MILFORD_FONT, milford_font_logic, 0, -280+(60*cnt), FONT_XC, 2, 0, 0,0 );
		cnt++;

		shell->master_option_active=NO;

		if ( cjoy->db & PAD_UP )
			{
			shell->current_item--;
			if ( shell->current_item < 0 ) shell->current_item=2;
			}

		if ( cjoy->db & PAD_DN )
			{
			shell->current_item++;
			if ( shell->current_item == 3 ) shell->current_item=0;
			}


		if ( cjoy->db & PAD_LT )
			{
			if ( (shell->current_item==0) && ( pts1>0) )
				pts1--;
			if ( (shell->current_item==1) && ( pts2>0) )
				pts2--;
			if ( (shell->current_item==2) )
				{
				share->game_stat--;
				if ( share->game_stat<0 )
					share->game_stat = TOTAL_GAME_STATS-1;
				}

			}

		if ( cjoy->db & PAD_RT )
			{
			if ( shell->current_item==0 )
				pts1++;
			if ( shell->current_item==1 )
				pts2++;
			if ( shell->current_item==2 )
				{
				share->game_stat++;
				if ( share->game_stat == TOTAL_GAME_STATS )
					share->game_stat = 0;
				}

			}



		if ( cjoy->db & PAD_X )
			abort=YES;


		//FntPrint("\n\n\n");
		bg_loop();
		}

	kill_rdb_gadget( bug_rdb.gadget );
	share->team1_pts = pts1;
	share->team2_pts = pts2;

}

void show_message( BYTE *string )
{
#if DEBUG
WORD cnt;
DB dbs[2];

	cdb = (cdb==&dbs[0])? &dbs[1]: &dbs[0]; 
//	ClearOTagR( cdb->ot,  OT_SIZE);	/* clear ordering table */

//	swap_display_buffers();
	update_display();
	FntPrint( "\n\n\n     %s\n",string );
	FntFlush(-1);

	cdb = (cdb==&dbs[0])? &dbs[1]: &dbs[0]; 
//	ClearOTagR( cdb->ot,  OT_SIZE);	/* clear ordering table */

//	swap_display_buffers();
	update_display();

	while(1)
		{
		read_pads();
		if ( joy[0].db )
			break;
		}
#endif
}


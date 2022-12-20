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
#include "cdaudio.h"
#include "cmplogic.h"
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

#define DEBUG_SQU ]
#define DEBUG_TRI \
#define DEBUG_CIR [
#define DEBUG_X 	^

void setup_system_font()
{
#if DEBUG == YES

#define FONT_VRAM_X 512+(5*64)
#define FONT_VRAM_Y 1*256 // 0 or 1

RECT font_rect={FONT_VRAM_X,FONT_VRAM_Y,128/4,32};
RECT pal_rect={FONT_VRAM_X,FONT_VRAM_Y+128,32,1};


// load normal system font
	FntLoad( FONT_VRAM_X, FONT_VRAM_Y );
	FntOpen( 16, NTSC?9:6, screen_xres-16, screen_yres, 0, 1600);

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


	sprintf ( &text_str[0], "cpu %d.%d       ",cpu_time/10,cpu_time%10 );
	sprintf ( &text_str[10], "/%d.%d     \n ",peak_cpu_time/10,peak_cpu_time%10 );

	sprintf ( &text_str[18], "gpu %d.%d       ",gpu_time/10,gpu_time%10 );
	sprintf ( &text_str[28], "/%d.%d     \n",peak_gpu_time/10,peak_gpu_time%10 );

	FntPrint( text_str );

//	FntPrint(  "MAX ALLOC=%d MAX_STACK=%d\n",largest_alloc_size, largest_stack_size );


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




void scene_debug()
{
	debug_adjust_rdb();
	debug_adjust_textstring();
	debug_sfx();
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
	init_memory_alloc();
	init_frontend();
//	init_shell();

//	disable_display();
//	init_game_loop();
//	init_display( 512,256, 25,31,125, YES,YES,NO );	//screen xy res & bg color
//	setup_system_font();

	add_process( em );

// 	enable_display( 2 );
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
		add_text_string ( tslot_poses[SHELL_TSLOT]+9 , debug_ts, TINY_FONT, milford_font_logic, debug_tsx, debug_tsy, modes[debug_ts_mode], 0, 0, 0, 0,0 );

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
			track++;

		if ( cjoy->db & PAD_TRI )
			bob_ff = -1;

		if ( cjoy->db & PAD_CIR )
			bob_ff = 1;



		if ( cjoy->db & PAD_SQU )
			{
			play_xa_audio_track( track );
			}

		FntPrint("[ to play track (%d)\n\\ to rew\n] to ff\nLt,Rt to select track (pos=%d)\n\n",track, bob_audio_pos );
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
WORD wd,ht;

	if ( NTSC == YES )
		ht = 240;
	else
		ht = 256;

	wd = 512;


	lx = -512 + (xb*2);
	ty = -512+(yb*4);
	rx = -512 + ((wd-xb)*2);
	by = -512 + ((ht-yb)*4);
	draw_line( lx, ty, rx,ty, 128,128,128 );
	draw_line( lx, by, rx,by, 128,128,128 );
	draw_line( lx, ty, lx,by, 128,128,128 );
	draw_line( rx, ty, rx,by, 128,128,128 );

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
Database_team *db_teams = (Database_team *)&ability_file_addr; //[MAX_NON_USER_TEAMS+MAX_CLUB_TEAMS];

WORD team_num, team, goals, shirt_num, index;
WORD t1_red_award=-1;
WORD t2_red_award=-1;
WORD pos;
	team=0;


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


	if ( (share->team1_cpu == NO) )
		{
		pts1 = pts_hi;
		pts2 = pts_lo;
		}
	else
		{
		pts1 = pts_lo;
		pts2 = pts_hi;
		}


	set_rdb_gadget( 0, &bug_rdb, -340,-284,680,552, 0 );

	shell->current_item=0;

	while(abort==NO)
		{

		if ( cjoy->db & PAD_TRI )
			team=1-team;
		if ( team == 0 )
			team_num=share->team1;
		else
			team_num=share->team2;




		FntPrint("team = %s  press TRI\n",get_team_name( team_num ) );

		for(cnt=0;cnt<22;cnt++)
			{
			shirt_num=db_teams[ team_num ].player[ cnt ].shirt_num;
			goals = share->goals_scored[ team_num ][ cnt ];

		//	FntPrint("%d %s(shirt=%d)\n",goals,db_teams[ team_num ].player[ cnt ].name, db_teams[ team_num ].player[ cnt ].shirt_num   );
			}

		for(cnt=0;cnt<3;cnt++)
			{
			index=top_team_scorers[team_num][cnt];
		  //	FntPrint("%d %s\n",index, db_teams[ team_num ].player[index].name );
			}



		cnt=0;
		txt="Choose Winning Team\n\nLT and RT to adjust points\nor game stat\nX......Exit\n\n";
		add_text_string ( tslot_poses[MAIN_TSLOT]+cnt , txt, MILFORD_FONT, milford_font_logic, 0, -280+(60*cnt), FONT_XC, -2, 0, 0,0,0 );
		cnt+=5;


		sprintf( text_buffer, "%s(%s) %d pts\n", get_team_name( share->team1), share->team1_cpu?"cpu":"human",pts1 );
		add_text_string ( tslot_poses[MAIN_TSLOT]+cnt , text_buffer, MILFORD_FONT, milford_font_logic, 0, -280+(60*cnt), FONT_XC, 0, 0, 0,0,0 );
		cnt++;

		sprintf( text_buffer," %s(%s) %d pts\n", get_team_name( share->team2), share->team2_cpu?"cpu":"human", pts2 );
		add_text_string ( tslot_poses[MAIN_TSLOT]+cnt , text_buffer, MILFORD_FONT, milford_font_logic, 0, -280+(60*cnt), FONT_XC, 1, 0, 0,0,0 );
		cnt++;

		sprintf( text_buffer, "Game stat = '%s'",stats[ share->game_stat ]);
		add_text_string ( tslot_poses[MAIN_TSLOT]+cnt , text_buffer, MILFORD_FONT, milford_font_logic, 0, -280+(60*cnt), FONT_XC, 2, 0, 0,0,0 );
		cnt++;

		sprintf( text_buffer, "Award red card to %s's shirt number %d",get_team_name( share->team1), t1_red_award );
		add_text_string ( tslot_poses[MAIN_TSLOT]+cnt , text_buffer, MILFORD_FONT, milford_font_logic, 0, -280+(60*cnt), FONT_XC, 3, 0, 0,0,0 );
		cnt++;

		sprintf( text_buffer, "Award red card to %s's shirt number %d",get_team_name( share->team2), t2_red_award );
		add_text_string ( tslot_poses[MAIN_TSLOT]+cnt , text_buffer, MILFORD_FONT, milford_font_logic, 0, -280+(60*cnt), FONT_XC, 4, 0, 0,0,0 );
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
			if ( shell->current_item == 5 ) shell->current_item=0;
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


		if ( shell->current_item == 3 )
			{
			if ( (cjoy->r & PAD_LT) && (t1_red_award != -1) )
				t1_red_award--;
			if ( (cjoy->r & PAD_RT) && (t1_red_award != 21) )
				t1_red_award++;
			}

		if ( shell->current_item == 4 )
			{
			if ( (cjoy->r & PAD_LT) && (t2_red_award != -1) )
				t2_red_award--;
			if ( (cjoy->r & PAD_RT) && (t2_red_award != 21) )
				t2_red_award++;

			}



		if ( cjoy->db & PAD_X )
			abort=YES;


		//FntPrint("\n\n\n");
		bg_loop();
		}

	if ( share->team1_cpu==NO )
		{
		if ( t1_red_award != -1 )
			{
			pos = set_human_team_pos( share->team1 );
			share->red_cards_awarded[ pos ][ t1_red_award ] = 1;
			}
		}
	if ( share->team2_cpu==NO )
		{
		if ( t2_red_award != -1 )
			{
			pos = set_human_team_pos( share->team2 );
			share->red_cards_awarded[ pos ][ t2_red_award ] = 1;
			}
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



void debug_print_mess(BYTE *mess, WORD time )
{
#define MESSWD 512
#define MESSHT 256

DB test_db;

	ResetGraph(3);						/* reset graphic subsystem (0:cold,1:warm) */
//	SetGraphDebug(0);					/* set debug mode (0:off, 1:monitor, 2:dump) */

	FntLoad( 768,0 );
	FntOpen( 16, 0, MESSWD,MESSHT, 0, 1800);

	SetVideoMode(NTSC?MODE_NTSC:MODE_PAL);

	SetDispMask(1); /* screen on */

	SetDefDrawEnv( &test_db.draw, 0, 0,   MESSWD,MESSHT );
	SetDefDispEnv( &test_db.disp, 0, 0,   MESSWD,MESSHT );

// hardware screen xy offs and wd/ht...
	test_db.disp.screen.x = 10;
	test_db.disp.screen.y = 20;
	test_db.disp.screen.h = MESSHT;  
	test_db.disp.screen.w = 256;  
	test_db.disp.isrgb24  = 0;	
	test_db.draw.isbg = 1;
	test_db.draw.dtd = test_db.draw.dfe = 0;
	setRGB0 ( &test_db.draw, 40,40,200 );
	

	while(1)
		{
		FntPrint( mess );
		PutDrawEnv(&test_db.draw);
		PutDispEnv(&test_db.disp);
		FntFlush(-1);
		VSync(0);

		if ( time != 0 )
			{
			time--;
			if ( time == 0 ) break;
			}
		
		#if REAL_PLAYSTATION == NO
			pollhost(); 
		#endif
		
		}

	printf("Out of debug print mess\n");

}




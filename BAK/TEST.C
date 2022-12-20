/****************************************************
 *																	 *
 *	 Test functions											 *
 *																	 *
 ****************************************************/

#if 0

#include	"incs.h"


/****************************************************
 *																	 *
 *	 Prototypes													 *
 *																	 *
 ****************************************************/

//void test_scene_update( Scene_obj *scene_obj );
void init_test_scene1();
void init_test_scene2();
void draw_scene_test();
void laser_test();


/****************************************************
 *																	 *
 *  Vars															 *
 *																	 *
 ****************************************************/


Scene_startup test_scene={

	TEST_SDT,		// scene s01 data
	TEST_TEX,		// scene texture data
	TEST_MDT,		// scene matrix animation list
	-1, //TEST_LDT,		// scene light animation
	0,0,0,			// environment map matrix multiplied by this vector
};

WORD red_laser_list[]={ 3,1,2, -1 };
WORD white_laser_list[]={ 6,5,4, -1 };
WORD blue_laser_list[]={ 0,7, -1 };


Laser_data blue_laser_data={
	
	5,						// max laser polys per object
	L1_FRUIT,			// laser name
	10,10,40,			// laser color at base
	1,						// transparency rate
	blue_laser_list,	// objects to draw to
	1,						// set to 1
	80,					// total polys for this laser
	40,800,0				// laser base pos           
};

Laser_data white_laser_data={
	
	5,						// max laser polys per object
	L2_FRUIT,			// laser name
	30,30,30,			// laser color at base
	1,						// transparency rate
	white_laser_list, // objects to draw to
	1,						// set to 1
	80,					// total polys for this laser
	20,800,0				// laser base pos           
};

	
Laser_data red_laser_data={
	
	5,						// max laser polys per object
	L3_FRUIT,			// laser name
	170,10,10,			// laser color at base
	1,						// transparency rate
	red_laser_list,	// objects to draw to
	1,						// set to 1
	80,					// total polys for this laser
	0,800,0				// laser base pos           
};




WORD nasher_laser[]={
	rl_Na_Shad,
	rl_Na_Bod,
	rl_Na_T02,
	rl_Na_T03,
	rl_Na_T04,
	rl_Na_T01,
	rl_Na_B02,
	rl_Na_B03,
	rl_Na_B04,
	rl_Na_B01,
	-1
};



WORD harry_laser[]={
	rl_Ha_Shad,
	rl_Ha_Bod,
	rl_Ha_RHan,
	rl_Ha_LHan,
	rl_Ha_LEye,
	rl_Ha_LBro,
	rl_Ha_REye,
	rl_Ha_REye,
	rl_Ha_RBro,
	rl_Ha_Sos,
	rl_Ha_LLeg,
	rl_Ha_LFot,
	rl_Ha_LSol,
	rl_Ha_RLeg,
	rl_Ha_RFot,
	rl_Ha_RSol,
	-1
};


Laser_data harry_laser_data={
	
	2,					// max laser polys per object
	L1_HOT,			// laser name
	10,40,10,		// laser color at base
	1,					// transparency rate
	harry_laser,	// objects to draw to
	1,					// set to 1
	80,				// total polys for this laser
	200,800,0		// laser base pos           
};

Laser_data nasher_laser_data={
	
	2,					// max laser polys per object
	L2_HOT,			// laser name
	30,15,15,		// laser color at base
	1,					// transparency rate
	nasher_laser, 	// objects to draw to
	1,					// set to 1
	80,				// total polys for this laser
	200,600,0		// laser base pos           
};



//SOD_header sod_header_area;
//SOD_header *sod_header = &sod_header_area;



void run_test()
{

WORD abort_seq = NO;



	while(1)
		{
		push(0);
		abort_seq = NO;
		init_test_scene1();


		while ( abort_seq == NO )
			{
			bg_loop();

			if ( joy[0].c & PAD_STA ) abort_seq = YES;
			}
		pop(0);

		if ( share->stub_code == STUB_CODE )
			{
			break;
			}

#if 0
		push(0);
		init_test_scene2();

		abort_seq=NO;
		while ( abort_seq == NO )
			{
			bg_loop();

			if ( joy[0].c & PAD_STA ) abort_seq = YES;
			}
		pop(0);
#endif


		}

		

}

Sprite *xx;


WORD anims[]={ 3,3,3,4,4,4,5,5,5,5,4,4,4,3,3,3, -1 };

void poo( Sprite *sprite )
{

	if ( sprite->timer1 == sprite->timer2 )
		{
		if ( anims[ sprite->temp ] != -1 )
			{
			sprite->frame = anims[ sprite->temp ];
			sprite->temp++;
			}
		else
			{
			sprite->temp = make_random_number ( 15 );
			}
		}
	else
		sprite->timer1++;
		
}

void init_test_scene1()
{
WORD cnt;
WORD x,y;


	disable_display();
	init_game_loop();

	show_loadpic();
	enable_display( 0 );

	init_display( DEFAULT_XRES, DEFAULT_YRES, 20,20,40, NO );	//screen xy res & bg color


//	sfxScriptOn( (WORD *)&sfxFlyingPie );

	init_scene_draw( &test_scene );

//	init_sprite_loader();

	add_process( debug_menu );		// debug menu
	add_process( draw_scene_test );
					
//	load_m01_file ( DISK1_MATS, 3 );	// load this m01 anim to m01 slot 3
//	load_m01_file ( DISK2_MATS, 4 );	// load this m01 anim to m01 slot 3


	test_scene.model_list = 0;	// list of objects to display ( 0= all )
  	start_scene( &test_scene );

	init_all_lasers();

//	add_laser ( &blue_laser_data,0  );
//	add_laser ( &white_laser_data, 1 );
//	add_laser ( &red_laser_data, 2 );


//	add_laser ( &harry_laser_data,0  );
//	add_laser ( &nasher_laser_data, 1 );

	add_process( render_scene );
//	add_process( draw_lasers );


#if 0

//*** Test sprite bits ***
	add_process( draw_sprites );
	load_san_file ( TEST_SAN_FILE, 256 );
	cnt=0;
	for ( y=0;y<11;y++)
	for ( x=0;x<22;x++)
		{
		spawn_sprite ( 0 );
		sprite->frame = 3;
		sprite->x.w.hi = (x*11) + 16;
		sprite->y.w.hi = (y*9) + 16;
		sprite->upd_rtn = (void *)poo;
		sprite->timer2 = (cnt) + 5;
		cnt++;
		}
#endif


//	sod_header->sod_list[0].active=0;

}







void draw_scene_test()
{
WORD cnt;
SOD *sod;


	move_camera();
	debug_lights_on_off();


	if ( share-> animation_reset_flag )
		{
		//sfxScriptOn( (WORD *)&sfxFlyingPie );
		share->animation_reset_flag =0;
		}

//	pause_animation();

//	start_scene_anim ( 1 );	//play anim from slot 0
//	start_camera_mdt_anim ( 0 );

//	if ( joy[0].db & PAD_X )
// 		start_scene_anim ( 1 );	//play anim from slot 1

//	if ( joy[0].db & PAD_TRI )
// 		stop_scene_anim ( 1 );	//play anim from slot 1



//	sod_header->sod_list[ rlE_x ].rot.vy += 20;
//	sod_header->sod_list[pl_zogg].rot.vy += 20;

	select_a_scenecam();





// Spin the central cylinder
//	(sod_header->sod_list + rlE_008 ) ->rot.vy += 15;
//	(sod_header->sod_list + rl_009 ) ->rot.vy += 15;


// Spin the background disk
//	(sod_header->sod_list + rl_001 ) ->rot.vz += 2;
//	(sod_header->sod_list + rl_002 ) ->rot.vz += 2;






}



#endif

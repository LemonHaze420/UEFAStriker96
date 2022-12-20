/*****************************************************************************
 *																									  *
 *																									  *
 *   VRAM peeker																				  *
 *																									  *
 *																									  *
 *****************************************************************************/

//*** This file is included by utils.c *****


#define	PEEK_X_MAX			1034

#if NTSC == YES
 #define	PEEK_Y_MAX			534
#else
 #define	PEEK_Y_MAX			514
#endif

#define	PEEK_X_MIN			-24
#define	PEEK_Y_MIN			-12

void peek( void )
{
WORD xres_list[8]={256,320,384,512,640,0,0,0};
WORD x,y,tlx,tly,blx,bly;
WORD xres,yres,xres_cnt;
DB db;
WORD frame_update_store = frame_update;




	xres = screen_xres;
	yres = screen_yres;

	if ( (joy[0].c & (PAD_L1 | PAD_L2)) != (PAD_L1 | PAD_L2)  )
		return;

	frame_update = 1;

	read_pads();
	x = y = xres_cnt = 0;

	init_prim(&db, 0, NO );	/* initialize primitive buffer #0 */

	while ( !(joy[0].db & (PAD_L1|PAD_L2)) )
		{
		if( joy[0].c & PAD_RT )	x += 16;
		if( joy[0].c & PAD_LT )	x -= 16;
		if( joy[0].c & PAD_DN )	y += 16;
		if( joy[0].c & PAD_UP )	y -= 16;

		if ( joy[0].db & PAD_R1 )
			{
			if ( xres_list[xres_cnt] == 0 )	xres_cnt = 0;

			xres = xres_list[xres_cnt];
			xres_cnt++;
			}

		if ( joy[0].db & PAD_R2 )
			{
			yres += 256;
			if ( yres > 512 ) yres = 256;
			}


		tlx = PEEK_X_MIN;
		tly = PEEK_Y_MIN;
		blx = PEEK_X_MAX - ( xres );
		bly = PEEK_Y_MAX - ( yres );

		if( x <= tlx )	x = tlx;
		if( y <= tly )	y = tly;
		if( x >= blx )	x = blx;
		if( y >= bly )	y = bly;

		DrawSync(0);	// hang cpu until GPU has finished drawing
		wait_vblank();
//		VSync(0);		

		SetDefDispEnv(&db.disp, x, y, xres, yres );

		// hardware screen xy offs and wd/ht...
		db.disp.screen.x = 0;  
		db.disp.screen.y = SCREEN_Y;
		db.disp.screen.h = 256;	//num lines visible
		db.disp.screen.w = 256; //256= full width 128=half

		PutDispEnv(&db.disp); // update display enviroment 
		read_pads();
		pollhost();		
	  	}
 
	read_pads();


	frame_update = frame_update_store;

}



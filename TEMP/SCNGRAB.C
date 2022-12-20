
/****************************************************
 *																	 *
 *     Vram screen grabber									 *
 *																	 *
 * Converts screen to 24bit TIM format					 *
 * press L1 and R2 to grab									 *
 *																	 *
 * Outputs to c:\temp\scngrabx.tim						 *
 *																	 *
 ****************************************************/

typedef struct
{
	LONG tim_id;	//always 0x00000016
	LONG bpp;		//bits 0-2 = bpp, bit3=1=file uses a clut
	LONG file_size;
	LONG pad2; //could be clut pointer
	WORD wd;
	WORD ht;
}TIM;

//*** This file is included by utils.c *****

static WORD grabnum = 0;

WORD	grab_frame_num = 0;

void save_screen_grab( WORD screen_xres, WORD screen_yres, WORD bpp )
{
RECT rect;

WORD fh,i,c,r,g,b,x;
UWORD screenline[1024];
UBYTE outbuff[1024*3];
BYTE gfname[32];
TIM tim_header;
  
	if ( (joy[0].c & (PAD_L2|PAD_R2|PAD_CIR)) != (PAD_L2|PAD_R2|PAD_CIR) )
		return;

	printf("doing grab\n");

	sprintf(gfname,"d:\\share\\scngrab%d.tim",grab_frame_num);
	grab_frame_num++;
   fh = PCcreat(gfname,0);
   if (fh<0)
		{
      printf("couldn't create file %s!\n",gfname);
		FntPrint( "cant create grab file\n" );
		FntFlush(-1);
      return;
      }
	PCclose(fh);
   fh = PCopen(gfname,1,0);
   if (fh<0)
      {
      printf("couldn't open file %s!\n",gfname);
		FntPrint( "cant open grab file\n" );
		FntFlush(-1);
        
		return;
      }


//write TIM header
	tim_header.tim_id = 16;
	tim_header.bpp = bpp;
	tim_header.file_size = (screen_xres*screen_yres*bpp)+12;
	tim_header.pad2 = 0;
	tim_header.wd = (screen_xres/2)*bpp;	// width in bytes
	tim_header.ht = screen_yres;
  	PCwrite(fh,(BYTE *)&tim_header,20);

//write body
	for (i=0; i<screen_yres; i++)
		{
		setRECT(&rect,0,i,screen_xres,1);

		StoreImage(&rect,(u_long *)screenline);

		c = screen_xres;
		x=0;

		//convert line of 16 bit words to 24bit bpp
		//word %0bbbgggrrr to UBYTE r,g,b
		while( c--)
			{
			r = ( UBYTE ) ( (screenline[x/bpp] >> 0) & 0x1f );
			g = ( UBYTE ) ( (screenline[x/bpp] >> 5) & 0x1f );
			b = ( UBYTE ) ( (screenline[x/bpp] >> 10) & 0x1f );

			outbuff[x+0] = r*8; 
			outbuff[x+1] = g*8; 
			outbuff[x+2] = b*8; 
			x+=bpp;

			}

		//write a line to disk
		PCwrite(fh,outbuff,screen_xres*bpp);

		}

	FntPrint( "done..." );
	FntFlush(-1);

	PCclose(fh);
}



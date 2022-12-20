/****************************************************
 *																	 *
 *	 																 *
 * Team and player edit										 *
 *	 																 *
 *																	 *
 ****************************************************/


#include "incs.h"

#ifdef PSX
#include "\global\s01.h"
#include	"sprite.h"
#include	"frontend.h"
#include "gadget.h"
#include "shell.h"
#include	"pad.h"
#include "anim.def"
#include "font.h"
#include "hardware.h"
#include "render.h"
#include "datafile.h"
#include "skindraw.h"
#include "sod.h"
#include	"scene.h"
#include "anim.h"
#include "..\..\striker\frontend\mod.def"
#include "text.h"
#include "entname.h"
#endif

void show_selected_swatch();
void draw_skin_player();

LONG sfx_handle;

BYTE pl_ed_text_buffer[64];

UWORD pl_ed_shell_buttons[]={ SHELL_TE_ED,  SHELL_QUIT,0};
UWORD te_ed_shell_buttons[]={ SHELL_PL_ED,  SHELL_QUIT,0};

WORD te_old_ci;
BYTE *pl_ed_number_names[]= { 
	"Number",
	"gNumber",
	"fNumber",
	"sNumber",
	"iNumber",
	"dNumber",
};

BYTE *pl_ed_player_words[2][6]={
	"Player",
	"gPlayer",
	"fPlayer",
	"sPlayer",
	"iPlayer",
	"dPlayer",

	"Goalkeeper",
	"gPlayer",
	"fPlayer",
	"sPlayer",
	"iPlayer",
	"dPlayer",

};



BYTE *pl_ed_button_names[][9]= { 
	"Change name\n",
	"Face type\n",
	"Facial hair\n",
	"Skin color\n",
	"Hair style\n",
  	"Hair color\n",
	"Boot color\n",
	pl_ed_text_buffer,
	0,
};


BYTE *team_words[]={
	"team",
	"fteam",
	"gteam",
	"iteam",
	"steam",
	"dteam",
};





/****************************************************
 *																	 *
 *	 Defines														 *
 *																	 *
 ****************************************************/


#define PLAYER_EDITOR 0
#define TEAM_EDITOR 1

#define NORMAL_PLAYER 0
#define GOALKEEPER	 1	// MUST BE = 1 !!!!!!!!!

#define MAX_PL_FACE_TSIZE	200000	// player edit face/hair/facehair textures
#define MAX_TE_STRIP_TSIZE		125000	// team strip textures
#define MAX_TE_PL_NUMS_TSIZE	50000	// num textures for back of shirt

#define TE_YM (8*4)

#define FACES_PER_SKIN 6


#define FACE_TSIZE (32*64)			// size of face texture
#define HAIR_TSIZE (32*64)			// size of head hair texture
#define FACE_HAIR_TSIZE (32*64)	// size of head hair texture
#define SHIRT_NUM_TSIZE (32*64)	// size of shirt digit texture
#define HEAD_HAIRS 12
#define FACE_HAIRS 10

#define FACE_TSTART 0
#define HAIR_TSTART 			(FACE_TSTART + (FACE_TSIZE*6*FACES_PER_SKIN))
#define FACE_HAIR_TSTART 	(HAIR_TSTART + (HAIR_TSIZE*HEAD_HAIRS))
#define THIGH_TSTART 		(FACE_HAIR_TSTART + (FACE_HAIR_TSIZE*10))

#define TE_TOTAL_SHIRTS 16	// including goalkeeper shirts
#define TE_SHIRTS 12
#define TE_SHORTS 5
#define TE_SOCKS  5
#define TE_SLEEVES 5
#define SHIRT_FSIZE (52*64)
#define SHIRT_BSIZE (32*64)
#define SLEEVE_SIZE (32*32)
#define SHORT_SIZE  (48*64)
#define SOCK_SIZE   (32*32)
#define SHIRT_FSTART 0
#define SHIRT_BSTART ( TE_TOTAL_SHIRTS * SHIRT_FSIZE)
#define ARM_TEX_SIZE ( 44*64 )
#define GOALIE_SHIRT_FSTART (SHIRT_FSTART + (SHIRT_FSIZE*TE_SHIRTS ))
#define GOALIE_SHIRT_BSTART (SHIRT_BSTART + (SHIRT_BSIZE*TE_SHIRTS ))

#define SLEEVE_START SHIRT_BSTART + ( TE_TOTAL_SHIRTS * SHIRT_BSIZE)
#define SHORT_START SLEEVE_START + ( TE_SLEEVES * SLEEVE_SIZE)
#define SOCK_START SHORT_START + ( TE_SHORTS * SHORT_SIZE)
#define GOALIE_SLEEVE_START SOCK_START + ( TE_SOCKS * SOCK_SIZE )


//** textures for player from 'PLAYER_FACE_TEXTURES' file


#define Face_pals (0*128)	// used on face only
#define Hair_pals (6*128)
#define Skin_pals (13*128)	// used on arms and legs



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



void glow_upd( Sprite *sprite );
void control_team_edit_crsr();
void do_bloke_shadow();
void kill_player_edit();
void update_player_textures( WORD editor );
void kill_team_edit();
void update_te_ed_name();
void update_pl_ed_number();
void pl_ed_select();
void update_kit_textures();

WORD set_te_player_type();


void update_home_team_tv_kit();
void update_away_team_tv_kit();

void download_kit_to_vram( WORD team );

void scale_vector( VECTOR *sv1, VECTOR *sv2, VECTOR *dest_sv, WORD percent );
void scale_svector( SVECTOR *sv1, SVECTOR *sv2, SVECTOR *dest_sv, WORD percent );
void set_ed_zoom_pos();
void set_te_clip_sprites();
void kill_te_clip_sprites();



/****************************************************
 *																	 *
 *  Vars															 *
 *																	 *
 ****************************************************/


WORD shirt_num, shirt_color1, shirt_color2;
WORD short_num, short_color1, short_color2;
WORD sock_num, sock_color1, sock_color2;
WORD sleeve_num, sleeve_color1, sleeve_color2;
WORD skin_num, face_num, hair_num, hair_color, face_hair_num, shirt_digit, boot_color;
WORD call_enter_name;

WORD te_anim_timer;

WORD pl_ed_current_player;
WORD pl_ed_current_team;

WORD te_player_type,te_player_type_store;

Skin_obj sk_obj;
Skin_header_src *sk_header_src;
//Textstring_gadget pl_ed_ts_gad;
WORD player_tex_update;

//BYTE pl_ed_name_buffer[64];

WORD te_clip_sprites_active = YES;

Rdb_gadget pl_ed_rdbs[9];

VECTOR man_pos1,man_pos2;
SVECTOR man_rot1, man_rot2;
WORD ed_zoom_pos;

Sprite *te_clip_sp1;
Sprite *te_clip_sp2;
Sprite *bloke_spr;

RECT te_clip_rect={ 340,40,140,155+10  };
BYTE *te_ed_player_name_ptr;

Sprite *glow_spr;



UBYTE *pl_ed_pal_bank;
UBYTE *pl_textures, *pl_textures_pak;
UBYTE *pl_num_textures, *pl_num_textures_pak;

UBYTE *te_textures, *te_textures_pak;
UBYTE *te_pal_bank;
UBYTE *te_pals;

WORD pl_ed_old_item;

void init_player_edit_bootup()
{
//** called every time frontend is booted **

	call_enter_name=NO;
	face_num = face_hair_num = skin_num = hair_num = boot_color = hair_color = -1;

	pl_ed_current_player = 0;
	pl_ed_current_team = 0;


	pl_textures_pak = (UBYTE *)read_datafile_alloc( PLAYER_TEXTURES );
	pl_ed_pal_bank = (UBYTE *)read_datafile_alloc( PLAYER_EDIT_PAL_BANK );


	sk_header_src = (Skin_header_src *)read_datafile_alloc( PLAYER_TEST_SKD );



//**  Load anim for bouncy ball **
	//load_m01_file( BALL_BOUNCE_MDT, BALL_BOUNCE_SLOT ); //removed 19/6/98


	pl_num_textures_pak = (UBYTE *)read_datafile_alloc( PLAYER_EDIT_NUMS );
	


//*** Setup skinned player, but don't make visible ***
   
	setup_skinned_object( sk_header_src, &sk_obj );
	sk_obj.active=NO;
	sk_obj.pos2.vz = -150;



// bloke zoomed up position
	man_pos1.vx = 1100;
	man_pos1.vy = 3000;
	man_pos1.vz = 2000;
	man_rot1.vx = 0;
	man_rot1.vy = 2500;
	man_rot1.vz = 0;

	man_pos1.vx = 1200;
	man_pos1.vy = 3000;
	man_pos1.vz = 2000;
	man_rot1.vx = 0;
	man_rot1.vy = 300;
	man_rot1.vz = 0;


//bloke normal position

	man_pos2.vx = 4000; // normal sized bloke
	man_pos2.vy = 1600;
	man_pos2.vz = 7500;
	man_rot2.vx = 0;
	man_rot2.vy = 0;
	man_rot2.vz = 0;


	ed_zoom_pos = 100;



}


void init_player_edit_once()
{
//** called ONCE only... **

WORD player,team;
Player_detail *pl;



	pl_ed_current_player = 0;
	pl_ed_current_team = 0;



	for ( team=0;team<MAX_USER_TEAMS;team++ )
		{
		for ( player=0;player<MAX_USER_PLAYERS_PER_TEAM;player++ )
			{
			config->player_detail[ team ] [ player ] = custom_players[team][player];
			}	
		}


	te_player_type = set_te_player_type();

//	player_tex_update=YES;
//	update_kit_textures();

}


void init_player_edit()
{
WORD cnt;
SOD *sod;
Player_detail *pl;
WORD x,y;

	ed_zoom_pos = 100;

	te_player_type_store = te_player_type; // STORE this var ( restore at kill_player_edit )

	te_player_type = set_te_player_type();	// goalie or normal player

//** called every time player edit is selected from wheel **

	te_clip_sprites_active = NO;

/** UNPACK textures to ram **/


 	push(0);
	pl_textures = allocate_mem( 0, MAX_PL_FACE_TSIZE );	// alloc 200k
	unpropak( pl_textures_pak, pl_textures );

	te_textures = allocate_mem( 0, MAX_TE_STRIP_TSIZE );	// alloc 100k
	unpropak( te_textures_pak, te_textures );

	pl_num_textures = allocate_mem( 0, MAX_TE_PL_NUMS_TSIZE );	// alloc 50k
	unpropak( pl_num_textures_pak, pl_num_textures );


	pl = &config->player_detail[ pl_ed_current_team ][ pl_ed_current_player ];


	download_kit_to_vram( pl_ed_current_team );

	update_pl_ed_number();




//*** Setup Player name button ***
	y=0;
	cnt=7;
	set_rdb_gadget( 0, &pl_ed_rdbs[cnt], (-200+40-(548/2))-24, (-(64*4)+(y*21*4)+10)+100, 548,(14*4), cnt );


//*** Set up 6 buttons for face type, skin color etc ***
	cnt=0;
	for(y=1;y<4;y++ )
		{
		for(x=0;x<2;x++ )
			{
			set_rdb_gadget( 0, &pl_ed_rdbs[cnt], -340+( x*70*4)+40-(268/2)-24, -(65*4)+(y*21*4)+100+16, 268, (14*4), cnt );
			cnt++;
			}
		}


	cnt=6;
	set_rdb_gadget( 0, &pl_ed_rdbs[cnt], -200+40-(548/2)-24, -(65*4)+(y*21*4)+100+16, 548,(14*4), cnt  );


	pl_ed_old_item = 4;



// Make skinned player visible...
	sk_obj.active=YES;

	player_tex_update = YES;	// update all player's textures once
	update_player_textures( PLAYER_EDITOR );


	set_te_clip_sprites();


	set_title_lolly( edit_player_txt[config->language] , game_types_text[ config->language ][ share->game_type ]  );



	if ( call_enter_name == 0 )
		set_shell_bottom_buttons( pl_ed_shell_buttons,0 );
	else
		{
		shell->current_item = 0;
		call_enter_name=NO;
		}

}



void update_pl_ed_number()
{
Player_detail *pl;

	te_player_type = set_te_player_type();	// goalie or normal player
	pl = &config->player_detail[ pl_ed_current_team ][ pl_ed_current_player ];

	if ( te_player_type == GOALKEEPER ) 
		{
		//*** print " GOALKEEPER : JOE BLOGGS" ***
		sprintf(pl_ed_text_buffer, "%s : %s\n",pl_ed_player_words[te_player_type][config->language], pl->name );
		}
	else
		{
		//*** print " PLAYER x : JOE BLOGGS" ***
		sprintf(pl_ed_text_buffer, "%s %d : %s\n",pl_ed_player_words[te_player_type][config->language], pl_ed_current_player+1, pl->name );
		}

	download_kit_to_vram( pl_ed_current_team );
}


void update_player_edit()
{
Player_detail *pl;
WORD x,y, cnt;
	set_ed_zoom_pos();

	te_player_type = set_te_player_type();	// goalie or normal player

	pl_ed_select();
	pl = &config->player_detail[ pl_ed_current_team ][ pl_ed_current_player ];
	te_player_type = set_te_player_type();	// goalie or normal player

	if ( shell->keyboard_active == NO )
		{
		if ( cjoy->c & PAD_L1 )
			sk_obj.rot2.vy += 60;

		if ( cjoy->c & PAD_L2 )
			sk_obj.rot2.vy -= 60;

 		sk_obj.current_anim_frame = 257; //timer % 300; //timer&31;

	//*** Show team selected for player edit ***
		
		//add_text_string ( tslot_poses[MAIN_TSLOT]+10 , te_ed_player_name_ptr, MILFORD_FONT, milford_font_logic, -320, -240, FONT_XC, -1, 0, 128, 0 );

		update_player_textures( PLAYER_EDITOR );


	/* Player name */
		y=0;
		cnt=7;
		add_text_string ( tslot_poses[MAIN_TSLOT]+30+cnt , pl_ed_text_buffer, MILFORD_FONT, milford_font_logic, ((-96*4)+40)/2, -(65*4)+(y*21*4)+100, FONT_XC, 7, 0, 128, 0 );



	/*** 6 buttons for face type, skin color etc ***/
		cnt=0;
		for(y=1;y<4;y++ )
			{
			for(x=0;x<2;x++ )
				{
				add_text_string ( tslot_poses[MAIN_TSLOT]+30+cnt , pl_ed_button_names[config->language][cnt], MILFORD_FONT, milford_font_logic, (-(170*4)+( x*140*4)+40)/2, -(65*4)+(y*21*4)+100, FONT_XC, cnt, 0, 128, 0 );
				cnt++;
				}
			}


	/** Boot color **/
		cnt=6;
		add_text_string ( tslot_poses[MAIN_TSLOT]+30+cnt , pl_ed_button_names[config->language][cnt], MILFORD_FONT, milford_font_logic, (-(170*4)+( 70*4)+40)/2, -(65*4)+(y*21*4)+100, FONT_XC, 6, 0, 128, 0 );
		}



}


void pl_ed_select()
{
Player_detail *pl;





//*** Modify buttons ***

	if ( shell->keyboard_active == NO )
		{

		if ( (shell->master_option_active == YES) && ( cjoy->db & PAD_UP ) )
			{		 
			sfxSpot( HIT1, 30 );
		
			shell->current_item = 6;

			shell->master_option_active = NO;
			return;
			}
			

		if ( (shell->current_item == 0) || (shell->current_item == 1) )
			{
			if ( cjoy->db & PAD_UP )
				{
				sfxSpot( HIT1, 30 );

				pl_ed_old_item = shell->current_item;
				shell->current_item = 7;
				return;
				}
			}


		if ( (shell->current_item == 4) || (shell->current_item == 5) )
			{
			if ( cjoy->db & PAD_DN )
				{
				sfxSpot( HIT1, 30 );
				pl_ed_old_item = shell->current_item;
				shell->current_item = 6;
				return;
				}
			}



		if ( shell->current_item == 7 )
			{
			}
		else
			{
			if ( shell->current_item <= 5 )	
				{
				modify_current_item_array ( 0, 2, 3 );

				}
			else
				{
				if ( shell->current_item == 6 )
					{
					if ( cjoy->db & PAD_UP )
						{
						sfxSpot( HIT1, 30 );

						shell->current_item = pl_ed_old_item;
						return;
						}

					if ( cjoy->db & PAD_DN )
						{
						goto_shell_buttons();
						}

					}

				}
			}




		if ( ( shell->current_item == 0 ) && ( cjoy->db & PAD_X )  )
			{
			//user wants to change player name...
			//sfxSpot( BUTVOC, 30 );
			kill_player_edit();

			pl = &config->player_detail[ pl_ed_current_team ][ pl_ed_current_player ];

			//strcpy( text_buffer, pl->name );

			entname_text = pl->name;

			shell->module_abort=YES;
			call_enter_name=YES;
			}
		}




}



void kill_player_edit()
{
WORD cnt;
Player_detail *pl;

	pop(0);	// restore mem allocated for player edit

// Make skinned player invisible
	sk_obj.active=NO;


	for ( cnt=0;cnt<8;cnt++ )
		{
		kill_rdb_gadget( pl_ed_rdbs[ cnt ].gadget );
		}

	kill_te_clip_sprites();


	te_player_type = te_player_type_store;
	
	kill_shell_bottom_buttons();
}



void update_player_textures( WORD editor )
{
BYTE *face_texture, *hair_texture, *face_hair_texture, *num_texture,*shirt_btexture;
Player_detail *pl;
BYTE *thigh_texture;




	if ( editor == PLAYER_EDITOR )
		{
		if ( shell->current_item == 7 )
			{
			if ( cjoy->db & PAD_DN )
				{
			  	shell->current_item = pl_ed_old_item;
				sfxSpot( HIT1, 30 );
				}

			if ( modify_word( &pl_ed_current_player, 0, MAX_USER_PLAYERS_PER_TEAM-1 ) )
				{
				player_tex_update=YES;
				}
			}
		}

	pl = &config->player_detail[ pl_ed_current_team ] [ pl_ed_current_player ];


	if ( editor == PLAYER_EDITOR )
		{

		if ( shell->current_item == 1 )
			modify_byte( &pl->face_type, 0, 5 );

		if ( shell->current_item == 2 )
			{
			modify_byte( &pl->face_hair_style, 0, 9 );
			}

		if ( shell->current_item == 3 )
			{
			modify_byte( &pl->skin_color, 0, 5 );
			if ( cjoy->db & PAD_UP )
				{
				pl_ed_old_item = shell->current_item;
				}
			}

		if ( shell->current_item == 4 )
			{
			modify_byte( &pl->head_hair_style, 0, 11 );
			if ( cjoy->db & PAD_UP )
				{
				pl_ed_old_item = shell->current_item;
				}
			}
		if ( shell->current_item == 5 )
			{
			modify_byte( &pl->hair_color, 0, 5 );
			if ( cjoy->db & PAD_UP )
				{
				pl_ed_old_item = shell->current_item;
				}
			}

		if ( shell->current_item == 6 )
			{
			modify_byte( &pl->boot_color, 0, 4 );
			}


		}
	else
		{
		//te_player_type = set_te_player_type();	// goalie or normal player
		player_tex_update = YES;
		}




//** face type **

	if ( face_num != pl->face_type )
		player_tex_update=YES;

	if ( face_hair_num != pl->face_hair_style )
		player_tex_update=YES;

	if ( skin_num != pl->skin_color )
		player_tex_update=YES;

	if ( hair_num != pl->head_hair_style )
		player_tex_update=YES;

	if ( hair_color != pl->hair_color )
		player_tex_update=YES;

	if ( boot_color != pl->boot_color )
		player_tex_update=YES;


	face_num=pl->face_type;
	face_hair_num=pl->face_hair_style;
	skin_num=pl->skin_color;
	hair_num=pl->head_hair_style;
	hair_color=pl->hair_color;
	boot_color=pl->boot_color;



	if ( player_tex_update )
		{

		update_pl_ed_number();

		face_texture      = pl_textures + FACE_TSTART + ((( skin_num * FACES_PER_SKIN)+face_num ) * FACE_TSIZE);
		hair_texture      = pl_textures + HAIR_TSTART + ( hair_num  * HAIR_TSIZE );
		face_hair_texture = pl_textures + FACE_HAIR_TSTART + ( face_hair_num * FACE_HAIR_TSIZE);
		thigh_texture		= pl_textures + THIGH_TSTART;

		overlay_textures( face_texture, hair_texture, face_hair_texture, TED_PLAYER_FACE );
		download_clut( (BYTE *)pl_ed_pal_bank, TED_PLAYER_FACE, Face_pals + (skin_num*128),0,  128 );
		download_clut( (BYTE *)pl_ed_pal_bank, TED_PLAYER_FACE, Hair_pals +( 128*hair_color),128,128 );
		download_clut( (BYTE *)pl_ed_pal_bank, TED_PLAYER_ARMS, Skin_pals + (skin_num*128),0,  128 );
		download_clut( (BYTE *)pl_ed_pal_bank, TED_PLAYER_LEGS, Skin_pals + (skin_num*128),0,  128 );
		download_clut( (BYTE *)te_pals, TED_BOOT, (boot_color+15)*32,192,32 );
		}

	player_tex_update=NO;
}







/****************************************************************************
 *																									 *
 *																									 *
 *        T E A M   E D I T O R															 *
 *																									 *
 *																								  	 *
 ****************************************************************************/

BYTE *te_pl_type_name;
BYTE te_pl_type_name_buff[64];

//WORD shirt_pre[3]={0,0,0};	//preset store of style,color1,color2
//WORD short_pre[3]={0,0,0};
//WORD sock_pre[3]={0,0,0};
//WORD old_shirt_pre[3]={0,0,0};
//WORD old_short_pre[3]={0,0,0};
//WORD old_sock_pre[3]={0,0,0};

BYTE te_ed_name_buffer[64];
//BYTE te_ed_num_buffer[64];

BYTE *te_ed_team_button_names[]={
	"Team",
	"gteam",
	"fteam",
	"iteam",
	"steam",
	"dteam",
};



BYTE *te_ed_shirt_txt[]={
	"shirt",
};

BYTE *te_ed_sleeve_txt[]={
	"sleeves",
};

BYTE *te_ed_shorts_txt[]={
	"shorts",
};

BYTE *te_ed_socks_txt[]={
	"socks",
};







Rdb_gadget te_ed_bigbox_rdb;
Rdb_gadget te_ed_rdbs[ 7 ];
WORD te_ed_old_item;

Sprite *te_swatches[4][2];
Sprite *te_outer_swatches[4][2];
UWORD swatch_col[4][2];

void init_team_edit_bootup()
{

	pl_ed_current_player = 0;
	pl_ed_current_team = 0;

	te_textures_pak = (UBYTE *)read_datafile_alloc( KIT_TEXTURES );
	te_pals 		    = (UBYTE *)read_datafile_alloc( KIT_PALS );

	player_tex_update=YES;

	push(0);
	pl_textures = allocate_mem( 0, MAX_PL_FACE_TSIZE );	// alloc 200k
	unpropak( pl_textures_pak, pl_textures );


	te_textures = allocate_mem( 0, MAX_TE_STRIP_TSIZE );	// alloc 100k
	unpropak( te_textures_pak, te_textures );

	pl_num_textures = allocate_mem( 0, MAX_TE_PL_NUMS_TSIZE );	// alloc 50k
	unpropak( pl_num_textures_pak, pl_num_textures );

	pop(0);

	call_enter_name=NO;


}

void init_team_edit_once()
{
Team_detail *te;
WORD cnt;


	te = config->team_detail;

	for(cnt=0;cnt<MAX_USER_TEAMS;cnt++)
	 	config->team_detail[cnt] = custom_teams[cnt];


}


void init_team_edit()
{
Team_detail *te;
WORD cnt,x,y;
Player_detail *pl;
Gadget *gad;   
WORD xposes[]={ -360, -108, -8 };

	te_anim_timer=0;

/** UNPACK textures to ram **/

	ed_zoom_pos = 100;
 
	te_ed_old_item = 3;

	pl = &config->player_detail[ pl_ed_current_team ][ pl_ed_current_player ];


/** Color swatches for shirt , sleeves colors ***/

	for ( y=0; y<4; y++ )
		{
		for ( x=0; x<2; x++ )
			{
			spawn_sprite( -1 );
			te_swatches[y][x] = sprite;
			sprite->gouraud = YES;
			sprite->untextured_frame.w = 62;
			sprite->untextured_frame.h = 40;
			sprite->depth = NEAREST_DEPTH+2;
			sprite->transp_rate = -1;
			sprite->group_num = 4 + ( y*3 ) + x;
			sprite->x.w.hi = -58*2 + ( 100*(x) );
			sprite->y.w.hi = -6*4 + ( (y)*21*4 ) - TE_YM;

			spawn_sprite( SWATCH );
			te_outer_swatches[y][x] = sprite;
			sprite->x.w.hi = -54*2 + ( 100*(x) )-22;
			sprite->y.w.hi = -5*4 + ( (y)*21*4 )-16 - TE_YM;
			sprite->group_num = 4 + ( y*3 ) + x;
//			sprite->frame = 6;
			sprite->depth = NEAREST_DEPTH+1;
			}
		}

	glow_spr = spawn_sprite( GLOW );
	sprite->upd_rtn = (void *)glow_upd;
	sprite->display=NO;
	sprite->transp_rate=1;
	sprite->depth = NEAREST_DEPTH;
	
	push(0);

	te_textures = allocate_mem( 0, MAX_TE_STRIP_TSIZE );	// alloc 100k
	unpropak( te_textures_pak, te_textures );

	pl_num_textures = allocate_mem( 0, MAX_TE_PL_NUMS_TSIZE );	// alloc 50k
	unpropak( pl_num_textures_pak, pl_num_textures );

//***** Decompress and download skin textures for player *****
	push(0);
	pl_textures = allocate_mem( 0, MAX_PL_FACE_TSIZE );	// alloc 200k
	unpropak( pl_textures_pak, pl_textures );
	player_tex_update=YES;
	update_player_textures(TEAM_EDITOR);
	pop(0);


	set_ed_zoom_pos();	// set bloke pos


//*** Enable skined player ***
	sk_obj.active=YES;

	te = &config->team_detail[ pl_ed_current_team ];

	te_ed_player_name_ptr = te->name;

	//update_te_ed_name();

//*** print 'team x : blahblah' ***
	cnt=0;
	y=0;
	gad=set_rdb_gadget( 0, &te_ed_rdbs[cnt], -424, -(60*4) + (y*21*4)-(11*4)-TE_YM,568, (14*4), cnt );
	gad->rdb_gadget->max_tint = 40;
	gad->rdb_gadget->group = cnt;
		
//*** Change name ***
	cnt=1;
	y=1;
	gad=set_rdb_gadget( 0, &te_ed_rdbs[cnt], -84*4, -(60*4) + (y*21*4)-(11*4)-TE_YM,352, (14*4), cnt );
	gad->rdb_gadget->max_tint = 40;
	gad->rdb_gadget->group = cnt;

	cnt=2;
	y=2;
	gad=set_rdb_gadget( 0, &te_ed_rdbs[cnt], -84*4, -(60*4) + (y*21*4)-(11*4)-TE_YM,352, (14*4),cnt );
	gad->rdb_gadget->max_tint = 40;
	gad->rdb_gadget->group = cnt;



//*** print other button gadgets ***
	cnt=3;
	for ( y=3;y<7; y++ )
		{
	  	gad=set_rdb_gadget( 0, &te_ed_rdbs[cnt], -(360),-(60*4) + (y*21*4)-(11*4)-TE_YM,216, (14*4), cnt );
		gad->rdb_gadget->max_tint = 40;
		gad->rdb_gadget->group = cnt;		
		cnt++;
		}




	player_tex_update = YES;
	update_kit_textures();

	show_selected_swatch();


	set_title_lolly( edit_team_txt[config->language], game_types_text[ config->language ][ share->game_type ]  );


	if ( call_enter_name == 0 )
		set_shell_bottom_buttons( te_ed_shell_buttons,0 );
	else
		{
		shell->current_item = 1;
		call_enter_name=NO;
		}

}

void update_team_edit()
{
BYTE *text;
WORD index;
Team_detail *te;
WORD y;

	if ( (shell->keyboard_active == NO) )
		{
		control_team_edit_crsr();



		if ( te_anim_timer == 0 )
			{
			sfx_handle=sfxScriptOn( (WORD*)&sfxAnimPlayer ); 
			FntPrint("script on\n");
			}

		te_anim_timer++;
		te_anim_timer %= 257;
		}

	

/** Change team num ***/
	if ( shell->current_item == 0 )
		{
		if ( modify_word( &pl_ed_current_team, 0, MAX_USER_TEAMS-1 ) )
			{
			}
		}

	te = &config->team_detail[ pl_ed_current_team ];



/** Change goalie/ player ***/
	if ( shell->current_item == 2 )
		{
		if ( modify_word( &te_player_type, 0, 1 ) )
			{
			}
		}



/*** Shirt num ***/
	if ( shell->current_item == 3 )
		{
		if ( te_player_type == GOALKEEPER )
			modify_byte( &te->shirt_style[ te_player_type ], 0, 3 );
		else
			modify_byte( &te->shirt_style[ te_player_type ], 0, TE_SHIRTS-1 );
		}
	if ( shell->current_item == 4 )
		modify_byte( &te->shirt_color1[ te_player_type ], 0, 15 ); 
	if ( shell->current_item == 5 )
		modify_byte( &te->shirt_color2[ te_player_type ], 0, 15 ); 

/** Change sleeves **/
	if ( shell->current_item == 6 )
		modify_byte( &te->sleeve_style[ te_player_type ], 0, TE_SLEEVES-1 ); 
	if ( shell->current_item == 7 )
		modify_byte( &te->sleeve_color1[ te_player_type ], 0, 15 ); 
	if ( shell->current_item == 8 )
		modify_byte( &te->sleeve_color2[ te_player_type ], 0, 15 ); 



/** Change shorts **/
	if ( shell->current_item == 9 )
		modify_byte( &te->short_style[ te_player_type ], 0, TE_SHORTS-1 ); 
	if ( shell->current_item == 10 )
		modify_byte( &te->short_color1[ te_player_type ], 0, 15 ); 
	if ( shell->current_item == 11 )
		modify_byte( &te->short_color2[ te_player_type ], 0, 15 ); 

/** Change socks **/
	if ( shell->current_item == 12 )
		modify_byte( &te->sock_style[ te_player_type ], 0, TE_SOCKS-1 ); 
	if ( shell->current_item == 13 )
		modify_byte( &te->sock_color1[ te_player_type ], 0, 15 ); 
	if ( shell->current_item == 14 )
		modify_byte( &te->sock_color2[ te_player_type ], 0, 15 ); 


	if ( (shell->keyboard_active == NO) )
		{

		y=0;
		sprintf(text_buffer, "%s %d : %s",te_ed_team_button_names[config->language], pl_ed_current_team+1, te->name );
		add_text_string ( tslot_poses[MAIN_TSLOT]+30+y , text_buffer, MILFORD_FONT, milford_font_logic, (-70*4)/2, -(62*4) + (y*21*4)-(16*4)-TE_YM, FONT_XC, 0, 0, 128, 0 );

		y=1;
		add_text_string ( tslot_poses[MAIN_TSLOT]+30+y , pl_ed_button_names[config->language][0], MILFORD_FONT, milford_font_logic, (-84*4)/2, -(62*4) + (y*21*4)-(16*4)-TE_YM, FONT_XC, 1, 0, 128, 0 );

		y=2;
		add_text_string ( tslot_poses[MAIN_TSLOT]+30+y , pl_ed_player_words[ te_player_type ][ config->language ], MILFORD_FONT, milford_font_logic, (-84*4)/2, -(62*4) + (y*21*4)-(16*4)-TE_YM, FONT_XC, 2, 0, 128, 0 );

		y=3;
		add_text_string ( tslot_poses[MAIN_TSLOT]+30+y , te_ed_shirt_txt[ config->language ], MILFORD_FONT, milford_font_logic, (-(126*4))/2, -(62*4) + (y*21*4)-(16*4)-TE_YM, FONT_XC, 3, 0, 128, 0 );

		y=4;
		add_text_string ( tslot_poses[MAIN_TSLOT]+30+y , te_ed_sleeve_txt[ config->language ], MILFORD_FONT, milford_font_logic, (-(126*4))/2, -(62*4) + (y*21*4)-(16*4)-TE_YM, FONT_XC, 6, 0, 128, 0 );

		y=5;
		add_text_string ( tslot_poses[MAIN_TSLOT]+30+y , te_ed_shorts_txt[ config->language ], MILFORD_FONT, milford_font_logic, (-(126*4))/2, -(62*4) + (y*21*4)-(16*4)-TE_YM, FONT_XC, 9, 0, 128, 0 );

		y=6;
		add_text_string ( tslot_poses[MAIN_TSLOT]+30+y , te_ed_socks_txt[ config->language ], MILFORD_FONT, milford_font_logic, (-(126*4))/2, -(62*4) + (y*21*4)-(16*4)-TE_YM, FONT_XC, 12, 0, 128, 0 );


		if ( cjoy->c & PAD_L1 )
			sk_obj.rot2.vy += 60;

		if ( cjoy->c & PAD_L2 )
			sk_obj.rot2.vy -= 60;

 		sk_obj.current_anim_frame = te_anim_timer;


		//*** Change team name ***
		if ( shell->current_item == 1 )
			{
			if ( cjoy->db & PAD_UP )
				{
				}
			if ( cjoy->db & PAD_X )
				{
				shell->module_abort=YES;
				call_enter_name=YES;
				te = &config->team_detail[ pl_ed_current_team ];
				te_ed_player_name_ptr = te->name;

				entname_text = te_ed_player_name_ptr;

				}
			}



		}


	update_kit_textures();
	show_selected_swatch();
}

void kill_team_edit()
{
WORD cnt;
WORD x,y;

	pop(0);

	for ( cnt=0; cnt<7; cnt++ )
		kill_rdb_gadget( te_ed_rdbs[ cnt ].gadget ); //zzz

	kill_shell_bottom_buttons();

	sfxScriptOff( sfx_handle );
									

//*** Kill swatches ***
	for(y=0;y<4;y++)
		{
		for(x=0;x<2;x++)
			{
			kill_sprite( te_swatches[y][x] );
			kill_sprite( te_outer_swatches[y][x] );
			}
		}

	kill_sprite( glow_spr );	
	
	share->title_func=0;


}







void control_team_edit_crsr()
{

		if ( (shell->current_item == 3) || (shell->current_item == 4) || (shell->current_item == 5) )
			{
			if ( cjoy->db & PAD_UP )
				{
				sfxSpot( HIT1, 30 );
				te_ed_old_item = shell->current_item;
				shell->current_item = 2;
				return;
				}
			}
		else
			{
			if ( shell->current_item == 2 )
				{
				if ( cjoy->db & PAD_DN )
					{
					sfxSpot( HIT1, 30 );

					shell->current_item = te_ed_old_item;
					return;
					}
				else
					{
					if ( cjoy->db & PAD_UP )
						{
						sfxSpot( HIT1, 30 );
				 
						shell->current_item = 1;
						return;
						}
					}
				}

			}




		if ( shell->current_item >= 3 )
			{
			modify_current_item_array( 3,  3,4 );
			}
		else
			{
			if ( shell->current_item <= 1 )
				modify_current_item( 3 );
			}

}












void update_kit_textures()
{
//Text_wheel_gadget *gad;

Player_detail *pl;
UBYTE *num_texture;
BYTE *texture;
WORD shirt_digit,sleeve,cnt;
Team_detail *te;

	pl = &config->player_detail[ pl_ed_current_team ][ pl_ed_current_player ];
	te = &config->team_detail[ pl_ed_current_team ];
	

	if ( shirt_num != te->shirt_style[ te_player_type] )
		{
		shirt_num = te->shirt_style[ te_player_type];
		player_tex_update = YES;
		}
	if ( shirt_color1 != te->shirt_color1[ te_player_type] )
		{
		shirt_color1 = te->shirt_color1[ te_player_type];
		player_tex_update = YES;
		}
	if ( shirt_color2 != te->shirt_color2[ te_player_type] )
		{
		shirt_color2 = te->shirt_color2[ te_player_type];
		player_tex_update = YES;
		}

	if ( short_num != te->short_style[ te_player_type] )
		{
		short_num = te->short_style[ te_player_type];
		player_tex_update = YES;
		}
	if ( short_color1 != te->short_color1[ te_player_type] )
		{
		short_color1 = te->short_color1[ te_player_type];
		player_tex_update = YES;
		}
	if ( short_color2 != te->short_color2[ te_player_type] )
		{
		short_color2 = te->short_color2[ te_player_type];
		player_tex_update = YES;
		}

	if ( sock_num != te->sock_style[ te_player_type] )
		{
		sock_num = te->sock_style[ te_player_type];
		player_tex_update = YES;
		}
	if ( sock_color1 != te->sock_color1[ te_player_type] )
		{
		sock_color1 = te->sock_color1[ te_player_type];
		player_tex_update = YES;
		}
	if ( sock_color2 != te->sock_color2[ te_player_type] )
		{
		sock_color2 = te->sock_color2[ te_player_type];
		player_tex_update = YES;
		}

	if ( sleeve_num != te->sleeve_style[ te_player_type] )
		{
		sleeve_num = te->sleeve_style[ te_player_type];
		player_tex_update = YES;
		}
	if ( sleeve_color1 != te->sleeve_color1[ te_player_type] )
		{
		sleeve_color1 = te->sleeve_color1[ te_player_type];
		player_tex_update = YES;
		}
	if ( sleeve_color2 != te->sleeve_color2[ te_player_type] )
		{
		sleeve_color2 = te->sleeve_color2[ te_player_type ];
		player_tex_update = YES;
		}



	if ( player_tex_update )
		{
		download_kit_to_vram ( pl_ed_current_team );
		}

	player_tex_update=NO;


}






WORD set_te_player_type()
{
Player_detail *pl = &config->player_detail[ pl_ed_current_team ][ pl_ed_current_player ];
WORD player_type;
	if ( (pl->shirt_number != 0)  )
		{
		player_type = NORMAL_PLAYER;
		}
	else
		player_type = GOALKEEPER;

	return player_type;
}









void download_kit_to_vram( WORD team )
{
Team_detail *te;
BYTE *texture;
WORD shirt_digit;
Player_detail *pl;
BYTE *num_texture;
WORD sleeve;
WORD shirt_num, shirt_color1, shirt_color2;
WORD short_num,short_color1, short_color2;
WORD sock_num, sock_color1, sock_color2;
WORD sleeve_num, sleeve_color1, sleeve_color2;
UWORD *wptr;

	pl = &config->player_detail[ pl_ed_current_team ][ pl_ed_current_player ];

/* 
	Set team = team strip to display 0-72 = non user teams
												73-8? = user definable teams
*/


	te = &config->team_detail[ team ];

	shirt_num    = te->shirt_style [ te_player_type ];
	shirt_color1 = te->shirt_color1[ te_player_type ];
	shirt_color2 = te->shirt_color2[ te_player_type ];
	short_num    = te->short_style [ te_player_type ];
	short_color1 = te->short_color1[ te_player_type ];
	short_color2 = te->short_color2[ te_player_type ];
	sock_num     = te->sock_style  [ te_player_type ];
	sock_color1  = te->sock_color1 [ te_player_type ];
	sock_color2  = te->sock_color2 [ te_player_type ];
	sleeve_num     = te->sleeve_style  [ te_player_type ];
	sleeve_color1  = te->sleeve_color1 [ te_player_type ];
	sleeve_color2  = te->sleeve_color2 [ te_player_type ];

	wptr = (UWORD *)te_pals;
	swatch_col[0][0] = *(wptr + ( shirt_color1*32 ));
	swatch_col[0][1] = *(wptr + ( shirt_color2*32 ));
	swatch_col[1][0] = *(wptr + ( sleeve_color1*32 ));
	swatch_col[1][1] = *(wptr + ( sleeve_color2*32 ));
	swatch_col[2][0] = *(wptr + ( short_color1*32 ));
	swatch_col[2][1] = *(wptr + ( short_color2*32 ));
	swatch_col[3][0] = *(wptr + ( sock_color1*32 ));
	swatch_col[3][1] = *(wptr + ( sock_color2*32 ));

//*** Shirt FRONT/BACK/NUMBERS/SLEEVES ***

	//*** Shirt FRONT ***

	if ( te_player_type == NORMAL_PLAYER )
		{
		texture = te_textures + GOALIE_SLEEVE_START;
		download_texture( texture, TED_PLAYER_ARMS );
		texture = te_textures + SHIRT_FSTART + ( shirt_num * SHIRT_FSIZE );
		}
	else
		{
		//** download goalie lower arm sleeve
		texture = te_textures + GOALIE_SLEEVE_START + ( 1 * ARM_TEX_SIZE ) ;
		download_texture( texture, TED_PLAYER_ARMS );

		download_clut( (BYTE *)te_pals, TED_PLAYER_ARMS, shirt_color1*32,160,  32 );
		download_clut( (BYTE *)te_pals, TED_PLAYER_ARMS, 2*32,224,  32 );

		texture = te_textures + GOALIE_SHIRT_FSTART + ( shirt_num * SHIRT_FSIZE );
		}

	download_texture( texture, TED_SHIRT_FRONT );
	

//*** Shirt BACK ***
	if ( te_player_type == NORMAL_PLAYER )
		texture = te_textures + SHIRT_BSTART + ( shirt_num * SHIRT_BSIZE );
	else				
		texture = te_textures + GOALIE_SHIRT_BSTART + ( shirt_num * SHIRT_BSIZE );


//** numbers on back of shirt **
	shirt_digit = pl->shirt_number;
	num_texture = pl_num_textures + ( shirt_digit * SHIRT_NUM_TSIZE );
	overlay_textures( texture,num_texture, 0, TED_SHIRT_BACK );


//** Download SLEEVES **

	if ( te_player_type == NORMAL_PLAYER )
		sleeve = sleeve_num;
	else
		sleeve = sleeve_num; // goalie always use blank sleeve

	texture = te_textures + SLEEVE_START + ( sleeve * SLEEVE_SIZE );


	download_texture( texture, TED_SLEEVE );
	

	download_clut( (BYTE *)te_pals, TED_SHIRT_FRONT, shirt_color1*32,0,  32 );


//** Download shirt front/back/sleeve cluts **

		
	//** download shirt color ranges 1+2 **
	download_clut( (BYTE *)te_pals, TED_SHIRT_FRONT, shirt_color1*32,0,  32 );
	download_clut( (BYTE *)te_pals, TED_SHIRT_FRONT, shirt_color2*32,32, 32 );



//** download shirt color ranges 1+2 **
	download_clut( (BYTE *)te_pals, TED_SHIRT_BACK, shirt_color1*32,0,  32 );
	download_clut( (BYTE *)te_pals, TED_SHIRT_BACK, shirt_color2*32,32,  32 );

//** Sleeve cluts **

	download_clut( (BYTE *)te_pals, TED_SLEEVE,  sleeve_color1*32, 0, 32 );
	download_clut( (BYTE *)te_pals, TED_SLEEVE,  sleeve_color2*32,32, 32 );


	//** numbers clut **

	download_clut( (BYTE *)te_pals, TED_SHIRT_BACK, 32*2,208,32 );



//** Download SHORTS **
	texture = te_textures + SHORT_START + ( short_num * SHORT_SIZE );

	download_texture( texture, TED_SHORT );
	download_clut( (BYTE *)te_pals, TED_SHORT, short_color1*32, 0, 32 );
	download_clut( (BYTE *)te_pals, TED_SHORT, short_color2*32,32, 32 );


//** Download SOCKS **

	texture = te_textures + SOCK_START + ( sock_num * SOCK_SIZE );
	download_texture( texture, TED_SOCK );

	download_clut( (BYTE *)te_pals, TED_SOCK,  sock_color1*32, 0, 32 );
	download_clut( (BYTE *)te_pals, TED_SOCK,  sock_color2*32,32, 32 );

	player_tex_update=NO;

}



void scale_vector( VECTOR *sv1, VECTOR *sv2, VECTOR *dest_sv, WORD percent )
{

	dest_sv->vx = sv1->vx - (((sv1->vx-sv2->vx)*percent)/100);
	dest_sv->vy = sv1->vy - (((sv1->vy-sv2->vy)*percent)/100);
	dest_sv->vz = sv1->vz - (((sv1->vz-sv2->vz)*percent)/100);


}

void scale_svector( SVECTOR *sv1, SVECTOR *sv2, SVECTOR *dest_sv, WORD percent )
{

	dest_sv->vx = sv1->vx - (((sv1->vx-sv2->vx)*percent)/100);
	dest_sv->vy = sv1->vy - (((sv1->vy-sv2->vy)*percent)/100);
	dest_sv->vz = sv1->vz - (((sv1->vz-sv2->vz)*percent)/100);


}

WORD dest_ed_zoom_pos=0;

void set_ed_zoom_pos()
{
WORD dest_rot;

//**** zoom player face up/down depending on which icon is selected
//**** in player edit


		if ( (shell->current_item == 6) || (shell->current_item == 8)  )
			{
			//*** zoomed out ***
			dest_ed_zoom_pos = 100;
			}
		if ( (shell->current_item >= 1) && (shell->current_item <= 5) )
			{
			//*** zoomed in ***
			dest_ed_zoom_pos = 0;
			}

	if ( dest_ed_zoom_pos == 100 )
		{
		//bloke_spr->display=YES;
		//zoomed out
		ed_zoom_pos += 8;
		if ( ed_zoom_pos >=dest_ed_zoom_pos )
			ed_zoom_pos = dest_ed_zoom_pos;
		}
	if ( dest_ed_zoom_pos == 0 )
		{
		//bloke_spr->display=NO;
		ed_zoom_pos -= 8;
		if ( ed_zoom_pos < dest_ed_zoom_pos )
			ed_zoom_pos = dest_ed_zoom_pos;
		}


	scale_vector( &man_pos1, &man_pos2, &sk_obj.pos, ed_zoom_pos );


	scale_svector( &man_rot1, &man_rot2, &sk_obj.rot, ed_zoom_pos );


//*** Spin player to view shirt back when changing player number
	if ( shell->current_item == 8 )
		dest_rot = 1700;
	else
		dest_rot = 00;

	sk_obj.rot2.vy += (dest_rot-sk_obj.rot2.vy)/4;

}



void set_te_clip_sprites()
{
	if ( te_clip_sprites_active == NO )
		{
		te_clip_sp1 = spawn_clip_sprite( &te_clip_rect, PLASMA_DEPTH-1 );
		te_clip_sp2 = spawn_null_clip_sprite( MIDDLE_DEPTH+10 );
		te_clip_sprites_active = YES;
		}
}



void kill_te_clip_sprites()
{
	if ( te_clip_sprites_active == YES )
		{
		kill_sprite ( te_clip_sp1 );	
		kill_sprite ( te_clip_sp2 );	
		te_clip_sprites_active = NO;
		}
}



void do_bloke_shadow()
{
#if 0
	spawn_sprite( MANSHAD );
	sprite->depth = PLASMA_DEPTH-1;
	sprite->x.w.hi = 225;
	sprite->y.w.hi = 250;
	sprite->tint.r = 35;
	sprite->tint.g = 35;
	sprite->tint.b = 35;
	sprite->transp_rate = 2;
	//bloke_spr = sprite;
#endif
}







void show_selected_swatch()
{
WORD x,y,col1, col2;


UWORD col;

UWORD col_list[4][2];
UBYTE r,g,b;	
WORD xoffs[]={ 0, 4, 8, 10,12,14,12,10,8,4 };
WORD wd;


	for ( y=0;y<4; y++ )
		{
		for ( x=0;x<2; x++ )
			{
			sprite = te_swatches[y][x];


			col = swatch_col[ y ] [ x ];



			r = (col & 31) << 3;
			g = (( col >> 5) & 31)<<3;
			b = (( col >> 10) & 31)<<3;

			sprite->r0 = r;
			sprite->g0 = g;
			sprite->b0 = b;

			sprite->r1 = r;
			sprite->g1 = g;
			sprite->b1 = b;

			sprite->r2 = r;
			sprite->g2 = g;
			sprite->b2 = b;

			sprite->r3 = r;
			sprite->g3 = g;
			sprite->b3 = b;
			}
		}
}




void draw_skin_player()
{
	if ( shell->keyboard_active == NO )
		draw_skin_objs( &sk_obj );

}



void glow_upd( Sprite *sprite )
{
Sprite *col, *spr;
WORD x,y;

	sprite->display = NO;

	for(x=0;x<2;x++)
		{
		for( y=0; y<4; y++ )
			{
			spr = te_outer_swatches[y][x];

			if ( spr->group_num == shell->current_item )
				{
				sprite->x.w.hi = spr->x.w.hi;
				sprite->y.w.hi = spr->y.w.hi;
				sprite->display = YES;

				col = te_swatches[y][x];
				sprite->tint.r = col->r0;
				sprite->tint.g = col->g0;
				sprite->tint.b = col->b0;

				return;
				}
			}
		}
}







WORD thread_ok_butt2[]={SHELL_OK, 0 };

void team_edit()
{
void *process;

	push(0);


//	init_shell( FRONTEND );

	process = add_process( draw_skin_player );

/***** Loop for team/player edit, called by title *****/
	while( 1 )
		{
		exec_shell( init_team_edit, update_team_edit, kill_team_edit );

		if ( call_enter_name )
			{
			enter_name();
			}
		else
			{
			if ( shell->current_button == SHELL_QUIT )
				break;

loop:		exec_shell( init_player_edit, update_player_edit, kill_player_edit );
			if ( call_enter_name )
				{
				enter_name();
				goto loop;
				}
			else
				{
				if ( shell->current_button == SHELL_QUIT )
					break;
				}
			}
		}

	kill_process( process );

	pop(0);


}





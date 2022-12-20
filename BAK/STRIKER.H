/**************************************************************
 *																				  *
 *																				  *
 * Striker share structure ( for all formats )					  *
 *																				  *
 *																				  *
 **************************************************************/

// game types

//#define TOTAL_COMPETITIONS 6
#define USER_TEAM_NAME_LEN 	14
#define PREDEF_TEAM_NAME_LEN 	20
#define FR_MONO	0
#define FR_STEREO	1



#define FRIENDLY			 0
#define LEAGUE				 1
#define KNOCKOUT			 2
#define TERRITORY 		 3
#define CLASSIC			 4
#define EURO2000			 5		// Hula I just love using double backslashes!!!
#define EUROFINAL			 6
#define WORLDCUP			 7
#define EURO2000_CALC	 8
#define EURO2000_KO		 9
#define EUROFINAL_KO		 10
#define FRIENDLY_SAVE	 11
#define CONFIG				 12	// used for load/save of config file
#define TRAINING			 13
#define TOTAL_GAME_TYPES 14


#define TOTAL_EURO_GAMES 220
#define TOTAL_GROUPS 10
#define TOTAL_LEAGUE_TEAMS 	 8		// total num teams that play in whole game
#define TOTAL_FRIENDLY_TEAMS	 2
#define MAX_TEAMS_IN_COMPETITION		50	// max num of teams in any one competition
#define MAX_CLASSIC_MATCHES 8

#define GAME_INIT 			0
#define GAME_IN_PROGRESS 	1
#define GAME_OVER 			2
#define ROUND_OVER			3
#define QUIT_GAME				4
#define QUIT_COMPETITION	5
#define TOTAL_GAME_STATS	6

#define TRAINING_MODES 7

/** indexes into options_presets array **/
enum {
 OPT_WEATHER,
 OPT_OFFSIDES,
 OPT_DAYNIGHT,
 OPT_DURATION,
 OPT_EXTRAPLAY,
 OPT_STADIUM,
 OPT_REFEREE,
};

#define MAX_USER_TEAMS 8
#define MAX_NON_USER_TEAMS 74
#define MAX_CLUB_TEAMS 46
#define TOTAL_FLAGS ( MAX_USER_TEAMS+MAX_NON_USER_TEAMS)
#define MAX_USER_PLAYERS_PER_TEAM 22

#define MAX_TEAMS_IN_EURO2000_GROUP 6


#define TOTAL_EUROPE_FLAGS 6 // total flags for EUROPE territory
#define TOTAL_SAMERICA_FLAGS 5
#define TOTAL_ASIA_FLAGS 6
#define TOTAL_AFRICA_FLAGS 6
#define TOTAL_AUSTRALASIA_FLAGS 2
#define TOTAL_AMERICAS_FLAGS 4
#define TOTAL_EURO2000_FLAGS 49
#define TOTAL_EUROFINAL_FLAGS 51
#define TOTAL_WORLDCUP_FLAGS 32
#define ENG 0
#define FRE 1
#define GER 2
#define ITI 3
#define SPA 4
#define DUT 5
#define MAX_LANGS 6

#define TOTAL_PRESETS 8
#define TOTAL_OPTIONS 7

#define TM_PLAYER_STATS 6 // team manager stats for each player

typedef struct
{
//*** This is the info needed for 1 friendly preset ***

	BYTE init;	/* Yes=data has been entered for this preset **/
	BYTE teams[2];
	BYTE option_preset[ TOTAL_OPTIONS ];	// friendly preset game type

}Preset_data;

typedef struct 
{
	BYTE points[16];

}Training_data;


typedef struct
{
	BYTE  name[16];	
	UBYTE shirt_number;
	UBYTE skin_color;
	UBYTE head_shape;	//** pc only **
	UBYTE hair_color;
	UBYTE head_hair_style;
	UBYTE face_type;
	UBYTE face_hair_style;
	UBYTE boot_color;

}Player_detail;

typedef struct
{
	BYTE  name[16];	// Team name	
	UBYTE shirt_style[2];	// first element is for normal player
	UBYTE shirt_color1[2];	// second for goalkeeper
	UBYTE shirt_color2[2];
	UBYTE sleeve_style[2];
	UBYTE sleeve_color1[2];
	UBYTE sleeve_color2[2];
	UBYTE short_style[2];
	UBYTE short_color1[2];
	UBYTE short_color2[2];
	UBYTE sock_style[2];
	UBYTE sock_color1[2];
	UBYTE sock_color2[2];


}Team_detail;


typedef struct
{
	UBYTE played;
	UBYTE won;
	UBYTE drawn;
	UBYTE lost;
	UWORD goals_for;		// goals scored for this team
	UWORD goals_against; // goals let it
	WORD  goal_diff;		// goal_against minus goal_for
	UWORD points;
	WORD  win_pos;			// current position of this team 0=top
}League_data;		  

typedef struct
{
	BYTE teams[5][16];
	BYTE played[5][16];	// has this team played yet?
	WORD current_round;	// current knockout round being played
	WORD draws_to_do;		// remaining amount of draws to perform
	WORD winning_list_pos;	//
	WORD page_mode, page_timer;
	WORD total_draws;
}Ko_data;

typedef struct
{
	BYTE current_match;
	BYTE completed[ 8 ];	// which of first 4 matches have been won
	BYTE available;		// no of matches available to play
}Classic_data;

typedef struct
{
	BYTE completed;	// num of territories completed
	BYTE current_territory;
}Territory_data;

typedef struct
{
	UBYTE team1;
	UBYTE team1_score;
	UBYTE team2;
	UBYTE team2_score;
}Euro_game_result;

typedef struct
{
	UBYTE teams[ 16 ];
	UBYTE runner_teams[ 8 ];	// runner up teams who go through to knockout
}Euro_final_data;

typedef struct
{
	BYTE group_over[ TOTAL_GROUPS ];
	Euro_game_result result[ TOTAL_EURO_GAMES ];	// teams who played and result of every EuroQual game played
	WORD games_played;
}Euro2000_data;

typedef struct
{
	WORD game;	// current game
}Worldcup_data;
/** shared structure **/
typedef struct
	{
/*** Game vars ***/
//	ULONG checksum;	// mcard checksum

	BYTE options_presets[ TOTAL_OPTIONS ];
	BYTE demo_mode;					// YES or NO
	WORD team1_pts;	// points scored by team1
	WORD team2_pts;	// points scored by team2
	BYTE team1_name_string[ PREDEF_TEAM_NAME_LEN ];
	BYTE team2_name_string[ PREDEF_TEAM_NAME_LEN ];
	BYTE human_player_side[ 8 ];	// -1 = human home, 0=human not playing 1=human away
	BYTE team1;							// num of 1st team to play this match
	BYTE team2; 						// num of 2nd team to play this match
	BYTE team1_cpu; 					// home team is CPU? yes or no
	BYTE team2_cpu; 					// Away team is CPU? yes or no

/** Fe vars **/
	void (*title_func)();
	BYTE thread_pos;	
	BYTE game_stat;	// GAME_IN_PROGRESS etc also this can be returned by game
	BYTE perform_draw;
	ULONG stub_code;			// if equal to STUB_CODE,  stub is resident
	ULONG init_once_code;	// if equal to INIT_ONCE_PERFORMED_CODE, once only initialisation has been performed

	BYTE new_game;				// YES or NO
	BYTE game_type;			// friendly/ league etc


	Training_data training_data;	/** training data returned by steve , to be stored in config **/
	Territory_data territory_data;
	Euro2000_data	euro2000_data;
	Ko_data		 ko_data;
	Classic_data classic_data;
	League_data  league_data[ TOTAL_GROUPS ][ TOTAL_LEAGUE_TEAMS ];
	BYTE	teams_in_group[ TOTAL_GROUPS ];
	BYTE  league_draw_array[ TOTAL_GROUPS ][ TOTAL_LEAGUE_TEAMS ][ TOTAL_LEAGUE_TEAMS ];
	BYTE  group_teams[ TOTAL_GROUPS ][ TOTAL_LEAGUE_TEAMS ];
	WORD cpu_teams_index_list[ TOTAL_FLAGS ]; 	// eg index to byte 44=YES means team 44 is CPU
	Euro_final_data euro_final_data;	// teams to go through to final
	Worldcup_data worldcup_data;

	BYTE current_group;
	BYTE total_groups;

	BYTE max_teams_to_pick;		// Max number of teams user can pick
	BYTE teams_in_competition;	// Amount of teams to play in this competition
	WORD teams_selection_size;	// Amount of teams to choose from
	BYTE total_human_teams;
	BYTE game_difficulty;

	BYTE team1_list_pos;	// Position in selected_teams_list of team1 to play 
	BYTE team2_list_pos;	// Position in selected_teams_list of team2 to play 

/** List of teams to play output by 'team_select' **/
	WORD selected_teams_list[ MAX_TEAMS_IN_COMPETITION+1 ];
	BYTE winning_team;
	BYTE config_loaded;

	BYTE multitaps_connected_list[2];

	UBYTE strategy[8];	/** Storage for up to 8 human teams **/
	UBYTE formation[8];
	UBYTE order[8][22];

}Share;




typedef struct
{
	BYTE 			  screen_xpos;			// hardware screen xpos
	BYTE 			  screen_ypos;			// hardware screen ypos
	BYTE 			  language;				// language
	Player_detail player_detail[ MAX_USER_TEAMS ][ MAX_USER_PLAYERS_PER_TEAM ];
	Team_detail   team_detail[ MAX_USER_TEAMS ]; //data for USER teams
	WORD 			  cd_volume;		// passed to rob's cdaudio.c
	UBYTE			  master_volume;	//
	UBYTE			  music_volume;	// controls cd vol
	UBYTE			  com_volume;
	UBYTE			  sfx_volume;
	UBYTE 		  sound_mode;		// FR_STEREO or FR_MONO
	BYTE 			  pad_config[ 8 ]; 		  	// configuration num for each pad
	Preset_data   preset_data[ TOTAL_PRESETS ];
	Training_data training_data;
	BYTE team1_cpu; 					// home team is CPU? yes or no
	BYTE team2_cpu; 					// Away team is CPU? yes or no
	BYTE store_performed;		// flag to show if config has been stored
}Config;


typedef struct
{
	UBYTE name[19];
	UBYTE shirt_num;
	UBYTE speed;
	UBYTE strength;
	UBYTE passing;
	UBYTE tackling;
	UBYTE shooting;
	UBYTE ball_control;
	UBYTE position;	// goalkeeper, striker etc (0-10)
}Database_player;

typedef struct
{
	UBYTE formation;
	UBYTE strategy;
	Database_player player[22];
}Database_team;


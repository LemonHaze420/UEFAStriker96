extern BYTE *team_names[][ MAX_NON_USER_TEAMS ];
extern BYTE *preset_not_enough_pads_req[];

extern BYTE *opt_lolly_text[];
extern BYTE *edit_team_txt[];
extern BYTE *edit_player_txt[];
extern BYTE *edit_name_txt[];
extern BYTE *audio_config_lolly_text[];
extern BYTE *screen_pos_lolly_text[];
extern BYTE *save_config_lolly_text[];
extern BYTE *save_game_lolly_text[];


extern Player_detail custom_players[MAX_USER_TEAMS][MAX_USER_PLAYERS_PER_TEAM];
extern Team_detail custom_teams[];


extern BYTE *friendly_shell_opts[];
extern BYTE *te_ed_shell_opts[];
extern BYTE *pl_ed_shell_opts[];
extern BYTE *audio_config_shell_opts[];



extern UBYTE ts_all_teams_list[][23*4];

extern UBYTE territory_teams_list[][6];
extern UBYTE ts_euro2000_teams_list[][ TOTAL_EURO2000_FLAGS ];
extern UBYTE ts_eurofinal_teams_list[][ TOTAL_EUROFINAL_FLAGS ];
extern BYTE euro2000_teams[9][6];
extern BYTE worldcup_fixlist[48][2];
extern BYTE worldcup_group_teams[8][4];









BYTE *game_types_text[MAX_LANGS][ TOTAL_GAME_TYPES ];
BYTE *thread_game_over_text[MAX_LANGS][ TOTAL_GAME_TYPES ];
BYTE *thread_quit_text[MAX_LANGS];
BYTE *preset_save_request[MAX_LANGS];


extern BYTE *group_text[];



#define Albania         	 0
#define Andorra				 1
#define Argentina				 2
#define Armenia				 3
#define Australia				 4
#define Austria				 5
#define Azerbaijan			 6
#define Belarus				 7 
#define Belgium				 8
#define Bosnia					 9
#define Brazil					 10
#define Bulgaria				 11
#define Cameroon				 12
#define Canada					 13
#define Chile					 14
#define Colombia				 15
#define Croatia				 16
#define Cyprus				 	 17
#define Czech_Republic		 18
#define Denmark				 19
#define Egypt			  		 20
#define England		  		 21
#define Estonia		  		 22
#define Faroe_Islands 		 23
#define Finland		  		 24
#define France		  		 	 25
#define Georgia		  		 26
#define Germany		  		 27
#define Greece		  		 	 28
#define Hungary		  		 29
#define Iceland		  		 30
#define India         		 31
#define Iran			  		 32
#define Israel		  		 	 33
#define Italy			  		 34
#define Jamaica		  		 35
#define Japan			  		 36
#define Korean_Republic		 37
#define Latvia		  		 	 38
#define Liechtenstein 		 39
#define Lithuania	  		 	 40
#define Luxembourg	  		 41
#define Macedonia	  		 	 42
#define Malta			  		 43
#define Mexico		  		 	 44
#define Moldova		  		 45
#define Morocco		  		 46
#define Netherlands	  		 47
#define New_Zealand	  		 48
#define Nigeria		  		 49
#define Northern_Ireland    50
#define Norway		  		 	 51
#define Pakistan		  		 52
#define Paraguay		  		 53
#define Poland		  		 	 54
#define Portugal		  		 55
#define Republic_of_Ireland 56
#define Romania      		 57
#define Russia		 		 	 58
#define San_Marino	 		 59
#define Saudi_Arabia 		 60
#define Scotland     		 61
#define Slovakia				 62
#define Slovenia		 		 63
#define South_Africa 		 64
#define Spain			 		 65
#define Sweden		 		 	 66
#define Switzerland	 		 67
#define Tunisia		 		 68
#define Turkey		 		 	 69
#define Ukraine		 		 70
#define United_States		 71
#define Wales        		 72
#define Yugoslavia	 		 73

BYTE *humcpu_txt[MAX_LANGS][2];
BYTE *autoload_conf_file_txt[MAX_LANGS];


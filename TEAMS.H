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

/****************  Domestic  ************************/
#define AC_Milan 				 74
#define AFC_Ajax				 75
#define Arsenal				 76
#define AS_Monaco				 77
#define Aston_Villa			 78
#define Bayer_04_Leverkusen 79
#define Borussia_Dortmund   80
#define Brondby_IF			 81
#define Celta_de_Vigo		 82
#define Celtic					 83
#define Chelsea				 84
#define Club_Atletico_de_Madrid 85
#define Club_Brugge_KV		 86
#define Dynamo_Kyiv			 87
#define FC_Barcelona	       88
#define FC_Bayern_Munchen   89
#define FC_Girondins_de_Bordeaux 90
#define FC_Porto 				 91
#define FC_Steaua_Bucuresti 92
#define Feyenoord           93
#define Fiorentina          94
#define Galatasaray_SK      95
#define Grasshopper_Club    96
#define IFK_Goteborg        97
#define Internazionale      98
#define Juventus            99
#define Kaiserslautern      100
#define Liverpool           101
#define Manchester_Utd      102
#define NK_Croatia_Zagreb   103
#define Olympique_de_Marseille 104
#define Olympique_Lyonnais  105
#define Panathinaikos       106
#define Paris_Saint_Germain 107
#define Parma               108
#define PSV_Eindhoven       109
#define Rangers             110
#define Real_Madrid_CF      111
#define Rosenborg_BK        112
#define RSC_Anderlecht      113
#define SK_Rapid_Wien       114
#define SL_Benfica          115
#define Sporting_Clube_de_Portugal 116
#define SV_Werder_Bremen           117

#define User_teams_start	 118										


extern BYTE ts_country_teams_list[ MAX_LANGS ][ MAX_COUNTRY_TEAMS + MAX_USER_TEAMS ];
extern BYTE ts_club_teams_list[ MAX_CLUB_TEAMS ];

extern BYTE territory_teams_list[][ 6 ];
extern BYTE ts_euro2000_teams_list[][ TOTAL_EURO2000_FLAGS ];
extern BYTE ts_eurofinal_teams_list[][ TOTAL_EUROFINAL_FLAGS ];
extern BYTE euro2000_teams[9][6];
extern BYTE worldcup_fixlist[48][2];
extern BYTE worldcup_group_teams[8][4];


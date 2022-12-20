/*************************************************************************************
 *																												 *
 *																												 *
 * Frontend texts																							 *
 * in all languages																						 *
 *																												 *
 *																												 *
 *************************************************************************************/
#include "incs.h"

#include "teamedit.h"
#include "text.h"

#include "teams.h"
#include "teams.c"



/*** These MUST be in SAME order as english ***/
BYTE *team_names[][ MAX_NON_USER_TEAMS ]={
"Albania",            //00
"Andorra",				 //01
"Argentina",			 //02
"Armenia",				 //03
"Australia",			 //04
"Austria",				 //05
"Azerbaijan",			 //06
"Belarus",				 //07
"Belgium",				 //08
"Bosnia",				 //09
"Brazil",				 //10
"Bulgaria",				 //11
"Cameroon",				 //12
"Canada",				 //13
"Chile",					 //14
"Colombia",				 //15
"Croatia",				 //16
"Cyprus",				 //17
"Czech Republic",		 //18
"Denmark",				 //19
"Egypt",			  		 //20
"England",		  		 //21
"Estonia",		  		 //22
"Faroe Islands", 		 //23
"Finland",		  		 //24
"France",		  		 //25
"Georgia",		  		 //26
"Germany",		  		 //27
"Greece",		  		 //28
"Hungary",		  		 //29
"Iceland",		  		 //30
"India",         		 //31
"Iran",			  		 //32
"Israel",		  		 //33
"Italy",			  		 //34
"Jamaica",		  		 //35
"Japan",			  		 //36
"Korean Republic",	 //37
"Latvia",		  		 //38
"Liechtenstein", 		 //39
"Lithuania",	  		 //40
"Luxembourg",	  		 //41
"Macedonia",	  		 //42
"Malta",			  		 //43
"Mexico",		  		 //44
"Moldova",		  		 //45
"Morocco",		  		 //46
"Netherlands",	  		 //47
"New Zealand",	  		 //48
"Nigeria",		  		 //49
"Northern Ireland",   //50
"Norway",		  		 //51
"Pakistan",		  		 //52
"Paraguay",		  		 //53
"Poland",		  		 //54
"Portugal",		  		 //55
"Republic of Ireland",//56
"Romania",      		 //57
"Russia",		 		 //58
"San Marino",	 		 //59
"Saudi Arabia", 		 //60
"Scotland",     		 //61
"Slovakia",		 		 //62
"Slovenia",		 		 //63
"South Africa", 		 //64
"Spain",			 		 //65
"Sweden",		 		 //66
"Switzerland",	 		 //67
"Tunisia",		 		 //68
"Turkey",		 		 //69
"Ukraine",		 		 //70
"United States",		 //71
"Wales",        		 //72
"Yugoslavia",	 		 //73

/****** Domestic *******/

"AC Milan",                       //74
"AFC Ajax",								 //75
"Arsenal",								 //76
"AS Monaco",							 //77
"Aston Villa",							 //78
"Bayer 04 Leverkusen",				 //79
"Borussia Dortmund",					 //80
"Brondby IF",							 //81
"Celta de Vigo",						 //82
"Celtic",								 //83
"Chelsea",								 //84
"Club Atletico de Madrid",			 //85
"Club Brugge KV",						 //86
"Dynamo Kyiv",							 //87
"FC Barcelona",						 //88
"FC Bayern Munchen",					 //89
"FC Girondins de Bordeaux",		 //90
"FC Porto",								 //91
"FC Steaua Bucuresti",				 //92
"Feyenoord",							 //93
"Fiorentina",							 //94
"Galatasaray SK",						 //95
"Grasshopper-Club",					 //96
"IFK Goteborg",						 //97
"Internazionale",						 //98
"Juventus",								 //99
"Kaiserslautern",						 //100
"Liverpool",							 //101
"Manchester Utd",						 //102
"NK Croatia Zagreb",					 //103
"Olympique de Marseille",			 //104
"Olympique Lyonnais",				 //105
"Panathinaikos",						 //106
"Paris Saint-Germain",				 //107
"Parma",									 //108
"PSV Eindhoven",						 //109
"Rangers",								 //110
"Real Madrid CF",						 //111
"Rosenborg BK",						 //112
"RSC Anderlecht",						 //113
"SK Rapid Wien",						 //114
"SL Benfica",							 //115
"Sporting Clube de Portugal",		 //116
"SV Werder Bremen",					 //117
								  
#if 1							  
"fAlbania00",			 //00
"fAndorra",				 //01
"fArgentina",			 //02
"fArmenia",				 //03
"fAustralia",			 //04
"fAustria",				 //05
"fAzerbaijan",			 //06
"fBelarus",				 //07
"fBelgium",				 //08
"fBosnia",				 //09
"fBrazil",				 //10
"fVulvaria",			 //11
"fCameroon",			 //12
"fCanada00",          //13
"fChile",				 //14
"fColombia",			 //15
"fCroatia",				 //16
"fCyprus",				 //17
"fCzech Republic",	 //18
"fDenmark",				 //19
"fEgypt",			  	 //20
"fEngland",		  		 //21
"fEstonia",		  		 //22
"fFaroe Islands", 	 //23
"fFinland",		  		 //24
"fFrance",		  		 //25
"fGeorgia",		  		 //26
"fGermany",		  		 //27
"fGreece",		  		 //28
"fHungary",		  		 //29
"fIceland",		  		 //30
"fIndia",         	 //31
"fIran",			  		 //32
"fIsrael",		  		 //33
"fItaly",			  	 //34
"fJamaica",		  		 //35
"fJapan",			  	 //36
"fKorean Republic",	 //37
"fLatvia",		  		 //38
"fLiechtenstein", 	 //39
"fLithuania",	  		 //40
"fLuxembourg",	  		 //41
"fMacedonia",	  		 //42
"fMalta",			  	 //43
"fMexico",		  		 //44
"fMoldova",		  		 //45
"fMorocco",		  		 //46
"fNetherlands",	  	 //47
"fNew Zealand",	  	 //48
"fNigeria",		  		 //49
"fNorthern Ireland",  //50
"fNorway",		  		 //51
"fPakistan",		  	 //52
"fParaguay",		 	 //53
"fPoland",		  		 //54
"fPortugal",		 	 //55
"fRepublic of Ireland",//56
"fRomania",      		 //57
"fRussia",		 		 //58
"fSan Marino",	 		 //59
"fSaudi Arabia", 		 //60
"fScotland",     		 //61
"fSlovakia",		 	 //62
"fSlovenia",		 	 //63
"fSouth Africa", 		 //64
"fSpain",			 	 //65
"fSweden",		 		 //66
"fSwitzerland",	 	 //67
"fTunisia",		 		 //68
"fTurkey",		 		 //69
"fUkraine",		 		 //70
"fUnited States",		 //71
"fWales",        		 //72
"fYugoslavia",	 		 //73

/****** Frog Domestic *******/

"f",    /** Padding ***/
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
"f",                      
 
#endif

};











/***********************************
 *
 * Custom team strip data
 *
 ***********************************/

Team_detail custom_teams[MAX_USER_TEAMS]={
	"Rage united",
	3,2,	//player, goalie shirt style
	5,6,	//player, goalie shirt color1
	10,0,	//player, goalie shirt color2

	1,1,	//sleeve style
	5,6,	//sleeve color1
	10,6,	//sleeve color2

	2,3,	//short style
	10,6,	//short color1
	10,6,	//short color2

	2,2,
	10,0,
	5,6,


	"Biscuit city",
	3,3,
	5,7,
	5,8,

	4,1,
	5,7,
	6,7,

	2,4,
	6,7,
	6,8,

	3,2,
	6,10,
	6,7,


	"Bad things",
	6,3,
	13,10,
	8,6,

	4,0,
	8,6,
	8,10,

	0,1,
	13,6,
	8,0,

	3,1,
	8,8,
	13,0,


	"Good things",
	2,0,
	3,11,
	0,0,

	4,4,
	0,11,
	6,10,

	0,1,
	6,8,
	0,0,

	4,1,
	0,1,
	6,1,

	"Foodville",
	7,1,
	9,8,
	10,3,

	0,0,
	9,3,
	10,8,

	4,2,
	10,3,
	10,11,

	2,0,
	10,2,
	9,11,


	"Body squad",
	2,0,
	10,9,
	6,6,

	3,4,
	6,9,
	10,6,

	0,3,
	10,9,
	6,6,

	0,1,
	6,6,
	10,0,


	"World XI",
	1,2,
	6,2,
	5,3,

	0,0,
	5,2,
	6,3,

	4,0,
	2,2,
	5,3,

	0,3,
	5,2,
	6,3,

	"Custom 8",
	2,1,
	4,3,
	2,1,

	0,0,
	5,2,
	6,3,

	4,0,
	2,2,
	5,3,

	0,3,
	5,2,
	6,3,

};




/************************************
 *												*
 * Custom team player data				*
 *												*
 ************************************/

Player_detail custom_players[MAX_USER_TEAMS][MAX_USER_PLAYERS_PER_TEAM]={

/***** Rage team *****/
	"G.Theobald",
	0,0,0,0,0,0,0,	//shirt,skin_col,head,hair,hairstyle,facetype,facehairstyle,bootcol

	"D.Clarke",
	1,0,0,0,0,0,1,

	"A.Argentieri",
	2,0,0,0,0,0,2,

	"S.Williams",
	3,0,0,0,0,0,0,

	"T.McCabe",
	4,0,0,0,0,0,0,

	"P.Kerby",
	5,0,0,0,0,0,1,

	"J.Smedley",
	6,0,0,0,0,0,3,

	"L.Seabridge",
	7,0,0,0,0,0,0,

	"J.ODowd",
	8,0,0,0,0,0,0,

	"R.Morrall",
	9,0,0,0,0,0,0,

	"C.Dolman",
	10,0,0,0,0,0,3,

	"D.Cook",
	11,0,0,0,0,0,0,

	"S.Johnson",
	12,0,0,0,0,0,0,

	"R.Mann",
	13,0,0,0,0,0,0,

	"T.Williams",
	14,0,0,0,0,0,0,

	"J.Curtis",
	15,0,0,0,0,0,1,

	"J.Court",
	16,0,0,0,0,0,0,

	"C.Southgate",
	17,0,0,0,0,0,0,

	"D.Hunt",
	18,0,0,0,0,0,0,

	"J.Widdows",
	19,0,0,0,0,0,9,

	"A.Taylor",
	20,0,0,0,0,0,0,

	"D.Bent",
	21,0,0,0,0,0,0,




/****** Biscuits ******/

	"Wagon wheel",
	0,0,0,0,0,0,0,

	"Bourbon cream",
	1,0,0,0,0,0,0,

	"Pink wafer",
	2,0,0,0,0,0,0,

	"Digestive",
	3,0,0,0,0,0,0,

	"Custard cream",
	4,0,0,0,0,0,0,

	"Jammy dodger",
	5,0,0,0,0,0,0,

	"Garibaldi",
	6,0,0,0,0,0,0,

	"Fig roll",
	7,0,0,0,0,0,0,

	"Nice",
	8,0,0,0,0,0,0,

	"Hob nob",
	9,0,0,0,0,0,0,

	"Rich tea",
	10,0,0,0,0,0,0,

	"Cadburys finger",
	11,0,0,0,0,0,2,

	"Oatmeal crunch",
	12,0,0,0,0,0,0,

	"Malted milk",
	13,0,0,0,0,0,0,

	"Pengiun",
	14,0,0,0,0,0,0,

	"Taxi",
	15,0,0,0,0,0,0,

	"Lemon puff",
	16,0,0,0,0,0,0,

	"Viscount",
	17,0,0,0,0,0,1,

	"Yo-yo",
	18,0,0,0,0,0,0,

	"Coconut cream",
	19,0,0,0,0,0,0,

	"Ginger nut",
	20,0,0,0,0,0,0,

	"Blue riband",
	21,0,0,0,0,0,0,


/*** Bad Things!!!! ******************************************/	

	"Hard work",
	0,0,0,0,0,0,0,

	"School",
	1,0,0,0,0,0,0,

	"Vindaloo",
	2,0,0,0,0,0,0,

	"Windows",
	3,0,0,0,0,0,0,

	"Thrash metal",
	4,0,0,0,0,0,0,

	"Skodas",
	5,0,0,0,0,0,0,

	"Rainy day",
	6,0,0,0,0,0,0,

	"Dentist",
	7,0,0,0,0,0,0,

	"Monday",
	8,0,0,0,0,0,0,

	"Late train",
	9,0,0,0,0,0,0,

	"Richard",
	10,0,0,0,0,0,0,

	"Judy",
	11,0,0,0,0,0,0,

	"Traffic jam",
	12,0,0,0,0,0,0,

	"Mad dictator",
	13,0,0,0,0,0,0,

	"Flat beer",
	14,0,0,0,0,0,0,

	"Hollywood",
	15,0,0,0,0,0,0,

	"No milk",
	16,0,0,0,0,0,0,

	"Naff telly",
	17,0,0,0,0,0,0,

	"Nose bleed",
	18,0,0,0,0,0,0,

	"Bad games",
	19,0,0,0,0,0,0,

	"Homework",
	20,0,0,0,0,0,0,

	"Not us",
	21,0,0,0,0,0,0,






/*** Good things? **************************************************/	

	"Friday",
	0,0,0,0,0,0,0,

	"Loud music",
	1,0,0,0,0,0,0,

	"Holiday",
	2,0,0,0,0,0,0,

	"Fine wine",
	3,0,0,0,0,0,0,

	"Beer",
	4,0,0,0,0,0,0,

	"Nookie",
	5,0,0,0,0,0,0,

	"Jethro tull",
	6,0,0,0,0,0,0,

	"Sunny day",
	7,0,0,0,0,0,0,

	"TV",
	8,0,0,0,0,0,0,

	"Dog turd",
	9,0,0,0,0,0,0,

	"Chips",
	10,0,0,0,0,0,0,

	"Fizzy pop",
	11,0,0,0,0,0,0,

	"Chicken tikka",
	12,0,0,0,0,0,0,

	"RPG",
	13,0,0,0,0,0,0,

	"Screwdriver",
	14,0,0,0,0,0,0,

	"Shopping",
	15,0,0,0,0,0,0,

	"Chocolate",
	16,0,0,0,0,0,0,

	"Ferret",
	17,0,0,0,0,0,0,

	"Nice car",
	18,0,0,0,0,0,0,

	"Coffee",
	19,0,0,0,0,0,0,

	"Tea",
	20,0,0,0,0,0,0,

	"Ice cream",
	21,0,0,0,0,0,0,




/*** Foodville (replace pizzas!)***************************************************/	

	"Turnip",
	0,0,0,0,0,0,0,

	"Trifle",
	1,0,0,0,0,0,0,

	"Sausage",
	2,0,0,0,0,0,0,

	"Cheescake",
	3,0,0,0,0,0,0,

	"Burger",
	4,0,0,0,0,0,0,

	"Potato",
	5,0,0,0,0,0,0,

	"Fried egg",
	6,0,0,0,0,0,0,

	"Pea",
	7,0,0,0,0,0,0,

	"Bacon",
	8,0,0,0,0,0,0,

	"Carrot",
	9,0,0,0,0,0,0,

	"Chips",
	10,0,0,0,0,0,0,

	"Pasta",
	11,0,0,0,0,0,0,

	"Dumpling",
	12,0,0,0,0,0,0,

	"Gravy",
	13,0,0,0,0,0,0,

	"Butty",
	14,0,0,0,0,0,0,

	"Mint",
	15,0,0,0,0,0,0,

	"Pizza",
	16,0,0,0,0,0,0,

	"Toast",
	17,0,0,0,0,0,0,

	"Yorks pud",
	18,0,0,0,0,0,0,

	"Oyster",
	19,0,0,0,0,0,0,

	"Duck",
	20,0,0,0,0,0,0,

	"Chocolate",
	21,0,0,0,0,0,0,



/*** Anatomy squad ***************************************************/	

	"Hand",
	0,0,0,0,0,0,0,
	 
	"Elbow",
	1,0,0,0,0,0,0,

	"Artery",
	2,0,0,0,0,0,0,

	"Appendix",
	3,0,0,0,0,0,0,

	"Nose",
	4,0,0,0,0,0,0,

	"Anvil",
	5,0,0,0,0,0,0,

	"Kidney",
	6,0,0,0,0,0,0,

	"Eye",
	7,0,0,0,0,0,0,

	"Heart",
	8,0,0,0,0,0,0,

	"Finger",
	9,0,0,0,0,0,0,

	"Foot",
	10,0,0,0,0,0,0,

	"Brain",
	11,0,0,0,0,0,0,

	"Leg",
	12,0,0,0,0,0,0,

	"Epiglotis",
	13,0,0,0,0,0,0,

	"Spleen",
	14,0,0,0,0,0,0,

	"Stomach lining",
	15,0,0,0,0,0,0,
	 
	"Pancreas",
	16,0,0,0,0,0,0,

	"Lung",
	17,0,0,0,0,0,0,

	"Arm",
	18,0,0,0,0,0,0,

	"Duodenum",
	19,0,0,0,0,0,0,

	"Sphincter",
	20,0,0,0,0,0,0,

	"Ear",
	21,0,0,0,0,0,0,



/*** World XI ***************************************************/	

	"Chilavert",
	0,0,0,0,0,0,0,

	"Campbell",
	1,0,0,0,0,0,0,

	"Carlos",
	2,0,0,0,0,0,0,

	"De boer",
	3,0,0,0,0,0,0,

	"Blanc",
	4,0,0,0,0,0,0,

	"Zidane",
	5,0,0,0,0,0,0,

	"Ortega",
	6,0,0,0,0,0,0,

	"Finidi",
	7,0,0,0,0,0,0,

	"Ronaldo",
	8,0,0,0,0,0,0,

	"Owen",
	9,0,0,0,0,0,0,

	"Overmars",
	10,0,0,0,0,0,0,

	"Salas",
	11,0,0,0,0,0,0,

	"Petrescu",
	12,0,0,0,0,0,0,

	"Schmeichel",
	13,0,0,0,0,0,0,

	"Del piero",
	14,0,0,0,0,0,0,

	"Vieri",
	15,0,0,0,0,0,0,

	"Ayala",
	16,0,0,0,0,0,0,

	"Suker",
	17,0,0,0,0,0,0,

	"Nakata",
	18,0,0,0,0,0,0,

	"Ladic",
	19,0,0,0,0,0,0,

	"Okocha",
	20,0,0,0,0,0,0,

	"West",
	21,0,0,0,0,0,0,


/*** Team eight ***************************************************/	

	"Zippy",
	0,0,0,0,0,0,0,

	"George",
	1,0,0,0,0,0,0,

	"Geoffrey",
	2,0,0,0,0,0,0,

	"Bungle",
	3,0,0,0,0,0,0,

	"Rod",
	4,0,0,0,0,0,0,

	"Jane",
	5,0,0,0,0,0,0,

	"Freddy",
	6,0,0,0,0,0,0,

	"Humpty",
	7,0,0,0,0,0,0,

	"Gemmima",
	8,0,0,0,0,0,0,

	"Hamble",
	9,0,0,0,0,0,0,

	"Big Ted",
	10,0,0,0,0,0,0,

	"Little Ted",
	11,0,0,0,0,0,0,

	"Father Ted",
	12,0,0,0,0,0,0,

	"Father Christmas",
	13,0,0,0,0,0,0,

	"Happy Christmas",
	14,0,0,0,0,0,0,

	"Happy Days",
	15,0,0,0,0,0,0,

	"The Fonz",
	16,0,0,0,0,0,0,

	"Arnold",
	17,0,0,0,0,0,0,

	"Joey",
	18,0,0,0,0,0,0,

	"Marion",
	19,0,0,0,0,0,0,

	"Pottsy",
	20,0,0,0,0,0,0,

	"Brad",
	21,0,0,0,0,0,0,


};



/****************************************************
 *
 * Lolly texts
 *
 ****************************************************/
BYTE *edit_name_txt[]={
	"Edit Name",
	"fEdit name",
	"gEdit name",
	"iEdit name",
	"sEdit name",
	"dEdit name"
};

BYTE *edit_player_txt[]={
	"Edit Player", 
	"fEdit player",
	"gEdit player", 
	"iEdit player",
	"sEdit player", 
	"dEdit player",
};

BYTE *edit_team_txt[]={
	"Edit Team",
	"fEdit team",
	"gEdit team",
	"iEdit team",
	"sEdit team",
	"dEdit team",
};

BYTE *opt_lolly_text[]={
	"Options\n",
	"fOptions\n",
	"gOptions\n",
	"iOptions\n",
	"sOptions\n",
	"dOptions\n",
};

BYTE *audio_config_lolly_text[]={
	"Audio Adjust\n",
	"fAudio adjust\n",
	"gAudio adjust\n",
	"iAudio adjust\n",
	"sAudio adjust\n",
	"dAudio adjust\n",
};

BYTE *screen_pos_lolly_text[]={
	"Position Screen\n",
	"fPosition screen\n",
	"gPosition screen\n",
	"iPosition screen\n",
	"sPosition screen\n",
	"dPosition screen\n",
};


BYTE *save_config_lolly_text[]={
	"Save Configuration",
	"fSave configuration",
	"gSave configuration",
	"iSave configuration",
	"sSave configuration",
	"dSave configuration",
};

BYTE *save_game_lolly_text[]={
	"Save a Game",
	"fSave a game",
	"gSave a game",
	"iSave a game",
	"sSave a game",
	"dSave a game",
};







/*************** FRONTEND.C TEXT *******************/




BYTE *group_text[]={
	"Group %d",
	"fGroup %d",
	"gGroup %d",
	"iGroup %d",
	"sGroup %d",
	"dGroup %d",
};




BYTE *thread_quit_text[MAX_LANGS]={
	
	"Are You Sure You Want\nTo Return To The\nTitle Screen?",
#if 1
	"fAre you sure you want\nto return to the\nTitle screen?",
	"gAre you sure you want\nto return to the\nTitle screen?",
	"iAre you sure you want\nto return to the\nTitle screen?",
	"sAre you sure you want\nto return to the\nTitle screen?",
	"dAre you sure you want\nto return to the\nTitle screen?",
#endif
};


BYTE *thread_game_over_text[MAX_LANGS][ TOTAL_GAME_TYPES ]={
	"poo",
	"No Human Team Came\nTop Of The League\nGame Over",
	"No Human Team Qualified\nFor The Knockout\nGame Over",
	"You Failed To\nWin This Territory\nGame Over",
	"poo",
	"No Human Team Qualified\nFor The Final\nGame Over",
	"Final poo",
	"No Human Team Qualified\nFor The World Cup\nGame Over",
	"No Human Team Qualified\nFor The Final\nGame Over",
	"No Human Team Qualified\nFor The Final\nGame Over",
	"No Human Team Remains In\nThe Final\nGame Over",

#if 1
	"fpoo",
	"fNo human team came\ntop of the League\nGame Over",
	"fNo human team qualified\nFor the Knockout\nGame Over",
	"fYou failed to\nWin this territory\nGame Over",
	"fpoo",
	"fNo human team qualified\nFor the Final\nGame Over",
	"fFinal poo",
	"fNo human team qualified\nFor the World Cup\nGame Over",
	"fNo human team qualified\nFor the Final\nGame Over",
	"fNo human team qualified\nFor the Final\nGame Over",
	"fNo human team remains in\nThe Final\nGame Over",
#endif
};


BYTE *preset_save_request[]={
	"Are You Sure You Want\nTo Overwrite Preset %d?\n",

#if 1
	"fAre you sure you want\nTo save into preset %d?\n",
	"gAre you sure you want\nTo save into preset %d?\n",
	"iAre you sure you want\nTo save into preset %d?\n",
	"sAre you sure you want\nTo save into preset %d?\n",
	"dAre you sure you want\nTo save into preset %d?\n",
#endif
};


BYTE *game_types_text[ MAX_LANGS ][ TOTAL_GAME_TYPES ]={
	"Friendly",
	"League",
	"Knockout",		
	"Territories Cup", 	
	"Classic Match",	
	"Euro 2000 Qualifiers",	
	"Euro 2000 Finals",		
	"World Cup '98",		
	"Euro 2000 Qualifiers",
	"Euro 2000 2nd Stage",	
	"Euro 2000 2nd Stage",	
	"Configuration",
	"Configuration",
	"Training",

#if 1

	"fFriendly",
	"fLeague",
	"fKnockout",		
	"fTerritories Cup", 	
	"fClassic Match",	
	"fEuro 2000 Qualifiers",	
	"fEuro 2000 Finals",		
	"fWorld Cup '98",		
	"fEuro 2000 Qualifiers",
	"fEuro 2000 2nd Stage",	
	"fEuro 2000 2nd Stage",	
	"fConfiguration",
	"fConfiguration",
	"fTraining",

	"gFriendly",
	"gLeague",
	"gKnockout",		
	"gTerritory cup", 	
	"gClassic match",	
	"gEuro 2000 Qualifiers",	
	"gEuro 2000 Final",		
	"gWorld Cup 98",		
	"gEuro 2000 Qualifiers",
	"gEuro 2000 2nd stage",	
	"gEuro 2000 2nd stage",	
	"gpoo",
	"gConfiguration",
	"gTraining",

	"Friendly",
	"League",
	"Knockout",		
	"Territory cup", 	
	"Classic match",	
	"Euro 2000 Qualifiers",	
	"Euro 2000 Final",		
	"World Cup 98",		
	"Euro 2000 Qualifiers",
	"Euro 2000 2nd stage",	
	"Euro 2000 2nd stage",	
	"poo",
	"Configuration",
	"Configuration",

	"Friendly",
	"League",
	"Knockout",		
	"Territory cup", 	
	"Classic match",	
	"Euro 2000 Qualifiers",	
	"Euro 2000 Final",		
	"World Cup 98",		
	"Euro 2000 Qualifiers",
	"Euro 2000 2nd stage",	
	"Euro 2000 2nd stage",	
	"poo",
	"Configuration",
	"Configuration",

#endif
	
};	




BYTE *preset_not_enough_pads_req[]={
	"This Match Requires At\nLeast Two Controllers\n",

#if 1
	"fThis match requires at\nleast two controllers\n",
	"gThis match requires at\nleast two controllers\n",
	"iThis match requires at\nleast two controllers\n",
	"sThis match requires at\nleast two controllers\n",
	"dThis match requires at\nleast two controllers\n",
#endif
};


BYTE *humcpu_txt[MAX_LANGS][2]={
	"player",
	"cpu",

	"fhuman",
	"fcpu",

	"ghuman",
	"gcpu",

	"ihuman",
	"icpu",

	"shuman",
	"scpu",

	"dhuman",
	"dcpu",


};


BYTE *autoload_conf_file_txt[]={
	"Loading Configuration File\nPlease Wait",
	"fAutoloading configuration file\nPlease Wait",
	"gAutoloading configuration file\nPlease Wait",
	"iAutoloading configuration file\nPlease Wait",
	"sAutoloading configuration file\nPlease Wait",
	"dAutoloading configuration file\nPlease Wait",
};







/********* Team edit text ************/
BYTE *pl_ed_number_names[]= { 
	"Number",
	"Number",
	"gNumber",
	"iNumber",
	"sNumber",
	"dNumber",
};

BYTE *pl_ed_player_words[2][6]={
	"Player",
	"fPlayer",
	"gPlayer",
	"iPlayer",
	"sPlayer",
	"dPlayer",

	"Goalkeeper",
	"fPlayer",
	"gPlayer",
	"iPlayer",
	"sPlayer",
	"dPlayer",

};



BYTE *pl_ed_button_names[][9]= { 
	"Change Name\n",
	"Face Type\n",
	"Facial Hair\n",
	"Skin Color\n",
	"Hair Style\n",
  	"Hair Color\n",
	"Boot Color\n",
	0,
	0,

#if 1
	"fChange name\n",
	"fFace type\n",
	"fFacial hair\n",
	"fSkin color\n",
	"fHair style\n",
  	"fHair color\n",
	"fBoot color\n",
	0,
	0,

	"Change name\n",
	"Face type\n",
	"Facial hair\n",
	"Skin color\n",
	"Hair style\n",
  	"Hair color\n",
	"Boot color\n",
	0,
	0,

	"Change name\n",
	"Face type\n",
	"Facial hair\n",
	"Skin color\n",
	"Hair style\n",
  	"Hair color\n",
	"Boot color\n",
	0,
	0,

	"Change name\n",
	"Face type\n",
	"Facial hair\n",
	"Skin color\n",
	"Hair style\n",
  	"Hair color\n",
	"Boot color\n",
	0,
	0,

	"Change name\n",
	"Face type\n",
	"Facial hair\n",
	"Skin color\n",
	"Hair style\n",
  	"Hair color\n",
	"Boot color\n",
	0,
	0,
#endif
};


BYTE *team_words[]={
	"Team",
	"fteam",
	"gteam",
	"iteam",
	"steam",
	"dteam",
};



BYTE *te_ed_team_button_names[]={
	"Team",
	"fteam",
	"gteam",
	"iteam",
	"steam",
	"dteam",
};



BYTE *te_ed_shirt_txt[]={
	"Shirt",
	"fShirt",
	"gShirt",
	"iShirt",
	"sShirt",
	"dShirt",
};

BYTE *te_ed_sleeve_txt[]={
	"Sleeves",
	"fSleeves",
	"gSleeves",
	"iSleeves",
	"sSleeves",
	"dSleeves",
};

BYTE *te_ed_shorts_txt[]={
	"Shorts",
	"fShorts",
	"gShorts",
	"iShorts",
	"sShorts",
	"dShorts",
};

BYTE *te_ed_socks_txt[]={
	"Socks",
	"fSocks",
	"gSocks",
	"iSocks",
	"sSocks",
	"dSocks",
};






/************* Config *****************/
BYTE *conf_save_req[MAX_LANGS]={
	"Configuration Has Changed\nDo You Wish To Save To A Memory Card?",
	"fConfiguration has changed\nDo you wish to save to a Memory Card?",
	"gConfiguration has changed\nDo you wish to save to a Memory Card?",
	"iConfiguration has changed\nDo you wish to save to a Memory Card?",
	"sConfiguration has changed\nDo you wish to save to a Memory Card?",
	"dConfiguration has changed\nDo you wish to save to a Memory Card?",
};

BYTE *pc_button_text[][8]={
	"High Action",
	"Pass",
	"Shoot",
	"Low Action",
	"Sprint",
	"Control",
	"Push",
	"Change Player",

#if 1
	"fHigh action",
	"fPass",
	"fShoot",
	"fLow action",
	"fSprint",
	"fControl",
	"fPush",
	"fChange player",

	"High action",
	"Pass",
	"Shoot",
	"Low action",
	"Sprint",
	"Control",
	"Push",
	"Change player",

	"High action",
	"Pass",
	"Shoot",
	"Low action",
	"Sprint",
	"Control",
	"Push",
	"Change player",

	"High action",
	"Pass",
	"Shoot",
	"Low action",
	"Sprint",
	"Control",
	"Push",
	"Change player",

	"High action",
	"Pass",
	"Shoot",
	"Low action",
	"Sprint",
	"Control",
	"Push",
	"Change player",
#endif
};



BYTE *restore_config_lolly_text[]={
	"Restore Configuration",
	"fRestore configuration",
	"gRestore configuration",
	"iRestore configuration",
	"sRestore configuration",
	"dRestore configuration",
};

BYTE *restore_config_txt[]={
	"Restore Default\nConfiguration Settings?",
	"fRestore default\nconfiguration settings?",
	"gRestore default\nconfiguration settings?",
	"iRestore default\nconfiguration settings?",
	"sRestore default\nconfiguration settings?",
	"dRestore default\nconfiguration settings?",
};

BYTE *config_restored[]={
	"Configuration Restored\n",
	"fConfiguration restored\n",
	"gConfiguration restored\n",
	"iConfiguration restored\n",
	"sConfiguration restored\n",
	"dConfiguration restored\n",
};


BYTE *pad_config_words[]={
	"Config",
	"fConfig",
	"gConfig",
	"iConfig",
	"sConfig",
	"dConfig",
};
BYTE *sp_info_text[]={
	"Press X To\nPosition screen",
	"fPress X to\nposition screen",
	"gPress X to\nposition screen",
	"iPress X to\nposition screen",
	"sPress X to\nposition screen",
	"dPress X to\nposition screen",
};


BYTE *audio_word1[][3]={
	"Mono\n",
	"Stereo\n",
	"Inverse Stereo\n",

	"fmono\n",
	"fstereo\n",
	"finverse stereo\n",

	"gmono\n",
	"gstereo\n",
	"ginverse stereo\n",

	"imono\n",
	"istereo\n",
	"iinverse stereo\n",

	"smono\n",
	"sstereo\n",
	"sinverse stereo\n",

	"dmono\n",
	"dstereo\n",
	"dinverse stereo\n",


};

BYTE *audio_word2[]={
	"Sound Mode : ",
	"fsound mode :",
	"gsound mode",
	"isound mode",
	"ssound mode",
	"dsound mode",
};


BYTE *ac_tgad_text[][4]={
	"Master Volume",
	"Music Volume",
	"Commentary Volume",
	"Sfx Volume",

#if 1
	"fMaster volume\n",
	"fMusic volume\n",
	"fcommentary volume\n",
	"fsfx volume\n",

	"Master volume\n",
	"Music volume\n",
	"commentary volume\n",
	"sfx volume\n",

	"Master volume\n",
	"Music volume\n",
	"commentary volume\n",
	"sfx volume\n",

	"Master volume\n",
	"Music volume\n",
	"commentary volume\n",
	"sfx volume\n",

	"Master volume\n",
	"Music volume\n",
	"commentary volume\n",
	"sfx volume\n",
#endif

};


BYTE *pad_config_lolly_txt[]={
	"Configure Controller",
	"fConfigure controller",
	"gConfigure controller",
	"iConfigure controller",
	"sConfigure controller",
	"dConfigure controller",
};


BYTE *pad_assign_lolly_txt[]={
	"Assign Controller",
	"fAssign controller",
	"gAssign controller",
	"iAssign controller",
	"sAssign controller",
	"dAssign controller",
};




/******** Knockout ********/

BYTE *knock_next_match_lolly_text[]={
	"Next Match",
	"fNext match",
	"gNext match",
	"iNext match",
	"sNext match",
	"dNext match",
};

BYTE *winner_lolly_text[]={
	"Winner",
	"fWinner",
	"gWinner",
	"iWinner",
	"sWinner",
	"dWinner",
};

BYTE *winners_lolly_text[]={
	"Winners",
	"fWinners",
	"gWinners",
	"iWinners",
	"sWinners",
	"dWinners",
};



/************* League *****************/
BYTE *euro_finalist_teams_lolly_text[]={
	"Finalists",
	"fFinalists",
	"gFinalists",
	"iFinalists",
	"sFinalists",
	"dFinalists",
};

BYTE *eq_group_winner_text[]={
	"Group winners",
	"fGroup winners",
	"gGroup winners",
	"iGroup winners",
	"sGroup winners",
	"dGroup winners",
};

BYTE *eq_group_runner_text[]={
	"Group Runner Up",
	"fGroup runner up",
	"gGroup runner up",
	"iGroup runner up",
	"sGroup runner up",
	"dGroup runner up",
};

BYTE *euro_game_over_lolly_text[]={
	"Final Standings",
	"fFinal standings",
	"gFinal standings",
	"iFinal standings",
	"sFinal standings",
	"dFinal standings",
};

BYTE *euro_result_lolly_text[]={
	"Results",
	"fResults",
	"gResults",
	"iResults",
	"sResults",
	"dResults",
};

BYTE *euro_next_match_lolly_text[]={
	"Next Match",
	"fNext match",
	"gNext match",
	"iNext match",
	"sNext match",
	"dNext match",
};

BYTE *euro_qual_teams_lolly_text[]={
	"Qualifiers",
	"fQualifiers",
	"gQualifiers",
	"iQualifiers",
	"sQualifiers",
	"dQualifiers",
};

BYTE *worldcup_game_over_lolly_text[]={
	"End Of Round 1",
	"fEnd of round 1",
	"gEnd of round 1",
	"iEnd of round 1",
	"sEnd of round 1",
	"dEnd of round 1",
};

BYTE *league_stat_lolly_text[]={
	"League Standings",
	"fLeague standings",
	"gLeague standings",
	"iLeague standings",
	"sLeague standings",
	"dLeague standings",
};


BYTE *final_league_stat_lolly_text[]={
	"Final League Standings",
	"fFinal league standings",
	"gFinal league standings",
	"iFinal league standings",
	"sFinal league standings",
	"dFinal league standings",
};



BYTE *territory_stat_lolly_text[6][MAX_LANGS]={
	"Europe 1 Standings",
	"fSouth American standings",
	"gSouth American standings",
	"iSouth American standings",
	"sSouth American standings",
	"dSouth American standings",

	"Europe 2 Standings",
	"fAmericas standings",
	"gAmericas standings",
	"iAmericas standings",
	"sAmericas standings",
	"dAmericas standings",

	"Europe 3 Standings",
	"fAsian standings",
	"gAsian standings",
	"iAsian standings",
	"sAsian standings",
	"dAsian standings",

	"The Americas Standings",
	"fEuropean standings",
	"gEuropean standings",
	"iEuropean standings",
	"sEuropean standings",
	"dEuropean standings",

	"Africa Standings",
	"Africa Standings",
	"Africa Standings",
	"Africa Standings",
	"Africa Standings",
	"Africa Standings",

	"South Americas Standings",
	"South Americas Standings",
	"South Americas Standings",
	"South Americas Standings",
	"South Americas Standings",
	"South Americas Standings",


};

BYTE *final_territory_stat_lolly_text[6][MAX_LANGS]={

	"Final Europe 1 Standings",
	"fSouth American standings",
	"gSouth American standings",
	"iSouth American standings",
	"sSouth American standings",
	"dSouth American standings",

	"Final Europe 2 Standings",
	"fAmericas standings",
	"gAmericas standings",
	"iAmericas standings",
	"sAmericas standings",
	"dAmericas standings",

	"Final Europe 3 Standings",
	"fAsian standings",
	"gAsian standings",
	"iAsian standings",
	"sAsian standings",
	"dAsian standings",

	"Final The Americas Standings",
	"fEuropean standings",
	"gEuropean standings",
	"iEuropean standings",
	"sEuropean standings",
	"dEuropean standings",

	"Final Africa Standings",
	"Africa Standings",
	"Africa Standings",
	"Africa Standings",
	"Africa Standings",
	"Africa Standings",

	"Final South Americas Standings",
	"South Americas Standings",
	"South Americas Standings",
	"South Americas Standings",
	"South Americas Standings",
	"South Americas Standings",


};


BYTE *lstat_txt[][8]={
	"Pld", "W", "D", "L", "F", "A", "GD", "Pts",
	"fPld", "W", "D", "L", "F", "A", "GD", "Pts",
	"gPld", "W", "D", "L", "F", "A", "GD", "Pts",
	"iPld", "W", "D", "L", "F", "A", "GD", "Pts",
	"sPld", "W", "D", "L", "F", "A", "GD", "Pts",
	"dPld", "W", "D", "L", "F", "A", "GD", "Pts",
};



/******** Memcard ***********/
BYTE *save_config_prompt[]={
	"Overwrite Current\nConfiguration?",

#if 1
	"fOverwrite current\nconfiguration?",
	"gOverwrite current\nconfiguration?",
	"iOverwrite current\nconfiguration?",
	"sOverwrite current\nconfiguration?",
	"dOverwrite current\nconfiguration?",
#endif
};

BYTE *load_game_lolly_text[]={
	"Load A Game",
	"fLoad a game",
	"gLoad a game",
	"iLoad a game",
	"sLoad a game",
	"dLoad a game",
};

BYTE *mc_free_slot_text[2][MAX_LANGS]={
	"Create New Game",
	"fcreate new game",
	"gcreate new game",
	"icreate new game",
	"screate new game",
	"dcreate new game",

	"Create New Config",
	"fcreate new config",
	"gcreate new config",
	"icreate new config",
	"screate new config",
	"dcreate new config",

};



BYTE *mc_delete_text[]={
	"Select A Block To Delete",
	"fSelect a block to delete",
	"gSelect a block to delete",
	"iSelect a block to delete",
	"sSelect a block to delete",
	"dSelect a block to delete",
};

BYTE *mc_overwrite_text[]={
	"Select A Block To Overwrite",
	"fSelect a block to overwrite",
	"gSelect a block to overwrite",
	"iSelect a block to overwrite",
	"sSelect a block to overwrite",
	"dSelect a block to overwrite",
};

BYTE *mc_loading_text[]={
	"Loading Data\nDo Not Remove Memory Card",
	"fLoading data\nDo not remove Memory card",
	"gLoading data\nDo not remove Memory card",
	"iLoading data\nDo not remove Memory card",
	"sLoading data\nDo not remove Memory card",
	"dLoading data\nDo not remove Memory card",
};

BYTE *mc_load_info_text[]={
	"Select A Block To Load",
	"fSelect a block to load",
	"gSelect a block to load",
	"iSelect a block to load",
	"sSelect a block to load",
	"dSelect a block to load",
};

BYTE *mc_load_text[]={
	"Load Game",
	"fLoad game",
	"gLoad game",
	"iLoad game",
	"sLoad game",
	"dLoad game",
};

BYTE *mc_saving_text[]={
	"Saving Data...Do Not Remove Memory Card!",
	"fSaving data...Do not remove Memory card!",
	"gSaving data...Do not remove Memory card!",
	"iSaving data...Do not remove Memory card!",
	"sSaving data...Do not remove Memory card!",
	"dSaving data...Do not remove Memory card!",
};

BYTE *mc_overwriting_text[]={
	"Overwriting Data..Do Not Remove Memory Card",
	"fOverwriting data..Do not remove Memory card",
	"gOverwriting data..Do not remove Memory card",
	"iOverwriting data..Do not remove Memory card",
	"sOverwriting data..Do not remove Memory card",
	"dOverwriting data..Do not remove Memory card",
};


BYTE *mc_no_cards_present_text[]={
	"Please Insert A Memory Card \nInto Either Memory Card Slot",
	"fPlease insert a Memory card \nInto either Memory card slot",
	"gPlease insert a Memory card \nInto either Memory card slot",
	"iPlease insert a Memory card \nInto either Memory card slot",
	"sPlease insert a Memory card \nInto either Memory card slot",
	"dPlease insert a Memory card \nInto either Memory card slot",
};

BYTE *mc_no_lt_card_present_text[]={
	"Please Insert A Memory Card \nInto Memory Card Slot 1",
	"fPlease insert a Memory card \nInto Memory card slot 1",
	"gPlease insert a Memory card \nInto Memory card slot 1",
	"iPlease insert a Memory card \nInto Memory card slot 1",
	"sPlease insert a Memory card \nInto Memory card slot 1",
	"dPlease insert a Memory card \nInto Memory card slot 1",
};

BYTE *mc_no_lt_card_present_text_tap[]={
	"Please Insert A Memory Card \nInto Memory Card Slot 1 or 1a",
	"fPlease insert a Memory card \nInto Memory card slot 1 or 1a",
	"gPlease insert a Memory card \nInto Memory card slot %s",
	"iPlease insert a Memory card \nInto Memory card slot %s",
	"sPlease insert a Memory card \nInto Memory card slot %s",
	"dPlease insert a Memory card \nInto Memory card slot %s",
};


BYTE *mc_save_ok_text[]={
	"Block '%s'\nSaved Successfully",
	"fBlock '%s'\nSaved successfully",
	"gBlock '%s'\nSaved successfully",
	"iBlock '%s'\nSaved successfully",
	"sBlock '%s'\nSaved successfully",
	"dBlock '%s'\nSaved successfully",
};

BYTE *mc_load_ok_text[]={
	"Block '%s'\nLoaded Successfully",
	"fBlock '%s'\nLoaded successfully",
	"gBlock '%s'\nLoaded successfully",
	"iBlock '%s'\nLoaded successfully",
	"sBlock '%s'\nLoaded successfully",
	"dBlock '%s'\nLoaded successfully",
};


BYTE *mc_overwrite_ok_text[]={
	"Block '%s'\nOverwritten Successfully",
	"fBlock '%s'\nOverwritten successfully",
	"gBlock '%s'\nOverwritten successfully",
	"iBlock '%s'\nOverwritten successfully",
	"sBlock '%s'\nOverwritten successfully",
	"dBlock '%s'\nOverwritten successfully",
};

BYTE *mc_save_cancelled_text[]={
	"Save Cancelled",
	"fSave cancelled",
	"gSave cancelled",
	"iSave cancelled",
	"sSave cancelled",
	"dSave cancelled",
};

BYTE *mc_load_cancelled_text[]={
	"Load Cancelled",
	"fLoad cancelled",
	"gLoad cancelled",
	"iLoad cancelled",
	"sLoad cancelled",
	"dLoad cancelled",
};


BYTE *mc_write_error_text[]={
	"Save Failed! Please Try Again",
	"fSave failed! Please try again",
	"gSave failed! Please try again",
	"iSave failed! Please try again",
	"sSave failed! Please try again",
	"dSave failed! Please try again",
};

BYTE *mc_rewrite_text[]={
	"Save Failed! Try Again?",
	"fSave failed! Try again?",
	"gSave failed! Try again?",
	"iSave failed! Try again?",
	"sSave failed! Try again?",
	"dSave failed! Try again?",
};


BYTE *mc_load_error_text[]={
	"Load Failed! Please Try Again",
	"fLoad failed! Please try again",
	"gLoad failed! Please try again",
	"iLoad failed! Please try again",
	"sLoad failed! Please try again",
	"dLoad failed! Please try again",
};

BYTE *mc_delete_error_text[]={
	"Delete Failed! Please Try Again",
	"fDelete failed! Please try again",
	"gDelete failed! Please try again",
	"iDelete failed! Please try again",
	"sDelete failed! Please try again",
	"dDelete failed! Please try again",
};

BYTE *mc_save_new_text[]={
	"Save New Game", 
	"fSave new game", 
	"gSave new game", 
	"iSave new game", 
	"sSave new game", 
	"dSave new game", 
};


BYTE *mc_no_specific_game_files_text[]={
	"Error\nCannot Find Any\n'%s'\nBlocks To Load",
	"fError\nCannot find any\n'%s'\nBlocks to Load",
	"gError\nCannot find any\n'%s'\nBlocks to Load",
	"iError\nCannot find any\n'%s'\nBlocks to load",
	"sError\nCannot find any\n'%s'\nBlocks to load",
	"dError\nCannot find any\n'%s'\nBlocks to load",
};

BYTE *mc_are_you_sure_overwrite_text[]={
	"Overwrite Block '%s'?",
	"foverwrite block '%s'?",
	"goverwrite block '%s'?",
	"ioverwrite block '%s'?",
	"soverwrite block '%s'?",
	"doverwrite block '%s'?",
};

BYTE *mc_are_you_sure_overwrite_with_text[]={
	 "Overwrite Block '%s'\nWith '%s'?",
	"fOverwrite Block '%s'\nWith '%s'?",
	"gOverwrite Block '%s'\nWith '%s'?",
	"iOverwrite Block '%s'\nWith '%s'?",
	"sOverwrite Block '%s'\nWith '%s'?",
	"dOverwrite Block '%s'\nWith '%s'?",
};

BYTE *mc_are_you_sure_del_text[]={
	"Are You Sure You Want To\nDelete Block '%s'?",
	"fAre you sure you want to\ndelete block '%s'?",
	"gAre you sure you want to\ndelete block '%s'?",
	"iAre you sure you want to\ndelete block '%s'?",
	"sAre you sure you want to\ndelete block '%s'?",
	"dAre you sure you want to\ndelete block '%s'?",
};

BYTE *mc_deleting_text[]={
	"Deleting Block",
	"fDeleting block",
	"gDeleting block",
	"iDeleting block",
	"sDeleting block",
	"dDeleting block",
};

BYTE *mc_overwrite_error_text[]={
	"Error Overwriting Block\nOperation Cancelled",
	"fError overwriting block\nOperation cancelled",
	"gError overwriting block\nOperation cancelled",
	"iError overwriting block\nOperation cancelled",
	"sError overwriting block\nOperation cancelled",
	"dError overwriting block\nOperation cancelled",
};


BYTE *mc_select_overwrite_text[]={
	"Select a Block To Overwrite", 
	"fSelect a block to overwrite", 
	"gSelect a block to overwrite", 
	"iSelect a block to overwrite", 
	"sSelect a block to overwrite", 
	"dSelect a block to overwrite", 

};


BYTE *mc_overwrite_cancelled_text[]={
	"Overwrite Cancelled",
	"fOverwrite cancelled",
	"gOverwrite cancelled",
	"iOverwrite cancelled",
	"sOverwrite cancelled",
	"dOverwrite cancelled",
};

BYTE *mc_delete_cancelled_text[]={
	"Delete Cancelled",
	"fDelete cancelled",
	"gDelete cancelled",
	"iDelete cancelled",
	"sDelete cancelled",
	"dDelete cancelled",
};

BYTE *mc_please_wait_text[]={
	"Please Wait...Checking Memory Cards",
	"fPlease wait...Checking Memory cards",
	"gPlease wait...Checking Memory cards",
	"iPlease wait...Checking Memory cards",
	"sPlease wait...Checking Memory cards",
	"dPlease wait...Checking Memory cards",
};


BYTE *mc_unformat_text[]={
	"Memory Card In Memory Card Slot %s Is Unformatted.\nFormat Memory Card?",

#if 1
	"fMemory card in Memory card slot %s is unformatted.\nFormat Memory card?",
	"gMemory card in Memory card slot %s is unformatted.\nFormat Memory card?",
	"iMemory card in Memory card slot %s is unformatted.\nFormat Memory card?",
	"sMemory card in Memory card slot %s is unformatted.\nFormat Memory card?",
	"dMemory card in Memory card slot %s is unformatted.\nFormat Memory card?",
#endif
};

BYTE *mc_reformat_text[]={
	"Error Formatting Card\nDo You Wish To Re-Format It?",
	"fError formatting card\ndo you wish to re-format it?",
	"gError formatting card\ndo you wish to re-format it?",
	"iError formatting card\ndo you wish to re-format it?",
	"sError formatting card\ndo you wish to re-format it?",
	"dError formatting card\ndo you wish to re-format it?",
};

BYTE *mc_format_text[]={
	"Formatting Card\nPlease Wait",
	"fFormatting card\nPlease wait",
	"gFormatting card\nPlease wait",
	"iFormatting card\nPlease wait",
	"sFormatting card\nPlease wait",
	"dFormatting card\nPlease wait",
};

BYTE *mc_format_done_text[]={
	"Format Successful",
	"fFormat successful",
	"gFormat successful",
	"iFormat successful",
	"sFormat successful",
	"dFormat successful",
};

BYTE *mc_no_slots_to_overwrite_text[]={
	"Memory Card is Full\nInsert a Memory Card With At Least\n1 Free Block or Delete Some Blocks\nUsing the Console's Internal\nMemory Card Manager",
	"fMemory card is full\nInsert a memory card with at least\n1 free block or delete some blocks\nusing the Console's internal\nMemory Card Manager",
	"gMemory card is full\nInsert a memory card with at least\n1 free block or delete some blocks\nusing the Console's internal\nMemory Card Manager",
	"iMemory card is full\nInsert a memory card with at least\n1 free block or delete some blocks\nusing the Console's internal\nMemory Card Manager",
	"sMemory card is full\nInsert a memory card with at least\n1 free block or delete some blocks\nusing the Console's internal\nMemory Card Manager",
	"dMemory card is full\nInsert a memory card with at least\n1 free block or delete some blocks\nusing the Console's internal\nMemory Card Manager",
};

BYTE *card_num_text[]={
	"Slot",
	"fSlot",
	"gSlot",
	"iSlot",
	"sSlot",
	"dSlot",
};

BYTE *only_text[]={
	"Only",	 // as in "save to slot B only"
	"fonly",
	"gonly",
	"ionly",
	"sonly",
	"donly",
};




/*********** Options ***********/
BYTE *opt_txts[MAX_LANGS][TOTAL_OPTIONS]={
	"Weather",
	"Offsides",
	"Lighting",
	"Duration",
	"Extra Time",
	"Stadium",
	"Camera Mode",
	"Camera Dist",

#if 1
	"Weather",
	"Offsides",
	"Lighting",
	"Duration",
	"Extra Time",
	"Stadium",
	"Camera Mode",
	"Camera Dist",

	"Weather",
	"Offsides",
	"Lighting",
	"Duration",
	"Extra Time",
	"Stadium",
	"Camera Mode",
	"Camera Dist",

	"Weather",
	"Offsides",
	"Lighting",
	"Duration",
	"Extra Time",
	"Stadium",
	"Camera Mode",
	"Camera Dist",

	"Weather",
	"Offsides",
	"Lighting",
	"Duration",
	"Extra Time",
	"Stadium",
	"Camera Mode",
	"Camera Dist",

	"Weather",
	"Offsides",
	"Lighting",
	"Duration",
	"Extra Time",
	"Stadium",
	"Camera Mode",
	"Camera Dist",

#endif
};


BYTE *opt_button_list4[]={

	"2 Mins",
	"5 Mins",
	"10 Mins",
	"20 Mins",
	"45 Mins",
	0,

#if 1
	"f2 mins",
	"f5 mins",
	"f10 mins",
	"f20 mins",
	"f45 mins",
	0,

	"g2 mins",
	"g5 mins",
	"g10 mins",
	"g20 mins",
	"g45 mins",
	0,

	"g2 mins",
	"g5 mins",
	"g10 mins",
	"g20 mins",
	"g45 mins",
	0,

	"g2 mins",
	"g5 mins",
	"g10 mins",
	"g20 mins",
	"g45 mins",
	0,

	"g2 mins",
	"g5 mins",
	"g10 mins",
	"g20 mins",
	"g45 mins",
	0,
#endif
};	  


BYTE *opt_button_list6[]={
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	0,

	"fA",
	"fB",
	"fC",
	"fD",
	"fE",
	"fF",
	"fG",
	"fH",
	0,

	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	0,

	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	0,

	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	0,

	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	0,


};

BYTE *opt_button_list5[]={
	"None",
	"Extra Time",
	"Penalites",
	"Golden Goal",
	"Extra Time\nand Penalties",
	"Golden Goal\nand Penalties",
	0,

#if 1
	"fExtra time",
	"fExtra time\nand Pens",
	"fGolden goal\nand pens",
	"fNone",
	"fPens and\nExtra time",
	"fPens and\nGolden Goal",
	0,

	"gExtra time",
	"gExtra time\nand Pens",
	"gGolden goal\nand pens",
	"gNone",
	"gPens and\nExtra time",
	"gPens and\nGolden Goal",
	0,

	"Extra time",
	"Extra time\nand Pens",
	"Golden goal\nand pens",
	"None",
	"Pens and\nExtra time",
	"Pens and\nGolden Goal",
	0,

	"Extra time",
	"Extra time\nand Pens",
	"Golden goal\nand pens",
	"None",
	"Pens and\nExtra time",
	"Pens and\nGolden Goal",
	0,

	"Extra Time",
	"Extra time\nand Pens",
	"Golden goal\nand pens",
	"None",
	"Pens and\nExtra time",
	"Pens and\nGolden Goal",
	0,
#endif

};	  

BYTE *opt_button_list2[]={
	"Off",
	"On",
	0,

	"fOff",
	"fOn",
	0,

	"Off",
	"On",
	0,

	"Off",
	"On",
	0,

	"Off",
	"On",
	0,

	"Off",
	"On",
	0,


};	  

BYTE *opt_button_list1[]={
	"Fine",
	"Rain",
	"Snow",
	"Random",
	0,

	"fFine",
	"fRain",
	"fSnow",
	"fRandom",
	0,

	"Sun",
	"Rain",
	"Snow",
	"Random",
	0,

	"Sun",
	"Rain",
	"Snow",
	"Random",
	0,

	"Sun",
	"Rain",
	"Snow",
	"Random",
	0,

	"Sun",
	"Rain",
	"Snow",
	"Random",
	0,


};	  

BYTE *opt_button_list3[]={
	"Night",
	"Day",
	0,

	"fNight",
	"fDay",
	0,

	"Night",
	"Day",
	0,

	"Day",
	"Night",
	0,

	"Day",
	"Night",
	0,

	"Day",
	"Night",
	0,


};	  

BYTE *opt_button_list7[]={
	"Sideline",
	"Bird's Eye",   
	0,

	"fRef 3",
	"fRef 4",
	0,

	"Ref 3",
	"Ref 4",
	0,

	"Ref 3",
	"Ref 4",
	0,

	"Ref 3",
	"Ref 4",
	0,

	"Ref 3",
	"Ref 4",
	0,

};	  


BYTE *opt_button_list8[]={
	"Near",
	"Middle",   
	"Far",   
	0,

	"Near",
	"Middle",   
	"Far",   
	0,

	"Near",
	"Middle",   
	"Far",   
	0,

};



/******* Preset **********/
BYTE *preset_save_lolly_text[]={
	"Save Friendly Settings",
	"fStore Friendly settings",
	"gStore Friendly settings",
	"iStore Friendly settings",
	"sStore Friendly settings",
	"dStore Friendly settings",
};

BYTE *preset_choose_lolly_text[]={
	"Choose a Preset Friendly",
	"fChoose a preset Friendly",
	"gChoose a preset Friendly",
	"iChoose a preset Friendly",
	"sChoose a preset Friendly",
	"dChoose a preset Friendly",
};

BYTE *empty_text[]={
	"Empty",
	"fEmpty",
	"gEmpty",
	"iEmpty",
	"sEmpty",
	"dEmpty",
};


/************ Teamsel ***********/

BYTE *def_team_lolly_text[]={
	"Select Teams\n",
	"fSelect teams\n",
	"gSelect teams\n",
	"iSelect teams\n",
	"sSelect teams\n",
	"dSelect teams\n",
};

BYTE *def_team_single_lolly_text[]={
	"Select Team\n",
	"fSelect Team\n",
	"gSelect Team\n",
	"iSelect Team\n",
	"sSelect team\n",
	"dSelect team\n",
};


BYTE *select_num_teams_lolly_text[]={
	"Team Attributes",
	"Team Attributes",
	"Team Attributes",
	"Team Attributes",
	"Team Attributes",
	"Team Attributes",
};


BYTE *snt_totteams_txt[]={
	"Total Teams",
	"fTotal teams",
	"gTotal teams",
	"iTotal teams",
	"sTotal teams",
	"dTotal teams",
};

BYTE *snt_humteams_txt[]={
	"Human Teams",
	"fHuman teams",
	"gHuman teams",
	"iHuman teams",
	"sHuman teams",
	"dHuman teams",
};


BYTE *snt_diff_txt1[][3]={
	"Easy",
	"Medium",
	"Hard",

	"fEasy",
	"fMedium",
	"fHard",

	"Easy",
	"Medium",
	"Hard",

	"Easy",
	"Medium",
	"Hard",

	"Easy",
	"Medium",
	"Hard",

	"Easy",
	"Medium",
	"Hard",


};				  

BYTE *snt_diff_txt2[]={
	"Difficulty",
	"fDifficulty",
	"gDifficulty",
	"iDifficulty",
	"sDifficulty",
	"dDifficulty",
};				  




/************* Territory **************/

BYTE *territ_select_lolly_text[]={
	"Choose a Territory",
	"fChoose a Territory",
	"gChoose a Territory",
	"iChoose a Territory",
	"sChoose a Territory",
	"dChoose a Territory",
};

BYTE *cm_select_lolly_text[]={
	"Choose a Classic Match",
	"fChoose a Classic Match",
	"gChoose a Classic Match",
	"iChoose a Classic Match",
	"sChoose a Classic Match",
	"dChoose a Classic Match",
};


BYTE *territory_names[][6]={
	"Europe 1",
	"Europe 2",
	"Europe 3",
	"The Americas",
	"Africa",
	"South America",

	"fSouth America",
	"fAmericas",
	"fAsia",
	"fEurope",
	"fAfrica",
	"fAustralasia",

	"gSouth America",
	"gAmericas",
	"gAsia",
	"gEurope",
	"gAfrica",
	"gAustralasia",

	"iSouth America",
	"iAmericas",
	"iAsia",
	"iEurope",
	"iAfrica",
	"iAustralasia",

	"sSouth America",
	"sAmericas",
	"sAsia",
	"sEurope",
	"sAfrica",
	"sAustralasia",

	"dSouth America",
	"dAmericas",
	"dAsia",
	"dEurope",
	"dAfrica",
	"dAustralasia",


};

BYTE *classic_match_texts[][8]={
	"Brazil are 2-0 down with only 6 minutes\nuntil I go home and make a nice cup of\ntea and put some arse arse cosy slippers\non in front of my steaming log effect fire\n\nPlay?",
	"Austria vs Azerbaijan\nAustria are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Bulgaria vs Morocco\nBulgaria are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"England vs Colombia\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Argentina vs Brazil\nBrazil are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Rhyl vs Northend\nRhyl are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Iran vs Hungary\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Republic of Ireland vs Rebublic of Korea\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",

#if 1
	"fBrazil are 2-0 down with only 6 minutes\nuntil I go home and make a nice cup of\ntea and put some arse arse cosy slippers\non in front of my steaming log effect fire\n\nPlay?",
	"fAustria vs Azerbaijan\nAustria are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"fBulgaria vs Morocco\nBulgaria are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"fEngland vs Colombia\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"fArgentina vs Brazil\nBrazil are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"fRhyl vs Northend\nRhyl are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"fIran vs Hungary\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"fRepublic of Ireland vs Rebublic of Korea\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",

	"Brazil are 2-0 down with only 6 minutes\nuntil I go home and make a nice cup of\ntea and put some arse arse cosy slippers\non in front of my steaming log effect fire\n\nPlay?",
	"Austria vs Azerbaijan\nAustria are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Bulgaria vs Morocco\nBulgaria are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"England vs Colombia\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Argentina vs Brazil\nBrazil are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Rhyl vs Northend\nRhyl are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Iran vs Hungary\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Republic of Ireland vs Rebublic of Korea\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",

	"Brazil are 2-0 down with only 6 minutes\nuntil I go home and make a nice cup of\ntea and put some arse arse cosy slippers\non in front of my steaming log effect fire\n\nPlay?",
	"Austria vs Azerbaijan\nAustria are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Bulgaria vs Morocco\nBulgaria are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"England vs Colombia\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Argentina vs Brazil\nBrazil are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Rhyl vs Northend\nRhyl are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Iran vs Hungary\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Republic of Ireland vs Rebublic of Korea\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",

	"Brazil are 2-0 down with only 6 minutes\nuntil I go home and make a nice cup of\ntea and put some arse arse cosy slippers\non in front of my steaming log effect fire\n\nPlay?",
	"Austria vs Azerbaijan\nAustria are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Bulgaria vs Morocco\nBulgaria are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"England vs Colombia\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Argentina vs Brazil\nBrazil are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Rhyl vs Northend\nRhyl are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Iran vs Hungary\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Republic of Ireland vs Rebublic of Korea\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",

	"Brazil are 2-0 down with only 6 minutes\nuntil I go home and make a nice cup of\ntea and put some arse arse cosy slippers\non in front of my steaming log effect fire\n\nPlay?",
	"Austria vs Azerbaijan\nAustria are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Bulgaria vs Morocco\nBulgaria are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"England vs Colombia\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Argentina vs Brazil\nBrazil are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Rhyl vs Northend\nRhyl are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Iran vs Hungary\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
	"Republic of Ireland vs Rebublic of Korea\nEngland are 2-0 down with\nonly 6 minutes until I go\nhome and make a nice cup\nof tea and put some\ncosy slippers on in front of my\nroaring log fire\n\nPlay?",
#endif
};


/******* Title *********/
BYTE *title_opt_list0[][7]={
	"New Friendly",
	"Play Preset Friendly",
	"New Competition",
	"Load Competition",
	"Configuration",
	"Training",
	0,

#if 1
	"fNew Friendly",
	"fPlay preset Friendly",
	"fNew competition",
	"fLoad competition",
	"fConfiguration",
	"fTraining",
	0,

	"New Friendly",
	"Play preset Friendly",
	"New competition",
	"Load competition",
	"Configuration",
	"Training",
	0,

	"New Friendly",
	"Play preset Friendly",
	"New competition",
	"Load competition",
	"Configuration",
	"Training",
	0,

	"New Friendly",
	"Play preset Friendly",
	"New competition",
	"Load competition",
	"Configuration",
	"Training",
	0,

	"New Friendly",
	"Play preset Friendly",
	"New competition",
	"Load competition",
	"Configuration",
	"Training",
	0,
#endif

};

BYTE *title_opt_list1[][9]={
	"World Cup '98",
	"Euro 2000 Qualifiers",
	"Euro 2000 Finals",
	"Territories Cup",
	"Knockout",
	"League",
	"Classic Match",
	"Previous",
	0,

#if 1
	"fWorld cup '98",
	"fEuro 2000 Qualifiers",
	"fEuro 2000 Finals",
	"fTerritories Cup",
	"fKnockout",
	"fLeague",
	"fClassic match",
	"fPrevious",
	0,

	"World cup '98",
	"Euro 2000 Qualifiers",
	"Euro 2000 Finals",
	"Territories Cup",
	"Knockout",
	"League",
	"Classic match",
	"Previous",
	0,

	"World cup '98",
	"Euro 2000 Qualifiers",
	"Euro 2000 Finals",
	"Territories Cup",
	"Knockout",
	"League",
	"Classic match",
	"Previous",
	0,

	"World cup '98",
	"Euro 2000 Qualifiers",
	"Euro 2000 Finals",
	"Territories Cup",
	"Knockout",
	"League",
	"Classic match",
	"Previous",
	0,

	"World cup '98",
	"Euro 2000 Qualifiers",
	"Euro 2000 Finals",
	"Territories Cup",
	"Knockout",
	"League",
	"Classic match",
	"Previous",
	0,
#endif

};

BYTE *title_opt_list2[][7]={
	"Edit Custom Teams",
	"Audio",
	"Controllers",
	"Screen Position",
	"Default Configuration",
	"Previous",
	0,

#if 1
	"fEdit custom teams",
	"fAudio",
	"fControllers",
	"fScreen position",
	"fSave configuration",
	"fDefault configuration",
	"fPrevious",
	0,

	"Edit custom teams",
	"Audio",
	"Controllers",
	"Screen position",
	"Save configuration",
	"Default configuration",
	"Previous",
	0,

	"Edit custom teams",
	"Audio",
	"Controllers",
	"Screen position",
	"Save configuration",
	"Default configuration",
	"Previous",
	0,

	"Edit custom teams",
	"Audio",
	"Controllers",
	"Screen position",
	"Save configuration",
	"Default configuration",
	"Previous",
	0,

	"Edit Custom Teams",
	"Audio",
	"Controllers",
	"Screen Position",
	"Save Configuration",
	"Default Configuration",
	"Previous",
	0,
#endif

};

BYTE *title_opt_list3[][4]={
	"New Game",
	"Load Game",
	"Previous",
	0,

	"fNew game",
	"fLoad game",
	"fPrevious",
	0,

	"New Game",
	"Load Game",
	"Previous",
	0,

	"New Game",
	"Load Game",
	"Previous",
	0,

	"New Game",
	"Load Game",
	"Previous",
	0,

	"New Game",
	"Load Game",
	"Previous",
	0,

};


BYTE *training_lolly_text[]={
	"Training High Scores",
	"fTraining High Scores",
	"gTraining High Scores",
	"iTraining High Scores",
	"sTraining High Scores",
	"dTraining High Scores",
};

BYTE *training_types[TRAINING_MODES][MAX_LANGS]={
	"Shooting",
	"Shooting",
	"Shooting",
	"Shooting",
	"Shooting",
	"Shooting",

	"Passing",
	"Passing",
	"Passing",
	"Passing",
	"Passing",
	"Passing",

	"Defending",
	"Defending",
	"Defending",
	"Defending",
	"Defending",
	"Defending",

	"Penalties",
	"Penalties",
	"Penalties",
	"Penalties",
	"Penalties",
	"Penalties",

	"Setpieces",
	"Setpieces",
	"Setpieces",
	"Setpieces",
	"Setpieces",
	"Setpieces",

	"Freeplay",
	"Freeplay",
	"Freeplay",
	"Freeplay",
	"Freeplay",
	"Freeplay",
};


BYTE *team_manager_lolly_text[]={
	"Team Management",
	"Team Management",
	"Team Management",
	"Team Management",
	"Team Management",
	"Team Management",
};


BYTE *tm_form_text[ MAX_LANGS ]={
	"Formation",
	"Formation",
	"Formation",
	"Formation",
	"Formation",
	"Formation",
};

BYTE *tm_variant_text[ MAX_LANGS ]={
	"Variant",
	"Variant",
	"Variant",
	"Variant",
	"Variant",
	"Variant",
};


BYTE *tm_strat_text[MAX_LANGS][2]={
	"Strategy : Aggressive",
	"Strategy : Defensive",

	"Strategy : Agressive",
	"Strategy : Defensive",

	"Strategy : Agressive",
	"Strategy : Defensive",

	"Strategy : Agressive",
	"Strategy : Defensive",

};

BYTE *tm_player_stats[ MAX_LANGS ][ TM_PLAYER_STATS ]={
	"Speed",
	"Strength",
	"Passing",
	"Tackling",
	"Shooting",
	"Ball control",
};


BYTE *gk_text[MAX_LANGS]={
	"Gk",	// abbreviation of goalkeeper
	"Gk",
	"Gk",
	"Gk",
	"Gk",
	"Gk",

};


BYTE *manager_cant_swap_req[MAX_LANGS]={
	"Goalkeepers can only be swapped\nwith Goalkeepers\n",
};


BYTE *team_stat_lolly_text[MAX_LANGS]={
	"Team Statistics",
	"Team Statistics",
	"Team Statistics",
	"Team Statistics",
	"Team Statistics",
	"Team Statistics",
};

BYTE *vs_text[MAX_LANGS]={
	"vs",
	"vs",
	"vs",
	"vs",
	"vs",
	"vs",
};

BYTE *goals_for_text[MAX_LANGS]={
	"Goals For",
	"Goals For",
	"Goals For",
	"Goals For",
	"Goals For",
	"Goals For",
};


BYTE *goals_against_text[MAX_LANGS]={
	"Goals Against",
	"Goals Against",
	"Goals Against",
	"Goals Against",
	"Goals Against",
	"Goals Against",
};

BYTE *top_scorers_text[ MAX_LANGS]={
	"Top Scorers",
	"Top Scorers",
	"Top Scorers",
	"Top Scorers",
	"Top Scorers",
	"Top Scorers",
};

BYTE *none_text[ MAX_LANGS ]={
	"None",
	"None",
	"None",
	"None",
	"None",
	"None",
};

BYTE *starting_text[MAX_LANGS]={
	"Starting Line-up",
	"Starting Eleven",
	"Starting Eleven",
	"Starting Eleven",
	"Starting Eleven",
	"Starting Eleven",
};

BYTE *subs_text[MAX_LANGS]={
	"Substitutions",
	"Substitutions",
	"Substitutions",
	"Substitutions",
	"Substitutions",
	"Substitutions",
};

BYTE *comp_mode_text[MAX_LANGS]={
	"Type",
	"Type",
	"Type",
	"Type",
	"Type",
	"Type",
};

BYTE *club_int_text[MAX_LANGS][2]={
	"International Teams",
	"Club Teams",

	"International Teams",
	"Club Teams",

	"International Teams",
	"Club Teams",

	"International Teams",
	"Club Teams",

	"International Teams",
	"Club Teams",

	"International Teams",
	"Club Teams",

};

BYTE *red_card_player_text[MAX_LANGS]={
	"Starting Line-Up must not\nContain a Player with a Red Card\n",
	"Starting Line-Up must not have a\nPlayer with a Red Card\n",
	"Starting Line-Up must not have a\nPlayer with a Red Card\n",
	"Starting Line-Up must not have a\nPlayer with a Red Card\n",
	"Starting Line-Up must not have a\nPlayer with a Red Card\n",
	"Starting Line-Up must not have a\nPlayer with a Red Card\n",
};

























































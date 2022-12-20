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

/*************************************************************************************
 *																												 *
 * OPTIONS texts																							 *
 *																												 *
 *************************************************************************************/


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
"Vulvaria",				 //11
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

"fAlbania",
"fAndorra",
"fArgentina",
"fArmenia",
"fAustralia",
"fAustria",
"fAzerbaijan",
"fBelarus",
"fBelgium",
"fBosnia",
"fBrazil",
"fBulgaria",
"fCameroon",
"fCanada",
"fChile",
"fColombia",
"fCroatia",
"fCyprus",
"fCzech Republic",
"fDenmark",
"fEgypt",
"fEngland",
"fEstonia",
"fFaroe Islands",
"fFinland",
"fFrance",
"fGeorgia",
"fGermany",
"fGreece",
"fHungary",
"fIceland",
"fIndia",
"fIran",
"fIsrael",
"fItaly",
"fJamaica",
"fJapan",
"fLatvia",
"fLiechtenstein",
"fLithuania",
"fLuxembourg",
"fMacedonia",
"fMalta",
"fMexico",
"fMoldova",
"fMorocco",
"fNetherlands",
"fNew Zealand",
"fNigeria",
"fNorthern Ireland",
"fNorway",
"fPakistan",
"fParaguay",
"fPoland",
"fPortugal",
"fRepublic of Ireland",
"fRepublic of Korea",
"fRomania",
"fRussia",
"fSan Marino",
"fSaudi Arabia",
"fScotland",
"fSlovenia",
"fSouth Africa",
"fSpain",
"fSweden",
"fSwitzerland",
"fTunisia",
"fTurkey",
"fUkraine",
"fUnited States",
"fWales",
"fYugoslavia",

};











/***********************************
 *
 * Custom team strip data
 *
 ***********************************/

Team_detail custom_teams[7]={
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
};




/************************************
 *												*
 * Custom team player data				*
 *												*
 ************************************/

Player_detail custom_players[MAX_USER_TEAMS][MAX_USER_PLAYERS_PER_TEAM]={

/***** Rage team *****/
	"A.Argetaria",
	0,0,0,0,0,0,0,4,	//shirt,skin_col,head,hair,hairstyle,facetype,facehairstyle,bootcol

	"D.Clarke",
	1,0,0,0,0,0,0,4,

	"G.Theobladd",
	2,0,0,0,0,0,0,4,

	"S.Williams",
	3,0,0,0,0,0,0,4,

	"T.McCabe",
	4,0,0,0,0,0,0,4,

	"P.Kerby",
	5,0,0,0,0,0,0,4,

	"J.Smedley",
	6,0,0,0,0,0,0,4,

	"L.Seabridge",
	7,0,0,0,0,0,0,4,

	"J.ODowd",
	8,0,0,0,0,0,0,4,

	"R.Morrall",
	9,0,0,0,0,0,0,4,

	"A.Williams",
	10,0,0,0,0,0,0,4,

	"T.Williams",
	11,0,0,0,0,0,0,4,

	"L.Mather",
	12,0,0,0,0,0,0,4,

	"R.Mann",
	13,0,0,0,0,0,0,4,

	"Z.Qamar",
	14,0,0,0,0,0,0,4,

	"J.Curtis",
	15,0,0,0,0,0,0,4,

	"J.Court",
	16,0,0,0,0,0,0,4,

	"C.Southgate",
	17,0,0,0,0,0,0,4,

	"S.Johnson",
	18,0,0,0,0,0,0,4,

	"J.Widdows",
	19,0,0,0,0,0,0,4,

	"A.Taylor",
	20,0,0,0,0,0,0,4,

	"D.Bent",
	21,0,0,0,0,0,0,4,




/****** Biscuits ******/

	"Wagon wheel",
	0,0,0,0,0,0,0,4,

	"Bourbon cream",
	1,0,0,0,0,0,0,4,

	"Pink wafer",
	2,0,0,0,0,0,0,4,

	"Digestive",
	3,0,0,0,0,0,0,4,

	"Custard cream",
	4,0,0,0,0,0,0,4,

	"Jammy dodger",
	5,0,0,0,0,0,0,4,

	"Garibaldi",
	6,0,0,0,0,0,0,4,

	"Fig roll",
	7,0,0,0,0,0,0,4,

	"Nice",
	8,0,0,0,0,0,0,4,

	"Hob nob",
	9,0,0,0,0,0,0,4,

	"Rich tea",
	10,0,0,0,0,0,0,4,

	"Cadburys finger",
	11,0,0,0,0,0,0,4,

	"Oatmeal crunch",
	12,0,0,0,0,0,0,4,

	"Malted milk",
	13,0,0,0,0,0,0,4,

	"Pengiun",
	14,0,0,0,0,0,0,4,

	"Taxi",
	15,0,0,0,0,0,0,4,

	"Lemon puff",
	16,0,0,0,0,0,0,4,

	"Viscount",
	17,0,0,0,0,0,0,4,

	"Yo-yo",
	18,0,0,0,0,0,0,4,

	"Coconut cream",
	19,0,0,0,0,0,0,4,

	"Ginger nut",
	20,0,0,0,0,0,0,4,

	"Blue riband",
	21,0,0,0,0,0,0,4,


/*** Bad Things!!!! ******************************************/	

	"Hard work",
	0,0,0,0,0,0,0,4,

	"School",
	1,0,0,0,0,0,0,4,

	"Vindaloo",
	2,0,0,0,0,0,0,4,

	"Windows",
	3,0,0,0,0,0,0,4,

	"Thrash metal",
	4,0,0,0,0,0,0,4,

	"Skodas",
	5,0,0,0,0,0,0,4,

	"Rainy day",
	6,0,0,0,0,0,0,4,

	"Dentist",
	7,0,0,0,0,0,0,4,

	"Monday",
	8,0,0,0,0,0,0,4,

	"Late train",
	9,0,0,0,0,0,0,4,

	"Richard",
	10,0,0,0,0,0,0,4,

	"Judy",
	11,0,0,0,0,0,0,4,

	"Traffic jam",
	12,0,0,0,0,0,0,4,

	"Mad dictator",
	13,0,0,0,0,0,0,4,

	"Flat beer",
	14,0,0,0,0,0,0,4,

	"Hollywood",
	15,0,0,0,0,0,0,4,

	"No milk",
	16,0,0,0,0,0,0,4,

	"Naff telly",
	17,0,0,0,0,0,0,4,

	"Nose bleed",
	18,0,0,0,0,0,0,4,

	"Bad games",
	19,0,0,0,0,0,0,4,

	"Homework",
	20,0,0,0,0,0,0,4,

	"Not us",
	21,0,0,0,0,0,0,4,






/*** Good things? **************************************************/	

	"Friday",
	0,0,0,0,0,0,0,4,

	"Loud music",
	1,0,0,0,0,0,0,4,

	"Holiday",
	2,0,0,0,0,0,0,4,

	"Fine wine",
	3,0,0,0,0,0,0,4,

	"Beer",
	4,0,0,0,0,0,0,4,

	"Nookie",
	5,0,0,0,0,0,0,4,

	"Jethro tull",
	6,0,0,0,0,0,0,4,

	"Sunny day",
	7,0,0,0,0,0,0,4,

	"TV",
	8,0,0,0,0,0,0,4,

	"Dog turd",
	9,0,0,0,0,0,0,4,

	"Chips",
	10,0,0,0,0,0,0,4,

	"Fizzy pop",
	11,0,0,0,0,0,0,4,

	"Chicken tikka",
	12,0,0,0,0,0,0,4,

	"RPG",
	13,0,0,0,0,0,0,4,

	"Screwdriver",
	14,0,0,0,0,0,0,4,

	"Shopping",
	15,0,0,0,0,0,0,4,

	"Chocolate",
	16,0,0,0,0,0,0,4,

	"Ferret",
	17,0,0,0,0,0,0,4,

	"Nice car",
	18,0,0,0,0,0,0,4,

	"Coffee",
	19,0,0,0,0,0,0,4,

	"Tea",
	20,0,0,0,0,0,0,4,

	"Ice cream",
	21,0,0,0,0,0,0,4,




/*** Foodville (replace pizzas!)***************************************************/	

	"Turnip",
	0,0,0,0,0,0,0,4,

	"Trifle",
	1,0,0,0,0,0,0,4,

	"Sausage",
	2,0,0,0,0,0,0,4,

	"Cheescake",
	3,0,0,0,0,0,0,4,

	"Burger",
	4,0,0,0,0,0,0,4,

	"Potato",
	5,0,0,0,0,0,0,4,

	"Fried egg",
	6,0,0,0,0,0,0,4,

	"Pea",
	7,0,0,0,0,0,0,4,

	"Bacon",
	8,0,0,0,0,0,0,4,

	"Carrot",
	9,0,0,0,0,0,0,4,

	"Chips",
	10,0,0,0,0,0,0,4,

	"Pasta",
	11,0,0,0,0,0,0,4,

	"Dumpling",
	12,0,0,0,0,0,0,4,

	"Gravy",
	13,0,0,0,0,0,0,4,

	"Butty",
	14,0,0,0,0,0,0,4,

	"Mint",
	15,0,0,0,0,0,0,4,

	"Pizza",
	16,0,0,0,0,0,0,4,

	"Toast",
	17,0,0,0,0,0,0,4,

	"Yorks pud",
	18,0,0,0,0,0,0,4,

	"Oyster",
	19,0,0,0,0,0,0,4,

	"Duck",
	20,0,0,0,0,0,0,4,

	"Chocolate",
	21,0,0,0,0,0,0,4,



/*** Anatomy squad ***************************************************/	

	"Hand",
	0,0,0,0,0,0,0,4,
	 
	"Elbow",
	1,0,0,0,0,0,0,4,

	"Artery",
	2,0,0,0,0,0,0,4,

	"Appendix",
	3,0,0,0,0,0,0,4,

	"Nose",
	4,0,0,0,0,0,0,4,

	"Anvil",
	5,0,0,0,0,0,0,4,

	"Kidney",
	6,0,0,0,0,0,0,4,

	"Eye",
	7,0,0,0,0,0,0,4,

	"Heart",
	8,0,0,0,0,0,0,4,

	"Finger",
	9,0,0,0,0,0,0,4,

	"Foot",
	10,0,0,0,0,0,0,4,

	"Brain",
	11,0,0,0,0,0,0,4,

	"Leg",
	12,0,0,0,0,0,0,4,

	"Epiglotis",
	13,0,0,0,0,0,0,4,

	"Spleen",
	14,0,0,0,0,0,0,4,

	"Stomach lining",
	15,0,0,0,0,0,0,4,
	 
	"Pancreas",
	16,0,0,0,0,0,0,4,

	"Lung",
	17,0,0,0,0,0,0,4,

	"Arm",
	18,0,0,0,0,0,0,4,

	"Duodenum",
	19,0,0,0,0,0,0,4,

	"Sphincter",
	20,0,0,0,0,0,0,4,

	"Ear",
	21,0,0,0,0,0,0,4,



/*** World XI ***************************************************/	

	"Chilavert",
	0,0,0,0,0,0,0,4,

	"Campbell",
	1,0,0,0,0,0,0,4,

	"Carlos",
	2,0,0,0,0,0,0,4,

	"De boer",
	3,0,0,0,0,0,0,4,

	"Blanc",
	4,0,0,0,0,0,0,4,

	"Zidane",
	5,0,0,0,0,0,0,4,

	"Ortega",
	6,0,0,0,0,0,0,4,

	"Finidi",
	7,0,0,0,0,0,0,4,

	"Ronaldo",
	8,0,0,0,0,0,0,4,

	"Owen",
	9,0,0,0,0,0,0,4,

	"Overmars",
	10,0,0,0,0,0,0,4,

	"Salas",
	11,0,0,0,0,0,0,4,

	"Petrescu",
	12,0,0,0,0,0,0,4,

	"Schmeichel",
	13,0,0,0,0,0,0,4,

	"Del piero",
	14,0,0,0,0,0,0,4,

	"Vieri",
	15,0,0,0,0,0,0,4,

	"Ayala",
	16,0,0,0,0,0,0,4,

	"Suker",
	17,0,0,0,0,0,0,4,

	"Nakata",
	18,0,0,0,0,0,0,4,

	"Ladic",
	19,0,0,0,0,0,0,4,

	"Okocha",
	20,0,0,0,0,0,0,4,

	"West",
	21,0,0,0,0,0,0,4,

};



/****************************************************
 *
 * Lolly texts
 *
 ****************************************************/
BYTE *edit_name_txt[]={
	"Edit name",
	"Edit name",
	"Edit name",
	"Edit name",
	"Edit name",
};

BYTE *edit_player_txt[]={
	"Edit player", 
	"Edit player", 
	"Edit player", 
	"Edit player", 
	"Edit player", 
};

BYTE *edit_team_txt[]={
	"Edit team",
	"Edit team",
	"Edit team",
	"Edit team",
	"Edit team",
};

BYTE *opt_lolly_text[]={
	"Options\n",
	"Options\n",
	"Options\n",
	"Options\n",
	"Options\n",
};

BYTE *audio_config_lolly_text[]={
	"Audio adjust\n",
	"Audio adjust\n",
	"Audio adjust\n",
	"Audio adjust\n",
	"Audio adjust\n",
};

BYTE *screen_pos_lolly_text[]={
	"Position screen\n",
	"Position screen\n",
	"Position screen\n",
	"Position screen\n",
	"Position screen\n",
};


BYTE *save_config_lolly_text[]={
	"Save configuration",
};

BYTE *save_game_lolly_text[]={
	"Save a game",
};







BYTE *friendly_shell_opts[]={
	"ok\n",
	"prev\n",
	"cancel\n",0,

	"fok",
	"fprev",
	"fcancel",0,

};

BYTE *te_ed_shell_opts[]={
	"Edit players","Save","Exit",0,
	"Edit players","Save","Exit",0,
	"Edit players","Save","Exit",0,
};

BYTE *pl_ed_shell_opts[]={
	"Edit teams","Save","Exit",0,
	"Edit teams","Save","Exit",0,
	"Edit teams","Save","Exit",0,
};

BYTE *audio_config_shell_opts[]={
	"ok","cancel",0,
	"ok","cancel",0,
	"ok","cancel",0,
};



























/*****************************************
 *
 * Flag nums
 *
 ******************************************/

UBYTE ts_all_teams_list[2][23*4]={

//*** Team flags in English alphbetical order **

	Albania,
	Andorra,				
	Argentina,				
	Armenia,				
	Australia,
	Austria,				
	Azerbaijan,
	Belarus,				
	Belgium,				
	Bosnia,					
	Brazil,					
	Bulgaria,				
	Cameroon,				
	Canada,					
	Chile,					
	Colombia,				
	Croatia,				
	Cyprus,				 	
	Czech_Republic,		
	Denmark,				
	Egypt,			  		
	England,		  		
	Estonia,		  		
	Faroe_Islands,
	Finland,
	France,		  		 	
	Georgia,		  		
	Germany,		  		
	Greece,		  		 	
	Hungary,		  		
	Iceland,		  		
	India,         		
	Iran,			  		
	Israel,		  		 	
	Italy,			  		
	Jamaica,		  		
	Japan,			  		
	Korean_Republic,
	Latvia,		  		 	
	Liechtenstein, 		
	Lithuania,	  		 	
	Luxembourg,	  		
	Macedonia,	  		 	
	Malta,			  		
	Mexico,		  		 	
	Moldova,		  		
	Morocco,		  		
	Netherlands,	  		
	New_Zealand,	  		
	Nigeria,		  		
	Northern_Ireland,   
	Norway,		  		 	
	Pakistan,		  		
	Paraguay,		  		
	Poland,		  		 	
	Portugal,		  		
	Republic_of_Ireland,
	Romania,      		
	Russia,		 		 	
	San_Marino,	 		
	Saudi_Arabia, 		
	Scotland,     		
	Slovakia,				
	Slovenia,		 		
	South_Africa, 		
	Spain,			 		
	Sweden,		 		 	
	Switzerland,	 		
	Tunisia,		 		
	Turkey,		 		 	
	Ukraine,		 		
	United_States,		
	Wales,        		
	Yugoslavia,	 		
	74,75,76,77,78,79,80,81,82,83,



//*** Team flags in French alphbetical order **
	2,  0, 1, 3,
	4,  5, 6, 7,
	8,  9,10,11,
	12,13,14,15,
	16,17,18,19,
	20,21,22,23,
	24,25,26,27,
	28,29,30,31,
	32,33,34,35,
	36,37,38,39,
	40,41,42,43,
	44,45,46,47,
	48,49,50,51,
	52,53,54,55,
	56,57,58,59,
	60,61,62,63,
	64,65,66,67,
	68,69,70,71,
	72,73,74,75,
	76,77,78,79,
	80,81,82,83,
	84,85,86,87,
	88,89,90,91,

};


UBYTE territory_teams_list[][6]={
/* South America **/
	Brazil,
	Argentina,
	Colombia,
	Paraguay,
	Chile,-1,		

/* Americas **/
	United_States,
	Canada,
	Mexico,
	Jamaica,-1,-1,

/* Asia **/
	Japan,
	Korean_Republic,
	Saudi_Arabia,
	India,
	Pakistan,
	Iran,

/* Europe **/
	England,
	France,
	Germany,
	Italy,
	Netherlands,
	Spain,

/* Efrica **/
	Nigeria,
	Cameroon,
	Egypt,
	South_Africa,
	Morocco,
	Tunisia,

/* Australasia **/
	Australia,
	New_Zealand,-1,-1,-1,-1,		
};


UBYTE ts_euro2000_teams_list[][ TOTAL_EURO2000_FLAGS ]={
	Albania,
	Andorra,   
	Armenia,
	Austria,
	Azerbaijan,
	Belarus,
	Bosnia,
	Bulgaria,
	Croatia,
	Cyprus,
	Czech_Republic,
	Denmark,
	England,
	Estonia,
	Faroe_Islands,
	Finland,
	France,
	Germany, 
	Georgia,
	Greece,
	Hungary,
	Iceland,
	Israel,
	Italy,
	Latvia,
	Liechtenstein,
	Lithuania,
	Luxembourg,
	Macedonia,
	Malta,
	Moldova,
	Northern_Ireland,
	Norway,
	Poland,
	Portugal,
	Republic_of_Ireland,
	Romania,
	Russia,
	San_Marino,
	Scotland,
	Slovakia,
	Slovenia,
	Spain,
	Sweden,
	Switzerland,
	Turkey,
	Ukraine,
	Wales,
	Yugoslavia,
	
};

BYTE euro2000_teams[9][6]={
	Italy, Belarus, Wales, Switzerland, Denmark,-1,
	Norway, Slovenia, Latvia, Georgia, Greece, Albania,
	Germany, Moldova, Northern_Ireland, Finland, Turkey, -1,
	Russia, Armenia, Iceland, Ukraine, France, Andorra,
	England, Luxembourg, Poland, Sweden, Bulgaria, -1,
	Spain, San_Marino, Cyprus, Israel, Austria, -1,
	Romania, Liechtenstein, Hungary, Slovakia, Portugal, Azerbaijan,
	Yugoslavia, Malta, Macedonia, Republic_of_Ireland, Croatia, -1,
	Scotland, Faroe_Islands, Bosnia, Lithuania, Czech_Republic, Estonia,
};

UBYTE ts_eurofinal_teams_list[][ TOTAL_EUROFINAL_FLAGS ]={
	Albania,
	Andorra,   
	Armenia,
	Austria,
	Azerbaijan,
	Belarus,
	Belgium,
	Bosnia,
	Bulgaria,
	Croatia,
	Cyprus,
	Czech_Republic,
	Denmark,
	England,
	Estonia,
	Faroe_Islands,
	Finland,
	France,
	Germany, 
	Georgia,
	Greece,
	Hungary,
	Iceland,
	Israel,
	Italy,
	Latvia,
	Liechtenstein,
	Lithuania,
	Luxembourg,
	Macedonia,
	Malta,
	Moldova,
	Netherlands,
	Northern_Ireland,
	Norway,
	Poland,
	Portugal,
	Republic_of_Ireland,
	Romania,
	Russia,
	San_Marino,
	Scotland,
	Slovakia,
	Slovenia,
	Spain,
	Sweden,
	Switzerland,
	Turkey,
	Ukraine,
	Wales,
	Yugoslavia,
	
};


/*************** FRONTEND.C TEXT *******************/




BYTE *group_text[]={
	"Group %d",
	"Group %d",
	"Group %d",
};




BYTE *thread_quit_text[MAX_LANGS]={
	
	"Are you sure you want\nto return to the\nTitle screen?",
#if 0
	"Are you sure you want\nto quit this league?",
	"Are you sure you want\nto quit this Knockout?",
	"Are you sure you want to\nquit this Territories cup?",
	"Are you sure you want to\nquit this Classic Match?",
	"Are you sure you want\nto quit the Euro2000?",
	"Are you sure you want\nto quit the Euro2000 Final?",
	"Are you sure you want\nto quit the World Cup?",
	"Are you sure you want\nto quit the Euro2000?",
	"Are you sure you want\nto quit the Euro2000?",
	"Are you sure you want\nto quit the Euro2000 Final?",
	"xxx",

	"Are you sure you want\nto quit the Friendly?",
	"Are you sure you want\nto quit this league?",
	"Are you sure you want\nto quit this Knockout?",
	"Are you sure you want to\nquit this Territories cup?",
	"Are you sure you want to\nquit this Classic Match?",
	"Are you sure you want\nto quit the Euro2000?",
	"Are you sure you want\nto quit the Euro2000 Final?",
	"Are you sure you want\nto quit the World Cup?",
	"Are you sure you want\nto quit the Euro2000?",
	"Are you sure you want\nto quit the Euro2000?",
	"Are you sure you want\nto quit the Euro2000 Final?",
	"xxx",
#endif
};


BYTE *thread_game_over_text[MAX_LANGS][ TOTAL_GAME_TYPES ]={
	"poo",
	"No human team came\ntop of the League\nGame Over",
	"No human team qualified\nFor the Knockout\nGame Over",
	"You failed to\nWin this territory\nGame Over",
	"poo",
	"No human team qualified\nFor the Final\nGame Over",
	"Final poo",
	"No human team qualified\nFor the World Cup\nGame Over",
	"No human team qualified\nFor the Final\nGame Over",
	"No human team qualified\nFor the Final\nGame Over",
	"No human team remains in\nThe Final\nGame Over",

};


BYTE *preset_save_request[]={
	"Are you sure you want\nTo save into preset %d?\n",
};


BYTE *game_types_text[ MAX_LANGS ][ TOTAL_GAME_TYPES ]={
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

	
};	



BYTE worldcup_group_teams[8][4]={
	Belgium,
	Brazil,
	Norway,
	France,

	Northern_Ireland,
	Republic_of_Ireland,
	Wales,
	Scotland,

	England,
	Japan,
	Greece,
	Faroe_Islands,

	Germany,
	Italy,
	Latvia,
	Estonia,

	Israel,
	Iceland,
	Poland,
	Portugal,

	Russia,
	Macedonia,
	Malta,
	Nigeria,

	Yugoslavia,
	Sweden,
	Turkey,
	United_States,

	Egypt,
	Mexico,
	Iran,
	India,
};


BYTE worldcup_fixlist[48][2]={
	
	Belgium,Brazil,
	Belgium,Norway,
	Belgium,France,
	Brazil, Norway,
	Brazil, France,
	Norway, France,

	Northern_Ireland, Republic_of_Ireland,
	Northern_Ireland, Wales,
	Northern_Ireland, Scotland,
	Republic_of_Ireland, Wales,
	Republic_of_Ireland, Scotland,
	Wales, Scotland,

	England, Japan,
	England, Greece,
	England, Faroe_Islands,
	Japan, Greece,
	Japan, Faroe_Islands,
	Greece, Faroe_Islands,

	Germany, Italy,
	Germany, Latvia,
	Germany, Estonia,
	Italy, Latvia,
	Italy, Estonia,
	Latvia, Estonia,

	Israel, Iceland,
	Israel, Poland,
	Israel, Portugal,
	Iceland, Poland,
	Iceland, Portugal,
	Poland, Portugal,

	Russia, Macedonia,
	Russia, Malta,
	Russia, Nigeria,
	Macedonia,Malta,
	Macedonia, Nigeria,
	Malta, Nigeria,

	Yugoslavia,Sweden,
	Yugoslavia,Turkey,
	Yugoslavia,United_States,
	Sweden, Turkey,
	Sweden, United_States,
	Turkey, United_States,

	Egypt, Mexico,
	Egypt, Iran,
	Egypt, India,
	Mexico, Iran,
	Mexico, India,
	Iran, India,



};


BYTE *preset_not_enough_pads_req[]={
	"This match requires at\nleast two controllers\n",
};


BYTE *humcpu_txt[MAX_LANGS][2]={
	"human",
	"cpu",

	"hum",
	"cpu",
};


BYTE *autoload_conf_file_txt[]={
	"Autoloading configuration file\nPlease Wait",
};

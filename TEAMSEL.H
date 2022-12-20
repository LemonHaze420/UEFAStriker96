void init_team_select_bootup();
void init_team_select_once();
void set_cpu_teams_list();
void team_select();
void select_num_teams();
void add_team_to_ts_list( WORD team_num );

extern BYTE team_select_text_buffer[];


extern BYTE *get_team_name( WORD team );

void call_teamsel();
//BYTE *get_country_text( WORD country );


extern UBYTE *ts_source_team_list;
extern WORD ts_total_teams;	// total amount of teams currently selected



extern WORD ts_pick_teams_list[];
void init_select_num_teams_bootup();

void create_cpu_teams_index_list();

extern UBYTE ts_europe_territory_teams_list[2][TOTAL_EUROPE_FLAGS];




extern WORD snt_team_cnt;






void init_select_num_teams_once();
void create_euro2000_remaining_teams();


void reset_selected_teams_list();


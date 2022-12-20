extern BYTE top_team_scorers[TOTAL_TEAMS][3];


void reset_competition_data();

void calc_ko_draws_to_do();
void init_knockout_data();
void init_league_draw();
WORD do_league_draw();
void do_knockout_draw();
WORD calculate_winner();
void simulate_game_winner();
void calc_league_stats();
void init_worldcup_draw();
WORD do_worldcup_draw();
void do_worldcup_knockout_draw();

void ko_calc_results();

void init_euro2000_draw();
WORD check_human_territory_winner();
WORD do_euro2000_draw();



void replay_euro2000();
void calc_euro2000_2nd_place();
void add_ko_teams_to_euro_final();

void reset_training_points();
void update_training_points();


void reset_game_stats();
void update_game_stats();

extern BYTE *get_database_player_name( WORD team, WORD player );

//extern BYTE red_cards_awarded[TOTAL_TEAMS][22];
extern void reset_share_red_cards();
extern void process_game_return_data();
extern void update_red_cards_awarded();


extern void set_top_team_scorers();
extern WORD set_human_team_pos( WORD team_num );


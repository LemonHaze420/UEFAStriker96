

extern BYTE text_buffer[];
extern BYTE text_buffer2[];


void training_thread();
void run_title();
void init_options();
void friendly_thread();
void league_thread();
void knockout_thread();
void classic_thread();
void friendly_preset_thread();
void territory_thread();
void euro2000_thread();
void euro_final_thread();
void worldcup_thread();

extern WORD current_item;
void restore_game_undo_info();


extern WORD abort_game,abort_frontend;
void set_demo_mode_data();
void set_game_share_data();



void start_title_track();
void start_frontend_track();
void reset_competition_data();
void fade_off_music();
void show_start_movies();

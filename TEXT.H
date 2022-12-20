										
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


extern BYTE *game_types_text[MAX_LANGS][ TOTAL_GAME_TYPES ];
extern BYTE *thread_game_over_text[MAX_LANGS][ TOTAL_GAME_TYPES ];
extern BYTE *thread_quit_text[MAX_LANGS];
extern BYTE *preset_save_request[MAX_LANGS];


extern BYTE *group_text[];




extern BYTE *humcpu_txt[MAX_LANGS][2];
extern BYTE *autoload_conf_file_txt[MAX_LANGS];


extern BYTE *pl_ed_number_names[];
extern BYTE *pl_ed_player_words[2][6];
extern BYTE *pl_ed_button_names[][9]; 
extern BYTE *team_words[];
extern BYTE *te_ed_team_button_names[];
extern BYTE *te_ed_shirt_txt[];
extern BYTE *te_ed_sleeve_txt[];
extern BYTE *te_ed_shorts_txt[];
extern BYTE *te_ed_socks_txt[];


extern BYTE *conf_save_req[MAX_LANGS];
extern BYTE *pc_button_text[MAX_LANGS][8];
extern BYTE *restore_config_lolly_text[MAX_LANGS];
extern BYTE *restore_config_txt[MAX_LANGS];
extern BYTE *config_restored[MAX_LANGS];
extern BYTE *cancelled_txt[MAX_LANGS];
extern BYTE *pad_config_words[MAX_LANGS];
extern BYTE *sp_info_text[MAX_LANGS];
extern BYTE *audio_word1[MAX_LANGS][3];
extern BYTE *audio_word2[MAX_LANGS];
extern BYTE *ac_tgad_text[MAX_LANGS][4];
extern BYTE *screen_pos_shell_opts[MAX_LANGS];
extern BYTE *pad_config_lolly_txt[MAX_LANGS];
extern BYTE *pad_assign_lolly_txt[MAX_LANGS];
extern BYTE *knock_next_match_lolly_text[];
extern BYTE *winner_lolly_text[];
extern BYTE *winners_lolly_text[];

extern BYTE *euro_finalist_teams_lolly_text[];
extern BYTE *eq_group_winner_text[];
extern BYTE *eq_group_runner_text[];
extern BYTE *euro_game_over_lolly_text[];
extern BYTE *euro_result_lolly_text[];
extern BYTE *euro_next_match_lolly_text[];
extern BYTE *euro_qual_teams_lolly_text[];
extern BYTE *worldcup_game_over_lolly_text[];
extern BYTE *league_stat_lolly_text[];
extern BYTE *final_league_stat_lolly_text[];
extern BYTE *territory_stat_lolly_text[6][MAX_LANGS];
extern BYTE *final_territory_stat_lolly_text[6][MAX_LANGS];
extern BYTE *lstat_txt[][8];


extern BYTE *save_config_prompt[];
extern BYTE *load_game_lolly_text[];
extern BYTE *mc_free_config_text[];
extern BYTE *mc_free_slot_text[2][MAX_LANGS];
extern BYTE *mc_delete_text[];
extern BYTE *mc_overwrite_text[];
extern BYTE *mc_loading_text[];
extern BYTE *mc_load_info_text[];
extern BYTE *mc_load_text[];
extern BYTE *mc_saving_text[];
extern BYTE *mc_overwriting_text[];
extern BYTE *mc_no_cards_present_text[];
extern BYTE *mc_no_lt_card_present_text[];
extern BYTE *mc_no_lt_card_present_text_tap[];
extern BYTE *mc_save_ok_text[];
extern BYTE *mc_load_ok_text[];
extern BYTE *mc_overwrite_ok_text[];
extern BYTE *mc_save_cancelled_text[];
extern BYTE *mc_load_cancelled_text[];
extern BYTE *mc_write_error_text[];
extern BYTE *mc_rewrite_text[];
extern BYTE *mc_load_error_text[];
extern BYTE *mc_delete_error_text[];
extern BYTE *mc_save_new_text[];
extern BYTE *mc_no_specific_game_files_text[];
extern BYTE *mc_are_you_sure_overwrite_text[];
extern BYTE *mc_are_you_sure_overwrite_with_text[];
extern BYTE *mc_are_you_sure_del_text[];
extern BYTE *mc_deleting_text[];
extern BYTE *mc_overwrite_error_text[];
extern BYTE *mc_select_overwrite_text[];
extern BYTE *mc_overwrite_cancelled_text[];
extern BYTE *mc_delete_cancelled_text[];
extern BYTE *mc_please_wait_text[];
extern BYTE *mc_no_card_present_text[];
extern BYTE *mc_unformat_text[];
extern BYTE *mc_reformat_text[];
extern BYTE *mc_format_text[];
extern BYTE *mc_format_done_text[];
extern BYTE *mc_no_slots_to_overwrite_text[];
extern BYTE *card_num_text[];
extern BYTE *only_text[];


extern BYTE *opt_txts[MAX_LANGS][TOTAL_OPTIONS];
extern BYTE *opt_button_list1[];
extern BYTE *opt_button_list2[];
extern BYTE *opt_button_list3[];
extern BYTE *opt_button_list4[];
extern BYTE *opt_button_list5[];
extern BYTE *opt_button_list6[];
extern BYTE *opt_button_list7[];
extern BYTE *opt_button_list8[];


extern BYTE *preset_save_lolly_text[];
extern BYTE *preset_choose_lolly_text[];
extern BYTE *empty_text[];

extern BYTE *def_team_lolly_text[];
extern BYTE *def_team_single_lolly_text[];
extern BYTE *select_num_teams_lolly_text[];
extern BYTE *snt_totteams_txt[];
extern BYTE *snt_humteams_txt[];
extern BYTE *snt_diff_txt1[][3];
extern BYTE *snt_diff_txt2[];

extern BYTE *territ_select_lolly_text[];
extern BYTE *cm_select_lolly_text[];
extern BYTE *territory_names[][6];
extern BYTE *classic_match_texts[][8];

extern BYTE *title_opt_list0[][7];
extern BYTE *title_opt_list1[][9];
extern BYTE *title_opt_list2[][7];
extern BYTE *title_opt_list3[][4];
extern BYTE *training_lolly_text[TRAINING_MODES];
extern BYTE *training_types[TRAINING_MODES][MAX_LANGS];


extern BYTE *team_manager_lolly_text[];
extern BYTE *tm_strat_text[MAX_LANGS][2];
extern BYTE *tm_variant_text[ MAX_LANGS ];
extern BYTE *tm_form_text[MAX_LANGS];

extern BYTE *tm_player_stats[ MAX_LANGS ][ TM_PLAYER_STATS ];
extern BYTE *gk_text[MAX_LANGS];

extern BYTE *manager_cant_swap_req[MAX_LANGS];
extern BYTE *team_stat_lolly_text[MAX_LANGS];
extern BYTE *vs_text[MAX_LANGS];
extern BYTE *goals_for_text[MAX_LANGS];
extern BYTE *top_scorers_text[ MAX_LANGS];
extern BYTE *goals_against_text[MAX_LANGS];
extern BYTE *none_text[ MAX_LANGS ];

extern BYTE *starting_text[MAX_LANGS];
extern BYTE *subs_text[MAX_LANGS];
extern BYTE *club_int_text[MAX_LANGS][2];
extern BYTE *comp_mode_text[MAX_LANGS];
extern BYTE *red_card_player_text[MAX_LANGS];


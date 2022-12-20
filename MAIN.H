
extern Share  *share;
extern Comp *comp, *comp_store;
extern Config *config;
extern Config *config_store;

void init_once();

extern ULONG timer;
extern UWORD bg_loop_abort;
extern void bg_loop();
extern void bg_loop_predraw();
//extern Scene_startup *start_scene_struct;

extern void bg_loop_test();

extern WORD flash_counter;
extern WORD stub_present;

#define FRONTEND_UPDATE_RATE 2

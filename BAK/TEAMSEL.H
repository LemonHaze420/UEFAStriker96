typedef struct
{
	UBYTE *team_name;
	UBYTE shirt_texture_num;
	UBYTE shirt_clut1;
	UBYTE shirt_clut2;
	UBYTE lt_sleeve_clut;
	UBYTE rt_sleeve_clut;
	UBYTE lt_short_clut;
	UBYTE rt_short_clut;
}Team_strip_data;


extern Team_strip_data team_strip_data[];
extern UWORD team_strip_color_ranges[];	// must be a minimum of 2k
extern void kill_team_select();
extern void update_team_select();
//void mod_team_strips();
void init_team_select();

void init_teamsel_once();

void show_friendly_strips();

#include "main.h"
#include "tabletop.h"

void play_game(SDL_Window *w, SDL_Renderer *r);

int summon_pause_menu();

void render_floor(SDL_Window *w, SDL_Renderer *r, level floor, int x, int y, int ww, int wh);

void render_tiles();

void render_pieces();

//  Saves the game under the specified name (in the game's directory under "saves")
int save_game(char * save_name);
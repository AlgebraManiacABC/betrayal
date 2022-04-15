#include "main.h"
#include "tabletop.h"

#define DEFAULT_ZOOM 20

void play_game(SDL_Window *w, SDL_Renderer *r);

int summon_pause_menu();

void render_floor(SDL_Renderer *r, level floor, int x, int y, int ww, int wh, int zoom);

void render_room(SDL_Renderer *r, tile room, int x, int y, int ww, int wh, int zoom);

void render_pieces();

//  Saves the game under the specified name (in the game's directory under "saves")
int save_game(char * save_name);

/** What to save?
 * 
 * Zoom level
 * X/Y render pos
 * Current room layout and deck
 * 
 * 
 */
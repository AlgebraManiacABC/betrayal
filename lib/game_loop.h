#include "main.h"
#include "tabletop.h"

#define DEFAULT_ZOOM 20

void play_game(SDL_Window *w, SDL_Renderer *r);

int summon_pause_menu();

void mousepos2tile(int mx, int my, int offx, int offy, int ww, int wh, int tilewidth, int * tilex, int * tiley);

void render_bg(SDL_Renderer *r, SDL_Texture * bg);

void render_floor(SDL_Renderer *r, level floor, int x, int y, int ww, int wh, int zoom);

void render_room(SDL_Renderer *r, tile room, int x, int y, int ww, int wh, int zoom);

void render_selection(SDL_Renderer *r, tile highlight, int offx, int offy, int ww, int wh, int zoom);

void render_pieces();

int discover_room(tabletop table, int floor, int x, int y, Uint8 from);

int uint8_bin(Uint8 bin);

void connect_four(tabletop table, int floor, tile here, int x, int y);

tile get_room(tabletop table, int floor, int x, int y);

bool is_room(tabletop table, int floor, int x, int y, char * roomname);

//  Saves the game under the specified name (in the game's directory under "saves")
int save_game(char * save_name);

/** What to save?
 * 
 * Zoom level
 * X/Y render pos
 * Current room layout and deck
 * Turn + turn count + turn order
 * Background
 * 
 */
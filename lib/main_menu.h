#include "main.h"
#include "myttf.h"

#define MAIN_MENU_ERROR    -1
#define MAIN_MENU_START     0
#define MAIN_MENU_OPTIONS   1
#define MAIN_MENU_QUIT      2
#define START_JOIN          3
#define START_HOST          4
#define START_BACK          5

#define STATE_MAIN      0
#define STATE_START     1

#define MASK_START      0b00000001
#define MASK_OPTIONS    0b00000010
#define MASK_QUIT       0b00000100
#define MASK_JOIN       0b00001000
#define MASK_HOST       0b00010000
#define MASK_BACK       0b00100000

#define MAIN_MENU_COUNT 6

/**
 * @brief Summons the main menu.
 * Main menu options:
 * * Start (single/multiplayer/host/join)
 * * Options (volume, etc?)
 * * Quit
 * 
 * @param r the SDL_Renderer
 * @return a value corresponding to the option chosen
 */
int summon_main_menu(SDL_Window *w, SDL_Renderer *r);

/**
 * @brief determines what main menu buttons are pressed
 * 
 * @param hover a pointer to a bitmask that will be updated
 * to show what is being hovered over
 * @return an int relating to the selection of buttons
 */
int evaluateClicks(Uint32 * hover, int state, bool click_up, bool click_down);

/**
 * @brief Functions that tell if the mouse is hovering over the button
 * 
 * @param x mouse x
 * @param y mouse y
 * @return true if it's hovering over the button, false if not
 */
bool mouse_over_start(int x, int y);
bool mouse_over_options(int x, int y);
bool mouse_over_quit(int x, int y);
bool mouse_over_join(int x, int y);
bool mouse_over_host(int x, int y);
bool mouse_over_back(int x, int y);

/**
 * @brief Places the main menu option text on the screen.
 * 
 * @param r the SDL_Renderer
 * @param start the START text texture (selected or deselected)
 * @param options the OPTIONS text texture (selected or deselected)
 * @param quit the QUIT text texture (selected or deselected)
 *
 */
void present_main_options(SDL_Renderer *r, SDL_Texture * start,
    SDL_Texture * options, SDL_Texture * quit);

/**
 * @brief Places the main menu option text on the screen.
 * 
 * @param r the SDL_Renderer
 * @param join the JOIN text texture (selected or deselected)
 * @param host the HOST text texture (selected or deselected)
 * @param back the BACK text texture (selected or deselected)
 *
 */
void present_start_options(SDL_Renderer *r, SDL_Texture * join,
    SDL_Texture * host, SDL_Texture * back);
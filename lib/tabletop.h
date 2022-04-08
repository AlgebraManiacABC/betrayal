#include "main.h"

#define FLOOR_COUNT 4   //  Basement, Ground, Upper, Roof

//  The tabletop itself. How will I arrange it?
typedef struct
{

}   tabletop;

//  Will resize the window to borderless maximized
void prepare_tabletop();

//  The main game loop. Will likely be in effect for the rest of the game
void play_game();

//  This means we will need to summon a pause menu
int summon_pause_menu();

//  We need to know where we're looking and print the board
void render_tiles();

//  Might the above also render characters, etc?
void render_pieces();
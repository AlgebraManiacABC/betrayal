#include "main.h"
#include "list.h"

#define FLOOR_COUNT 3   //  Basement, Ground, Upper, Roof

#define FLOOR_BASEMENT 0
#define FLOOR_GROUND 1
#define FLOOR_UPPER 2
#define FLOOR_ROOF 3

#define MASK_BASEMENT   0b0000001
#define MASK_GROUND     0b0000010
#define MASK_UPPER      0b0000100
#define MASK_ROOF       0b0001000

#define MASK_N      0b0001000
#define MASK_S      0b0000100
#define MASK_E      0b0000010
#define MASK_W      0b0000001

#define CARD_EVENT 1
#define CARD_ITEM  2
#define CARD_OMEN  3

#define FORWARD true
#define BACKWARD false
#define BG_COUNT 5

typedef struct tile * tile;

typedef struct tile
{
    char * name;
    SDL_Texture * img;
    int type;
    Uint8 legal_floors;
    Uint8 doors;
    bool dumbwaiter;
    bool outside;
    bool window;
    tile N;
    tile E;
    tile S;
    tile W;
    Uint32 orientation;
    int x;  //  Position relative to the anchor
    int y;
    bool render;

}   tile_s;

typedef struct
{
    tile anchor;
    list rooms;
    int maxtilex;
    int mintilex;
    int maxtiley;
    int mintiley; //  Distances from the anchor to the farthest tile in respective location

}   level_s;

typedef level_s * level;

//  The tabletop itself. How will I arrange it?
typedef struct
{
    level floors[FLOOR_COUNT];
    list deck;
    SDL_Texture ** bgs;
    int bg_id;
    tile highlight;

}   tabletop_s;

typedef tabletop_s * tabletop;

//  Will return a prepared tabletop struct
tabletop init_tabletop(SDL_Window *w, SDL_Renderer *r);

SDL_Texture ** load_bgs(SDL_Renderer *r);

void cycle_bg(SDL_Renderer *r, tabletop table, bool dir);

level init_floor(SDL_Renderer *r, int floor, list deck);

list load_rooms(SDL_Renderer *r);

SDL_Texture * get_tile_img(SDL_Renderer *r, char * tilename);

tile draw_specific_room(list deck, char * tilename);

tile draw_valid_room(list deck, Uint32 floor_mask);

list shuffle_deck(list deck);

void delete_tabletop(tabletop table);

void delete_floor(level floor);
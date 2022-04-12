#include "main.h"

#define LEVEL_TEX 4
#define LEVEL_SURF 3
#define LEVEL_REND 2
#define LEVEL_WIN 1
#define LEVEL_INIT 0

int mySDL_Init(Uint32 init_flags, SDL_Window ** w, int win_w, int win_h,
    Uint32 win_flags, char * win_name, SDL_Renderer ** r);

int mySDL_Maximize_Window(SDL_Window ** w, SDL_Renderer ** r);

int mySDL_Close(int level, ...);
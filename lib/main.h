#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "assets.h"

#ifndef MAIN_MENU_WIDTH

#define MAIN_MENU_WIDTH 1189
#define MAIN_MENU_HEIGHT 794

#define MASK_NONE 0

#define DINKFONT "src/font/Dink/Dink-vKXE.ttf"
#define SANSFONT "src/font/FreeSans/FreeSans/FreeSans-LrmZ.ttf"

#define ROOMS_TABLE "lib/rooms.csv"

#endif
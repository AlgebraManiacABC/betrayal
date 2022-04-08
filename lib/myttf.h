#include "main.h"

SDL_Rect renderText(SDL_Renderer * r, SDL_Texture *text, int x, int y, int pt);

SDL_Texture * makeSolidTextureFromText
    (SDL_Renderer * r, TTF_Font * font, char * text, SDL_Color color);
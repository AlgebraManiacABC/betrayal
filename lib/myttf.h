#include "main.h"

struct myFont
{
    TTF_Font * font;
};

//  Things to recall:
//
//  - TTF_Font is a structure containing a font at a certain size
//  - TTF_RenderText_Solid() takes a font, string, and color, and returns a SDL_Surface
//  - (This surfacce must be turned into a texture)

//  - Every time text is needed, just use my makeSolidTextureFromText function (ez)

SDL_Rect renderText(SDL_Renderer * r, SDL_Texture *text, int x, int y, int pt);

SDL_Texture * makeSolidTextureFromText
    (SDL_Renderer * r, TTF_Font * font, char * text, SDL_Color color);
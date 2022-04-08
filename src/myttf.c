#include "../lib/myttf.h"

SDL_Rect renderText(SDL_Renderer * r, SDL_Texture * text, int x, int y, int pt)
{
    SDL_Rect box = {x,y,0,0};
    SDL_QueryTexture(text,NULL,NULL,&box.w,&box.h);
    float ratio = (float)pt/box.h;
    box.h *= ratio;
    box.w *= ratio;
    return box;
}

SDL_Texture * makeSolidTextureFromText
    (SDL_Renderer * r, TTF_Font * font, char * text, SDL_Color color)
{
    SDL_Surface * temp = TTF_RenderText_Solid(font,text,color);
    SDL_Texture * tex = SDL_CreateTextureFromSurface(r,temp);
    SDL_FreeSurface(temp);
    return tex;
}
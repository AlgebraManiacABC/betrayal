#include "../lib/game_loop.h"

void play_game(SDL_Window *w, SDL_Renderer *r)
{
    tabletop table = init_tabletop(w,r);
    int ww,wh;
    SDL_GetWindowSize(w,&ww,&wh);
    int x=0,y=0;

    bool close = false;
    while(!close)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    close = true;
                    break;
                case SDL_KEYDOWN:
                    if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) close = true;
                    break;
                default:
                    break;
            }
        }

        SDL_RenderClear(r);
        render_floor(w,r,table->floors[FLOOR_GROUND],x,y,ww,wh);
        SDL_RenderPresent(r);
    }

    delete_tabletop(table);
}

void render_floor(SDL_Window *w, SDL_Renderer *r, level floor, int x, int y, int ww, int wh)
{
    SDL_Rect room = {0,0,400,400};
    room.x = ww/2 - 200;
    room.y = wh/2 - 200;
    if(!floor->anchor)
    {
        fprintf(stderr,"Floor missing anchor!!\n");
        return;
    }
    if(!floor->anchor->img)
    {
        fprintf(stderr,"Floor anchor missing img!!\n");
        return;
    }
    if(SDL_RenderCopy(r,floor->anchor->img,NULL,&room))
    {
        fprintf(stderr,"Render copy failed. %s\n",SDL_GetError());
        return;
    }
}
#include "../lib/mysdl.h"

int mySDL_Init(Uint32 init_flags, SDL_Window ** w, int win_w, int win_h,
    Uint32 win_flags, char * win_name, SDL_Renderer ** r)
{
    if(SDL_Init(init_flags))
    {
        fprintf(stderr,"Initialization failure: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }

    *w = SDL_CreateWindow(win_name,
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          win_w,win_h,win_flags);
    if(!(*w))
    {
        fprintf(stderr,"Window creation failure: %s\n",SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    *r = SDL_CreateRenderer(*w,-1,
                SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if(!(*r))
    {
        fprintf(stderr,"Renderer creation failure: %s\n",SDL_GetError());
        mySDL_Close(LEVEL_WIN,*w);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int mySDL_Maximize_Window(SDL_Window ** w, SDL_Renderer ** r)
{
    SDL_DisplayMode disp;
    if(SDL_GetDesktopDisplayMode(0,&disp))
    {
        fprintf(stderr,"Couldn't get display mode! %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_DestroyWindow(*w);
    *w = SDL_CreateWindow("Betrayal at House on the Hill",
                SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                disp.w,disp.h,
                SDL_WINDOW_MAXIMIZED | SDL_WINDOW_BORDERLESS);
    if(!(*w))
    {
        fprintf(stderr,"Window could not be resized! %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_DestroyRenderer(*r);
    *r = SDL_CreateRenderer(*w,-1,
                SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if(!(*r))
    {
        fprintf(stderr,"Window could not be resized! %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int mySDL_Close(int level, ...)
{
    va_list vars;
    va_start(vars,level);
    switch(level)
    {
        case LEVEL_TEX:
            SDL_DestroyTexture(va_arg(vars,SDL_Texture*));
        case LEVEL_REND:
            SDL_DestroyRenderer(va_arg(vars,SDL_Renderer*));
        case LEVEL_WIN:
            SDL_DestroyWindow(va_arg(vars,SDL_Window*));
        case LEVEL_INIT:
            SDL_Quit();
            break;
        default:
            fprintf(stderr,"Unknown mySDL_Close level!\n");
            va_end(vars);
            return EXIT_FAILURE;
            break;
    }
    va_end(vars);
    return EXIT_SUCCESS;
}
#include "../lib/mysdl.h"
#include "../lib/main_menu.h"

int main(int argc, char ** argv)
{
    srand((Uint64) main);
    SDL_Window * w;
    SDL_Renderer * r;
    int win_w = MAIN_MENU_WIDTH;
    int win_h = MAIN_MENU_HEIGHT;
    if(mySDL_Init(SDL_INIT_EVERYTHING,&w,win_w,win_h,
                  SDL_WINDOW_BORDERLESS,
                  "Betrayal at House on the Hill",&r))
    {
        return EXIT_FAILURE;
    }
    if(TTF_Init())
    {
        fprintf(stderr,"TTF_Init failure: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT, 2, 2048))
    {
        fprintf(stderr,"Mix_OpenAudio failure: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_RenderClear(r);
    SDL_RenderPresent(r);
    int next = summon_main_menu(w,r);
    switch(next)
    {
        case MAIN_MENU_ERROR:
            fprintf(stderr,"An unknown error occurred in the main menu! Quitting...\n");
            break;
        case START_HOST:    //  TODO: Begin Host game. Decide what to do at that moment (player count? Assume for now)
            break;
        case START_JOIN:
            break;
        case MAIN_MENU_OPTIONS:
            break;
        case MAIN_MENU_QUIT:
            break;
    }
    Mix_CloseAudio();
    TTF_Quit();
    mySDL_Close(LEVEL_REND,r,w);
    return EXIT_SUCCESS;
}
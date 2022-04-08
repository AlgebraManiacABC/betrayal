#include "../lib/main_menu.h"

int summon_main_menu(SDL_Window *w, SDL_Renderer *r)
{
    SDL_SetWindowSize(w,MAIN_MENU_WIDTH,MAIN_MENU_HEIGHT);
    SDL_SetWindowBordered(w,false);
    SDL_Texture * menu_bg_img = NULL;
    SDL_Surface * temp = IMG_Load(MENUBG);
    if(!temp)
    {
        fprintf(stderr,"Surface creation failure: %s\n",SDL_GetError());
        return MAIN_MENU_ERROR;
    }
    menu_bg_img = SDL_CreateTextureFromSurface(r,temp);
    if(!menu_bg_img)
    {
        fprintf(stderr,"Texture creation failure: %s\n",SDL_GetError());
        return MAIN_MENU_ERROR;
    }
    SDL_FreeSurface(temp);

    SDL_Rect menu_bg = {0,0,0,0};
    SDL_QueryTexture(menu_bg_img,NULL,NULL,&menu_bg.w,&menu_bg.h);

    TTF_Font *font72 = TTF_OpenFont(DINKFONT,72);
    if(!font72)
    {
        fprintf(stderr,"Font creation failure: %s\n",SDL_GetError());
        return MAIN_MENU_ERROR;
    }
    SDL_Color color_select[] =
    {
        {255,225,25},   //  Yellow, default
        {255,255,255}   //  White, selected
    };
    SDL_Texture *** txt_menu = malloc(sizeof(SDL_Texture **) * MAIN_MENU_COUNT);
    char menu_strings[MAIN_MENU_COUNT][10]=
        {"START","OPTIONS","QUIT","JOIN","HOST","< BACK"};
    for(int i=0; i<MAIN_MENU_COUNT; i++)
    {
        txt_menu[i] = malloc(sizeof(SDL_Texture *) * 2);
        txt_menu[i][false] =
            makeSolidTextureFromText(r,font72,menu_strings[i],color_select[false]);
        txt_menu[i][true]  =
            makeSolidTextureFromText(r,font72,menu_strings[i],color_select[true]);
    }
    TTF_CloseFont(font72);

    Mix_Music * ambience = Mix_LoadMUS(NIGHT_AMBIENCE);
    if(!ambience)
    {
        fprintf(stderr,"Music creation failure: %s\n",SDL_GetError());
        return MAIN_MENU_ERROR;
    }
    Mix_Chunk * creak    = Mix_LoadWAV(DOOR_CREAK);
    if(!creak)
    {
        fprintf(stderr,"Music creation failure: %s\n",SDL_GetError());
        return MAIN_MENU_ERROR;
    }

    Mix_PlayMusic(ambience,-1);

    int selection = MAIN_MENU_ERROR;
    int state = STATE_MAIN;
    while(selection == MAIN_MENU_ERROR || selection == MAIN_MENU_START || selection == START_BACK)
    {
        bool click_up = false;
        bool click_down = false;
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    selection = MAIN_MENU_QUIT;
                    break;
                case SDL_KEYDOWN:
                    if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                        selection = MAIN_MENU_QUIT;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(event.button.button == SDL_BUTTON_LEFT)
                        click_up = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(event.button.button == SDL_BUTTON_LEFT)
                        click_down = true;
                    break;
                default:
                    break;
            }
        }

        if(selection == MAIN_MENU_QUIT) break;
        Uint32 hover;
        selection = evaluateClicks(&hover, state, click_up, click_down);
        if(selection == MAIN_MENU_START)
            state = STATE_START;
        if(selection == START_BACK)
            state = STATE_MAIN;

        SDL_RenderClear(r);
        SDL_RenderCopy(r,menu_bg_img,NULL,&menu_bg);
        if(state == STATE_MAIN)
            present_main_options(r,
                txt_menu[MAIN_MENU_START][(bool)(hover&MASK_START)],
                txt_menu[MAIN_MENU_OPTIONS][(bool)(hover&MASK_OPTIONS)],
                txt_menu[MAIN_MENU_QUIT][(bool)(hover&MASK_QUIT)]);
        else if(state == STATE_START)
            present_start_options(r,
                txt_menu[START_JOIN][(bool)(hover&MASK_JOIN)],
                txt_menu[START_HOST][(bool)(hover&MASK_HOST)],
                txt_menu[START_BACK][(bool)(hover&MASK_BACK)]);
        SDL_RenderPresent(r);

    }

    for(int i=0; i<MAIN_MENU_COUNT; i++)
    {
        SDL_DestroyTexture(txt_menu[i][false]);
        SDL_DestroyTexture(txt_menu[i][true]);
        free(txt_menu[i]);
    }
    free(txt_menu);
    Mix_FreeMusic(ambience);
    Mix_FreeChunk(creak);

    switch(selection)
    {
        case START_JOIN:
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Not yet implemented","This feature is not yet implemented!",w);
            return MAIN_MENU_QUIT;
        case START_HOST:
            Mix_HaltMusic();
            Mix_PlayChannel(-1,creak,0);
            return START_HOST;
        case MAIN_MENU_OPTIONS:
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Not yet implemented","This feature is not yet implemented!",w);
            return MAIN_MENU_QUIT;
        case MAIN_MENU_QUIT:
            return MAIN_MENU_QUIT;
        default:
            return MAIN_MENU_ERROR;
    }
}

int evaluateClicks(Uint32 * hover, int state, bool click_up, bool click_down)
{
    //  The big idea:
    //
    //  Two states: Main Main Menu and Start State
    //
    //  Main Menu: only START/OPTIONS/QUIT shown, can hover over each and click.
    //  On click release (only happens one frame), if mouse was pressed and released on button, return that.
    //  START advances state to Start State
    //  
    //  Start: JOIN/HOST/BACK shown
    //  BACK returns to Main Menu

    static Uint32 click_held = MASK_NONE;

    int x,y;
    int selection = MAIN_MENU_ERROR;
    SDL_GetMouseState(&x,&y);
    if(state == STATE_MAIN)
    {
        if(mouse_over_start(x,y))
        {
            *hover = MASK_START;
            if(click_down)
                click_held = MASK_START;
            if(click_up && (click_held&MASK_START))
                selection = MAIN_MENU_START;
        }
        else if(mouse_over_options(x,y))
        {
            *hover = MASK_OPTIONS;
            if(click_down)
                click_held = MASK_OPTIONS;
            if(click_up && (click_held&MASK_OPTIONS))
                selection = MAIN_MENU_OPTIONS;
        }
        else if(mouse_over_quit(x,y))
        {
            *hover = MASK_QUIT;
            if(click_down)
                click_held = MASK_QUIT;
            if(click_up && (click_held&MASK_QUIT))
                selection = MAIN_MENU_QUIT;
        }
        else
            *hover = MASK_NONE;
    }
    else    //  state == STATE_START
    {
        if(mouse_over_join(x,y))
        {
            *hover = MASK_JOIN;
            if(click_down)
                click_held = MASK_JOIN;
            if(click_up && (click_held&MASK_JOIN))
                selection = START_JOIN;
        }
        else if(mouse_over_host(x,y))
        {
            *hover = MASK_HOST;
            if(click_down)
                click_held = MASK_HOST;
            if(click_up && (click_held&MASK_HOST))
                selection = START_HOST;
        }
        else if(mouse_over_back(x,y))
        {
            *hover = MASK_BACK;
            if(click_down)
                click_held = MASK_BACK;
            if(click_up && (click_held&MASK_BACK))
                selection = START_BACK;
        }
        else
            *hover = MASK_NONE;
    }

    if(click_up) click_held = MASK_NONE;
    return selection;
}

bool mouse_over_start(int x, int y)
{
    int start_w = 108;
    int start_h = 36;
    if( !((MAIN_MENU_WIDTH-250 < x)&&(x < MAIN_MENU_WIDTH-(250-start_w))) )
        return false;
    if((MAIN_MENU_HEIGHT/2 - 40 < y)&&(y < MAIN_MENU_HEIGHT/2 - 40 + start_h))
        return true;
    return false;
}

bool mouse_over_options(int x, int y)
{
    int options_w = 98;
    int options_h = 24;
    if( !((MAIN_MENU_WIDTH-250 < x)&&(x < MAIN_MENU_WIDTH-(250-options_w))) )
        return false;
    if((MAIN_MENU_HEIGHT/2 - 40 < y)&&(y < MAIN_MENU_HEIGHT/2 + options_h))
        return true;
    return false;
}

bool mouse_over_quit(int x, int y)
{
    int quit_w = 53;
    int quit_h = 24;
    if( !((MAIN_MENU_WIDTH-250 < x)&&(x < MAIN_MENU_WIDTH-(250-quit_w))) )
        return false;
    if((MAIN_MENU_HEIGHT/2 - 40 < y)&&(y < MAIN_MENU_HEIGHT/2 + 30 + quit_h))
        return true;
    return false;
}

void present_main_options
    (SDL_Renderer *r, SDL_Texture * start,
    SDL_Texture * options, SDL_Texture * quit)
{
    int start_x = MAIN_MENU_WIDTH - 250;
    int mid_y = MAIN_MENU_HEIGHT/2;
    SDL_Rect box_start = {start_x,mid_y-40};
    SDL_Rect box_options = {start_x,mid_y};
    SDL_Rect box_quit = {start_x,mid_y + 30};

    SDL_QueryTexture(start,NULL,NULL,&box_start.w,&box_start.h);
    SDL_QueryTexture(options,NULL,NULL,&box_options.w,&box_options.h);
    SDL_QueryTexture(quit,NULL,NULL,&box_quit.w,&box_quit.h);
    box_start.w /= 2;
    box_options.w /= 3;
    box_quit.w /= 3;
    box_start.h /= 2;
    box_options.h /= 3;
    box_quit.h /= 3;

    SDL_RenderCopy(r,start,NULL,&box_start);
    SDL_RenderCopy(r,options,NULL,&box_options);
    SDL_RenderCopy(r,quit,NULL,&box_quit);


    /*  Test to find out clickable regions
    static int print = 0;
    if(!print)
    {
        printf("box_start: [%d,%d] x [%d,%d] (%d x %d px)\n",
                box_start.x,box_start.x+box_start.w,box_start.y,box_start.y+box_start.h,box_start.w,box_start.h);
        printf("box_options: [%d,%d] x [%d,%d] (%d x %d px)\n",
                box_options.x,box_options.x+box_options.w,box_options.y,box_options.y+box_options.h,box_options.w,box_options.h);
        printf("box_quit: [%d,%d] x [%d,%d] (%d x %d px)\n",
                box_quit.x,box_quit.x+box_quit.w,box_quit.y,box_quit.y+box_quit.h,box_quit.w,box_quit.h);
        print++;
    }
    */
}

bool mouse_over_join(int x, int y)
{
    int join_w = 65;
    int join_h = 30;
    if( !((MAIN_MENU_WIDTH-300 < x)&&(x < MAIN_MENU_WIDTH-(300-join_w))) )
        return false;
    if((MAIN_MENU_HEIGHT/2 < y)&&(y < MAIN_MENU_HEIGHT/2 + join_h))
        return true;
    return false;
}

bool mouse_over_host(int x, int y)
{
    int host_w = 75;
    int host_h = 30;
    if( !((MAIN_MENU_WIDTH-200 < x)&&(x < MAIN_MENU_WIDTH-(200-host_w))) )
        return false;
    if((MAIN_MENU_HEIGHT/2 < y)&&(y < MAIN_MENU_HEIGHT/2 + host_h))
        return true;
    return false;
}

bool mouse_over_back(int x, int y)
{
    int back_w = 92;
    int back_h = 24;
    if( !((MAIN_MENU_WIDTH-270 < x)&&(x < MAIN_MENU_WIDTH-(270-back_w))) )
        return false;
    if((MAIN_MENU_HEIGHT/2 + 50 < y)&&(y < MAIN_MENU_HEIGHT/2 + 50 + back_h))
        return true;
    return false;
}

void present_start_options
    (SDL_Renderer *r, SDL_Texture * join,
    SDL_Texture * host, SDL_Texture * back)
{
    int join_x = MAIN_MENU_WIDTH - 300;
    int host_x = join_x + 100;
    int mid_y = MAIN_MENU_HEIGHT/2;
    SDL_Rect box_join = {join_x,mid_y};
    SDL_Rect box_host = {host_x,mid_y};
    SDL_Rect box_back = {join_x + 30,mid_y + 50};

    SDL_QueryTexture(join,NULL,NULL,&box_join.w,&box_join.h);
    SDL_QueryTexture(host,NULL,NULL,&box_host.w,&box_host.h);
    SDL_QueryTexture(back,NULL,NULL,&box_back.w,&box_back.h);
    box_join.w /= 2.4;
    box_host.w /= 2.4;
    box_back.w /= 3;
    box_join.h /= 2.4;
    box_host.h /= 2.4;
    box_back.h /= 3;

    SDL_RenderCopy(r,join,NULL,&box_join);
    SDL_RenderCopy(r,host,NULL,&box_host);
    SDL_RenderCopy(r,back,NULL,&box_back);


    //  Test to find out clickable regions
    /*
    static int print = 0;
    if(!print)
    {
        printf("box_join: [%d,%d] x [%d,%d] (%d x %d px)\n",
                box_join.x,box_join.x+box_join.w,box_join.y,box_join.y+box_join.h,box_join.w,box_join.h);
        printf("box_host: [%d,%d] x [%d,%d] (%d x %d px)\n",
                box_host.x,box_host.x+box_host.w,box_host.y,box_host.y+box_host.h,box_host.w,box_host.h);
        printf("box_back: [%d,%d] x [%d,%d] (%d x %d px)\n",
                box_back.x,box_back.x+box_back.w,box_back.y,box_back.y+box_back.h,box_back.w,box_back.h);
        print++;
    }
    */
}
#include "../lib/game_loop.h"

void play_game(SDL_Window *w, SDL_Renderer *r)
{
    tabletop table = init_tabletop(w,r);
    int ww,wh;
    SDL_GetWindowSize(w,&ww,&wh);
    int x=0,y=0;

    bool close = false;
    int zoom = DEFAULT_ZOOM;  //  Percentage of the screen's width that a room will take up
    bool mouseleftdown = false;
    int mx,my;
    int ogmx,ogmy;
    int current_floor = FLOOR_GROUND;
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
                    switch(event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_ESCAPE:
                            close = true;
                            break;
                        case SDL_SCANCODE_0:
                            current_floor = FLOOR_BASEMENT;
                            break;
                        case SDL_SCANCODE_1:
                            current_floor = FLOOR_GROUND;
                            break;
                        case SDL_SCANCODE_2:
                            current_floor = FLOOR_UPPER;
                            break;
                        case SDL_SCANCODE_LEFTBRACKET:
                            cycle_bg(r,table,FORWARD);
                            break;
                        case SDL_SCANCODE_RIGHTBRACKET:
                            cycle_bg(r,table,BACKWARD);
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    if(event.wheel.y > 0)
                    {
                        if(zoom < 50) zoom += 5;
                    }
                    else if(event.wheel.y < 0)
                    {
                        if(zoom > 5) zoom -= 5;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(event.button.button == SDL_BUTTON_LEFT)
                    {
                        mouseleftdown = true;
                        SDL_GetMouseState(&ogmx,&ogmy);
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(event.button.button == SDL_BUTTON_LEFT)
                        mouseleftdown = false;
                    break;
                default:
                    break;
            }
        }

        SDL_GetMouseState(&mx,&my);
        if(mouseleftdown)
        {
            x += mx - ogmx;
            ogmx = mx;
            y += my - ogmy;
            ogmy = my;
        }
        int maxx = (table->floors[current_floor]->d_left) * (ww * zoom/100.0) + ww/2;
        int minx = -1 * (table->floors[current_floor]->d_right) * (ww * zoom/100.0) - ww/2;
        int maxy = (table->floors[current_floor]->d_down) * (ww * zoom/100.0) + wh/2;
        int miny = -1 * (table->floors[current_floor]->d_up) * (ww * zoom/100.0) - wh/2;
        if(x > maxx) x = maxx;
        if(x < minx) x = minx;
        if(y > maxy) y = maxy;
        if(y < miny) y = miny;

        SDL_RenderClear(r);
        render_bg(r,table->bgs[table->bg_id]);
        render_floor(r,table->floors[current_floor],x,y,ww,wh,zoom);
        SDL_RenderPresent(r);
    }

    delete_tabletop(table);
}

void render_bg(SDL_Renderer *r, SDL_Texture * bg)
{
    if(!bg) return;
    SDL_Rect back = {0,0,0,0};
    SDL_QueryTexture(bg,NULL,NULL,&back.w,&back.h);
    SDL_RenderCopy(r,bg,NULL,&back);
}

void render_floor(SDL_Renderer *r, level floor, int x, int y, int ww, int wh, int zoom)
{
    if(!floor->anchor)
    {
        fprintf(stderr,"Floor missing anchor!!\n");
        return;
    }
    render_room(r,floor->anchor,x,y,ww,wh,zoom);
    floor->anchor->render = !floor->anchor->render;
    bool render = floor->anchor->render;
    list q = init_list();
    if(floor->anchor->N) insert_node(q,floor->anchor->N);
    if(floor->anchor->S) insert_node(q,floor->anchor->S);
    if(floor->anchor->E) insert_node(q,floor->anchor->E);
    if(floor->anchor->W) insert_node(q,floor->anchor->W);
    while(!empty(q))
    {
        int len = list_length(q);
        for(int i=0; i<len; i++)
        {
            tile room = remove_head(q);
            if(room->render == render) continue;

            if(room->N) insert_node(q,room->N);
            if(room->S) insert_node(q,room->S);
            if(room->E) insert_node(q,room->E);
            if(room->W) insert_node(q,room->W);
            render_room(r,room,x,y,ww,wh,zoom);
            room->render = render;
        }
    }
}

void render_room(SDL_Renderer *r, tile room, int x, int y, int ww, int wh, int zoom)
{
    if(!room->img)
    {
        if(!room->name)
        {
            fprintf(stderr,"Unknown room missing img!\n");
            return;
        }
        fprintf(stderr,"Room \"%s\" missing img!\n",room->name);
        return;
    }
    SDL_Rect square;
    square.w = ww * (zoom/100.0);//Need to worry about rounding?
    square.h = square.w;
    if(!strcmp(room->name,"entrancehall")) square.w += (square.w * 75.0/400.0);
    square.x = (ww/2) - (square.w/2) + (room->x * square.w) + x;
    square.y = (wh/2) - (square.h/2) + (room->y * square.h) + y;
    SDL_RenderCopy(r,room->img,NULL,&square);
}
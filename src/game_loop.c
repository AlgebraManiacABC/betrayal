#include "../lib/game_loop.h"

void play_game(SDL_Window *w, SDL_Renderer *r)
{
    tabletop table = init_tabletop(w,r);
    int ww,wh;
    SDL_GetWindowSize(w,&ww,&wh);
    int offx=0,offy=0;
    int maxoffx,minoffx,maxoffy,minoffy;
    int tilewidth;

    //TTF_Font * debugfont = TTF_OpenFont(SANSFONT,48);

    bool close = false;
    int zoom = DEFAULT_ZOOM;  //  Percentage of the screen's width that a room will take up
    bool zoomchange = true;
    bool update_offsets = true;
    bool mouseleftdown = false;
    bool mousedrag = false;
    bool mouseclick = false;
    int mx,my;
    int mx0,my0;
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
                            update_offsets = true;
                            break;
                        case SDL_SCANCODE_1:
                            current_floor = FLOOR_GROUND;
                            update_offsets = true;
                            break;
                        case SDL_SCANCODE_2:
                            current_floor = FLOOR_UPPER;
                            update_offsets = true;
                            break;
                        case SDL_SCANCODE_LEFTBRACKET:
                            cycle_bg(r,table,FORWARD);
                            break;
                        case SDL_SCANCODE_RIGHTBRACKET:
                            cycle_bg(r,table,BACKWARD);
                            break;
                        case SDL_SCANCODE_RETURN:
                            discover_room(table,current_floor,table->highlight->x,table->highlight->y,MASK_NONE);
                            update_offsets = true;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    if(event.wheel.y > 0)
                    {
                        if(zoom < 50)
                        {
                            zoom += 5;
                            zoomchange = true;
                            update_offsets = true;
                        }
                    }
                    else if(event.wheel.y < 0)
                    {
                        if(zoom > 5)
                        {
                            zoom -= 5;
                            zoomchange = true;
                            update_offsets = true;
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(event.button.button == SDL_BUTTON_LEFT)
                    {
                        mouseleftdown = true;
                        SDL_GetMouseState(&mx0,&my0);
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(event.button.button == SDL_BUTTON_LEFT)
                    {
                        mouseleftdown = false;
                        if(!mousedrag)
                        {
                            mouseclick = true;
                        }
                        mousedrag = false;
                    }
                    break;
                default:
                    break;
            }
        }

        SDL_GetMouseState(&mx,&my);
        if(mouseleftdown)
        {
            if(abs(mx-mx0)>2 || abs(my-my0)>2)
            {
                mousedrag = true;
                offx += mx - mx0;
                mx0 = mx;
                offy += my - my0;
                my0 = my;
            }
        }
        if(zoomchange)
        {
            tilewidth = ww * zoom/100.0;
            zoomchange = false;
        }
        if(update_offsets)
        {
            maxoffx = (table->floors[current_floor]->maxtilex) * tilewidth + ww/2;
            minoffx = (table->floors[current_floor]->mintilex) * tilewidth - ww/2;
            maxoffy = (table->floors[current_floor]->maxtiley) * tilewidth + wh/2;
            minoffy = (table->floors[current_floor]->mintiley) * tilewidth - wh/2;
            update_offsets = false;
        }
        if(offx > maxoffx) offx = maxoffx;
        if(offx < minoffx) offx = minoffx;
        if(offy > maxoffy) offy = maxoffy;
        if(offy < minoffy) offy = minoffy;
        if(mouseclick)
        {
            mouseclick = false;
            mousepos2tile(mx,my,offx,offy,ww,wh,tilewidth,&(table->highlight->x),&(table->highlight->y));
        }

        SDL_RenderClear(r);
        render_bg(r,table->bgs[table->bg_id]);
        render_floor(r,table->floors[current_floor],offx,offy,ww,wh,zoom);
        render_selection(r,table->highlight,offx,offy,ww,wh,zoom);
        SDL_RenderPresent(r);
    }

    delete_tabletop(table);
}

void mousepos2tile(int mx, int my, int offx, int offy, int ww, int wh, int tilewidth, int * tilex, int * tiley)
{
    int anchorx = (ww/2) - (tilewidth/2) + offx;
    int anchory = (wh/2) - (tilewidth/2) + offy;

    *tilex = floor((mx-anchorx)/(float)tilewidth);
    *tiley = ceil(-1 * (my-anchory)/(float)tilewidth);
}

void render_bg(SDL_Renderer *r, SDL_Texture * bg)
{
    if(!bg) return;
    SDL_Rect back = {0,0,0,0};
    SDL_QueryTexture(bg,NULL,NULL,&back.w,&back.h);
    SDL_RenderCopy(r,bg,NULL,&back);
}

void render_floor(SDL_Renderer *r, level floor, int offx, int offy, int ww, int wh, int zoom)
{
    if(!floor->anchor)
    {
        fprintf(stderr,"Floor missing anchor!!\n");
        return;
    }
    render_room(r,floor->anchor,offx,offy,ww,wh,zoom);
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
            render_room(r,room,offx,offy,ww,wh,zoom);
            room->render = render;
        }
    }
}

void render_room(SDL_Renderer *r, tile room, int offx, int offy, int ww, int wh, int zoom)
{
    if(!room) return;
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
    square.x = (ww/2) - (square.w/2) + (room->x * square.w) + offx;
    square.y = (wh/2) - (square.h/2) - (room->y * square.h) + offy;
    if(!strcmp(room->name,"entrancehall")) square.w += (square.w * 75.0/400.0);
    SDL_RenderCopy(r,room->img,NULL,&square);
}

void render_selection(SDL_Renderer *r, tile select, int offx, int offy, int ww, int wh, int zoom)
{
    render_room(r,select,offx,offy,ww,wh,zoom);
}

int discover_room(tabletop table, int floor, int x, int y, Uint8 from)
{
    if(get_room(table,floor,x,y) || is_room(table,floor,x-1,y,"entrancehall"))
    {
        return 0;//No room discovered (already discovered or cannot be discovered)
    }

    //  Check that new pos is adjacent to existing DOOR (not just room)
    tile check = get_room(table,floor,x,y+1);//North
    if(!check || !(check->doors & MASK_S))
    {
        check = get_room(table,floor,x+1,y);//East
        if(!check || !(check->doors & MASK_W))
        {
            check = get_room(table,floor,x,y-1);//South
            if(!check || !(check->doors & MASK_N))
            {
                check = get_room(table,floor,x-1,y);//West
                if(!check || !(check->doors & MASK_E))
                    return 0;//Room not adjacent to a door
            }
        }
    }

    tile new = draw_valid_room(table->deck,1<<floor);
    if(!new)
    {
        return 0;//Cannot be discovered (no rooms left)
    }

    //  Check that new room will not close off floor if placed

    //  If only one door, place. Else, let the user rotate until okay

    //  Check if room was last from floor

    insert_node(table->floors[floor]->rooms,new);
    connect_four(table,floor,new,x,y);
    new->x = x;
    new->y = y;
    //fprintf(stderr,"New room \"%s\" discovered at (%d,%d)\n",new->name,x,y);

    //  Increase distances from anchor if necessary for floor
    if(x > table->floors[floor]->maxtilex)      table->floors[floor]->maxtilex = x;
    else if(x < table->floors[floor]->mintilex) table->floors[floor]->mintilex = x;
    else if(y > table->floors[floor]->maxtiley) table->floors[floor]->maxtiley = y;
    else if(y < table->floors[floor]->mintiley) table->floors[floor]->mintiley = y;

    return 1;
}

int uint8_bin(Uint8 bin)
{
    int ten = 1;
    int dec = 0;
    for(int i=0; i<8; i++, ten*=10)
    {
        if(bin & (1<<i))
            dec += ten;
    }
    return dec;
}

void connect_four(tabletop table, int floor, tile here, int x, int y)
{
    tile N = get_room(table,floor,x,y+1);
    tile E = get_room(table,floor,x+1,y);
    tile S = get_room(table,floor,x,y-1);
    tile W = get_room(table,floor,x-1,y);

    if(N)
    {
        N->S = here;
        here->N = N;
    }
    if(E)
    {
        E->W = here;
        here->E = E;
    }
    if(S)
    {
        S->N = here;
        here->S = S;
    }
    if(W)
    {
        W->E = here;
        here->W = W;
    }
}

tile get_room(tabletop table, int floor, int x, int y)
{
    node check = table->floors[floor]->rooms;
    while((check = check->next))
    {
        if(((tile)check->obj)->x == x && ((tile)check->obj)->y == y)
            return (tile)check->obj;
    }
    return NULL;
}

bool is_room(tabletop table, int floor, int x, int y, char * roomname)
{
    node check = table->floors[floor]->rooms;
    while((check = check->next))
    {
        if(!strcmp(((tile)check->obj)->name,roomname))
        {
            if(((tile)check->obj)->x == x && ((tile)check->obj)->y == y)
                return true;
            else
                return false;
        }
    }
    return false;
}

/**
 * Earlier I was wondering whether to check if a room existed before I called discover_room.
 * I have decided to call room_exists within discover_room. I thought it was a bit like user-developer interaction.
 * Here, whatever function calling discover_room is the user, and discover_room is the developer's work.
 * We should let the user make assumptions and have the developer correct them; otherwise the developer
 * assumes the user is all-knowing. In general, the deeper you go into the function rabbit-hole,
 * the more checks you will have to make. Conversely, the more shallow you go, the less details you need to sweat.
 * Otherwise it would get very clunky upwards, and I don't like the sound of that.
 * 
 */
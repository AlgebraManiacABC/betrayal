#include "../lib/tabletop.h"
#include "../lib/main_menu.h"

tabletop init_tabletop(SDL_Window *w, SDL_Renderer *r)
{
    list deck = load_rooms(r);
    tabletop table = malloc(sizeof(tabletop_s));
    if(!table)
    {
        fprintf(stderr,"Memory error initializing tabletop.\n");
        return NULL;
    }
    for(int i=0; i<FLOOR_COUNT; i++)
        table->floors[i] = init_floor(r,i,deck);
    
    table->deck = deck;
    table->bgs = load_bgs(r);
    table->bg_id = 0;

    return table;
}

SDL_Texture ** load_bgs(SDL_Renderer *r)
{
    char * bg_files[BG_COUNT]={NULL,DARK_BG,LIGHT_BG,MONOCHROME_BG,ROUGH_BG};
    SDL_Texture ** bg_textures = malloc(sizeof(SDL_Texture *));
    SDL_Surface * temp;
    for(int i=0; i<BG_COUNT; i++)
    {
        if(!bg_files[i])
        {
            bg_textures[i] = NULL;
            continue;
        }
        temp = IMG_Load(bg_files[i]);
        bg_textures[i] = SDL_CreateTextureFromSurface(r,temp);
        SDL_FreeSurface(temp);
    }
    return bg_textures;
}

void cycle_bg(SDL_Renderer *r, tabletop table, bool dir)
{
    if(dir)
    {
        if(++(table->bg_id)>BG_COUNT-1)
            table->bg_id = 0;
    }
    else
    {
        if(--(table->bg_id)<0)
            table->bg_id = BG_COUNT-1;
    }
}

level init_floor(SDL_Renderer *r, int floor, list deck)
{
    level new = malloc(sizeof(level_s));
    if(!new)
    {
        fprintf(stderr,"Memory error initializing floor %d.\n",floor);
        return NULL;
    }
    new->d_up = 0;
    new->d_down = 0;
    new->d_left = 0;
    new->d_right = 0;

    switch(floor)
    {
        case FLOOR_BASEMENT:
            new->anchor = draw_specific_room(deck,"basementlanding");
            new->anchor->x = 0;
            new->anchor->y = 0;
            break;
        case FLOOR_GROUND:
            new->anchor = draw_specific_room(deck,"foyer");
            new->anchor->x = 0;
            new->anchor->y = 0;
            new->anchor->W = draw_specific_room(deck,"grandstaircase");
            new->anchor->W->E = new->anchor;
            new->anchor->W->x = -1;
            new->anchor->W->y = 0;
            new->anchor->E = draw_specific_room(deck,"entrancehall");
            new->anchor->E->E = new->anchor->W;
            new->anchor->E->x = 1;
            new->anchor->E->y = 0;
            new->d_left = 1;
            new->d_right = 1;
            break;
        case FLOOR_UPPER:
            new->anchor = draw_specific_room(deck,"upperlanding");
            new->anchor->x = 0;
            new->anchor->y = 0;
            break;
        case FLOOR_ROOF:
        default:
            new->anchor = NULL;
            break;
    }
    if(!new->anchor)
    {
        fprintf(stderr,"Failed to initialize floor!\n");
    }
    return new;
}

list load_rooms(SDL_Renderer *r)
{
    list head = init_list();
    if(!head) return NULL;

    FILE * fp = fopen(ROOMS_TABLE,"r");
    if(!fp)
    {
        fprintf(stderr,"File error! %s not found or could not be opened.\n",ROOMS_TABLE);
        return NULL;
    }

    node n = head;
    fscanf(fp,"%*[^\n]");
    while(fp && !feof(fp))
    {
        tile t = malloc(sizeof(tile_s));
        if(!t)
        {
            fprintf(stderr,"Memory error! Couldn't load tile!\n");
            return NULL;
        }

        char buf[50]={0};
        fscanf(fp," %[^,],",buf);
        if(buf[0] == '#')
        {
            fscanf(fp,"%*[^\n]");
            continue;
        }

        t->name = malloc(sizeof(char) * (strlen(buf)+1) );
        strcpy(t->name,buf);

        n->next = make_node(t,NULL);
        if(!n->next) return NULL;
        n = n->next;
        n->obj = t;
        
        t->img = get_tile_img(r,t->name);

        fscanf(fp,"%[^,],",buf);

        if(!strcmp(buf,"Omen"))
            t->type = CARD_OMEN;
        else if(!strcmp(buf,"Item"))
            t->type = CARD_ITEM;
        else if(!strcmp(buf,"Event"))
            t->type = CARD_EVENT;
        else
            t->type = 0;

        t->legal_floors = MASK_NONE;
        fscanf(fp,"%[^,],",buf);
        if(buf[0]) t->legal_floors |= MASK_ROOF;
        if(buf[1]) t->legal_floors |= MASK_UPPER;
        if(buf[2]) t->legal_floors |= MASK_GROUND;
        if(buf[3]) t->legal_floors |= MASK_BASEMENT;

        t->doors = MASK_NONE;
        fscanf(fp,"%[^,],",buf);
        if(buf[0]) t->doors |= MASK_DOOR_N;
        if(buf[1]) t->doors |= MASK_DOOR_S;
        if(buf[2]) t->doors |= MASK_DOOR_E;
        if(buf[3]) t->doors |= MASK_DOOR_W;

        t->N = NULL;
        t->S = NULL;
        t->E = NULL;
        t->W = NULL;
        t->orientation = O_NORTH;
        
        int a,b,c;
        fscanf(fp,"%d,%d,%d",&a,&b,&c);
        t->dumbwaiter = (a?true:false);
        t->outside    = (b?true:false);
        t->window     = (c?true:false);

        t->render = false;
    }
    fclose(fp);

    return head;
}

SDL_Texture * get_tile_img(SDL_Renderer *r, char * tilename)
{
    char * file = malloc(sizeof(char*) * (strlen(tilename)+strlen("src/img/rooms/x.jpg")));
    if(!file)
    {
        fprintf(stderr,"Memory error! Couldn't allocate memory for filename string!\n");
        return NULL;
    }
    strcpy(file,"src/img/rooms/");
    strcat(file,tilename);
    strcat(file,".jpg");
    SDL_Surface * temp = IMG_Load(file);
    if(!temp)
    {
        fprintf(stderr,"Couldn't create surface: %s\n",SDL_GetError());
        free(file);
        return NULL;
    }
    SDL_Texture * img = SDL_CreateTextureFromSurface(r,temp);
    SDL_FreeSurface(temp);
    free(file);
    return img;
}

tile draw_specific_room(list deck, char * tilename)
{
    node n = deck;
    while( (n->next) && (n->next->obj) && strcmp(((tile)n->next->obj)->name,tilename))
        n = n->next;
    if(n == NULL)
    {
        fprintf(stderr,"Couldn't find room \"%s\"!\n",tilename);
        return NULL;
    }
    node temp = n->next;
    tile found = (tile)temp->obj;
    n->next = temp->next;
    free(temp);
    return found;
}

void delete_tabletop(tabletop table)
{
    for(int i=0; i<FLOOR_COUNT; i++)
        delete_floor(table->floors[i]);
    free(table->bgs);
}

void delete_floor(level floor)
{
    //Apply BFS/DFS to destroy all rooms
}
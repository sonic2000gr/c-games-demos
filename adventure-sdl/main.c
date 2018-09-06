/* Adventure SDL
   Uses SDL_image library that has to be installed */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include <SDL_ttf.h>

// FRAME_MS -> millis for one frame. Set to adjust desired frame rate as 1000/FRAME_MS

#define FRAME_MS 17
#define LOSSROOMS 3
#define DIRECTIONS 4
#define NO_EXIT -1
#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3
#define SPEED 0.5f

// Window WIDTH and HEIGHT

#define WIDTH 600
#define HEIGHT 600

SDL_Window* window;
SDL_Surface* screen;
typedef struct {
    int x;
    int y;
} point;
typedef struct {
    SDL_Surface* wall;
    SDL_Surface* doorh;
    SDL_Surface* doorv;
    SDL_Surface* tile;
    SDL_Rect coord;
    SDL_Rect drect1;
    SDL_Rect drect2;
    SDL_Rect drect3;
    SDL_Rect drect4;
} roomelement;
typedef struct {
    SDL_Surface* human;
    float ux;
    float uy;
    SDL_Rect coord;
} sprite;

int move(sprite* human, roomelement r, uint32_t elapsedTime, int moves[][DIRECTIONS], int room)  {
    int lyu = 150;
    int lyd = 550;
    int lxl = 50;
    int lxr = 550;
    int temproom = room;
    human->coord.y += human->uy * elapsedTime;
    human->coord.x += human->ux * elapsedTime;

    if(moves[room][SOUTH] != NO_EXIT &&
            human->coord.x >= r.drect2.x &&
            human->coord.x <= r.drect2.x + r.drect2.w - human->coord.w)
        lyd = 600;
    if(moves[room][NORTH] != NO_EXIT &&
            human->coord.x >= r.drect1.x &&
            human->coord.x <= r.drect1.x + r.drect1.w - human->coord.w)
        lyu = 100;
    if(moves[room][EAST] != NO_EXIT &&
            human->coord.y >= r.drect3.y + 100 &&
            human->coord.y <= r.drect3.y + 100 + r.drect3.h - human->coord.h)
        lxr = 600;
    if(moves[room][WEST] != NO_EXIT &&
            human->coord.y >= r.drect4.y + 100 &&
            human->coord.y <= r.drect4.y + 100 + r.drect4.h - human->coord.h)
        lxl = 1;
    // printf("%u\n",r.drect4.h);
    if(human->coord.x < lxl)    {
        human->coord.x = lxl;
        if(lxl == 1) {
            temproom = moves[room][WEST];
            human->coord.x = 500;
        }
    }
    if(human->coord.y < lyu)    {
        human->coord.y = lyu;
        if(lyu == 100) {
            temproom = moves[room][NORTH];
            human->coord.y = 500;
        }
    }
    if(human->coord.x > lxr - human->human->w)  {
        human->coord.x = lxr - human->human->w;
        if(lxr == 600) {
            temproom = moves[room][EAST];
            human->coord.x = 50;
        }
    }
    if(human->coord.y > lyd - human->human->h)  {
        human->coord.y = lyd - human->human->h;
        if(lyd == 600) {
            temproom = moves[room][SOUTH];
            human->coord.y = 150;
        }
    }

    return temproom;
}

void centerImgH(SDL_Surface* wall, SDL_Surface* door, SDL_Rect* imgrect)  {
    imgrect->x = (wall->w - door->w) / 2;
}

void centerImgV(SDL_Surface* wall, SDL_Surface* door, SDL_Rect* imgrect)  {
    imgrect->y = (wall->h - door->h) / 2;
}

void centerMsg(SDL_Surface* message, SDL_Rect* msgrect) {
    msgrect->x = (screen->w - message->w) / 2;
}

int checkloss(int *loss, int room) {
    for (int i = 0; i < LOSSROOMS; i++ )
        if (room == loss[i])
            return 1;
    return 0;
}

SDL_Surface* hwall(SDL_Surface* wall, int width)   {
    float i;
    int j,k;
    SDL_Rect coord;
    SDL_Surface* result;
    coord.x = 0;
    coord.y = 0;

    i = (float)width / wall->w;
    j =  width / wall->w;
    if(i - j >= 0.5)
        j++;
    result = SDL_CreateRGBSurface(0, j * wall->w, wall->h, 32, 0, 0, 0, 0);
    for(k = 0; k < j; k++)  {
        SDL_BlitSurface(wall, 0, result, &coord);
        coord.x = coord.x + wall->w;
    }

    return result;
}

SDL_Surface* vwall(SDL_Surface* wall, int height)   {
    float i;
    int j,k;
    SDL_Rect coord;
    SDL_Surface* result;
    coord.x = 0;
    coord.y = 0;

    i = (float)height / wall->h;
    j =  height / wall->h;
    if(i - j >= 0.5)
        j++;
    result = SDL_CreateRGBSurface(0, wall->w,j * wall->h, 32, 0, 0, 0, 0);
    for(k = 0; k < j; k++)  {
        SDL_BlitSurface(wall, 0, result, &coord);
        coord.y = coord.y + wall->h;
    }

    return result;

}

SDL_Surface* drawRoom(roomelement* r, int moves[][DIRECTIONS], int room, int width, int height) {
    SDL_Surface* result,*w1,*w2,*f1;
    r->drect1.y = 0;
    r->drect3.x = 0;
    r->coord.x = 0;
    r->coord.y = 0;
    float i;
    int w,h,j;

    i = (float)width / r->wall->w;
    w =  width / r->wall->w;
    if(i - w >= 0.5)
        w++;
    i = (float)height / r->wall->h;
    h =  height / r->wall->h;
    if(i - h >= 0.5)
        h++;

    result = SDL_CreateRGBSurface(0, w * r->wall->w, h * r->wall->h, 32, 0, 0, 0, 0);
    f1 = hwall(r->tile, w* r->wall->w);
    for(j = 0; j < h * r->wall->h; j = j + r->tile->h)  {
        r->coord.y = j;
        SDL_BlitSurface(f1, 0, result, &r->coord);
    }
    w1 = hwall(r->wall, w * r->wall->w);
    w2 = vwall(r->wall, h * r->wall->h);
    r->coord.y = 0;
    centerImgH(w1, r->doorh, &r->drect1);
    SDL_BlitSurface(w1, 0, result, &r->coord);
    r->coord.y = r->wall->h * h - r->wall->h;
    r->drect2.y = r->coord.y;
    centerImgH(w1, r->doorh, &r->drect2);
    centerImgV(w2, r->doorv, &r->drect3);
    SDL_BlitSurface(w1, 0, result, &r->coord);
    r->coord.y = 0;
    SDL_BlitSurface(w2, 0, result, &r->coord);
    r->coord.x = r->wall->w * w - r->wall->w;
    r->drect4.x = r->coord.x;
    centerImgV(w2, r->doorv, &r->drect4);
    SDL_BlitSurface(w2, 0, result, &r->coord);
    if (moves[room][NORTH] != NO_EXIT)
        SDL_BlitSurface(r->doorh, 0, result, &r->drect1);
    if (moves[room][SOUTH] != NO_EXIT)
        SDL_BlitSurface(r->doorh, 0, result, &r->drect2);
    if (moves[room][WEST] != NO_EXIT)
        SDL_BlitSurface(r->doorv, 0, result, &r->drect3);
    if (moves[room][EAST] != NO_EXIT)
        SDL_BlitSurface(r->doorv, 0, result, &r->drect4);
// printf("w3=%u h3=%u w4=%u h4=%u\n", r->drect3.w, r->drect3.h, r->drect4.w, r->drect4.h);
    SDL_FreeSurface(w1);
    SDL_FreeSurface(w2);
    SDL_FreeSurface(f1);

    return result;
}

int main(int argc, char* argv[]) {
    SDL_Event event;
    TTF_Font* font;
    roomelement r;
    sprite human;
    SDL_Surface* msg2;
    SDL_Surface* result;
    const char *msgw = "You Won!";
    const char *msgl = "You Lost!";
    SDL_Color textColor = { 255, 255, 255, 0 };
    SDL_Surface* message;
    SDL_Rect msgrect,msgrect2;
    SDL_Rect wallrect;
    human.coord.x = 250;
    human.coord.y = 300;
    wallrect.x = 0;
    wallrect.y = 100;
    msgrect.x = 0;
    msgrect.y = 0;
    msgrect2.x = 0;
    msgrect2.y = 50;
    SDL_Surface* humanup = IMG_Load("humanu.png");
    SDL_Surface* humandown = IMG_Load("humand.png");
    SDL_Surface* humanleft = IMG_Load("humanl.png");
    SDL_Surface* humanright = IMG_Load("humanr.png");
    human.human = humandown;
    r.wall = IMG_Load("brick.png");
    r.doorh = IMG_Load("d1.png");
    r.doorv = IMG_Load("d2.png");
    r.tile = IMG_Load("tile.png");
    const char *rooms[] = { "You are in the garden. It's dark",
                            "You are in the bathroom. You hear noises.",
                            "You are in the hall. There is a staircase east. ",
                            "You are in the storage. You can barely breath. Path south, stairs west",
                            "You are in the living room. You found your father",
                            "Briskesai sto saloni. Akougetai mousiki.",
                            "Briskesai sthn trapezaria. Ta panta einai anastata",
                            "Briskesai sto diadromo. Einai skoteina. Skala notia",
                            "Eisai sthn kouzina. Akous fones",
                            "Eisai ston pano diadromo. Pantou hsyxia. Skala boreia",
                            "Eisai sto domation tou aderfou sou. O aderfos sou se martyra!",
                            "Eisai sto domatio ton goneon sou. H mhtera sou se epiase!",
                            "Eisai sto domatio sou. Eisai asfalis."
                          };

    int moves[][4] = {  { -1, 2, -1, -1 },
                        { -1, -1, 2, -1 },
                        { 0, 5, 3, 1 },
                        { -1, 8, -1, 2 },
                        { -1, 6, 5, -1 },
                        { 2, 8, -1, 4 },
                        { 4, 7, -1, -1 },
                        { 6, 9, 8, -1 },
                        { 5, -1, -1, 7 },
                        { 7, 11, 12, 10 },
                        { -1, -1, 9, -1 },
                        { 9, -1, -1, -1 },
                        { -1, -1, -1, 9 }
                    };

    int winroom = 12;
    int lossrooms[] = {4, 10, 11};
    int room = 0;
    int endgame = 0;
    int running = 1;

    // This is to keep the main loop in operation

    uint32_t elapsedTime, currentTime, lastTime;


    // Initialize SDL library or fail and exit

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0 || TTF_Init() < 0)
        exit(1);
    font = TTF_OpenFont( "impact.ttf", 20 );
    if(!font)
        printf("Failed to load font!");
    // Attempt to create window

    window = SDL_CreateWindow("Adventure",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WIDTH, HEIGHT,
                              SDL_WINDOW_SHOWN);

    // Exit if we fail to create window

    if(!window)
        exit(1);

    // Create surface
    screen = SDL_GetWindowSurface(window);

    srand(time(NULL));

    SDL_GL_SetSwapInterval(1);

    lastTime = SDL_GetTicks();
    while(running) {

        // Get millis since SDL initialization

        currentTime = SDL_GetTicks();

        // Compute last frame millis

        elapsedTime = currentTime - lastTime;

        // if frame lasted less than desired millis, wait
        // for the remainder of the time

        if (elapsedTime < FRAME_MS) {
            SDL_Delay(FRAME_MS - elapsedTime);
            elapsedTime = FRAME_MS;
        }

        human.ux = 0;
        human.uy = 0;

        // Check for events (currently quit only)

        if (SDL_PollEvent(&event))  {
            switch (event.type) {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q)
                    running = 0;
                if (event.key.keysym.sym == SDLK_UP)  {
                    human.uy = -SPEED;
                    human.human = humanup;
                }
                if (event.key.keysym.sym == SDLK_DOWN)  {
                    human.uy = SPEED;
                    human.human = humandown;
                }
                if (event.key.keysym.sym == SDLK_RIGHT)  {
                    human.ux = SPEED;
                    human.human = humanright;
                }
                if (event.key.keysym.sym == SDLK_LEFT)  {
                    human.ux = -SPEED;
                    human.human = humanleft;
                }
                break;
            }
        }

        if(!endgame)
            room = move(&human,r,elapsedTime,moves,room);

        // Fill the window with color

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 127, 255));
        message = TTF_RenderText_Solid(font, rooms[room], textColor);
        centerMsg(message,&msgrect);

        if(room == winroom) {
            msg2 = TTF_RenderText_Solid(font, msgw, textColor);
            centerMsg(msg2, &msgrect2);
            SDL_BlitSurface(msg2, 0, screen, &msgrect2);
            endgame = 1;
        } else if(checkloss(lossrooms, room))  {
            msg2 = TTF_RenderText_Solid(font, msgl, textColor);
            centerMsg(msg2, &msgrect2);
            SDL_BlitSurface(msg2, 0, screen, &msgrect2);
            endgame = 1;
        }

        result = drawRoom(&r, moves, room, 600, 500);

        SDL_BlitSurface(result, 0, screen, &wallrect);
        SDL_BlitSurface(human.human, 0, screen, &human.coord);
        SDL_BlitSurface(message, 0, screen, &msgrect);


        // Update frame and save the time

        SDL_UpdateWindowSurface(window);
        SDL_FreeSurface(result);
        SDL_FreeSurface(message);
        lastTime = currentTime;
    }
    SDL_FreeSurface(r.tile);
    SDL_FreeSurface(r.doorh);
    SDL_FreeSurface(r.doorv);
    SDL_FreeSurface(human.human);
    SDL_Quit();

    return 0;
}

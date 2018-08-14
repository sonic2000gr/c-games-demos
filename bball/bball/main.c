/* Very simple first SDL program */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <SDL.h>
#define FRAME_MS 8

SDL_Window* window;
SDL_Surface* screen;

void move(SDL_Surface* ball, float* ux, float* uy, uint32_t elapsedTime, SDL_Rect* coord) {
    coord->x = coord->x + (*ux) * elapsedTime;
    coord->y = coord->y + (*uy) * elapsedTime;
    if(coord->x > 600 - ball->w) {
        *ux = -(*ux);
        coord->x = 600 - ball->w;

    }

    if(coord->x < 0)   {
        *ux = -(*ux);
        coord->x = 0;
    }

    if(coord->y > 600 - ball->h)   {
        *uy = -(*uy);
        coord->y = 600 - ball->h;
    }

    if(coord->y < 0)   {
        *uy = -(*uy);
        coord->y = 0;
    }
}

int main(int argc, char* argv[]) {
    SDL_Event event;
    SDL_Rect coord;
    int running = 1; // This is to keep the main loop in operation
    coord.x = 200;
    coord.y = 300;
    float ux = 0.4;
    float uy = 0.2;
    uint32_t elapsedTime;
    uint32_t currentTime;
    uint32_t lastTime;
    SDL_Surface* ball;

    // Initialize SDL library or fail and exit

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        exit(1);



    // Attempt to create window

    window = SDL_CreateWindow("Hello SDL",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              600,600,
                              SDL_WINDOW_SHOWN);

    // Exit if we fail to create window

    if(!window)
        exit(1);

    // Create surface

    screen = SDL_GetWindowSurface(window);
    ball = SDL_LoadBMP("ball1.bmp");
    if (!ball) {
        printf("Failed to load bitmap!\n");
        exit(1);
    }
    SDL_GL_SetSwapInterval(0);
    lastTime = SDL_GetTicks();
    while(running) {
        currentTime = SDL_GetTicks();
        elapsedTime = currentTime - lastTime;
        if (elapsedTime < FRAME_MS) {
          SDL_Delay(FRAME_MS - elapsedTime);
          elapsedTime = FRAME_MS;
        }
        if (SDL_PollEvent(&event))  {
            switch (event.type) {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q)
                    running = 0;
                break;
            }
        }

    move(ball, &ux, &uy, elapsedTime, &coord);
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
    SDL_BlitSurface(ball, 0, screen, &coord);
    SDL_UpdateWindowSurface(window);
    lastTime = currentTime;
    }

    SDL_Quit();

    return 0;
}


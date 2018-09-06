/* Bouncing ball program
   Uses SDL_image library that has to be installed */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <time.h>

// FRAME_MS -> millis for one frame. Set to adjust desired frame rate as 1000/FRAME_MS

#define FRAME_MS 17

// Window WIDTH and HEIGHT

#define WIDTH 600
#define HEIGHT 600

SDL_Window* window;
SDL_Surface* screen;

typedef struct {
    SDL_Surface* ball;
    float ux;
    float uy;
    SDL_Rect coord;
} sprite;

void move(sprite* b, uint32_t elapsedTime) {
    b->coord.x = b->coord.x + b->ux * elapsedTime;
    b->coord.y = b->coord.y + b->uy * elapsedTime;
    if(b->coord.x > WIDTH - b->ball->w) {
        b->ux = -b->ux;
        b->coord.x = WIDTH - b->ball->w;

    }

    if(b->coord.x < 0)   {
        b->ux = -b->ux;
        b->coord.x = 0;
    }

    if(b->coord.y > HEIGHT - b->ball->h)   {
        b->uy = -b->uy;
        b->coord.y = HEIGHT - b->ball->h;
    }

    if(b->coord.y < 0)   {
        b->uy = -b->uy;
        b->coord.y = 0;
    }
}

void init(sprite* b)    {

      b->ball = IMG_Load("ball1.png");

      if (!b->ball) {
        printf("Failed to load bitmap!\n");
        exit(1);
      }
      b->coord.x = rand() % 301 + 100;
      b->coord.y = rand() % 301 + 100;

      b->ux = (rand() % 11 - 5) / 10.0f;
      b->uy = (rand() % 11 - 5) / 10.0f;

      if(b->ux > 0 && b->ux <= 0.1f)
        b->ux = 0.2f;
      if(b->ux < 0 && b->ux >= -0.1f)
        b->ux = -0.2f;
      if(b->uy > 0 && b->uy <= 0.1f)
        b->uy = 0.2f;
      if(b->uy < 0 && b->uy >= -0.1f)
        b->uy = -0.2f;


}

int main(int argc, char* argv[]) {
    SDL_Event event;

    // x and y coordinates for balls

    sprite theball;

    int running = 1; // This is to keep the main loop in operation

    uint32_t elapsedTime, currentTime, lastTime;

    // x-axis and y-axis speeds for balls

    // Initialize SDL library or fail and exit

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        exit(1);

    // Attempt to create window

    window = SDL_CreateWindow("Bouncing Ball",
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

    init(&theball);


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

        // Check for events (currently quit only)

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

        // Compute new positions for the balls


          move(&theball, elapsedTime);

        // Fill the window with color

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 127, 255));

        // Blit the balls to their new locations


        SDL_BlitSurface(theball.ball, 0, screen, &theball.coord);

        // Update frame and save the time

        SDL_UpdateWindowSurface(window);
        lastTime = currentTime;
    }

    SDL_Quit();

    return 0;
}

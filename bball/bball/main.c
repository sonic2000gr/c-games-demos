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

// NBALLS -> Number of balls to draw on screen

#define NBALLS 6

// Window WIDTH and HEIGHT

#define WIDTH 600
#define HEIGHT 600

SDL_Window* window;
SDL_Surface* screen;

void move(SDL_Surface* ball, float* ux, float* uy, uint32_t elapsedTime, SDL_Rect* coord) {
    coord->x = coord->x + (*ux) * elapsedTime;
    coord->y = coord->y + (*uy) * elapsedTime;
    if(coord->x > WIDTH - ball->w) {
        *ux = -(*ux);
        coord->x = WIDTH - ball->w;

    }

    if(coord->x < 0)   {
        *ux = -(*ux);
        coord->x = 0;
    }

    if(coord->y > HEIGHT - ball->h)   {
        *uy = -(*uy);
        coord->y = HEIGHT - ball->h;
    }

    if(coord->y < 0)   {
        *uy = -(*uy);
        coord->y = 0;
    }
}

int main(int argc, char* argv[]) {
    SDL_Event event;

    // x and y coordinates for balls

    SDL_Rect coord[NBALLS];
    SDL_Surface* ball[NBALLS];

    int running = 1; // This is to keep the main loop in operation
    int i;

    uint32_t elapsedTime, currentTime, lastTime;

    // x-axis and y-axis speeds for balls

    float ux[NBALLS], uy[NBALLS];

    // Initialize SDL library or fail and exit

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        exit(1);

    // Attempt to create window

    window = SDL_CreateWindow("Bouncing Balls",
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
    for(i = 0; i < NBALLS; i++)   {

      /* ball does not need to be an array
         when using a single image, but this is
         for demo purposes
         SDL_image allow the use of png images
         that support transparency */

      ball[i] = IMG_Load("ball2.png");
      if (!ball[i]) {
        printf("Failed to load bitmap!\n");
        exit(1);
      }

      // Use random coordinates from 100 to 400

      coord[i].x = rand() % 301 + 100;
      coord[i].y = rand() % 301 + 100;

      // Use random speeds from -5.0f to +5.0f


      ux[i] = (rand() % 11 - 5) / 10.0f;
      uy[i] = (rand() % 11 - 5) / 10.0f;

      // Adjust speeds to prevent edge cases

      if(ux[i] > 0 && ux[i] <= 0.1f)
        ux[i] = 0.2f;
      if(ux[i] < 0 && ux[i] >= -0.1f)
        ux[i] = -0.2f;
      if(uy[i] > 0 && uy[i] <= 0.1f)
        uy[i] = 0.2f;
      if(uy[i] < 0 && uy[i] >= -0.1f)
        uy[i] = -0.2f;
    }

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

        for(i = 0; i < NBALLS; i++)
          move(ball[i], &ux[i], &uy[i], elapsedTime, &coord[i]);

        // Fill the window with color

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 127, 255));

        // Blit the balls to their new locations

        for(i = 0; i < NBALLS; i++)
          SDL_BlitSurface(ball[i], 0, screen, &coord[i]);

        // Update frame and save the time

        SDL_UpdateWindowSurface(window);
        lastTime = currentTime;
    }

    SDL_Quit();

    return 0;
}

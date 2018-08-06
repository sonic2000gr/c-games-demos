/* Very simple first SDL program */

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

int main(int argc, char* argv[]) {

    SDL_Window* window;
    SDL_Renderer* render;
    SDL_Event event;
    int running = 1; // This is to keep the main loop in operation

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

    // Create renderer

    render = SDL_CreateRenderer(window, -1, 0);

    while(running) {
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
        SDL_SetRenderDrawColor(render, 0, 255, 255, 255);
        SDL_RenderClear(render);
        SDL_RenderPresent(render);
    }

    SDL_Quit();

    return 0;
}


/*

   Graphics Match Game
   Inspired by the TI-99/4A version (User's Manual)
   Original Game (C)1981 Texas Instruments
   This version (C) 2018 Manolis Kiagias
   Licensed under the BSD License

   This program requires the SDL2 and SDL2_ttf libraries
   to be correctly installed in your system to compile
   and run.

*/

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 430
#define HEIGHT 240
#define DISTANCE 5
#define SYMBOL_Y 20
#define TEXTDISTANCE_Y 30

bool initialize(void);
void finalize(void);
int mainloop(void);
int draw(int*);

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;

int draw(int spin[]) {
  int i;
  for (i = 0; i < 3; i++)
    spin[i] = rand() %6 ;

  // All equal -> 75 points (jackpot)
  if (spin[0] == spin[1] && spin[1] == spin[2])
    return 75;
  // First and third equal -> 10 points
  if (spin[0] == spin[2])
    return 10;
  // First two...
  if (spin[0] == spin[1]) {
    // If lemon, bar, cherry -> 40 points
    if (spin[0]==0 || spin[0]== 1 || spin[0] ==3)
      return 40;
    else  // otherwise 10 points
      return 10;
  }
  return -10;
}

int mainloop() {

    // Variables and Game Graphics loading

    int i;
    int score = 0;
    int spin[3];
    Sint16 x = 20;
    char scoremsg[50];
    SDL_Surface* symbols[6];
    const char *filenames[6] =  {"cherry.bmp",
                                 "bar.bmp",
                                 "bell.bmp",
                                 "lemon.bmp",
                                 "raspberry.bmp",
                                 "seven.bmp"};

    char msg[] = "Q / ESC to Quit, Any Other Key to Play";

    // Load symbols

    for (i = 0; i < 6; i++) {
      symbols[i] = SDL_LoadBMP(filenames[i]);
      if (!symbols[i]) {
        printf("Failed to load bitmaps! Exiting...\n");
        exit(1);
      }
    }

    // Fonts and color initialization

    TTF_Font* font = TTF_OpenFont( "impact.ttf", 28 );
    TTF_Font* fontsmall = TTF_OpenFont("impact.ttf", 20);
    SDL_Color textColor = { 255, 255, 255, 0 };
    if (!(font && fontsmall)) {
        printf("Failed to load fonts! Exiting...\n");
        exit(1);
    }

    // Coordinates for graphics and text messages

    SDL_Rect dstrect[3], textrect, textrect2;

    // Text Lines (y coordinate)

    textrect.y = symbols[0]->h + SYMBOL_Y + DISTANCE;
    textrect2.y = textrect.y  + TEXTDISTANCE_Y;

    // Symbol Coordinates

    for (i = 0; i < 3; i++) {
        dstrect[i].x = x;
        dstrect[i].y = SYMBOL_Y;
        x = x + symbols[0]->w + DISTANCE;
    }

    // program main loop

    score = draw(spin);
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                done = true;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q)
                   done = true;
                else
                   score = score + draw(spin);
                break;
            }
        }

        // Create the score message

        sprintf(scoremsg, "Score:  %d",score);
        SDL_Surface* message = TTF_RenderText_Solid(font, scoremsg, textColor);

        // Center it horizontally

        textrect.x = (screen->w - message->w) / 2;

        // Create the play again message

        SDL_Surface* msg2 = TTF_RenderText_Solid(fontsmall, msg, textColor);

        // Center it horizontally

        textrect2.x = (screen->w - msg2->w) / 2;

        // Clear the screen

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 127, 255));


        // draw bitmaps

        for (i = 0; i < 3; i++)
          SDL_BlitSurface(symbols[spin[i]], 0, screen, &dstrect[i]);


        // Show the messages

        SDL_BlitSurface(message, 0, screen, &textrect);
        SDL_BlitSurface(msg2, 0, screen, &textrect2);
        SDL_UpdateWindowSurface(window);

   }

    for (i = 0; i < 3; i++)
      SDL_FreeSurface(symbols[i]);
    return 0;
}


bool initialize() {
	// Initialize SDL and SDL_ttf

	if(SDL_Init( SDL_INIT_VIDEO ) < 0 || TTF_Init() < 0) {
		printf( "Could not initialize SDL or SDL_TTF!\n");
		return false;
	}
	else {
	    // Create the main window

		window = SDL_CreateWindow("Graphics Match", SDL_WINDOWPOS_UNDEFINED,
                                                    SDL_WINDOWPOS_UNDEFINED,
                                                    WIDTH,
                                                    HEIGHT,
                                                    SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf( "Could not create SDL Window: %s\n", SDL_GetError());
			return false;
		}
		else {

            // Get the surface of the entire initialized window

			screen = SDL_GetWindowSurface(window);
		}
	}
	return true;
}

void finalize() {
	// Free the surface and destroy the window

	SDL_FreeSurface(screen);
	screen = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}

int main(int argc, char* args[]) {
	int res;

	if(!initialize())
		printf( "Failed to initialize!\n" );
	else // Run the game
		res = mainloop();

	finalize();
	return res;
}

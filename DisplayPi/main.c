/*

   DisplayPI - C SDL Version
   For RaspberryPI - Rainbow Project

*/

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define WIDTH 1440
#define HEIGHT 900
#define LARGEFONT 32
#define SMALLFONT 24
#define COLORS 15

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;

const SDL_Color Black  =  {0,0,0};
const SDL_Color Green = {0x21, 0xC8, 0x42};
const SDL_Color LightGreen = {0x5E, 0xDC, 0x78};
const SDL_Color Blue = {0x54, 0x55, 0xED};
const SDL_Color LightBlue = {0x7D, 0x76, 0xFC};
const SDL_Color DarkRed = {0xD4, 0x52, 0x4D};
const SDL_Color Cyan = {0x42, 0xEB, 0xF5};
const SDL_Color Red = {0xFC, 0x55, 0x54};
const SDL_Color LightRed = {0xFF, 0x79, 0x78};
const SDL_Color DarkYellow = {0xD4, 0xC1, 0x54};
const SDL_Color LightYellow = {0xE6, 0xCE, 0x80};
const SDL_Color DarkGreen = {0x21, 0xB0, 0x3B};
const SDL_Color Magenta = {0xC9, 0x5B, 0xBA};
const SDL_Color Gray = {0xCC, 0xCC, 0xCC};
const SDL_Color White = {0xFF, 0xFF, 0xFF};

SDL_Color allcolors[] = { Black, Green, LightGreen, Blue, LightBlue,
                          DarkRed, Cyan, Red, LightRed, DarkYellow,
                          LightYellow, DarkGreen, Magenta, Gray,
                          White };

typedef struct {
        int width;
        int height;
        } Sizer;

typedef struct {
        int x;
        int y;
        } Positioner;

typedef struct {
        Positioner thepos;
        char msg[64];
        SDL_Color textcolor;
        SDL_Color surfacecolor;
        } Textrenderer;

typedef struct list {
        Textrenderer* textobject;
        struct list* next;
        } Renderlist;

SDL_Surface* rainbowlogo;
SDL_Color bordercolor = Red;
SDL_Color maincolor = Cyan;

// These affect our environment globally

Positioner windowpos;
int fontsize;

Textrenderer* addText(Positioner mypos, char textmsg[32], SDL_Color textcolor, SDL_Color surfacecolor) {
  Textrenderer* renderer = (Textrenderer*)malloc(sizeof(Textrenderer));
  if (renderer == NULL)
    return NULL;
  renderer->thepos = mypos;
  strcpy(renderer->msg, textmsg);
  renderer->textcolor = textcolor;
  renderer->surfacecolor = surfacecolor;
  return renderer;
}

void translatePos(Positioner textpos, SDL_Rect* graphicspos) {
  graphicspos->x = textpos.x * fontsize + windowpos.x;
  graphicspos->y = textpos.y * fontsize + windowpos.y;
}

int addtoRenderList(Renderlist **head, Renderlist **last, Textrenderer *thetext) {
  Renderlist* node = (Renderlist*)malloc(sizeof(Renderlist));
  if (node == NULL)
    return 0;
  node->textobject = thetext;
  node->next = NULL;
  if (*head == NULL) {
    *head = node;
    *last = *head;
    return 1;
  }
  (*last)->next = node;
  *last = node;
  return 1;
}

void deleteRenderList(Renderlist **head) {
  Renderlist *current, *next;
  current = *head;
  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
  *head = NULL;
}

void nextLinePos(Positioner* thepos) {
  thepos->y += 1;
  thepos->x  = 0;
  if (thepos->y == 25)
    thepos->y = 0;
}

void nextCharPos(Positioner* thepos) {
  thepos->x += 1;
  if (thepos->x == 32) {
    thepos->x = 0;
    nextLinePos(thepos);
  }
}

Positioner setPos(int x, int y) {
  Positioner pos = {x,y};
  return pos;
}

int mainloop(Sizer windowsize) {
    SDL_Rect mainarea = { windowpos.x, windowpos.y, windowsize.width, windowsize.height };
    SDL_Surface* thelogo;
    SDL_Surface* msg2;

    if (fontsize == SMALLFONT)
        thelogo = SDL_LoadBMP("logo4.bmp");
    else
        thelogo = SDL_LoadBMP("logo3.bmp");

    Renderlist* head = NULL;
    Renderlist* last = NULL;
    Renderlist* element = NULL;
    Positioner logopos;
    bool logovisible = false;
    Textrenderer* textrender;
    SDL_Rect textrect;
    SDL_Rect logorect;
    SDL_Rect cliprect;

    // Fonts and color initialization

    TTF_Font* font;
    TTF_Font* fontlarge = TTF_OpenFont( "PressStart2P.ttf", LARGEFONT);
    TTF_Font* fontsmall = TTF_OpenFont("PressStart2P.ttf", SMALLFONT);
    if (fontsize == LARGEFONT)
        font = fontlarge;
    else
        font = fontsmall;

    if (!(fontlarge && fontsmall)) {
        printf("Failed to load fonts! Exiting...\n");
        exit(1);
    }

    // Testing by TI Screen

    char msg[32];
    Positioner textpos = setPos(0,0);
    deleteRenderList(&head);
    logopos = setPos(13,6);
    logovisible = true;
    textpos = setPos(1,3);
    for (int i = 0; i < COLORS; i++) {
        strcpy(msg, "A");
        textrender = addText(textpos, msg, allcolors[i], allcolors[i]);
        addtoRenderList(&head, &last, textrender);
        nextCharPos(&textpos);
        textrender = addText(textpos, msg, allcolors[i], allcolors[i]);
        addtoRenderList(&head, &last, textrender);
        nextCharPos(&textpos);
    }
    nextLinePos(&textpos);
    nextCharPos(&textpos);
    for (int i = 0; i < COLORS; i++) {
        strcpy(msg, "A");
        textrender = addText(textpos, msg, allcolors[i], allcolors[i]);
        addtoRenderList(&head, &last, textrender);
        nextCharPos(&textpos);
        textrender = addText(textpos, msg, allcolors[i], allcolors[i]);
        addtoRenderList(&head, &last, textrender);
        nextCharPos(&textpos);
    }
    textpos = setPos(6,10);
    strcpy(msg, "THE RAINBOW PROJECT");
    textrender = addText(textpos, msg, Black, Cyan);
    addtoRenderList(&head, &last, textrender);
    textpos = setPos(3,12);
    strcpy(msg, "Z80 EXPERIMENTAL COMPUTER");
    textrender = addText(textpos, msg, Black, Cyan);
    addtoRenderList(&head, &last, textrender);
    textpos = setPos(3, 18);
    strcpy(msg, "READY - WAITING FOR Z80 CPU");
    textrender = addText(textpos, msg, Black, Cyan);
    addtoRenderList(&head, &last, textrender);
    textpos = setPos(1,20);
    for (int i = 0; i < COLORS; i++) {
        strcpy(msg, "A");
        textrender = addText(textpos, msg, allcolors[i], allcolors[i]);
        addtoRenderList(&head, &last, textrender);
        nextCharPos(&textpos);
        textrender = addText(textpos, msg, allcolors[i], allcolors[i]);
        addtoRenderList(&head, &last, textrender);
        nextCharPos(&textpos);
    }
    nextLinePos(&textpos);
    nextCharPos(&textpos);
    for (int i = 0; i < COLORS; i++) {
        strcpy(msg, "A");
        textrender = addText(textpos, msg, allcolors[i], allcolors[i]);
        addtoRenderList(&head, &last, textrender);
        nextCharPos(&textpos);
        textrender = addText(textpos, msg, allcolors[i], allcolors[i]);
        addtoRenderList(&head, &last, textrender);
        nextCharPos(&textpos);
    }
    textpos = setPos(2,23);
    strcpy(msg, "(C) 2017-2019 SCHOOLSPACE.GR");
    textrender = addText(textpos, msg, Black, Cyan);
    addtoRenderList(&head,  &last, textrender);

    // program main loop

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

            }
        }

        // Create border and main area

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, bordercolor.r, bordercolor.g, bordercolor.b));
        SDL_FillRect(screen, &mainarea, SDL_MapRGB(screen->format, maincolor.r, maincolor.g, maincolor.b));

        // Show the messages
        element = head;
        while (element !=NULL) {
          msg2 = TTF_RenderText_Solid(font, element->textobject->msg, element->textobject->textcolor);
          SDL_GetClipRect(msg2, &cliprect);
          translatePos(element->textobject->thepos, &textrect);
          cliprect.x = textrect.x;
          cliprect.y = textrect.y;
          SDL_FillRect(screen, &cliprect, SDL_MapRGB(screen->format,
                                                     element->textobject->surfacecolor.r,
                                                     element->textobject->surfacecolor.g,
                                                     element->textobject->surfacecolor.b));
          SDL_BlitSurface(msg2, 0, screen, &textrect);
          SDL_FreeSurface(msg2);
          element = element->next;
        }
        if (logovisible) {
            translatePos(logopos, &logorect);
            SDL_BlitSurface(thelogo, 0, screen, &logorect);
        }
        SDL_UpdateWindowSurface(window);
   }
   deleteRenderList(&head);
   return 0;
}

void getWindowSize(Sizer screensize, Sizer* windowsize) {
    if (screensize.height < 800) {
        windowsize->height = 600;
        windowsize->width = 800;
        fontsize = SMALLFONT;
    } else {
        windowsize->height = 800;
        windowsize->width = 1024;
        fontsize = LARGEFONT;
    }
}

void getWindowPos(Sizer screensize, Sizer windowsize) {
    int borderwidth, borderheight;
    borderwidth = screensize.width - windowsize.width;
    borderheight = screensize.height - windowsize.height;
    windowpos.x = borderwidth / 2;
    windowpos.y = borderheight / 2;
}

bool initialize(Sizer* thesize) {
	// Initialize SDL and SDL_ttf

	if(SDL_Init( SDL_INIT_VIDEO ) < 0 || TTF_Init() < 0) {
		printf( "Could not initialize SDL or SDL_TTF!\n");
		return false;
	}
	else {
	    // Create the main window

		window = SDL_CreateWindow("DisplayPI", SDL_WINDOWPOS_UNDEFINED,
                                                    SDL_WINDOWPOS_UNDEFINED,
                                                    WIDTH,
                                                    HEIGHT,
                                                    SDL_WINDOW_FULLSCREEN_DESKTOP);
		if (window == NULL) {
			printf( "Could not create SDL Window: %s\n", SDL_GetError());
			return false;
		}
		else {

            // Get the surface of the entire initialized window

			screen = SDL_GetWindowSurface(window);
			SDL_GetWindowSize(window, &(thesize->width), &(thesize->height));
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
    Sizer screensize;
    Sizer windowsize;

	if(!initialize(&screensize))
		printf( "Failed to initialize!\n" );
	else { // Run the game
        getWindowSize(screensize, &windowsize);
        getWindowPos(screensize, windowsize);
        res = mainloop(windowsize);
	}
	finalize();
	return res;
}

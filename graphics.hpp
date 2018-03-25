#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <string>
#include <SDL2/SDL.h>
#include "myFunctions.hpp"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum colors // Don't forget to change in colorStrToInt when editing this
{
    COLOR_SURFACE_WHITE,
    COLOR_SURFACE_RED,
    COLOR_SURFACE_GREEN,
    COLOR_SURFACE_BLUE,
    COLOR_SURFACE_YELLOW,
    COLOR_SURFACE_CROSS,
    COLOR_SURFACE_BLACK
};

extern SDL_Surface* gColorSurfaces[];

int handleKeyPress(SDL_Event, bool*);

bool init();
SDL_Surface* loadSurface( std::string path );
void close();
bool loadMedia();

extern SDL_Window* gWindow;
extern SDL_Surface* gScreenSurface;

/*
extern int xCoord;
extern int yCoord;
extern int nrRows;
extern int nrColumns;
*/

#endif // GRAPHICS_H

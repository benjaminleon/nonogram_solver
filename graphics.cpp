#ifndef GRAPHICS_CPP
#define GRAPHICS_CPP
#include "myFunctions.hpp"
#include "graphics.hpp"

#include <iostream>

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Loads individual image
SDL_Surface* loadSurface( std::string path );

SDL_Surface* gBlackBackground = NULL;

//The images that correspond to a keypress
SDL_Surface* gColorSurfaces[ COLOR_SURFACE_BLACK ];

// For placing the drawing of the game board on the screen
int xCoord = 100;
int yCoord = 100;

int handleKeyPress(SDL_Event e, bool* enableMove)
{
    if( e.type == SDL_KEYDOWN )
    {
        //Select surfaces based on key press
        switch( e.key.keysym.sym )
        {
        case SDLK_UP:
            if( *enableMove )
            {
                yCoord -= 40;
            }
            else
            {
                findCrosses();
                setCrossesForDisplay();
            }

            break;

        case SDLK_DOWN:
            if( *enableMove )
            {
                yCoord += 40;
            }
            else
            {
                printAllClues();
            }


            break;

        case SDLK_LEFT:
            if( *enableMove )
            {
                xCoord -= 40;
            }
            else
            {
                std::cout << "Cancelling posibilities" << std::endl;
                cancelPossibilitiesForEmptyHolders(); // TODO: Debug here
            }
            break;

        case SDLK_RIGHT:
            if( *enableMove )
            {
                xCoord += 40;
            }
            else
            {
                std::cout << "colorizing" << std::endl;
                if ( colorize() )
                {
                    std::cout << "colorize failed" << std::endl;
                    return 1;
                }
            }

            break;

        default:
            *enableMove = !*enableMove;
            if( *enableMove )
            {
                std::cout << "Started moving" << std::endl;
            }
            else
            {
                std::cout << "Stopped moving" << std::endl;
            }

            break;
        }
    }
    return 0; // Function exits without errors
}


bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "Nonogram solver", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }
    return success;
}

SDL_Surface* loadSurface( std::string path )
{
    //Load image at specified path
    SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }

    return loadedSurface;
}

void close()
{
    //Deallocate surfaces
    for( int i = 0; i < COLOR_SURFACE_BLACK; ++i )
    {
        SDL_FreeSurface( gColorSurfaces[ i ] );
        gColorSurfaces[ i ] = NULL;
    }

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}




bool loadMedia()
{
    //Loading success flag
    bool success = true;

    gColorSurfaces[ COLOR_SURFACE_WHITE ] = loadSurface( "imgs/white.bmp" );
    if( gColorSurfaces[ COLOR_SURFACE_WHITE ] == NULL )
    {
        printf( "Failed to load red image!\n" );
        success = false;
    }

    gColorSurfaces[ COLOR_SURFACE_RED ] = loadSurface( "imgs/red.bmp" );
    if( gColorSurfaces[ COLOR_SURFACE_RED ] == NULL )
    {
        printf( "Failed to load red image!\n" );
        success = false;
    }

    gColorSurfaces[ COLOR_SURFACE_GREEN ] = loadSurface( "imgs/green.bmp" );
    if( gColorSurfaces[ COLOR_SURFACE_GREEN ] == NULL )
    {
        printf( "Failed to load green image!\n" );
        success = false;
    }

    gColorSurfaces[ COLOR_SURFACE_BLUE ] = loadSurface( "imgs/blue.bmp" );
    if( gColorSurfaces[ COLOR_SURFACE_BLUE ] == NULL )
    {
        printf( "Failed to load blue image!\n" );
        success = false;
    }

    gColorSurfaces[ COLOR_SURFACE_YELLOW ] = loadSurface( "imgs/yellow.bmp" );
    if( gColorSurfaces[ COLOR_SURFACE_YELLOW ] == NULL )
    {
        printf( "Failed to load yellow image!\n" );
        success = false;
    }

    gColorSurfaces[ COLOR_SURFACE_CROSS ] = loadSurface( "imgs/cross.bmp" );
    if( gColorSurfaces[ COLOR_SURFACE_CROSS ] == NULL )
    {
        printf( "Failed to load cross image!\n" );
        success = false;
    }

    gColorSurfaces[ COLOR_SURFACE_BLACK ] = loadSurface( "imgs/black.bmp" );
    if( gColorSurfaces[ COLOR_SURFACE_BLACK ] == NULL )
    {
        printf( "Failed to load black image!\n" );
        success = false;
    }

    return success;
}


#endif // GRAPHICS_CPP

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#ifndef MAIN_CPP
#define MAIN_CPP

#include <SDL2/SDL.h>
//#include <SDL/SDL.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <assert.h>
#include "clue.h"
#include "graphics.h"
#include "myFunctions.h"

// TODO: When a bit is colorized in a row, try to determine which bit was colorized in the corresponding column

//Initialize the clues.
Clue clue0(1, "B"); //
Clue clue1(2, "B"); // Row clues
Clue clue2(1, "R"); //
Clue clue6(0, "Y");

Clue clue3(1, "B"); //
Clue clue4(2, "B"); // Column clues
Clue clue5(1, "R"); //

// Keep track of where the clues belong
Clue* row0[] = {&clue0, NULL};
Clue* row1[] = {&clue1, &clue2, NULL};
Clue* row2[] = {NULL};

Clue* column0[] = {&clue3, NULL};
Clue* column1[] = {&clue4, NULL};
Clue* column2[] = {&clue5, NULL};
Clue* column3[] = {NULL};

// Keep track of the order of the rows and columns
Clue** allRows[] = {row0, row1, row2, NULL};
Clue** allColumns[] = {column0, column1, column2, column3, NULL};


int main( int argc, char* args[] )
{

    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            initPossibleLocations();


            SDL_Event myEvent;

            bool enableMove = false;

            bool quit = false;
            while( !quit )
            {

                //Handle events on queue
                while( SDL_PollEvent( &myEvent ) != 0 )
                {

                    if( myEvent.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    else
                    {

                        if ( handleKeyPress(myEvent, &enableMove) )
                        {
                              std::cout << "handleKeyPress failed" << std::endl;
                              quit = true;
                          }
                        }
                    }

                    SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0x0F, 0x82, 0x8F) );

                    displayColorMatrix();

                    SDL_UpdateWindowSurface( gWindow );
                }
            }
        }

    //Free resources and close SDL
    close();

    return 0;
}
#endif // MAIN_CPP

#ifndef MYFUNCTIONS_CPP
#define MYFUNCTIONS_CPP

#include "myFunctions.hpp"
#include <iostream>
#include <assert.h>

// TODO: any way to return an iterable list of clue pointers? Then I wouldn't have to repeat the "for all clues"-code.
// Seems easy in python. Perhaps the way is to return a pointer to a list with pointers ending will null. Another cool
// idea is to send a piece of code to the "for all clues"-function. Like, for all clues, print all positions, or print their color.

const int nrColumns = 4; // I want to put these in main, but I don't
const int nrRows = 3;   // want to include  the main file. What to do?

int gBlockColor = -1; // Initialized to no color

int gColorMatrix[nrRows][nrColumns]; //Initializes with 0's (white)
int gCrossMatrix[nrRows][nrColumns];

void printAllClues()
{
    int clueIdx = 0;

    for( int rowIdx = 0; rowIdx < nrRows; rowIdx++ )
    {
        Clue** currentRowPtr = allRows[rowIdx];

        if( currentRowPtr != NULL )
        {
            Clue* currentCluePtr = *currentRowPtr;

            std::cout << "Row " << rowIdx << std::endl;
            while( currentCluePtr != NULL)
            {
                std::cout << "Clue " << clueIdx << ", remaining " << (*currentCluePtr).remaining << std::endl;
                printPossible(currentCluePtr);

                currentRowPtr++;
                currentCluePtr = *currentRowPtr;
                clueIdx++;
            }
        }

        else
        {
            break;
        }
    }

    for( int columnIdx = 0; columnIdx < nrColumns; columnIdx++ )
    {
        Clue** currentColumnPtr = allColumns[columnIdx];

        if( currentColumnPtr != NULL )
        {

            Clue* currentCluePtr = *currentColumnPtr;

            std::cout << "Column " << columnIdx << std::endl;
            while( currentCluePtr != NULL)
            {
                std::cout << "Clue " << clueIdx << ", remaining " << (*currentCluePtr).remaining << std::endl;
                printPossible(currentCluePtr);

                currentColumnPtr++;
                currentCluePtr = *currentColumnPtr;
                clueIdx++;
            }

        }
        else
        {
            break;
        }

    }

}

void displayColorMatrix()
{
    SDL_Rect stretchRectBackground; //Background is black
    stretchRectBackground.x = xCoord;
    stretchRectBackground.y = yCoord;
    stretchRectBackground.w = nrColumns*41 + 1;
    stretchRectBackground.h = nrRows*41 + 1;
    SDL_BlitScaled( gColorSurfaces[ COLOR_SURFACE_BLACK ], NULL, gScreenSurface, &stretchRectBackground);

    SDL_Rect stretchRect;
    stretchRect.x = xCoord -40;
    stretchRect.y = yCoord -40;

    for( int i = 0; i < nrRows; i++)
    {
        stretchRect.y += 41;

        for( int j = 0; j < nrColumns; j++)
        {
            stretchRect.x += 41;

            gBlockColor = gColorMatrix[i][j];
            SDL_BlitSurface( gColorSurfaces[ gBlockColor ], NULL, gScreenSurface, &stretchRect);
        }
        stretchRect.x = xCoord -40;
    }
}

int colorize()
{
    int freeSpace;
    int clueIdx = 0;
    std::cout << "Colorizing from clues in rows" << std::endl;
    for( int rowIdx = 0; rowIdx < nrRows; rowIdx++)
    {
        Clue** currentRowPtr = allRows[rowIdx];

        if( currentRowPtr != NULL) // Row might be empty
        {
            Clue* currentCluePtr = *currentRowPtr;

            while( currentCluePtr != NULL)
            {
                freeSpace = getFreeSpace(currentCluePtr, rowIdx, THIS_IS_A_ROW);
                std::cout << "Clue nr " << clueIdx << " in row " << rowIdx << " with length and color: "
                          << (*currentCluePtr).length << " " << (*currentCluePtr).color << " has " << (*currentCluePtr).remaining << " more blocks to colorize" << std::endl;
                std::cout << "Free space in the row: " << freeSpace << std::endl;

                if( !(*currentCluePtr).isUsedUp )
                {
                    if( (*currentCluePtr).remaining == freeSpace )
                    {
                        colorizeAllFreeInRowAndZeroOthers( currentCluePtr, rowIdx );
                        (*currentCluePtr).isUsedUp = true;

                    }
                    else if( (*currentCluePtr).remaining < freeSpace ) // Explore extreme positions
                    {
                        std::cout << "Explore extreme positions" << std::endl;
                    }
                    else
                    {
                        std::cout << "Remaining, free spots, clue nr, row:  " << (*currentCluePtr).remaining << ", " << freeSpace << ", " << clueIdx  << ", " << rowIdx << std::endl;
                        return 1;
                    }
                }
                clueIdx++;
                currentRowPtr++;
                currentCluePtr = *currentRowPtr;
            }
        }
        else
        {
            break;
        }
    }

    std::cout << "Colorizing from clues in columns" << std::endl;
    clueIdx = 0;
    for( int columnIdx = 0; columnIdx < nrColumns; columnIdx++)
    {
        Clue** currentColumnPtr = allColumns[columnIdx];

        if( currentColumnPtr != NULL ) // Column might be empty
        {
            std::cout << currentColumnPtr << " " << columnIdx << std::endl;
            Clue* currentCluePtr = *currentColumnPtr;

            while( currentCluePtr != NULL)
            {
                freeSpace = getFreeSpace(currentCluePtr, columnIdx, THIS_IS_A_COLUMN);
                std::cout << "Clue nr " << clueIdx << " in column " << columnIdx << " with length and color: "
                          << (*currentCluePtr).length << " " << (*currentCluePtr).color << " has " << (*currentCluePtr).remaining << " more blocks to colorize" << std::endl;
                std::cout << "Free space in the column: " << freeSpace << std::endl;

                if( !(*currentCluePtr).isUsedUp )
                {
                    if( (*currentCluePtr).remaining == freeSpace )
                    {
                        colorizeAllFreeInColumnAndZeroOthers( currentCluePtr, columnIdx );
                        (*currentCluePtr).isUsedUp = true;
                    }

                    else if( (*currentCluePtr).remaining < freeSpace ) // Explore extreme positions
                    {
                        std::cout << "Explore extreme positions" << std::endl;
                    }

                    else
                    {
                        std::cout << "Failing! Remaining, free spots, clue nr, column " << (*currentCluePtr).remaining << ", " << freeSpace << ", " << clueIdx  << ", " << columnIdx << std::endl;
                        return 1;
                    }

                }
                clueIdx++;
                currentColumnPtr++;
                currentCluePtr = *currentColumnPtr;
            }

        }
        else
        {
            break;
        }
    }
    std::cout << std::endl;
    return 0;
}

void colorizeAllFreeInRowAndZeroOthers(Clue* currentCluePtr, int rowIdx)
{
    for( int columnIdx = 0; columnIdx < nrColumns; columnIdx++)
    {
        if( *((*currentCluePtr).possiblePlacements[rowIdx] + columnIdx) == 1 )
        {
            colorizeBlock( currentCluePtr, rowIdx, columnIdx );
            (*currentCluePtr).remaining--;
            setOccupiedForAllBut( currentCluePtr, rowIdx, columnIdx );
        }
    }
}

void colorizeAllFreeInColumnAndZeroOthers(Clue* currentCluePtr, int columnIdx)
{
    for( int rowIdx = 0; rowIdx < nrRows; rowIdx++)
    {
        if( *((*currentCluePtr).possiblePlacements[rowIdx] + columnIdx) == 1 )
        {
            colorizeBlock( currentCluePtr, rowIdx, columnIdx );
            (*currentCluePtr).remaining--;
            setOccupiedForAllBut( currentCluePtr, rowIdx, columnIdx);
        }
    }
}



void colorizeBlock(Clue* currentCluePtr, int rowIdx, int columnIdx)
{
    gColorMatrix[rowIdx][columnIdx] = colorStrToInt( (*currentCluePtr).color );
}


int getFreeSpace(Clue* cluePtr, int holderIdx, int holderType)
{
    int freeSpace = 0;

    if( holderType == THIS_IS_A_ROW )
    {
        int holderLength = nrColumns;
        for(int i = 0; i < holderLength; i++)
        {
            if( *((*cluePtr).possiblePlacements[holderIdx] + i) == 1 )
            {
                freeSpace++;
            }
        }
    }

    else
    {
        int holderLength = nrRows;
        for(int i = 0; i < holderLength; i++)
        {
            if( *((*cluePtr).possiblePlacements[i] + holderIdx) == 1 )
            {
                freeSpace++;
            }
        }
    }
    return freeSpace;
}


int colorStrToInt(std::string color)
{
    if( color == "R")
    {
        return 1;
    }
    else if( color == "G")
    {
        return 2;
    }
    else if( color == "B")
    {
        return 3;
    }
    else if( color == "Y")
    {
        return 4;
    }
    else if( color == "B")
    {
        return 6;
    }
    else
    {
        assert(false);
    }
}


void setOccupiedForAllBut(Clue* winningClue, int winningRow, int winningColumn)
{
    int clueIdx = 0;
    for( int rowIdx = 0; rowIdx < nrRows; rowIdx++) // For all clues in all rows
    {
        Clue** currentRowPtr = allRows[rowIdx];
        Clue* currentCluePtr = *currentRowPtr; // TODO: row might be empty, currentRowPtr could be 0
        while( currentCluePtr != NULL)
        {
            if( currentCluePtr != winningClue )
            {
                if( winningClue == NULL || (*currentCluePtr).color != (*winningClue).color ) // winningClue == NULL to set all clues
                {
                    *((*currentCluePtr).possiblePlacements[winningRow] + winningColumn) = 0;
                }
            }

            currentRowPtr++;
            currentCluePtr = *currentRowPtr;
            clueIdx++; // For debugging
        }
    }
    clueIdx = 0;
    for( int columnIdx = 0; columnIdx < nrColumns; columnIdx++) // For all clues in all columns
    {
        Clue** currentColumnPtr = allColumns[columnIdx];
        Clue* currentCluePtr = *currentColumnPtr; // TODO: column might be empty
        while( currentCluePtr != NULL)
        {
            if( currentCluePtr != winningClue )
            {
                if( (winningClue == NULL) || ((*currentCluePtr).color != (*winningClue).color) ) // winningClue == NULL to set all clues
                {
                    *((*currentCluePtr).possiblePlacements[winningRow] + winningColumn) = 0;
                }
            }

            currentColumnPtr++;
            currentCluePtr = *currentColumnPtr;
            clueIdx++; // For debugging
        }
    }
}



void initPossibleLocations()
{
    for( int rowIdx = 0; rowIdx < nrRows; rowIdx++)
    {
        Clue** currentRowPtr = allRows[rowIdx]; // TODO: Check if currentRowPtr is null. row might be empty. Same for column

        int nrCluesInRow = getNrCluesInHolder(currentRowPtr);

        setValueInHolder(currentRowPtr, 1, rowIdx, THIS_IS_A_ROW, nrCluesInRow);

        setImpossibleBehind(currentRowPtr, THIS_IS_A_ROW, rowIdx, nrCluesInRow);
        setImpossibleAhead(currentRowPtr, THIS_IS_A_ROW, rowIdx, nrCluesInRow);
    }

    for( int columnIdx = 0; columnIdx < nrColumns; columnIdx++)
    {

        Clue** currentColumnPtr = allColumns[columnIdx];

        int nrCluesInColumn = getNrCluesInHolder(currentColumnPtr);

        setValueInHolder(currentColumnPtr, 1, columnIdx, THIS_IS_A_COLUMN, nrCluesInColumn);

        setImpossibleBehind(currentColumnPtr, THIS_IS_A_COLUMN, columnIdx, nrCluesInColumn);
        setImpossibleAhead(currentColumnPtr, THIS_IS_A_COLUMN, columnIdx, nrCluesInColumn);
    }
}

void cancelPossibilitiesForEmptyHolders() // TODO: Debug this
{
    for( int rowIdx = 0; rowIdx < nrRows; rowIdx++ )
    {
        Clue** currentRowPtr = allRows[rowIdx];

        if( *currentRowPtr == NULL )
        {
            cancelPossibilitiesForRow( rowIdx );
        }
    }

    for( int columnIdx = 0; columnIdx < nrColumns; columnIdx++ )
    {
        Clue** currentColumnPtr = allColumns[columnIdx];

        if( *currentColumnPtr == NULL )
        {
            cancelPossibilitiesForColumn( columnIdx );
        }
    }
}


void cancelPossibilitiesForRow(int forbiddenRow) // TODO: Debug this function
{
    for( int columnIdx = 0; columnIdx < nrColumns; columnIdx++ )
    {
        Clue** currentColumnPtr = allColumns[columnIdx];

        if( currentColumnPtr != NULL ) // Column might be empty
        {
            Clue* cluePtr = *currentColumnPtr;
            while( cluePtr != NULL )
            {
                *((*cluePtr).possiblePlacements[forbiddenRow] + columnIdx) = 0;

                currentColumnPtr++;
                cluePtr = *currentColumnPtr;
            }
        }
    }
}

void cancelPossibilitiesForColumn(int forbiddenColumn) // TODO: Debug this function
{
    for( int rowIdx = 0; rowIdx < nrRows; rowIdx++ )
    {
        Clue** currentRowPtr = allRows[rowIdx];

        if( currentRowPtr != NULL ) // Rowmight be empty
        {
            Clue* cluePtr = *currentRowPtr;
            while( cluePtr != NULL )
            {
                *((*cluePtr).possiblePlacements[rowIdx] + forbiddenColumn) = 0;

                currentRowPtr++;
                cluePtr = *currentRowPtr;
            }
        }
    }
}



int getNrCluesInHolder(Clue** holderPtr) // A holder is a row or a column
{
    int nrCluesFound = 0;

    if( holderPtr != NULL )
    {
        Clue* currentCluePtr = *holderPtr;

        while( currentCluePtr != NULL)
        {
            nrCluesFound++;
            holderPtr++;
            currentCluePtr = *holderPtr; // holderPtr points to the first item in the holder, which is the pointer to the first clue.
            // after that we want the second item in the holder, pointing to the second clue.
        }
    }
    return nrCluesFound;
}



void setValueInHolder(Clue** holderPtr, int value, int holderIdx, int holderType, int nrCluesInHolder) // Works fine
{
    for(int clueIdx = 0; clueIdx < nrCluesInHolder; clueIdx++)
    {
        Clue* currentCluePtr = holderPtr[clueIdx];

        if( holderType == THIS_IS_A_ROW)
        {
            for(int columnIdx = 0; columnIdx < nrColumns; columnIdx++)
            {
                *((*currentCluePtr).possiblePlacements[holderIdx] + columnIdx) = value;
            }
        }

        else
        {
            for(int rowIdx  = 0; rowIdx < nrRows; rowIdx++)
            {
                *((*currentCluePtr).possiblePlacements[rowIdx] + holderIdx) = value;
            }
        }
    }
}


void setImpossibleBehind(Clue** holderPtr, int holderType, int holderIdx, int nrCluesInHolder) // Holder is a row or a column
{
    std::string colorBehind = "";
    std::string currentColor = "";

    int behindLength = 0;
    int currentLength = 0;
    int nrImpossibleBehind = 0;

    for( int clueIdx = 0; clueIdx < nrCluesInHolder; clueIdx++) // Start with the first clue
    {
        Clue* currentCluePtr = holderPtr[clueIdx];

        currentColor = (*currentCluePtr).color;
        currentLength = (*currentCluePtr).length;

        if( currentColor == colorBehind)
        {
            nrImpossibleBehind += 1;
        }

        nrImpossibleBehind += behindLength;

        if( holderType == THIS_IS_A_ROW )
        {
            for( int i = 0; i < nrImpossibleBehind; i++ ) // Set zeros to the left
            {
                *((*currentCluePtr).possiblePlacements[holderIdx] + i) = 0;
            }

        }

        else
        {
            for( int i = 0; i < nrImpossibleBehind; i++) // Set zeros above
            {
                *((*currentCluePtr).possiblePlacements[i] + holderIdx) = 0;
            }
        }

        // Setup for the clue ahead
        behindLength = currentLength;
        colorBehind = currentColor;
    }

}

void setImpossibleAhead(Clue** holderPtr, int holderType, int holderIdx, int nrCluesInHolder)
{
    std::string colorAhead = "";
    std::string currentColor = "";

    int currentLength = 0;
    int aheadLength = 0;
    int nrImpossibleAhead = 0;

    for( int clueIdx = nrCluesInHolder - 1; clueIdx >= 0; clueIdx--) // Start with the last clue
    {
        Clue* currentCluePtr = holderPtr[clueIdx];

        currentColor = (*currentCluePtr).color;
        currentLength = (*currentCluePtr).length;

        if( currentColor == colorAhead ) // Next means to the right or below
        {
            nrImpossibleAhead += 1;
        }

        nrImpossibleAhead += aheadLength;

        if( holderType == THIS_IS_A_ROW )
        {
            for( int i = 0; i < nrImpossibleAhead; i++ ) // Set zeros to the right
            {
                *((*currentCluePtr).possiblePlacements[holderIdx] + (nrColumns - 1 -  i) ) = 0;
            }
        }

        else
        {
            for( int i = 0; i < nrImpossibleAhead; i++) // Set zeros below
            {
                *((*currentCluePtr).possiblePlacements[nrColumns - 1 - i] + holderIdx) = 0;
            }
        }

        // Setup for the clue behind
        aheadLength = currentLength;
        colorAhead = currentColor;
    }
}


void printPossible(Clue* cluePtr)
{
    for( int rowIdx = 0; rowIdx < nrRows; rowIdx++ )
    {
        for( int columnIdx = 0; columnIdx < nrColumns; columnIdx++ )
        {
            std::cout << *((*cluePtr).possiblePlacements[rowIdx] + columnIdx) << " ";
        }
        std::cout << std::endl;

    }
    std::cout << std::endl;
}

void resetCrossMatrix()
{
    for( int rowIdx = 0; rowIdx < nrRows; rowIdx++ )
    {
         for( int columnIdx = 0; columnIdx < nrColumns; columnIdx++ )
         {
             gCrossMatrix[rowIdx][columnIdx] = 0;
         }
    }

}


void findCrosses()
{
    // Where it is not possible for any clue to occupy a block, the global cross matrix will be 0.

    resetCrossMatrix();

    for( int rowIdx = 0; rowIdx < nrRows; rowIdx++ ) // For clues in all rows
    {
        Clue** currentRowPtr = allRows[rowIdx];

        if( currentRowPtr != NULL )
        {
            Clue* currentCluePtr = *currentRowPtr;

            while( currentCluePtr != NULL )
            {
                for( int i = 0; i < nrRows; i++ )
                {
                    for( int j = 0; j < nrColumns; j++ )
                    {
                        gCrossMatrix[i][j] +=  *((*currentCluePtr).possiblePlacements[i] + j);
                    }
                }

                currentRowPtr++;
                currentCluePtr = *currentRowPtr;
            }
        }
    }

    for( int columnIdx = 0; columnIdx < nrColumns; columnIdx++ ) // For clues in all columns
    {
        Clue** currentColumnPtr = allColumns[columnIdx];

        if( currentColumnPtr != NULL )
        {
            Clue* currentCluePtr = *currentColumnPtr;

            while( currentCluePtr != NULL )
            {
                for( int i = 0; i < nrRows; i++ )
                {
                    for( int j = 0; j < nrColumns; j++ )
                    {
                        gCrossMatrix[i][j] +=  *((*currentCluePtr).possiblePlacements[i] + j);
                    }
                }

                currentColumnPtr++;
                currentCluePtr = *currentColumnPtr;
            }
        }
    }
}

void setCrossesForDisplay()
{
    for( int rowIdx = 0; rowIdx < nrRows; rowIdx++ ) // For clues in all rows
    {
        for( int columnIdx = 0; columnIdx < nrColumns; columnIdx++ ) // For clues in all columns
        {
            if( gCrossMatrix[rowIdx][columnIdx] == 0 ) // No color can be here
            {
                gColorMatrix[rowIdx][columnIdx] = COLOR_SURFACE_CROSS;
            }
        }
    }
}








#endif // MYFUNCTIONS_CPP

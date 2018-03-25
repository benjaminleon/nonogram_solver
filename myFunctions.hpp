#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H
#include "clue.hpp"
#include "graphics.hpp"
#include <SDL2/SDL.h>

int colorize();
int colorStrToInt(std::string);
int getFreeSpace(Clue*, int, int);
int getNrCluesInHolder(Clue** holderPtr);

void cancelPossibilitiesForRow(int);
void cancelPossibilitiesForColumn(int);
void cancelPossibilitiesForEmptyHolders();

void findCrosses();
void printAllClues();
void printPossible(Clue*);
void displayColorMatrix();
void setCrossesForDisplay();
void initPossibleLocations();
void colorizeBlock(Clue*, int, int);
void setOccupiedForAllBut(Clue*, int, int);
void setValueInHolder(Clue**, int, int, int, int);
void setImpossibleAhead(Clue**, int, int, int);
void setImpossibleBehind(Clue**, int, int, int);
void colorizeAllFreeInRowAndZeroOthers(Clue*, int);
void colorizeAllFreeInColumnAndZeroOthers(Clue*, int);

extern Clue** allRows[];
extern Clue** allColumns[];

extern int xCoord;
extern int yCoord;
extern const int nrRows;
extern const int nrColumns;

enum holdertypes
{
    THIS_IS_A_COLUMN,
    THIS_IS_A_ROW
};










#endif // MYFUNCTIONS.H

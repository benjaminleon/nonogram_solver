#include "clue.hpp"


Clue::Clue(int number, std::string colorLetter)
{
    length = number;
    remaining = number;

    color = colorLetter;

    possiblePlacements = new int*[nrRows];
    for(int i = 0; i < nrRows; i++)
    {
        possiblePlacements[i] = new int[nrColumns];
    }
    for(int i = 0; i < nrRows; i++)
    {
        for(int j = 0; j < nrColumns;  j++)
        {
            *(possiblePlacements[i] + j) = 0;
        }
    }


    sketchBoard[nrRows][nrColumns] = {0};
    isUsedUp = false;
}

Clue::~Clue()
{
}
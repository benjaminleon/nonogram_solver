#ifndef CLUE_H
#define CLUE_H
#include <string>

extern const int nrRows;
extern const int nrColumns;

class Clue
{
    public:
        Clue(int, std::string);
        virtual ~Clue();
        int length;
        int remaining;
        std::string color;
        int** possiblePlacements;
        int sketchBoard[2][3]; // Used for temporary storing and manipulation
        bool isUsedUp;

    protected:

    private:
};

#endif // CLUE_H

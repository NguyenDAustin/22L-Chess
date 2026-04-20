#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include "pieces.h"

typedef struct {
    char file;   /* 'A' through 'J' */
    int rank;    /* 1 through 8 */
} Pos;

typedef struct {
    char move[4];
} Move;

Move parseInput(const char *input);
bool isValid(const char *input);

bool inputHasMove(void);
bool inputIsCastleKingSide(void);
bool inputIsCastleQueenSide(void);
bool inputIsUndo(void);
bool inputIsQuit(void);

Pos inputGetStartPos(void);
Pos inputGetEndPos(void);

#endif
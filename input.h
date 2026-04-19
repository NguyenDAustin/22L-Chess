#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>


typedef enum {
    KING = 0,
    QUEEN = 1,
    ANTEATER = 2,
    BISHOP = 3,
    KNIGHT = 4,
    ROOK = 5,
    ANT = 6
} Rank;

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
/*ai.h*/
#ifndef AI_H
#define AI_H

#include "pieces.h"
#include "move.h"
#include "board.h"
#include "enum.h"

typedef enum
{
    EASY = 0,
    MEDIUM = 1,
    HARD = 2
} Difficulty;

void setDifficulty(Difficulty d);
int aiSelectMove(Piece board[8][10], Color player, Move *out);

#endif

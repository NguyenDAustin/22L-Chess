/*ai.h*/
#ifndef AI_H
#define AI_H

#include "pieces.h"
#include "move.h"
#include "board.h"
#include "enum.h"

typedef enum
{
<<<<<<< HEAD
    AI_EASY,
    AI_MEDIUM,
    AI_HARD
} AIDifficulty;

int aiSelectMoveAtDifficulty(Piece board[8][10], Color player, AIDifficulty difficulty, Move *out);
=======
    EASY = 0,
    MEDIUM = 1,
    HARD = 2
} Difficulty;

void setDifficulty(Difficulty d);
>>>>>>> 162f63c1ea3de2f0975d79e2f8eff2231c3a6d09
int aiSelectMove(Piece board[8][10], Color player, Move *out);

#endif

/*ai.h*/
#ifndef AI_H
#define AI_H

#include "pieces.h"
#include "move.h"

typedef enum
{
    AI_EASY,
    AI_MEDIUM,
    AI_HARD
} AIDifficulty;

int aiSelectMoveAtDifficulty(Piece board[8][10], Color player, AIDifficulty difficulty, Move *out);
int aiSelectMove(Piece board[8][10], Color player, Move *out);

#endif

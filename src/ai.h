/*ai.h*/
#ifndef AI_H
#define AI_H

#include "pieces.h"
#include "move.h"

int aiSelectMove(Piece board[8][10], Color player, Move *out);
int aiSelectMoveAtDifficulty(Piece board[8][10], Color player, AIDifficulty difficulty, Move *out);

#endif

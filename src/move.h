#ifndef MOVE_H
#define MOVE_H

#include "pieces.h"

typedef struct {
    int startRow; //scr
    int startCol; //sc
    int endRow; //er
    int endCol; //ec
    int capture; //capture
} Move;

void executeMove (Piece board[8][10], Move *move);
void recordMove (Move move, const char *filename);
void executeCapture(Piece board[8][10], Move *move);
void executePawnCapture(Piece board[8][10], Move *move);
void executeAnteaterCapture(Piece board[8][10], Move *move);

/* Returns 1 if `move` is legal for `player` on `board`, else 0. */
int isLegalMove(Piece board[8][10], Move *move, Color player);

/* Returns 1 if `player`'s king is currently under attack on `board`. */
int isInCheck(Piece board[8][10], Color player);

/* Returns 1 if `player` has at least one legal move on `board`. */
int hasAnyLegalMove(Piece board[8][10], Color player);

#endif

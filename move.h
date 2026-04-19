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
#endif
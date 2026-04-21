#ifndef MOVE_H
#define MOVE_H

#include "pieces.h"

typedef struct {
    int startRow; //scr
    int startCol; //sc
    int endRow; //er
    int endCol; //ec
    int capture; //capture

    int enPassant;
    int castle;
    int promotion;
    Rank promotionType;
} Move;

void executeMove(Piece board[8][10], Move *move, Move lastMove);

void recordMove (Move move, const char *filename);
void executeCapture(Piece board[8][10], Move *move);
void executePawnCapture(Piece board[8][10], Move *move);
void executeAnteaterCapture(Piece board[8][10], Move *move);

void executeEnPassant(Piece board[8][10], Move *move);
void executeCastle(Piece board[8][10], Move *move);
//void promotePawn();
#endif
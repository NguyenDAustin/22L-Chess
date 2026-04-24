#ifndef MOVE_H
#define MOVE_H

#include "pieces.h"
#include "board.h"

typedef struct Move{
    int startRow; //scr
    int startCol; //sc
    int endRow; //er
    int endCol; //ec
    int capture; //capture

    int enPassant;
    int castle;
} Move;

void executeMove(Board *board, Move *move, Move lastMove);

void recordMove (Move move, const char *filename);
void executeCapture(Board *board, Move *move);
void executePawnCapture(Board *board, Move *move);
void executeAnteaterCapture(Board *board, Move *move);

//special moves
void executeEnPassant(Board *board, Move *move);
void executeCastle(Board *board, Move *move);

int legalMove(Board *board, Move *move, Color turn, Move lastMove);
int possibleMove(Board *board, Color turn, Move lastMove);

//checkmate stalemate
void copyBoard(&testBoard, board); //creates a board to test possible moves - new: new board, og: original board
int checkCheckmate(Board *board, Color turn, Move lastMove);
int checkStalemate(Board *board, Color turn, Move lastMove);

#endif

#ifndef MOVE_H
#define MOVE_H

#include "pieces.h"

typedef struct Move{
    int startRow; //scr
    int startCol; //sc
    int endRow; //er
    int endCol; //ec
    int capture; //capture

    int enPassant;
    int castle;
} Move;

void executeMove(Piece board[8][10], Move *move, Move lastMove);

void recordMove (Move move, const char *filename);
void executeCapture(Piece board[8][10], Move *move);
void executePawnCapture(Piece board[8][10], Move *move);
void executeAnteaterCapture(Piece board[8][10], Move *move);

//special moves
void executeEnPassant(Piece board[8][10], Move *move);
void executeCastle(Piece board[8][10], Move *move);

/* Someone already made it
//checkmate stalemate
void copyBoard(Piece new[8][10], Piece og[8][10]); //creates a board to test possible moves - new: new board, og: original board
int legalMove(Piece board[8][10], Move *move, Color turn, Move lastMove);
int possibleMove(Piece board[8][10], Color turn, Move lastMove);
int checkCheckmate(Piece board[8][10], Color turn, Move lastMove);
int checkStalemate(Piece board[8][10], Color turn, Move lastMove);
*/

#endif
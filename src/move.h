#ifndef MOVE_H
#define MOVE_H

#include "pieces.h"
#include "board.h" //remove

struct Move
{
    int startRow; // scr
    int startCol; // sc
    int endRow;   // er
    int endCol;   // ec
    int capture;  // capture

    int enPassant;
    int castle;
};
typedef struct Board_State Board_State;


/*
#include "pieces.h"


typedef struct Move
{
    int startRow; // scr
    int startCol; // sc
    int endRow;   // er
    int endCol;   // ec
    int capture;  // capture

    int enPassant;
    int castle;
} Move;
typedef struct Board Board;
typedef struct Board_State Board_State;
*/





void executeMove(Board *board, Move *move, Move lastMove);
void executeCapture(Board *board, Move *move);
void executePawnCapture(Board *board, Move *move);
void executeAnteaterCapture(Board *board, Move *move);

// special moves
void executeEnPassant(Board *board, Move *move);
void executeCastle(Board *board, Move *move);

int possibleMove(Board_State *boardState, Board *board, Color turn);


//checkmate stalemate
/*
void copyBoard(&testBoard, board); //creates a board to test possible moves - new: new board, og: original board
int checkCheckmate(Board *board, Color turn, Move lastMove);
int checkStalemate(Board *board, Color turn, Move lastMove);
int legalMove(Board *board, Move *move, Color turn, Move lastMove);
int possibleMove(Board *board, Color turn, Move lastMove);
void recordMove(Move move, const char *filename);

*/

#endif

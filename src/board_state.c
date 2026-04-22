#include "board_state.h"

bool aPieceWasClicked(const Board_State *boardState){
    return boardState->clickedPiece;
}

bool newPieceWasClicked(const Board_State *boardState, Piece *clickedPiece){
    return (clickedPiece != boardState->clickedPiece && clickedPiece != NULL);
}

bool hasUpdate(const Board_State *boardState){
    return boardState->hasUpdate;
}

void setUpdate(Board_State *boardState, bool update){
    boardState->hasUpdate = update;
}

Piece *getClickedPiece(const Board_State *boardState){
    return boardState->clickedPiece;
}

void setClickedPiece(Board_State *boardState, Piece *piece){
    boardState->clickedPiece = piece;
}

void resetClickedPiece(Board_State *boardState){
    setClickedPiece(boardState, NULL);
}


void getLegalMoveCount(){
    
} 

void setLegalMoveCount(); 
void resetLegalMoveCount(); 

//BOARD STATE - LEGAL MOVE FUNCTIONS 
void clearLegalMoves(Board_State* boardState){
    
}


void addLegalMove(Board_State* boardState, Pos pos);
bool isLegalMoveSquare(const Board_State* boardState, int row, int col);
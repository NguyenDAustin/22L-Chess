#ifndef BOARD_STATE_H 
#define BOARD_STATE_H 

#include "pieces.h"



//////////////////////////////////////////////////////////////////////////////////////////////////
//BOARD STATE FUNCTIONS

/*
typedef enum{  
    //gui to board 
    PIECE_WAS_CLICKED = 0,
    PIECE_WAS_MOVED = 1,

    //board to gui
    PIECE_WAS_CAPTURED = 2
} FLAG; 
 */ //perhaps will have use in future



typedef struct {
  Piece* clickedPiece; 
  bool hasUpdate; 


  Pos clickPos; 

  Pos legalMoves[BOARD_HEIGHT * BOARD_WIDTH];
  int legalMoveCount;

  //FLAG flags[NUMBER_OF_FLAGS]; //do certain things in array depending on what flag has been set
} Board_State; 


//BOARD STATE - STATE FUNCTIONS
bool aPieceWasClicked(const Board_State* boardState);
bool newPieceWasClicked(const Board_State* boardState, Piece* clickedPiece);
bool hasUpdate(const Board_State* boardState);

//BOARD STATE - GETTERS + SETTERS
void setUpdate(Board_State* boardState, bool update);
Piece* getClickedPiece(const Board_State* boardState);
void setClickedPiece(Board_State* boardState, Piece* piece);
void resetClickedPiece(Board_State* boardState);

void getLegalMoveCount(); 
void setLegalMoveCount(); 
void resetLegalMoveCount(); 

//BOARD STATE - LEGAL MOVE FUNCTIONS 
void clearLegalMoves(Board_State* boardState);
void addLegalMove(Board_State* boardState, Pos pos);
bool isLegalMoveSquare(const Board_State* boardState, int row, int col);


#endif 
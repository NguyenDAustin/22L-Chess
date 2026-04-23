#ifndef BOARD_STATE_H 
#define BOARD_STATE_H 

#include "pieces.h"
#include "board.h"

extern const int MAX_LEGAL_MOVES; 

typedef struct Board Board; 
//////////////////////////////////////////////////////////////////////////////////////////////////
//BOARD STATE FUNCTIONS

typedef struct Board_State {
  Piece* clickedPiece; 
  bool hasUpdate; 
  bool moveSuccess; 


  Pos clickPos; 

  Pos legalMoves[BOARD_HEIGHT * BOARD_WIDTH];
  int legalMoveCount;
  int movesMade; 

  //FLAG flags[NUMBER_OF_FLAGS]; //do certain things in array depending on what flag has been set
} Board_State; 

//BOARD - CTOR
void initializeBoardState(Board_State* boardState);


//BOARD STATE - STATE FUNCTIONS
bool aPieceWasClicked(const Board_State* boardState);
bool newPieceWasClicked(const Board_State* boardState, Piece* clickedPiece);
bool hasUpdate(const Board_State* boardState);
bool moveSucces(const Board_State* boardState);
bool canPieceGoTo(Board* board, Piece* piece, Pos start, Pos end);
bool isLegalMoveSquare(const Board_State* boardState, Pos pos);


//BOARD STATE - general functions 
void incrementMovesMade(Board_State* boardState);

//BOARD STATE - GETTERS + SETTERS
void setUpdate(Board_State* boardState, bool update);
Piece* getClickedPiece(const Board_State* boardState);
void setClickedPiece(Board_State* boardState, Piece* piece);
void resetClickedPiece(Board_State* boardState);
void setMovesMade(Board_State* boardState, int moves); 
int getMovesMade(const Board_State* boardState); 


//BOARD STATE - legal move getters + setters 
int getLegalMoveCount(const Board_State* boardState); 
void setLegalMoveCount(Board_State* boardState, int count); 
void resetLegalMoveCount(Board_State* boardState); 

//BOARD STATE - LEGAL MOVE FUNCTIONS 
void addLegalMove(Board_State* boardState, Pos pos);
void generateLegalMoves(Board_State* boardState, Board* board, Piece* piece, Pos start); 

#endif 
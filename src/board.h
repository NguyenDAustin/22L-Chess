#ifndef BOARD_H 
#define BOARD_H

#include "pieces.h"



extern const int NUMBER_OF_FLAGS; 
extern const int PIECE_ORDER[10]; 
extern const int BLACK_ANT_ROW; 
extern const int WHITE_ANT_ROW;
extern const int MIN_ROW; 
extern const int MAX_ROW; 
extern const int MIN_COL; 
extern const int MAX_COL; 

//BOARD FUNCTIONS

typedef struct Board {
    Piece* board[BOARD_HEIGHT][BOARD_WIDTH]; 
} Board; 


Piece_Icon* getImagePiece(Piece_Icon** images, Color color, Rank rank);
Piece_Icon* getWhiteImagePiece(Piece_Icon** images, Rank rank); 
Piece_Icon* getBlackImagePiece(Piece_Icon** images, Rank rank); 


Piece* createPiecePtr(Piece_Icon* img, Color color, Rank rank, Pos pos);


void createPieces( Board* mBoard, Piece_Icon** images, Color color);
void createWhitePieces(Board* mBoard, Piece_Icon** images); 
void createBlackPieces(Board* mBoard, Piece_Icon** images); 

void createAnts(Board* mBoard, Piece_Icon** images, Color color); 
void createBlackAnts(Board* mBoard, Piece_Icon** images);
void createWhiteAnts(Board* mBoard, Piece_Icon** images);

//BOARD INITIALIZATION FUNCTIONS
void defaultInitializeBoard(Board* mBoard); 
void initializeBoard(Board* mBoard, Piece_Icon** images); 

//BOARD 
bool hasPiece(const Board* board, int row, int col); 
bool isBoardEdge(int row, int col); 
bool isColEdge(int col);
bool isRowEdge(int row); 

Piece* getSquare(const Board* board, int row, int col); 

void setSquare(Board* board, Piece* piece, int row, int col);
void deletePiece(Board* board, Pos pos);
void addPiece(Board* board, Piece* piece, Pos pos);
void movePiece(Board* board, Pos oldPos, Pos newPos);



//////////////////////////////////////////////////////////////////////////////////////////////////
//BOARD STATE FUNCTIONS

typedef enum{  
    //gui to board 
    PIECE_WAS_CLICKED = 0,
    PIECE_WAS_MOVED = 1,

    //board to gui
    PIECE_WAS_CAPTURED = 2
} FLAG; 



typedef struct {
  Piece* clickedPiece; 
  bool hasUpdate; 
  Pos clickPos; 
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

//BOARD STATE - MAIN FUUNCTIONALITY 
void sendInput(Board* board, Board_State* boardState, Pos clickPos);


#endif 
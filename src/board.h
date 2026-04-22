#ifndef BOARD_H 
#define BOARD_H

#include "pieces.h"
#include "board_state.h"



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


//BOARD  - MAIN FUUNCTIONALITY 
void sendInput(Board* board, Board_State* boardState, Pos clickPos);
#endif 
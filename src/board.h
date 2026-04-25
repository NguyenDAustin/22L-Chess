#ifndef BOARD_H 
#define BOARD_H

#include "pieces.h"
#include "render.h"

typedef struct Board_State Board_State; 
typedef struct Board_Bundle Board_Bundle; 


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


Icon* getImagePiece(Icon** images, Color color, Rank rank);
Icon* getWhiteImagePiece(Icon** images, Rank rank); 
Icon* getBlackImagePiece(Icon** images, Rank rank); 


Piece* createPiecePtr(Icon* img, Color color, Rank rank, Pos pos);

void createPieces( Board* mBoard, Icon** images, Color color);
void createWhitePieces(Board* mBoard, Icon** images); 
void createBlackPieces(Board* mBoard, Icon** images); 

void createAnts(Board* mBoard, Icon** images, Color color); 
void createBlackAnts(Board* mBoard, Icon** images);
void createWhiteAnts(Board* mBoard, Icon** images);

//BOARD INITIALIZATION FUNCTIONS
void defaultInitializeBoard(Board* mBoard); 
void initializeBoard(Board* mBoard, Icon** images); 

//BOARD 
bool hasPiece(const Board* board, int row, int col); 
bool isBoardEdge(int row, int col); 
bool isColEdge(int col);
bool isRowEdge(int row); 

Piece* getSquare(const Board* board, int row, int col); 
void setSquare(Board* board, Piece* piece, int row, int col);

Piece* deletePiece(Board* board, Pos pos); //sets the piece* to null at pos
void addPiece(Board* board, Piece* piece, Pos pos);
void movePiece(Board *board, Board_State* boardState, Piece* piece, Pos newPos);
void capturePiece(Board* board, Board_State* boardState, Piece* piece, Pos capturePos);
void promotePiece(Board_Bundle* boardData, Pos pos, Rank newRank); 

#endif 

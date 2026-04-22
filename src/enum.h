#ifndef ENUM_H
#define ENUM_H

typedef enum  //sets up color
{   
    WHITE = 0,
    BLACK = 1
} Color;

typedef enum  //sets up the ranks
{
    EMPTY = -1,
    KING = 0, 
    QUEEN = 1,
    ANTEATER = 2,
    BISHOP = 3,
    KNIGHT = 4,
    ROOK = 5,
    PAWN = 6
} Rank;

typedef enum{ //for the board (by Queency)
    WHITE_KING = 0,
    BLACK_KING = 1,
    WHITE_QUEEN = 2, 
    BLACK_QUEEN = 3, 
    WHITE_ANTEATER = 4, 
    BLACK_ANTEATER = 5, 
    WHITE_BISHOP = 6, 
    BLACK_BISHOP = 7, 
    WHITE_KNIGHT = 8, 
    BLACK_KNIGHT = 9, 
    WHITE_ROOK = 10, 
    BLACK_ROOK = 11, 
    WHITE_ANT = 12, 
    BLACK_ANT = 13
} Piece_Type; 


enum { //for the board (by Queency)
    BOARD_HEIGHT = 8, 
    BOARD_WIDTH = 10,
    SQUARE_SIZE = 100,
    WINDOW_HEIGHT = 1500, 
    WINDOW_WIDTH = 1500
}; 

#endif

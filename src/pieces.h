#ifndef PIECES_H
#define PIECES_H

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdbool.h>
#include <stddef.h>

typedef enum  //sets up color
{   
    White = 0,
    Black = 1
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

typedef cairo_surface_t Piece_Icon;

typedef struct {
    int row;
    int col;
} Pos;

typedef struct Piece Piece;
typedef struct Move Move;

typedef struct PieceVTable{
    int (*canMove)(Piece board[8][10], Piece *piece, int sr, int sc, int er, int ec); //sr: starting row, sc: starting column, er: end row, ec: end column
    int (*canCapture)(Piece board[8][10], Piece *piece, int sr, int sc, int er, int ec);
} PieceVTable;


typedef struct Piece //sets up how the pieces move
{
    Piece_Icon *img;
    Color color;
    Rank type;
    PieceVTable *vtable;
    int value;
    Pos pos;
    int moved;
} Piece;

//Position Function (by Queency)
void posCtor(Pos *mPos, int row, int col);
bool isValid(int p, int limit);
bool isRowValid(int row);
bool isColValid(int col);
bool isPosValid(Pos pos);

//Piece Functions (by Queency)
void pieceCtor(Piece *mPiece, Piece_Icon *img, Color color, Rank type, Pos pos, PieceVTable *vtable);

Piece_Icon *getImage(const Piece *piece);
void setImage(Piece *piece, Piece_Icon *img);

Color getColor(const Piece *piece);
void setColor(Piece *piece, Color color);

Rank getType(const Piece *piece);
void setType(Piece *piece, Rank type);

Pos getPos(const Piece *piece);
void setPos(Piece *piece, Pos pos);

Piece createPiece(Color color, Rank type);  //connects the pieces together

//Movement and Capture
int rookCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);
int bishopCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);
int knightCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);
int queenCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);
int kingCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);
int pawnCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);
int anteaterCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);
int pawnCanCapture(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);
int anteaterCanCapture(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);

//special moves
int pawnCanEnPassant(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec, Move *lastMove);
int kingCanCastle(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);

//check
Pos findKing(Piece board[8][10], Color color);
int attackSquare(Piece board[8][10], int row, int col, Color attColor);
int kingCheck(Piece board[8][10], Color kingColor);

#endif
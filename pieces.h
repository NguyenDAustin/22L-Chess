#ifndef PIECES_H
#define PIECES_H

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
} PieceType;

typedef struct Piece Piece;

typedef struct PieceVTable{
    int (*canMove)(Piece board[8][10], Piece *piece, int sr, int sc, int er, int ec); //sr: starting row, sc: starting column, er: end row, ec: end column
    int (*canCapture)(Piece board[8][10], Piece *piece, int sr, int sc, int er, int ec);
} PieceVTable;


typedef struct Piece //sets up how the pieces move
{
    Color color;
    PieceType type;
    PieceVTable *vtable;
} Piece;

int rookCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);
int bishopCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);
int knightCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);
int queenCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);
int kingCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);
int pawnCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);
int anteaterCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);
int pawnCanCapture(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);
int anteaterCanCapture(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec);

Piece createPiece(Color color, PieceType type);  //connects the pieces together

#endif
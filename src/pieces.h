#ifndef PIECES_H
#define PIECES_H

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdbool.h>
#include <stddef.h>
#include "enum.h"

typedef cairo_surface_t Piece_Icon;

typedef struct {
    int row;
    int col;
} Pos;

typedef struct Board Board;
typedef struct Piece Piece;
typedef struct Move Move;

typedef struct PieceVTable {
    int (*canMove)(Board *board, Piece *piece, int sr, int sc, int er, int ec);
    int (*canCapture)(Board *board, Piece *piece, int sr, int sc, int er, int ec);
} PieceVTable;

struct Piece {
    Piece_Icon *img;
    Color color;
    Rank type;
    PieceVTable *vtable;
    int value;
    Pos pos;
    int moved;
};

void posCtor(Pos *mPos, int row, int col);
bool isValidPos(int p, int limit);
bool isRowValid(int row);
bool isColValid(int col);
bool isPosValid(Pos pos);

void pieceCtor(Piece *mPiece, Piece_Icon *img, Color color, Rank type, Pos pos, PieceVTable *vtable);

Piece_Icon *getImage(const Piece *piece);
void setImage(Piece *piece, Piece_Icon *img);

Color getColor(const Piece *piece);
void setColor(Piece *piece, Color color);

Rank getType(const Piece *piece);
void setType(Piece *piece, Rank type);

Pos getPos(const Piece *piece);
void setPos(Piece *piece, Pos pos);

int rookCanMove(Board *board, Piece *p, int sr, int sc, int er, int ec);
int rookCanCapture(Board *board, Piece *p, int sr, int sc, int er, int ec);
int bishopCanMove(Board *board, Piece *p, int sr, int sc, int er, int ec);
int bishopCanCapture(Board *board, Piece *p, int sr, int sc, int er, int ec);
int knightCanMove(Board *board, Piece *p, int sr, int sc, int er, int ec);
int knightCanCapture(Board *board, Piece *p, int sr, int sc, int er, int ec);
int queenCanMove(Board *board, Piece *p, int sr, int sc, int er, int ec);
int queenCanCapture(Board *board, Piece *p, int sr, int sc, int er, int ec);
int kingCanMove(Board *board, Piece *p, int sr, int sc, int er, int ec);
int kingCanCapture(Board *board, Piece *p, int sr, int sc, int er, int ec);
int pawnCanMove(Board *board, Piece *p, int sr, int sc, int er, int ec);
int pawnCanCapture(Board *board, Piece *p, int sr, int sc, int er, int ec);
int anteaterCanMove(Board *board, Piece *p, int sr, int sc, int er, int ec);
int anteaterCanCapture(Board *board, Piece *p, int sr, int sc, int er, int ec);


/*
Pos findKing(Piece board[8][10], Color color);
int attackSquare(Piece board[8][10], int row, int col, Color attColor);
int kingCheck(Piece board[8][10], Color kingColor);
*/

#endif
#include "pieces.h"
#include <stdlib.h>

int isEmpty(Piece board[8][10], int r, int c) { // checks if square is empty
    return board[r][c].type == EMPTY;
}

int isEnemy(Piece board[8][10], Piece *p, int r, int c) { // checks if square has enemy
    return board[r][c].type != EMPTY && board[r][c].color != p->color;
}

int isStraightPathClear(Piece board[8][10], int sr, int sc, int er, int ec) {
    int rStep = 0;
    int cStep = 0;

    if (sr == er) {
        cStep = (ec > sc) ? 1 : -1;
    } else if (sc == ec) {
        rStep = (er > sr) ? 1 : -1;
    } else {
        return 0;
    }

    int r = sr + rStep;
    int c = sc + cStep;

    while (r != er || c != ec) {
        if (!isEmpty(board, r, c)) {
            return 0;
        }
        r += rStep;
        c += cStep;
    }

    return 1;
}

int isDiagonalPathClear(Piece board[8][10], int sr, int sc, int er, int ec) {
    if (abs(sr - er) != abs(sc - ec)) {
        return 0; // not diagonal
    }

    int rStep;
    int cStep;

    if (er > sr) {
        rStep = 1;
    } else {
        rStep = -1;
    }

    if (ec > sc) {
        cStep = 1;
    } else {
        cStep = -1;
    }

    int r = sr + rStep;
    int c = sc + cStep;

    while (r != er || c != ec) {
        if (!isEmpty(board, r, c)) {
            return 0;
        }
        r += rStep;
        c += cStep;
    }

    return 1;
}


//movement
int rookCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec) 
{
    if (!isEmpty(board, er, ec)) {
        return 0;
    }

    if (sr == er || sc == ec) { // checks to see if it is within the same row or same column
        if (isStraightPathClear(board, sr, sc, er, ec)) {
            return 1;
        }
    }
    return 0;
}

int rookCanCapture(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec)
{
    if (!isEnemy(board, p, er, ec)) {
        return 0;
    }

    if (sr == er || sc == ec) {
        return isStraightPathClear(board, sr, sc, er, ec);
    }

    return 0;
}


int bishopCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec)
{   
    if (!isEmpty(board, er, ec)) {
        return 0;
    }

    if (abs(sr - er) == abs(sc - ec)) { // they need to be equal so it can go diagonal, dont have to keep color in mind because it shouldn't go off
        if (isDiagonalPathClear(board, sr, sc, er, ec)) {
            return 1;
        }
    }
    return 0;
}

int bishopCanCapture(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec)
{
    if (!isEnemy(board, p, er, ec)) {
        return 0;
    }

    if (abs(sr - er) == abs(sc - ec)) {
        return isDiagonalPathClear(board, sr, sc, er, ec);
    }

    return 0;
}

int knightCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec) {
    int mr = abs(sr - er);
    int mc = abs(sc - ec);

    if (!isEmpty(board, er, ec)) {
        return 0;
    }

    if ((mr == 2 && mc == 1) || (mr == 1 && mc == 2)) {
        return 1;
    }
    return 0;
}

int knightCanCapture(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec)
{
    int mr = abs(sr - er);
    int mc = abs(sc - ec);

    if (!isEnemy(board, p, er, ec)) {
        return 0;
    }

    if ((mr == 2 && mc == 1) || (mr == 1 && mc == 2)) {
        return 1;
    }

    return 0;
}



int queenCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec) 
{
    if (!isEmpty(board, er, ec)) {
        return 0;
    }

    if (sr == er || sc == ec) {
        return isStraightPathClear(board, sr, sc, er, ec); //rook move + bishop move
    }

    if (abs(sr - er) == abs(sc - ec)) {
        return isDiagonalPathClear(board, sr, sc, er, ec);
    }

    return 0;
}

int queenCanCapture(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec)
{
    if (!isEnemy(board, p, er, ec)) {
        return 0;
    }

    if (sr == er || sc == ec) {
        return isStraightPathClear(board, sr, sc, er, ec);
    }

    if (abs(sr - er) == abs(sc - ec)) {
        return isDiagonalPathClear(board, sr, sc, er, ec);
    }

    return 0;
}


int kingCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec) 
{
    int mr = abs(er - sr); //moved rows
    int mc = abs(ec - sc); //moved columns
    
    if (!isEmpty(board, er, ec)) {
        return 0;
    }

    if (mr == 0 && mc ==0){ //to check if the king moved
        return 0;
    }

    if (mr <= 1 && mc <= 1){ //checks if king move is legal, <= is to see if its diagonal
        return 1;
    }

    return 0;
    /*if () {return 2;}*/ //castle
}

int kingCanCapture(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec)
{
    int mr = abs(er - sr);
    int mc = abs(ec - sc);

    if (!isEnemy(board, p, er, ec)) {
        return 0;
    }

    if (mr == 0 && mc == 0) {
        return 0;
    }

    if (mr <= 1 && mc <= 1) {
        return 1;
    }

    return 0;
}


int pawnCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec) 
{
    int mr;

    if (p->color == White) {
        mr = 1;
    } else {
        mr = -1;
    }

    // move forward 1
    if (ec == sc && er == sr + mr && isEmpty(board, er, ec)) {
        return 1;
    }

    if (ec == sc && er == sr + 2 * mr){
        if (p->color == White && sr == 1 &&
            isEmpty(board, sr + mr, sc) &&
            isEmpty(board, er, ec)) {
            return 1;
        }

        if (p->color == Black && sr == 6 &&
            isEmpty(board, sr + mr, sc) &&
            isEmpty(board, er, ec)) {
            return 1;
        }
    }

    return 0;
}


int pawnCanCapture(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec) 
{
    int mr;

    if (p->color == White) {
        mr = 1;
    } else {
        mr = -1;
    }

    if (er == sr + mr && abs(ec - sc) == 1) {

        if (board[er][ec].type != EMPTY && board[er][ec].color != p->color) { //checks to see if the spot diagonal has an enemy piece
            return 1;
        }
    }


    return 0;
}


int anteaterCanMove(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec) 
{
    int mr = abs(er - sr);
    int mc = abs(ec - sc);

    if (mr == 0 && mc == 0) {
        return 0;
    }

    if (mr <= 1 && mc <= 1) {
        if (board[er][ec].type == EMPTY) {
            return 1;
        }
    }

    return 0;
}
int anteaterCanCapture(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec) 
{
    int mr = er - sr;
    int mc = ec - sc;

    int rowStep = 0;
    int colStep = 0;

    if (mr == 0 && mc == 0) {
        return 0;
    }

    if (mr != 0 && mc != 0) {
        return 0;
    }

    if (mr != 0) {
        rowStep = (mr > 0) ? 1 : -1;
    }
    if (mc != 0) {
        colStep = (mc > 0) ? 1 : -1;
    }

    int r = sr + rowStep;
    int c = sc + colStep;

    while (r != er || c != ec) {
        if (board[r][c].type != PAWN || board[r][c].color == p->color)  {
            return 0;
        }
        r += rowStep;
        c += colStep;
    }

    if (board[er][ec].type != PAWN || board[er][ec].color == p->color) {
        return 0;
    }

    return 1;
}

// virtual Tables
static PieceVTable rookTable = {rookCanMove, rookCanCapture};
static PieceVTable bishopTable = {bishopCanMove, bishopCanCapture};
static PieceVTable knightTable = {knightCanMove, knightCanCapture};
static PieceVTable queenTable = {queenCanMove, queenCanCapture};
static PieceVTable kingTable = {kingCanMove, kingCanCapture};
static PieceVTable pawnTable = {pawnCanMove, pawnCanCapture};
static PieceVTable anteaterTable = {anteaterCanMove, anteaterCanCapture};


Piece createPiece(Color color, PieceType type){  //constructor
    Piece p;
    p.color = color;
    p.type = type; 

    switch (type) {
        case ROOK: p.vtable = &rookTable; break;
        case BISHOP: p.vtable = &bishopTable; break;
        case KNIGHT: p.vtable = &knightTable; break;
        case QUEEN: p.vtable = &queenTable; break;
        case KING: p.vtable = &kingTable; break;
        case PAWN: p.vtable = &pawnTable; break;
        case ANTEATER: p.vtable = &anteaterTable; break;
        default: p.vtable = NULL; break;
    }
    return p;
}

//need to work on en passant and castle
#include "pieces.h"
#include <stdlib.h>

void posCtor(Pos *mPos, int row, int col) {
    if (!mPos) return;
    mPos->row = row;
    mPos->col = col;
}

bool isValid(int p, int limit) { //Queency
    return (p >= 0 && p < limit);
}

bool isRowValid(int row) { //Queency
    return isValid(row, BOARD_HEIGHT);
}

bool isColValid(int col) { //Queency
    return isValid(col, BOARD_WIDTH);
}

bool isPosValid(Pos pos) { //Queency
    return isRowValid(pos.row) && isColValid(pos.col);
}



void pieceCtor(Piece *mPiece, Piece_Icon *img, Color color, Rank type, Pos pos, PieceVTable *vtable) { //Queency
    if (!mPiece) return;
    mPiece->img = img;
    mPiece->color = color;
    mPiece->type = type;
    mPiece->pos = pos;
    mPiece->vtable = vtable;
    mPiece->moved = 0;
}

Piece_Icon *getImage(const Piece *piece) { return piece ? piece->img : NULL; } //Queency
void setImage(Piece *piece, Piece_Icon *img) { if (piece) piece->img = img; } //Queency

Color getColor(const Piece *piece) { return piece ? piece->color : White; } //Queency
void setColor(Piece *piece, Color color) { if (piece) piece->color = color; } //Queency

Rank getType(const Piece *piece) { return piece ? piece->type : EMPTY; }//Queency
void setType(Piece *piece, Rank type) { if (piece) piece->type = type; } //Queency

Pos getPos(const Piece *piece) {//Queency
    Pos bad = {-1, -1};
    return piece ? piece->pos : bad;
}

void setPos(Piece *piece, Pos pos) { if (piece) piece->pos = pos; }//Queency




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

int pawnCanEnPassant(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec, Move *lastMove)
{
    if (!lastMove) {return 0;}
    int mr = (p->color == White) ? 1 : -1;


    if (er != sr + mr || abs(ec - sc) != 1) { //move diagonally
        return 0;
    }

    if (!isEmpty(board, er, ec)) {
        return 0;
    }

    Piece lastPiece = board[lastMove->endRow][lastMove->endCol]; //pawn has to move two squares

    if (lastPiece.type != PAWN) {
        return 0;
    }

    if (lastPiece.color == p->color) {
        return 0;
    }

    if (abs(lastMove->endRow - lastMove->startRow) != 2) {
        return 0;
    }

    if (lastMove->endRow != sr || lastMove->endCol != ec) {     //pawn must be next to other pawn
        return 0;
    }

    return 1;
}

int kingCanCastle(Piece board[8][10], Piece *p, int sr, int sc, int er, int ec)
{
    if (p->type != KING || p->moved) {
        return 0;
    }

    // same row, moved two columns
    if (sr != er || abs(ec - sc) != 2) {
        return 0;
    }

    // check for empty
    if (!isEmpty(board, er, ec)) {
        return 0;
    }

    if (ec > sc) {
        // kingside rook
        int rookCol = BOARD_WIDTH - 1;

        if (board[sr][rookCol].type != ROOK ||
            board[sr][rookCol].color != p->color ||
            board[sr][rookCol].moved) {
            return 0;
        }

        for (int c = sc + 1; c < rookCol; c++) {
            if (!isEmpty(board, sr, c)) {
                return 0;
            }
        }
        return 1;
    } else {
        // queenside rook
        int rookCol = 0;

        if (board[sr][rookCol].type != ROOK ||
            board[sr][rookCol].color != p->color ||
            board[sr][rookCol].moved) {
            return 0;
        }

        for (int c = rookCol + 1; c < sc; c++) {
            if (!isEmpty(board, sr, c)) {
                return 0;
            }
        }
        return 1;
    }
}

// virtual Tables
static PieceVTable rookTable = {rookCanMove, rookCanCapture};
static PieceVTable bishopTable = {bishopCanMove, bishopCanCapture};
static PieceVTable knightTable = {knightCanMove, knightCanCapture};
static PieceVTable queenTable = {queenCanMove, queenCanCapture};
static PieceVTable kingTable = {kingCanMove, kingCanCapture};
static PieceVTable pawnTable = {pawnCanMove, pawnCanCapture};
static PieceVTable anteaterTable = {anteaterCanMove, anteaterCanCapture};


Piece createPiece(Color color, Rank type){  //constructor
    Piece p;
    p.color = color;
    p.type = type; 
    p.img = NULL;
    p.value = 0;
    p.pos.row = -1;
    p.pos.col = -1;
    p.moved = 0;

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
//need to work on promote
//anteater bug doesn't allow it to eat again
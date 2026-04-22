#include "pieces.h"
#include "board.h"
#include "move.h"

void posCtor(Pos *mPos, int row, int col) {
    if (!mPos) return;
    mPos->row = row;
    mPos->col = col;
}

bool isValidPos(int p, int limit) { //Queency
    return (p >= 0 && p < limit);
}

bool isRowValid(int row) { //Queency
    return isValidPos(row, BOARD_HEIGHT);
}

bool isColValid(int col) { //Queency
    return isValidPos(col, BOARD_WIDTH);
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

Color getColor(const Piece *piece) { return piece ? piece->color : WHITE; } //Queency
void setColor(Piece *piece, Color color) { if (piece) piece->color = color; } //Queency

Rank getType(const Piece *piece) { return piece ? piece->type : EMPTY; }//Queency
void setType(Piece *piece, Rank type) { if (piece) piece->type = type; } //Queency

Pos getPos(const Piece *piece) {//Queency
    Pos bad = {-1, -1};
    return piece ? piece->pos : bad;
}

void setPos(Piece *piece, Pos pos) { if (piece) piece->pos = pos; }//Queency


//boolean helper functions for piece 
bool isSameRow(int r1, int r2){ 
    return (r1 == r2); 
}
bool isSameCol(int c1, int c2){
    return (c1 == c2); 
}


bool isEmpty(Board* board, int r, int c) { // checks if square is empty
    Rank type = getType(board->board[r][c]); 
    return (type == EMPTY);
}

bool isEnemy(Board* board, Piece *p, int r, int c) { // checks if square has enemy
    Piece* piece = board->board[r][c]; 
    Rank type = getType(piece); 
    return type != EMPTY && getColor(piece) != p->color;
}



int isStraightPathClear(Board* board, int sr, int sc, int er, int ec) {
    int rStep = 0;
    int cStep = 0;

    if (isSameRow(sr, er)) {
        cStep = (ec > sc) ? 1 : -1;
    } else if (isSameCol(sc, ec)) {
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

int isDiagonalPathClear(Board* board, int sr, int sc, int er, int ec) {
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
int rookCanMove(Board* board, Piece *p, int sr, int sc, int er, int ec) 
{
    if(!isEmpty(board, er, ec))
        return false; 

    if ((isSameCol(sr, er) || isSameCol(ec, er)) && isStraightPathClear(board, sr, sc, er, ec)) { // checks to see if it is within the same row or same column
            return true;
    }

    return 0;
}

int rookCanCapture(Board* board, Piece *p, int sr, int sc, int er, int ec)
{
    if (!isEnemy(board, p, er, ec)) {
        return 0;
    }

    if (sr == er || sc == ec) {
        return isStraightPathClear(board, sr, sc, er, ec);
    }

    return 0;
}


int bishopCanMove(Board* board, Piece *p, int sr, int sc, int er, int ec)
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

int bishopCanCapture(Board* board, Piece *p, int sr, int sc, int er, int ec)
{
    if (!isEnemy(board, p, er, ec)) {
        return 0;
    }

    if (abs(sr - er) == abs(sc - ec)) {
        return isDiagonalPathClear(board, sr, sc, er, ec);
    }

    return 0;
}

int knightCanMove(Board* board, Piece *p, int sr, int sc, int er, int ec) {
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

int knightCanCapture(Board* board, Piece *p, int sr, int sc, int er, int ec)
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



int queenCanMove(Board* board, Piece *p, int sr, int sc, int er, int ec) 
{
    if (!isEmpty(board, er, ec)) {
        return 0;
    }

    if (isSameRow(sr, er) || isSameCol(sc, ec)) {
        return isStraightPathClear(board, sr, sc, er, ec); //rook move + bishop move
    }

    if (abs(sr - er) == abs(sc - ec)) {
        return isDiagonalPathClear(board, sr, sc, er, ec);
    }

    return 0;
}

int queenCanCapture(Board* board, Piece *p, int sr, int sc, int er, int ec)
{
    if (!isEnemy(board, p, er, ec)) {
        return 0;
    }

    if (isSameRow(sr, er) || isSameCol(sc, ec)) {
        return isStraightPathClear(board, sr, sc, er, ec);
    }

    if (abs(sr - er) == abs(sc - ec)) {
        return isDiagonalPathClear(board, sr, sc, er, ec);
    }

    return 0;
}


int kingCanMove(Board* board, Piece *p, int sr, int sc, int er, int ec) 
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
}

int kingCanCapture(Board* board, Piece *p, int sr, int sc, int er, int ec)
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


int pawnCanMove(Board* board, Piece *p, int sr, int sc, int er, int ec) 
{
    int mr;

    if (p->color == WHITE) {
        mr = 1;
    } else {
        mr = -1;
    }

    // move forward 1
    if (ec == sc && er == sr + mr && isEmpty(board, er, ec)) {
        return 1;
    }

    if (ec == sc && er == sr + 2 * mr){
        if (p->color == WHITE && sr == 1 &&
            isEmpty(board, sr + mr, sc) &&
            isEmpty(board, er, ec)) {
            return 1;
        }

        if (p->color == BLACK && sr == 6 &&
            isEmpty(board, sr + mr, sc) &&
            isEmpty(board, er, ec)) {
            return 1;
        }
    }

    return 0;
}


int pawnCanCapture(Board* board, Piece *p, int sr, int sc, int er, int ec) 
{
    int mr;

    Piece* piece = board->board[er][ec]; 

    if (p->color == WHITE) {
        mr = 1;
    } else {
        mr = -1;
    }

    if (er == sr + mr && abs(ec - sc) == 1) {

        if (getType(piece) != EMPTY && getColor(piece) != getColor(p)) { //checks to see if the spot diagonal has an enemy piece
            return 1;
        }
    }


    return 0;
}


int anteaterCanMove(Board* board, Piece *p, int sr, int sc, int er, int ec) 
{
    int mr = abs(er - sr);
    int mc = abs(ec - sc);
    Piece* piece = board->board[er][ec]; 

    if (mr == 0 && mc == 0) {
        return 0;
    }

    if (mr <= 1 && mc <= 1) {
        if (getType(piece) == EMPTY) {
            return 1;
        }
    }

    return 0;
}

int anteaterCanCapture(Board* board, Piece *p, int sr, int sc, int er, int ec) 
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

    Piece* piece1 = board->board[r][c]; 
    

    while (r != er || c != ec) {
        if (getType(piece1) != PAWN || getColor(piece1) == getColor(p))  {
            return 0;
        }
        r += rowStep;
        c += colStep;
    }

    Piece* piece2 = board->board[er][ec];

    if (getType(piece2) != PAWN || getColor(piece2) == getColor(p)) {
        return 0;
    }

    return 1;
}

int pawnCanEnPassant(Board* board, Piece *p, int sr, int sc, int er, int ec, Move *lastMove)
{
    if (!lastMove) {return 0;}
    int mr = (p->color == WHITE) ? 1 : -1;


    if (er != sr + mr || abs(ec - sc) != 1) { //move diagonally
        return 0;
    }

    if (!isEmpty(board, er, ec)) {
        return 0;
    }

    Piece* lastPiece = board->board[lastMove->endRow][lastMove->endCol]; //pawn has to move two squares

    if (lastPiece->type != PAWN) {
        return 0;
    }

    if (lastPiece->color == p->color) {
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

int kingCanCastle(Board* board, Piece *p, int sr, int sc, int er, int ec)
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

        if (board->board[sr][rookCol]->type != ROOK ||
            board->board[sr][rookCol]->color != p->color ||
            board->board[sr][rookCol]->moved) {
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

        if (board->board[sr][rookCol]->type != ROOK ||
            board->board[sr][rookCol]->color != p->color ||
            board->board[sr][rookCol]->moved) {
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

Pos findKing(Board* board, Color color)
{
    Pos k = {-1, -1};

    for (int r = 0; r < BOARD_HEIGHT; r++){
        for (int c = 0; c < BOARD_WIDTH; c++){
            if (board->board[r][c]->type == KING && board->board[r][c]->color == color){
                k.row = r;
                k.col = c;
                return k;
            }
        }
    }
    return k;
}

int attackSquare(Board* board, int row, int col, Color attColor)
{
    for (int r = 0; r < BOARD_HEIGHT; r++){
        for (int c = 0; c < BOARD_WIDTH; c++){
            Piece *p = board->board[r][c];

            if (p->type == EMPTY || p->color != attColor || p->vtable == NULL){
                continue;
            }

            if (p->vtable->canCapture(board, p, r, c, row, col)){
                return 1;
            }
        }
    }
    return 0;
}

int kingCheck(Board* board, Color kingColor)
{
    Pos kingPos = findKing(board, kingColor);

    if (kingPos.row == -1 || kingPos.col == -1){
        return 0;
    }

    Color enemyColor = (kingColor == WHITE) ? BLACK : WHITE;

    return attackSquare(board, kingPos.row, kingPos.col, enemyColor);
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

#include <stdio.h>
#include "move.h"
#include "board.h"
#include "pieces.h"

void executeMove(Board *board, Move *move, Move lastMove)
{
    Piece *moving = board->board[move->startRow][move->startCol];

    if (moving == NULL || moving->type == EMPTY || moving->vtable == NULL) {
        return;
    }

    move->capture = 0;
    move->enPassant = 0;
    move->castle = 0;

    // castling
    if (moving->type == KING &&
    abs(move->endCol - move->startCol) == 2) {

    if (kingCanCastle(board, moving,
        move->startRow, move->startCol,
        move->endRow, move->endCol)) {

        executeCastle(board, move);
    }

    return;
    }


    // en passant
    if (moving->type == PAWN &&
        pawnCanEnPassant(board, moving,
                         move->startRow, move->startCol,
                         move->endRow, move->endCol, &lastMove)) {
        executeEnPassant(board, move);
        return;
    }

    if (moving->vtable->canCapture(board, moving,
                                   move->startRow, move->startCol,
                                   move->endRow, move->endCol)) {
        if (moving->type == PAWN) {
            executePawnCapture(board, move);
        }
        else if (moving->type == ANTEATER) {
            executeAnteaterCapture(board, move);
        }
        else {
            executeCapture(board, move);
        }

        if (board->board[move->endRow][move->endCol] != NULL) {
            board->board[move->endRow][move->endCol]->moved = 1;
        }

        return;
    }

    if (moving->vtable->canMove(board, moving,
                                move->startRow, move->startCol,
                                move->endRow, move->endCol)) {

        board->board[move->endRow][move->endCol] = moving;

        moving->pos.row = move->endRow;
        moving->pos.col = move->endCol;
        moving->moved = 1;

        board->board[move->startRow][move->startCol] = NULL;

        // promotion
        if (moving->type == PAWN) {
            if ((moving->color == WHITE && move->endRow == 7) ||
                (moving->color == BLACK && move->endRow == 0)) {
                promotion(board, (Pos){move->endRow, move->endCol}, QUEEN);
            }
        }

        return;
    }
}

<<<<<<< HEAD
void recordMove(Move move, const char *filename) {
    FILE *fp = fopen(filename, "a");
    if (!fp) return;

    fprintf(fp, "%c%d %c%d\n",
            'A' + move.startCol, move.startRow + 1,
            'A' + move.endCol, move.endRow + 1);

    fclose(fp);
}

<<<<<<< HEAD
void executeCapture(Board* board, Move *move) {
=======
void executeCapture(Board *board, Move *move) {
>>>>>>> 6870849266b26b7f87d1d1cebfa9af8006b93cf7
    move->capture = 1;

    board->board[move->endRow][move->endCol] =
        board->board[move->startRow][move->startCol];

    if (board->board[move->endRow][move->endCol] != NULL) {
        board->board[move->endRow][move->endCol]->pos.row = move->endRow;
        board->board[move->endRow][move->endCol]->pos.col = move->endCol;
        board->board[move->endRow][move->endCol]->moved = 1;
    }

    board->board[move->startRow][move->startCol] = NULL;
}

<<<<<<< HEAD
void executePawnCapture(Board* board, Move *move) {
=======
void executePawnCapture(Board *board, Move *move) {
>>>>>>> 6870849266b26b7f87d1d1cebfa9af8006b93cf7
    move->capture = 1;

    board->board[move->endRow][move->endCol] =
        board->board[move->startRow][move->startCol];

    if (board->board[move->endRow][move->endCol] != NULL) {
        board->board[move->endRow][move->endCol]->pos.row = move->endRow;
        board->board[move->endRow][move->endCol]->pos.col = move->endCol;
        board->board[move->endRow][move->endCol]->moved = 1;
    }

    board->board[move->startRow][move->startCol] = NULL;
}

<<<<<<< HEAD
void executeAnteaterCapture(Board* board, Move *move) {
    Piece moving = board->board[move->startRow][move->startCol];
=======
void executeAnteaterCapture(Board *board, Move *move) 
=======
void executeCapture(Board *board, Move *move)
>>>>>>> 024457faadb29d965b6a79b21d1d09dce7f33a10
{
    move->capture = 1;

    Piece *moving = board->board[move->startRow][move->startCol];
>>>>>>> 6870849266b26b7f87d1d1cebfa9af8006b93cf7

    board->board[move->endRow][move->endCol] = moving;

    if (moving != NULL) {
        moving->pos.row = move->endRow;
        moving->pos.col = move->endCol;
        moving->moved = 1;
    }

    board->board[move->startRow][move->startCol] = NULL;
}

void executePawnCapture(Board *board, Move *move)
{
    move->capture = 1;

    Piece *moving = board->board[move->startRow][move->startCol];

    board->board[move->endRow][move->endCol] = moving;

    if (moving != NULL) {
        moving->pos.row = move->endRow;
        moving->pos.col = move->endCol;
        moving->moved = 1;
    }

    board->board[move->startRow][move->startCol] = NULL;
}

void executeAnteaterCapture(Board *board, Move *move)
{
    Piece *moving = board->board[move->startRow][move->startCol];

    int mr = move->endRow - move->startRow;
    int mc = move->endCol - move->startCol;

    int rowStep = 0;
    int colStep = 0;

    if (mr != 0)
        rowStep = (mr > 0) ? 1 : -1;

    if (mc != 0)
        colStep = (mc > 0) ? 1 : -1;

    int r = move->startRow + rowStep;
    int c = move->startCol + colStep;

    while (r != move->endRow || c != move->endCol) {
        board->board[r][c] = NULL; // remove pawns
        r += rowStep;
        c += colStep;
    }

    board->board[move->endRow][move->endCol] = moving;

    if (moving != NULL) {
        moving->pos.row = move->endRow;
        moving->pos.col = move->endCol;
        moving->moved = 1;
    }

    board->board[move->startRow][move->startCol] = NULL;

    move->capture = 1;
}

<<<<<<< HEAD
void executeEnPassant(Board* board, Move *move)
=======
void executeEnPassant(Board *board, Move *move)
>>>>>>> 6870849266b26b7f87d1d1cebfa9af8006b93cf7
{
    Piece *moving = board->board[move->startRow][move->startCol];

    board->board[move->endRow][move->endCol] = moving;

    if (moving != NULL) {
        moving->pos.row = move->endRow;
        moving->pos.col = move->endCol;
        moving->moved = 1;
    }

    board->board[move->startRow][move->endCol] = NULL;
    board->board[move->startRow][move->startCol] = NULL;

    move->capture = 1;
    move->enPassant = 1;
}

<<<<<<< HEAD
void executeCastle(Board* board, Move *move)
=======
void executeCastle(Board *board, Move *move)
>>>>>>> 6870849266b26b7f87d1d1cebfa9af8006b93cf7
{
    int row = move->startRow;

    int kingStartCol = move->startCol;
    int kingEndCol;
    int rookStartCol;
    int rookEndCol;

    if (move->endCol > move->startCol) {
        // kingside
        kingEndCol = kingStartCol + 2;
        rookStartCol = 9;      // your right rook is at far right
        rookEndCol = kingEndCol - 1;
    }
    else {
        // queenside
        kingEndCol = kingStartCol - 2;
        rookStartCol = 0;
        rookEndCol = kingEndCol + 1;
    }

    Piece *king = board->board[row][kingStartCol];
    Piece *rook = board->board[row][rookStartCol];

    board->board[row][kingEndCol] = king;
    board->board[row][kingStartCol] = NULL;

    board->board[row][rookEndCol] = rook;
    board->board[row][rookStartCol] = NULL;

    if (king != NULL) {
        king->pos.row = row;
        king->pos.col = kingEndCol;
        king->moved = 1;
    }

    if (rook != NULL) {
        rook->pos.row = row;
        rook->pos.col = rookEndCol;
        rook->moved = 1;
    }

    move->endCol = kingEndCol;
    move->castle = 1;
}

void copyBoard(Board *dest, Board *src)
{
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 10; c++) {
            dest->board[r][c] = src->board[r][c];
        }
    }
<<<<<<< HEAD
}

int legalMove(Board *board, Move *move, Color turn, Move lastMove)
{
    Piece *moving = board->board[move->startRow][move->startCol];

    if (moving == NULL || moving->type == EMPTY || moving->vtable == NULL){
        return 0;
    }

    if (moving->color != turn){
        return 0;
    }

    Board testBoard;
    copyBoard(&testBoard, board);

    Move testMove = *move;

    executeMove(&testBoard, &testMove, lastMove);

    if (testBoard.board[move->endRow][move->endCol] == NULL ||
        testBoard.board[move->endRow][move->endCol]->color != turn) { //check to see if move didn't happen
        return 0;
    }

    // if the original square hasn't change
    if (move->startRow != move->endRow || move->startCol != move->endCol) {
        if (testBoard.board[move->startRow][move->startCol] != NULL &&
            testBoard.board[move->startRow][move->startCol]->type == moving->type &&
            testBoard.board[move->startRow][move->startCol]->color == moving->color) {
            return 0;
        }
    }

    if (kingCheck(&testBoard, turn)) { //if the king will be in check
        return 0;
    }

    return 1;
}

<<<<<<< HEAD
int possibleMove(Board* board, Color turn, Move lastMove)
=======
int possibleMove(Board *board, Color turn, Move lastMove)
>>>>>>> 6870849266b26b7f87d1d1cebfa9af8006b93cf7
{
    for (int sr = 0; sr < 8; sr++){
        for (int sc = 0; sc < 10; sc++){
            if (board->board[sr][sc] == NULL ||
                board->board[sr][sc]->type == EMPTY ||
                board->board[sr][sc]->color != turn){
                continue;
            }
<<<<<<< HEAD
            for (int er = 0; er < 8; er++){
                for (int ec = 0; ec < 10; ec++){
                    Move move;
                    move.startRow = sr;
                    move.startCol = sc;
                    move.endRow = er;
                    move.endCol = ec;
                    move.capture = 0;
                    move.enPassant = 0;
                    move.castle = 0;

                    if (legalMove(board, &move, turn, lastMove)) {
=======

            for (int er = 0; er < BOARD_HEIGHT; er++)
            {
                for (int ec = 0; ec < BOARD_WIDTH; ec++)
                {
                    Move move = {sr, sc, er, ec, 0, 0, 0};

                    if (legalMove(board, &move, turn, lastMove))
                    {
>>>>>>> a6adb5723f847994783f2624c0bc2ebc71500f20
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

<<<<<<< HEAD

int checkCheckmate(Board *board, Color turn, Move lastMove)
{
    if (!kingCheck(board, turn)) {return 0;}

    if (possibleMove(board, turn, lastMove)) {return 0;}

    return 1;
}

int checkStalemate(Board *board, Color turn, Move lastMove){
    if (kingCheck(board, turn)) {return 0;}

    if (possibleMove(board, turn, lastMove)) {return 0;}

    return 1;
}
/* already made*/
=======
/* already made
int checkCheckmate(Board *board, Color turn, Move lastMove)
{
    if (!kingCheck(board, turn)) {return 0;}

    if (possibleMove(board, turn, lastMove)) {return 0;}

    return 1;
}

int checkStalemate(Board *board, Color turn, Move lastMove){
    if (kingCheck(board, turn)) {return 0;}

    if (possibleMove(board, turn, lastMove)) {return 0;}

    return 1;

}
*/
>>>>>>> a6adb5723f847994783f2624c0bc2ebc71500f20
=======
}
>>>>>>> 024457faadb29d965b6a79b21d1d09dce7f33a10

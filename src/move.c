#include <stdio.h>
#include "move.h"
#include "board.h"
#include "pieces.h"

static void promoteOnBoard(Board *board, Pos pos, Rank newRank)
{
    Piece *piece = board->board[pos.row][pos.col];

    if (piece == NULL) {
        return;
    }

    setType(piece, newRank);
    piece->vtable = getVtable(newRank);
}

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
                promoteOnBoard(board, (Pos){move->endRow, move->endCol}, QUEEN);
            }
        }

        return;
    }
}

void executeCapture(Board *board, Move *move)
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

void executeEnPassant(Board *board, Move *move)
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

void executeCastle(Board *board, Move *move)
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
}

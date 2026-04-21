#include <stdio.h>
#include "move.h"

void executeMove(Piece board[8][10], Move *move, Move lastMove)
{
    Piece moving = board[move->startRow][move->startCol];

    if (moving.type == EMPTY || moving.vtable == NULL) {
        return;
    }

    move->capture = 0;
    move->enPassant = 0;
    move->castle = 0;
    move->promotion = 0;

    // castling
    if (moving.type == KING && kingCanCastle(board, &moving,
        move->startRow, move->startCol, move->endRow, move->endCol)) {
        executeCastle(board, move);
        return;
    }

    // en passant
    if (moving.type == PAWN && pawnCanEnPassant(board, &moving, move->startRow, move->startCol, move->endRow, move->endCol, &lastMove)) {
        executeEnPassant(board, move);

        int lastRow = (board[move->endRow][move->endCol].color == White) ? BOARD_HEIGHT - 1 : 0;
        if (move->endRow == lastRow) {
            move->promotion = 1;
            if (move->promotionType == EMPTY) {
                move->promotionType = QUEEN;
            }
            promotePawn(board, move->endRow, move->endCol, move->promotionType);
        }
        return;
    }

    if (moving.vtable->canCapture(board, &moving, move->startRow, move->startCol, move->endRow, move->endCol)) {
        if (moving.type == PAWN) {
            executePawnCapture(board, move);
        }
        else if (moving.type == ANTEATER) {
            executeAnteaterCapture(board, move);
        }
        else {
            executeCapture(board, move);
        }

        board[move->endRow][move->endCol].moved = 1;

        int lastRow = (board[move->endRow][move->endCol].color == White) ? BOARD_HEIGHT - 1 : 0;
        if (board[move->endRow][move->endCol].type == PAWN &&
            move->endRow == lastRow) {
            move->promotion = 1;
            if (move->promotionType == EMPTY) {
                move->promotionType = QUEEN;
            }
            promotePawn(board, move->endRow, move->endCol, move->promotionType);
        }
        return;
    }

    if (moving.vtable->canMove(board, &moving, move->startRow, move->startCol, move->endRow, move->endCol)) {
        board[move->endRow][move->endCol] = moving;
        board[move->endRow][move->endCol].pos.row = move->endRow;
        board[move->endRow][move->endCol].pos.col = move->endCol;
        board[move->endRow][move->endCol].moved = 1;

        board[move->startRow][move->startCol].img = NULL;
        board[move->startRow][move->startCol].type = EMPTY;
        board[move->startRow][move->startCol].vtable = NULL;
        board[move->startRow][move->startCol].pos.row = move->startRow;
        board[move->startRow][move->startCol].pos.col = move->startCol;
        board[move->startRow][move->startCol].moved = 0;

        int lastRow = (board[move->endRow][move->endCol].color == White) ? BOARD_HEIGHT - 1 : 0;
        if (board[move->endRow][move->endCol].type == PAWN &&
            move->endRow == lastRow) {
            move->promotion = 1;
            if (move->promotionType == EMPTY) {
                move->promotionType = QUEEN;
            }
            promotePawn(board, move->endRow, move->endCol, move->promotionType);
        }
    }
}

void recordMove (Move move, const char *filename){
    FILE *fp = fopen(filename, "a");
    if (!fp) return;

    fprintf(fp, "%c%d %c%d\n", 'A' + move.startCol, move.startRow + 1, 'A' + move.endCol, move.endRow + 1);

    fclose(fp);
}

void executeCapture(Piece board[8][10], Move *move) {
    move->capture = 1;

    board[move->endRow][move->endCol] = board[move->startRow][move->startCol];
    board[move->endRow][move->endCol].pos.row = move->endRow;
    board[move->endRow][move->endCol].pos.col = move->endCol;

    board[move->startRow][move->startCol].img = NULL;
    board[move->startRow][move->startCol].type = EMPTY;
    board[move->startRow][move->startCol].vtable = NULL;
    board[move->startRow][move->startCol].pos.row = move->startRow;
    board[move->startRow][move->startCol].pos.col = move->startCol;
}

void executePawnCapture(Piece board[8][10], Move *move) {
    move->capture = 1;

    board[move->endRow][move->endCol] = board[move->startRow][move->startCol];
    board[move->endRow][move->endCol].pos.row = move->endRow;
    board[move->endRow][move->endCol].pos.col = move->endCol;

    board[move->startRow][move->startCol].img = NULL;
    board[move->startRow][move->startCol].type = EMPTY;
    board[move->startRow][move->startCol].vtable = NULL;
    board[move->startRow][move->startCol].pos.row = move->startRow;
    board[move->startRow][move->startCol].pos.col = move->startCol;
}

void executeAnteaterCapture(Piece board[8][10], Move *move) {
    Piece moving = board[move->startRow][move->startCol];

    int mr = move->endRow - move->startRow;
    int mc = move->endCol - move->startCol;

    int rowStep = 0;
    int colStep = 0;

    if (mr != 0) {
        rowStep = (mr > 0) ? 1 : -1;
    }
    if (mc != 0) {
        colStep = (mc > 0) ? 1 : -1;
    }

    int r = move->startRow + rowStep;
    int c = move->startCol + colStep;

    while (r != move->endRow || c != move->endCol) { //remove pawns
        board[r][c].type = EMPTY;
        board[r][c].vtable = NULL;
        r += rowStep;
        c += colStep;
    }

    board[move->endRow][move->endCol].img = NULL;
    board[move->endRow][move->endCol].type = EMPTY;
    board[move->endRow][move->endCol].vtable = NULL;
    board[move->endRow][move->endCol].pos.row = move->endRow;
    board[move->endRow][move->endCol].pos.col = move->endCol;

    board[move->endRow][move->endCol] = moving;
    board[move->endRow][move->endCol].pos.row = move->endRow;
    board[move->endRow][move->endCol].pos.col = move->endCol;

    board[move->startRow][move->startCol].img = NULL;
    board[move->startRow][move->startCol].type = EMPTY;
    board[move->startRow][move->startCol].vtable = NULL;
    board[move->startRow][move->startCol].pos.row = move->startRow;
    board[move->startRow][move->startCol].pos.col = move->startCol;

    move->capture = 1;
}

void executeEnPassant(Piece board[8][10], Move *move)
{
    Piece moving = board[move->startRow][move->startCol];

    board[move->endRow][move->endCol] = moving;
    board[move->endRow][move->endCol].pos.row = move->endRow;
    board[move->endRow][move->endCol].pos.col = move->endCol;
    board[move->endRow][move->endCol].moved = 1;

    board[move->startRow][move->endCol].img = NULL; //removes pawn
    board[move->startRow][move->endCol].type = EMPTY;
    board[move->startRow][move->endCol].vtable = NULL;
    board[move->startRow][move->endCol].pos.row = move->startRow;
    board[move->startRow][move->endCol].pos.col = move->endCol;
    board[move->startRow][move->endCol].moved = 0;

    // clears square
    board[move->startRow][move->startCol].img = NULL;
    board[move->startRow][move->startCol].type = EMPTY;
    board[move->startRow][move->startCol].vtable = NULL;
    board[move->startRow][move->startCol].pos.row = move->startRow;
    board[move->startRow][move->startCol].pos.col = move->startCol;
    board[move->startRow][move->startCol].moved = 0;

    move->capture = 1;
    move->enPassant = 1;
}

void executeCastle(Piece board[8][10], Move *move)
{
    Piece king = board[move->startRow][move->startCol];
    int row = move->startRow;

    board[move->endRow][move->endCol] = king;
    board[move->endRow][move->endCol].pos.row = move->endRow;
    board[move->endRow][move->endCol].pos.col = move->endCol;
    board[move->endRow][move->endCol].moved = 1;

    board[move->startRow][move->startCol].img = NULL;
    board[move->startRow][move->startCol].type = EMPTY;
    board[move->startRow][move->startCol].vtable = NULL;
    board[move->startRow][move->startCol].pos.row = move->startRow;
    board[move->startRow][move->startCol].pos.col = move->startCol;
    board[move->startRow][move->startCol].moved = 0;

    if (move->endCol > move->startCol) {
        // kingside
        int rookStartCol = BOARD_WIDTH - 1;
        int rookEndCol = move->endCol - 1;

        board[row][rookEndCol] = board[row][rookStartCol];
        board[row][rookEndCol].pos.row = row;
        board[row][rookEndCol].pos.col = rookEndCol;
        board[row][rookEndCol].moved = 1;

        board[row][rookStartCol].img = NULL;
        board[row][rookStartCol].type = EMPTY;
        board[row][rookStartCol].vtable = NULL;
        board[row][rookStartCol].pos.row = row;
        board[row][rookStartCol].pos.col = rookStartCol;
        board[row][rookStartCol].moved = 0;
    } else {
        // queenside
        int rookStartCol = 0;
        int rookEndCol = move->endCol + 1;

        board[row][rookEndCol] = board[row][rookStartCol];
        board[row][rookEndCol].pos.row = row;
        board[row][rookEndCol].pos.col = rookEndCol;
        board[row][rookEndCol].moved = 1;

        board[row][rookStartCol].img = NULL;
        board[row][rookStartCol].type = EMPTY;
        board[row][rookStartCol].vtable = NULL;
        board[row][rookStartCol].pos.row = row;
        board[row][rookStartCol].pos.col = rookStartCol;
        board[row][rookStartCol].moved = 0;
    }

    move->capture = 0;
    move->castle = 1;
}

/*
void promotePawn(Piece board[8][10], int row, int col, Rank newType)
{
  
}
*/


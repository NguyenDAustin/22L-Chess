#include <stdio.h>
#include "move.h"

void executeMove(Piece board[8][10], Move *move){
    Piece moving = board[move->startRow][move->startCol];

    if (moving.type == EMPTY || moving.vtable == NULL) {
        return;
    }

    if (moving.vtable->canCapture(board, &moving, move->startRow, move->startCol, move->endRow, move->endCol)) { //check capture

        if (moving.type == PAWN) {
            executePawnCapture(board, move);
        }
        else if (moving.type == ANTEATER) {
            executeAnteaterCapture(board, move);
        }
        else {
            executeCapture(board, move);
        }
        return;
    }

    if (moving.vtable->canMove(board, &moving, move->startRow, move->startCol, move->endRow, move->endCol)) { //check move

        board[move->endRow][move->endCol] = moving;

        board[move->startRow][move->startCol].type = EMPTY;
        board[move->startRow][move->startCol].vtable = NULL;

        move->capture = 0;
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

    board[move->startRow][move->startCol].type = EMPTY;
    board[move->startRow][move->startCol].vtable = NULL;
}

void executePawnCapture(Piece board[8][10], Move *move) {
    move->capture = 1;

    board[move->endRow][move->endCol] = board[move->startRow][move->startCol];

    board[move->startRow][move->startCol].type = EMPTY;
    board[move->startRow][move->startCol].vtable = NULL;
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

    board[move->endRow][move->endCol].type = EMPTY; //removes last pawn
    board[move->endRow][move->endCol].vtable = NULL;

    board[move->endRow][move->endCol] = moving; //move anteater

    board[move->startRow][move->startCol].type = EMPTY; //makes the start square empty
    board[move->startRow][move->startCol].vtable = NULL;

    move->capture = 1;
}

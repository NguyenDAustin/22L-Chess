#include <stdio.h>
#include <string.h>
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
        board[move->endRow][move->endCol].pos.row = move->endRow;
        board[move->endRow][move->endCol].pos.col = move->endCol;

        board[move->startRow][move->startCol].img = NULL;
        board[move->startRow][move->startCol].type = EMPTY;
        board[move->startRow][move->startCol].vtable = NULL;
        board[move->startRow][move->startCol].pos.row = move->startRow;
        board[move->startRow][move->startCol].pos.col = move->startCol;

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

int isLegalMove(Piece board[8][10], Move *move, Color player) {
    int sr = move->startRow, sc = move->startCol;
    int er = move->endRow,   ec = move->endCol;

    if (sr < 0 || sr >= 8 || sc < 0 || sc >= 10) return 0;
    if (er < 0 || er >= 8 || ec < 0 || ec >= 10) return 0;
    if (sr == er && sc == ec) return 0;

    Piece *p = &board[sr][sc];
    if (p->type == EMPTY || p->vtable == NULL) return 0;
    if (p->color != player) return 0;

    /* Target blocker: empty means we need canMove; enemy means canCapture;
     * friendly means illegal outright. */
    Piece *dst = &board[er][ec];
    if (dst->type != EMPTY && dst->color == player) return 0;

    if (dst->type == EMPTY) {
        return p->vtable->canMove ? p->vtable->canMove(board, p, sr, sc, er, ec) : 0;
    } else {
        return p->vtable->canCapture ? p->vtable->canCapture(board, p, sr, sc, er, ec) : 0;
    }
}

static void findKing(Piece board[8][10], Color player, int *outR, int *outC) {
    *outR = -1;
    *outC = -1;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 10; c++) {
            if (board[r][c].type == KING && board[r][c].color == player) {
                *outR = r;
                *outC = c;
                return;
            }
        }
    }
}

int isInCheck(Piece board[8][10], Color player) {
    int kr, kc;
    findKing(board, player, &kr, &kc);
    if (kr < 0) return 0;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 10; c++) {
            Piece *attacker = &board[r][c];
            if (attacker->type == EMPTY || attacker->color == player) continue;
            if (attacker->vtable && attacker->vtable->canCapture &&
                attacker->vtable->canCapture(board, attacker, r, c, kr, kc)) {
                return 1;
            }
        }
    }
    return 0;
}

/* Try a move on a copy of the board; return 1 if it leaves `player` not in check. */
static int moveLeavesKingSafe(Piece board[8][10], Color player, int sr, int sc, int er, int ec) {
    Piece saved_start = board[sr][sc];
    Piece saved_end   = board[er][ec];

    board[er][ec] = saved_start;
    board[er][ec].pos.row = er;
    board[er][ec].pos.col = ec;
    board[sr][sc].type = EMPTY;
    board[sr][sc].vtable = NULL;
    board[sr][sc].img = NULL;

    int safe = !isInCheck(board, player);

    board[sr][sc] = saved_start;
    board[er][ec] = saved_end;
    return safe;
}

int hasAnyLegalMove(Piece board[8][10], Color player) {
    for (int sr = 0; sr < 8; sr++) {
        for (int sc = 0; sc < 10; sc++) {
            Piece *p = &board[sr][sc];
            if (p->type == EMPTY || p->color != player || p->vtable == NULL) continue;

            for (int er = 0; er < 8; er++) {
                for (int ec = 0; ec < 10; ec++) {
                    if (sr == er && sc == ec) continue;
                    Move m = { sr, sc, er, ec, 0 };
                    if (!isLegalMove(board, &m, player)) continue;
                    if (moveLeavesKingSafe(board, player, sr, sc, er, ec)) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

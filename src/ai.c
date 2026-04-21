/*(easy mode)*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ai.h"
#include "pieces.h"
#include "move.h"


#define MAX_MOVES 512

/* Simulate the move on a copy; return 1 if it does NOT leave `player` in check. */
static int moveIsSafe(Piece board[8][10], Color player, int sr, int sc, int er, int ec)
{
    Piece saved_start = board[sr][sc];
    Piece saved_end   = board[er][ec];

    board[er][ec] = saved_start;
    board[er][ec].pos.row = er;
    board[er][ec].pos.col = ec;
    board[sr][sc].type = EMPTY;
    board[sr][sc].vtable = NULL;
    board[sr][sc].img = NULL;

    int safe = !kingCheck(board, player);

    board[sr][sc] = saved_start;
    board[er][ec] = saved_end;
    return safe;
}

static int generateLegalMoves(Piece board[8][10], Color player, Move out[], int maxMoves)
{
    int count = 0;
    for (int sr = 0; sr < 8 && count < maxMoves; sr++) {
        for (int sc = 0; sc < 10 && count < maxMoves; sc++) {
            Piece *p = &board[sr][sc];
            if (p->type == EMPTY || p->color != player || p->vtable == NULL) continue;

            for (int er = 0; er < 8 && count < maxMoves; er++) {
                for (int ec = 0; ec < 10 && count < maxMoves; ec++) {
                    if (sr == er && sc == ec) continue;
                    Move m = { sr, sc, er, ec, 0, 0, 0 };
                    Move noLast = {0};
                    if (!legalMove(board, &m, player, noLast)) continue;
                    if (!moveIsSafe(board, player, sr, sc, er, ec)) continue;
                    out[count++] = m;
                }
            }
        }
    }
    return count;
}

int aiSelectMove(Piece board[8][10], Color player, Move *out)
{
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned)time(NULL));
        seeded = 1;
    }

    Move moves[MAX_MOVES];
    int n = generateLegalMoves(board, player, moves, MAX_MOVES);
    if (n <= 0) return 0;

    *out = moves[rand() % n];
    return 1;
}

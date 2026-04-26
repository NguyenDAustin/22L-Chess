/*(easy mode)*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ai.h"
#include "pieces.h"
#include "move.h"

#define MAX_MOVES 512

static Difficulty currentDifficulty = EASY;

/* Simulate the move on a copy; return 1 if it does NOT leave `player` in check. */
static int moveIsSafe(Board *board, Color player, int sr, int sc, int er, int ec)
{
    Piece saved_start = board->board[sr][sc];
    Piece saved_end = board->board[er][ec];

    board->board[er][ec] = saved_start;
    board->board[er][ec].pos.row = er;
    board->board[er][ec].pos.col = ec;
    board->board[sr][sc].type = EMPTY;
    board->board[sr][sc].vtable = NULL;
    board->board[sr][sc].img = NULL;

    int safe = !kingCheck(board, player);

    board->board[sr][sc] = saved_start;
    board->board[er][ec] = saved_end;
    return safe;
}

static int generateLegalMoves(Board *board, Color player, Move out[], int maxMoves)
{
    int count = 0;
    for (int sr = 0; sr < 8 && count < maxMoves; sr++)
    {
        for (int sc = 0; sc < 10 && count < maxMoves; sc++)
        {
            Piece *p = &board[sr][sc];
            if (p->type == EMPTY || p->color != player || p->vtable == NULL)
                continue;

            for (int er = 0; er < 8 && count < maxMoves; er++)
            {
                for (int ec = 0; ec < 10 && count < maxMoves; ec++)
                {
                    if (sr == er && sc == ec)
                        continue;
                    Move m = {sr, sc, er, ec, 0, 0, 0};
                    Move noLast = {0};
                    if (!legalMove(board, &m, player, noLast))
                        continue;
                    if (!moveIsSafe(board, player, sr, sc, er, ec))
                        continue;
                    out[count++] = m;
                }
            }
        }
    }
    return count;
}

void setDifficulty(Difficulty d)
{
    currentDifficulty = d;
}

int easySelectMove(Board *board, Color player, Move *out)
{
    Move moves[MAX_MOVES];
    int n = generateLegalMoves(board, player, moves, MAX_MOVES);
    if (n <= 0)
    {
        return 0;
    }
    *out = moves[rand() % n];
    return 1;
}

int mediumSelectMove(Board *board, Color player, Move *out)
{
    Move moves[MAX_MOVES];
    int n = generateLegalMoves(board, player, moves, MAX_MOVES);
    if (n <= 0)
    {
        return 0;
    }

    int bestVal = -1, bestIndex = -1;
    for (int i = 0; i < n; i++)
    {
        Piece *target = &board->board[moves[i].er][moves[i].ec];
        if (target->type != EMPTY && target->color != player)
        {
            int val = target->value;
            if (val > bestVal)
            {
                bestVal = val;
                bestIndex = i;
            }
        }
    }

    *out = (bestIndex >= 0) ? moves[bestIndex] : moves[rand() % n];
    return 1;
}

static Piece applyMove(Board *board, Move *m)
{
    Piece displaced = board->board[m->er][m->ec];

    board->board[m->er][m->ec] = board->board[m->sr][m->sc];
    board->board[m->er][m->ec].pos.row = m->er;
    board->board[m->er][m->ec].pos.col = m->ec;
    board->board[m->sr][m->sc].type = EMPTY;
    board->board[m->sr][m->sc].vtable = NULL;
    board->board[m->sr][m->sc].img = NULL;

    return displaced;
}

static void undoMove(Board *board, Move *m, Piece displaced)
{
    board->board[m->sr][m->sc] = board->board[m->er][m->ec];
    board->board[m->sr][m->sc].pos.row = m->sr;
    board->board[m->sr][m->sc].pos.col = m->sc;
    board->board[m->er][m->ec] = displaced;
}

static int evalBoard(Board *board, Color aiColor)
{
    int score = 0;
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 10; c++)
        {
            Piece *p = &board->board[r][c];
            if (p->type == EMPTY)
                continue;
            int val = p->value;
            score += (p->color == aiColor) ? val : -val;
        }
    return score;
}

static int minimax(Board *board, Color aiColor, int depth, int alpha, int beta, int maximizing)
{
    if (depth == 0)
    {
        return evalBoard(board, aiColor);
    }

    Color current = maximizing ? aiColor : (1 - aiColor);
    Move moves[MAX_MOVES];
    int n = generateLegalMoves(board, current, moves, MAX_MOVES);
    if (n <= 0)
    {
        return evalBoard(board, aiColor);
    }

    if (maximizing)
    {
        int best = -9999;
        for (int i = 0; i < n; i++)
        {
            Piece displaced = applyMove(board, &moves[i]);
            int val = minimax(board, aiColor, depth - 1, alpha, beta, 0);
            undoMove(board, &moves[i], displaced);
            if (val > best)
            {
                best = val;
            }
            if (val > alpha)
            {
                alpha = val;
            }
            if (beta <= alpha)
            {
                break;
            }
        }
        return best;
    }
    else
    {
        int best = 9999;
        for (int i = 0; i < n; i++)
        {
            Piece displaced = applyMove(board, &moves[i]);
            int val = minimax(board, aiColor, depth - 1, alpha, beta, 1);
            undoMove(board, &moves[i], displaced);
            if (val < best)
            {
                best = val;
            }
            if (val < beta)
            {
                beta = val;
            }
            if (beta <= alpha)
            {
                break;
            }
        }
        return best;
    }
}

int hardSelectMove(Board *board, Color player, Move *out)
{
    Move moves[MAX_MOVES];
    int n = generateLegalMoves(board, player, moves, MAX_MOVES);
    if (n <= 0)
    {
        return 0;
    }

    int bestVal = -9999, bestIndex = -1;
    for (int i = 0; i < n; i++)
    {
        Piece displaced = applyMove(board, &moves[i]);
        int val = minimax(board, player, 3, -9999, 9999, 0, player);
        undoMove(board, &moves[i], displaced);
        if (val > bestVal)
        {
            bestVal = val;
            bestIndex = i;
        }
    }
    *out = moves[bestIndex];
    return 1;
}

int aiSelectMove(Piece board[8][10], Color player, Move *out)
{
    static int seeded = 0;
    if (!seeded)
    {
        srand((unsigned)time(NULL));
        seeded = 1;
    }

    switch (currentDifficulty)
    {
    case EASY:
    {
        return easySelectMove(board, player, out);
    }
    case MEDIUM:
    {
        return mediumSelectMove(board, player, out);
    }
    case HARD:
    {
        return hardSelectMove(board, player, out);
    }
    default:
    {
        return easySelectMove(board, player, out);
    }
    }
}

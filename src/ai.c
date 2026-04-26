/* ai.c */
#include <limits.h>
#include <stdlib.h>
#include <time.h>

#include "ai.h"
#include "board.h"
#include "pieces.h"

#define MAX_MOVES 512
#define HARD_SEARCH_DEPTH 2
#define CHECK_BONUS 75

static void seedRandomOnce(void);
static void rebuildBoardView(Piece state[8][10], Board *view);
static void copyState(Piece dest[8][10], Piece src[8][10]);
static void clearSquare(Piece *piece, int row, int col);
static int pieceValue(Rank type);
static int isCandidateMoveInView(Board *view, Color player, int sr, int sc, int er, int ec);
static void applyMoveToState(Piece state[8][10], Move move);
static int isMoveLegalInView(Piece state[8][10], Board *view, Color player, Move move);
static int generateAiLegalMoves(Piece state[8][10], Color player, Move out[], int maxMoves);
static int evaluateBoard(Piece state[8][10], Color player);
static int scoreMoveHeuristically(Piece state[8][10], Color player, Move move);
static int selectRandomMove(Move moves[], int count, Move *out);
static int selectBestImmediateMove(Piece state[8][10], Color player, Move moves[], int count, Move *out);
static int minimax(Piece state[8][10], Color rootPlayer, Color sideToMove, int depth, int alpha, int beta);
static int selectBestMinimaxMove(Piece state[8][10], Color player, Move moves[], int count, Move *out);

static void seedRandomOnce(void)
{
    static int seeded = 0;
    if (!seeded)
    {
        srand((unsigned)time(NULL));
        seeded = 1;
    }
}

static void rebuildBoardView(Piece state[8][10], Board *view)
{
    int r;
    int c;

    for (r = 0; r < BOARD_HEIGHT; r++)
    {
        for (c = 0; c < BOARD_WIDTH; c++)
        {
            if (state[r][c].type == EMPTY || state[r][c].vtable == NULL)
            {
                view->board[r][c] = NULL;
            }
            else
            {
                state[r][c].pos.row = r;
                state[r][c].pos.col = c;
                view->board[r][c] = &state[r][c];
            }
        }
    }
}

static void copyState(Piece dest[8][10], Piece src[8][10])
{
    int r;
    int c;

    for (r = 0; r < BOARD_HEIGHT; r++)
    {
        for (c = 0; c < BOARD_WIDTH; c++)
        {
            dest[r][c] = src[r][c];
        }
    }
}

static void clearSquare(Piece *piece, int row, int col)
{
    piece->img = NULL;
    piece->color = WHITE;
    piece->type = EMPTY;
    piece->vtable = NULL;
    piece->value = 0;
    piece->pos.row = row;
    piece->pos.col = col;
    piece->moved = 0;
}

static int pieceValue(Rank type)
{
    switch (type)
    {
    case PAWN:
        return 100;
    case KNIGHT:
        return 320;
    case BISHOP:
        return 330;
    case ROOK:
        return 500;
    case ANTEATER:
        return 525;
    case QUEEN:
        return 900;
    case KING:
        return 20000;
    default:
        return 0;
    }
}

static int isCandidateMoveInView(Board *view, Color player, int sr, int sc, int er, int ec)
{
    Piece *moving;
    Piece *target;

    if (sr == er && sc == ec)
    {
        return 0;
    }

    moving = view->board[sr][sc];
    target = view->board[er][ec];

    if (moving == NULL || moving->type == EMPTY || moving->color != player || moving->vtable == NULL)
    {
        return 0;
    }

    if (target != NULL && target->type == KING)
    {
        return 0;
    }

    if (moving->type == KING && kingCanCastle(view, moving, sr, sc, er, ec))
    {
        return 1;
    }

    if (target != NULL)
    {
        return moving->vtable->canCapture(view, moving, sr, sc, er, ec);
    }

    return moving->vtable->canMove(view, moving, sr, sc, er, ec);
}

static void applyMoveToState(Piece state[8][10], Move move)
{
    Piece moving = state[move.startRow][move.startCol];
    int isCastle = 0;

    if (moving.type == KING && move.startRow == move.endRow &&
        abs(move.endCol - move.startCol) == 2)
    {
        isCastle = 1;
    }

    if (isCastle)
    {
        int rookStartCol;
        int rookEndCol;
        Piece rook;

        state[move.endRow][move.endCol] = moving;
        state[move.endRow][move.endCol].pos.row = move.endRow;
        state[move.endRow][move.endCol].pos.col = move.endCol;
        state[move.endRow][move.endCol].moved = 1;
        clearSquare(&state[move.startRow][move.startCol], move.startRow, move.startCol);

        if (move.endCol > move.startCol)
        {
            rookStartCol = BOARD_WIDTH - 1;
            rookEndCol = move.endCol - 1;
        }
        else
        {
            rookStartCol = 0;
            rookEndCol = move.endCol + 1;
        }

        rook = state[move.startRow][rookStartCol];
        state[move.startRow][rookEndCol] = rook;
        state[move.startRow][rookEndCol].pos.row = move.startRow;
        state[move.startRow][rookEndCol].pos.col = rookEndCol;
        state[move.startRow][rookEndCol].moved = 1;
        clearSquare(&state[move.startRow][rookStartCol], move.startRow, rookStartCol);
        return;
    }

    state[move.endRow][move.endCol] = moving;
    state[move.endRow][move.endCol].pos.row = move.endRow;
    state[move.endRow][move.endCol].pos.col = move.endCol;
    state[move.endRow][move.endCol].moved = 1;
    clearSquare(&state[move.startRow][move.startCol], move.startRow, move.startCol);

    if (state[move.endRow][move.endCol].type == PAWN)
    {
        if ((state[move.endRow][move.endCol].color == WHITE && move.endRow == BOARD_HEIGHT - 1) ||
            (state[move.endRow][move.endCol].color == BLACK && move.endRow == 0))
        {
            state[move.endRow][move.endCol].type = QUEEN;
            state[move.endRow][move.endCol].vtable = getVtable(QUEEN);
        }
    }
}

static int isMoveLegalInView(Piece state[8][10], Board *view, Color player, Move move)
{
    Piece nextState[8][10];
    Board nextView;

    if (!isCandidateMoveInView(view, player, move.startRow, move.startCol, move.endRow, move.endCol))
    {
        return 0;
    }

    copyState(nextState, state);
    applyMoveToState(nextState, move);
    rebuildBoardView(nextState, &nextView);

    return !kingCheck(&nextView, player);
}

static int generateAiLegalMoves(Piece state[8][10], Color player, Move out[], int maxMoves)
{
    int count = 0;
    int sr;
    int sc;
    int er;
    int ec;
    Board view;

    rebuildBoardView(state, &view);

    for (sr = 0; sr < BOARD_HEIGHT && count < maxMoves; sr++)
    {
        for (sc = 0; sc < BOARD_WIDTH && count < maxMoves; sc++)
        {
            if (state[sr][sc].type == EMPTY || state[sr][sc].color != player || state[sr][sc].vtable == NULL)
            {
                continue;
            }

            for (er = 0; er < BOARD_HEIGHT && count < maxMoves; er++)
            {
                for (ec = 0; ec < BOARD_WIDTH && count < maxMoves; ec++)
                {
                    Move move;

                    move.startRow = sr;
                    move.startCol = sc;
                    move.endRow = er;
                    move.endCol = ec;
                    move.capture = 0;
                    move.enPassant = 0;
                    move.castle = 0;

                    if (isMoveLegalInView(state, &view, player, move))
                    {
                        out[count++] = move;
                    }
                }
            }
        }
    }

    return count;
}

static int evaluateBoard(Piece state[8][10], Color player)
{
    Board view;
    int score = 0;
    int r;
    int c;
    Color enemy = (player == WHITE) ? BLACK : WHITE;

    rebuildBoardView(state, &view);

    for (r = 0; r < BOARD_HEIGHT; r++)
    {
        for (c = 0; c < BOARD_WIDTH; c++)
        {
            Piece *piece = view.board[r][c];
            if (piece == NULL || piece->type == EMPTY)
            {
                continue;
            }

            if (piece->color == player)
            {
                score += pieceValue(piece->type);
            }
            else
            {
                score -= pieceValue(piece->type);
            }
        }
    }

    if (kingCheck(&view, enemy))
    {
        score += CHECK_BONUS;
    }
    if (kingCheck(&view, player))
    {
        score -= CHECK_BONUS;
    }

    return score;
}

static int scoreMoveHeuristically(Piece state[8][10], Color player, Move move)
{
    Piece nextState[8][10];
    int score;
    Rank captured = state[move.endRow][move.endCol].type;

    copyState(nextState, state);
    applyMoveToState(nextState, move);
    score = evaluateBoard(nextState, player);

    if (captured != EMPTY)
    {
        score += pieceValue(captured);
    }

    return score;
}

static int selectRandomMove(Move moves[], int count, Move *out)
{
    if (count <= 0)
    {
        return 0;
    }

    *out = moves[rand() % count];
    return 1;
}

static int selectBestImmediateMove(Piece state[8][10], Color player, Move moves[], int count, Move *out)
{
    int i;
    int bestScore = INT_MIN;
    int bestCount = 0;
    Move bestMoves[MAX_MOVES];

    if (count <= 0)
    {
        return 0;
    }

    for (i = 0; i < count; i++)
    {
        int score = scoreMoveHeuristically(state, player, moves[i]);

        if (score > bestScore)
        {
            bestScore = score;
            bestMoves[0] = moves[i];
            bestCount = 1;
        }
        else if (score == bestScore && bestCount < MAX_MOVES)
        {
            bestMoves[bestCount++] = moves[i];
        }
    }

    *out = bestMoves[rand() % bestCount];
    return 1;
}

static int minimax(Piece state[8][10], Color rootPlayer, Color sideToMove, int depth, int alpha, int beta)
{
    Move moves[MAX_MOVES];
    int moveCount;
    int i;
    Color enemy = (sideToMove == WHITE) ? BLACK : WHITE;

    if (depth == 0)
    {
        return evaluateBoard(state, rootPlayer);
    }

    moveCount = generateAiLegalMoves(state, sideToMove, moves, MAX_MOVES);
    if (moveCount == 0)
    {
        Board view;
        rebuildBoardView(state, &view);

        if (kingCheck(&view, sideToMove))
        {
            return (sideToMove == rootPlayer) ? -100000 : 100000;
        }
        return 0;
    }

    if (sideToMove == rootPlayer)
    {
        int best = INT_MIN;
        for (i = 0; i < moveCount; i++)
        {
            Piece nextState[8][10];
            int score;

            copyState(nextState, state);
            applyMoveToState(nextState, moves[i]);
            score = minimax(nextState, rootPlayer, enemy, depth - 1, alpha, beta);
            if (score > best)
            {
                best = score;
            }
            if (score > alpha)
            {
                alpha = score;
            }
            if (beta <= alpha)
            {
                break;
            }
        }
        return best;
    }

    {
        int best = INT_MAX;
        for (i = 0; i < moveCount; i++)
        {
            Piece nextState[8][10];
            int score;

            copyState(nextState, state);
            applyMoveToState(nextState, moves[i]);
            score = minimax(nextState, rootPlayer, enemy, depth - 1, alpha, beta);
            if (score < best)
            {
                best = score;
            }
            if (score < beta)
            {
                beta = score;
            }
            if (beta <= alpha)
            {
                break;
            }
        }
        return best;
    }
}

static int selectBestMinimaxMove(Piece state[8][10], Color player, Move moves[], int count, Move *out)
{
    int i;
    int bestScore = INT_MIN;
    int bestCount = 0;
    Move bestMoves[MAX_MOVES];
    Color enemy = (player == WHITE) ? BLACK : WHITE;

    if (count <= 0)
    {
        return 0;
    }

    for (i = 0; i < count; i++)
    {
        Piece nextState[8][10];
        int score;

        copyState(nextState, state);
        applyMoveToState(nextState, moves[i]);
        score = minimax(nextState, player, enemy, HARD_SEARCH_DEPTH, INT_MIN, INT_MAX);

        if (score > bestScore)
        {
            bestScore = score;
            bestMoves[0] = moves[i];
            bestCount = 1;
        }
        else if (score == bestScore && bestCount < MAX_MOVES)
        {
            bestMoves[bestCount++] = moves[i];
        }
    }

    *out = bestMoves[rand() % bestCount];
    return 1;
}

int aiSelectMoveAtDifficulty(Piece board[8][10], Color player, AIDifficulty difficulty, Move *out)
{
    Move moves[MAX_MOVES];
    int moveCount;

    seedRandomOnce();

    moveCount = generateAiLegalMoves(board, player, moves, MAX_MOVES);
    if (moveCount <= 0)
    {
        return 0;
    }

    switch (difficulty)
    {
    case AI_EASY:
        return selectRandomMove(moves, moveCount, out);
    case AI_HARD:
        return selectBestMinimaxMove(board, player, moves, moveCount, out);
    case AI_MEDIUM:
    default:
        return selectBestImmediateMove(board, player, moves, moveCount, out);
    }
}

int aiSelectMove(Piece board[8][10], Color player, Move *out)
{
    return aiSelectMoveAtDifficulty(board, player, AI_MEDIUM, out);
}

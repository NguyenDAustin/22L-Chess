#include <stdio.h>

#include "ai.h"
#include "board.h"
#include "move.h"

static void clearState(Piece state[BOARD_HEIGHT][BOARD_WIDTH], Board *board)
{
    for (int row = 0; row < BOARD_HEIGHT; row++)
    {
        for (int col = 0; col < BOARD_WIDTH; col++)
        {
            state[row][col].img = NULL;
            state[row][col].color = WHITE;
            state[row][col].type = EMPTY;
            state[row][col].vtable = NULL;
            state[row][col].value = 0;
            state[row][col].pos.row = row;
            state[row][col].pos.col = col;
            state[row][col].moved = 0;
            board->board[row][col] = NULL;
        }
    }
}

static void placePiece(Piece state[BOARD_HEIGHT][BOARD_WIDTH], Board *board,
                       int row, int col, Color color, Rank type)
{
    state[row][col].img = NULL;
    state[row][col].color = color;
    state[row][col].type = type;
    state[row][col].vtable = getVtable(type);
    state[row][col].value = 0;
    state[row][col].pos.row = row;
    state[row][col].pos.col = col;
    state[row][col].moved = 0;
    board->board[row][col] = &state[row][col];
}

static int testCpuMovesOwnPiece(void)
{
    Piece state[BOARD_HEIGHT][BOARD_WIDTH];
    Board board;
    Move cpuMove;
    Move lastMove = {-1, -1, -1, -1, 0, 0, 0};
    Piece *moving;

    clearState(state, &board);
    placePiece(state, &board, 0, 5, BLACK, KING);
    placePiece(state, &board, 1, 0, BLACK, PAWN);
    placePiece(state, &board, 7, 5, WHITE, KING);
    placePiece(state, &board, 6, 0, WHITE, PAWN);

    if (!aiSelectMoveAtDifficulty(state, BLACK, AI_EASY, &cpuMove))
    {
        fprintf(stderr, "CPU did not select a legal move.\n");
        return 1;
    }

    moving = board.board[cpuMove.startRow][cpuMove.startCol];
    if (moving == NULL || moving->color != BLACK)
    {
        fprintf(stderr, "CPU selected a non-black piece.\n");
        return 1;
    }

    executeMove(&board, &cpuMove, lastMove);

    if (board.board[cpuMove.endRow][cpuMove.endCol] != moving ||
        board.board[cpuMove.endRow][cpuMove.endCol]->color != BLACK)
    {
        fprintf(stderr, "CPU move was not applied to the selected black piece.\n");
        return 1;
    }

    if (board.board[cpuMove.startRow][cpuMove.startCol] != NULL)
    {
        fprintf(stderr, "CPU move left the starting square occupied.\n");
        return 1;
    }

    printf("CPU moved black piece from %c%d to %c%d.\n",
           'A' + cpuMove.startCol, cpuMove.startRow + 1,
           'A' + cpuMove.endCol, cpuMove.endRow + 1);
    return 0;
}

static int testCpuCannotCaptureKing(void)
{
    Piece state[BOARD_HEIGHT][BOARD_WIDTH];
    Board board;
    Move cpuMove;
    Move illegalKingCapture = {1, 5, 6, 5, 0, 0, 0};

    clearState(state, &board);
    placePiece(state, &board, 0, 0, BLACK, KING);
    placePiece(state, &board, 1, 5, BLACK, QUEEN);
    placePiece(state, &board, 6, 5, WHITE, KING);
    placePiece(state, &board, 7, 0, WHITE, ROOK);

    if (!aiSelectMoveAtDifficulty(state, BLACK, AI_MEDIUM, &cpuMove))
    {
        fprintf(stderr, "CPU did not select a legal move while king capture was available.\n");
        return 1;
    }

    if (cpuMove.startRow == 1 && cpuMove.startCol == 5 &&
        cpuMove.endRow == 6 && cpuMove.endCol == 5)
    {
        fprintf(stderr, "CPU selected a king capture.\n");
        return 1;
    }

    executeMove(&board, &illegalKingCapture, (Move){-1, -1, -1, -1, 0, 0, 0});
    if (board.board[6][5] == NULL || board.board[6][5]->type != KING ||
        board.board[6][5]->color != WHITE)
    {
        fprintf(stderr, "executeMove allowed a king capture.\n");
        return 1;
    }

    printf("CPU did not select or execute a king capture.\n");
    return 0;
}

int main(void)
{
    if (testCpuMovesOwnPiece() != 0)
    {
        return 1;
    }

    if (testCpuCannotCaptureKing() != 0)
    {
        return 1;
    }

    return 0;
}

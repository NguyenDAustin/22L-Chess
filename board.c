#include <stdio.h>
#include <stdlib.h>

// #include "chess.h"
#include "board.h"

// row 0 - 7: rank 1 - 8
// col 0 - 9: file a - j
Piece *board[NUM_ROWS][NUM_COLS];

#define PTS_KING 0
#define PTS_QUEEN 9
#define PTS_ANTEATER 100 // change it later
#define PTS_BISHOP 3
#define PTS_KNIGHT 3
#define PTS_ROOK 5
#define PTS_ANT 1

static void placePiece(int row, int col, Color color, Rank rank, int pointVal)
{
    Piece *p = malloc(sizeof(Piece));
    if (p == NULL)
    {
        fprintf(stderr, "board.c: malloc failed\n");
        return;
    }

    p->color = color;
    p->rank = rank;
    p->pointVal = pointVal;

    board[row][col] = p;
}

void initBoard()
{
    for (int i = 0; i < NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_COLS; j++)
        {
            board[i][j] = NULL;
        }
    }

    placePiece(0, 0, WHITE, ROOK, PTS_ROOK);
    placePiece(0, 1, WHITE, KNIGHT, PTS_KNIGHT);
    placePiece(0, 2, WHITE, BISHOP, PTS_BISHOP);
    placePiece(0, 3, WHITE, ANTEATER, PTS_ANTEATER);
    placePiece(0, 4, WHITE, QUEEN, PTS_QUEEN);
    placePiece(0, 5, WHITE, KING, PTS_KING);
    placePiece(0, 6, WHITE, ANTEATER, PTS_ANTEATER);
    placePiece(0, 7, WHITE, BISHOP, PTS_BISHOP);
    placePiece(0, 8, WHITE, KNIGHT, PTS_KNIGHT);
    placePiece(0, 9, WHITE, ROOK, PTS_ROOK);

    // white ants
    for (int c = 0; c < NUM_COLS; c++)
        placePiece(1, c, WHITE, ANT, PTS_ANT);

    // black ants
    for (int c = 0; c < NUM_COLS; c++)
        placePiece(6, c, BLACK, ANT, PTS_ANT);

    placePiece(7, 0, BLACK, ROOK, PTS_ROOK);
    placePiece(7, 1, BLACK, KNIGHT, PTS_KNIGHT);
    placePiece(7, 2, BLACK, BISHOP, PTS_BISHOP);
    placePiece(7, 3, BLACK, ANTEATER, PTS_ANTEATER);
    placePiece(7, 4, BLACK, QUEEN, PTS_QUEEN);
    placePiece(7, 5, BLACK, KING, PTS_KING);
    placePiece(7, 6, BLACK, ANTEATER, PTS_ANTEATER);
    placePiece(7, 7, BLACK, BISHOP, PTS_BISHOP);
    placePiece(7, 8, BLACK, KNIGHT, PTS_KNIGHT);
    placePiece(7, 9, BLACK, ROOK, PTS_ROOK);
}

void displayBoard()
{
    // wait
}

Piece *getPiece(int row, int col)
{
    if (!isInBounds(row, col))
    {
        fprintf(stderr, "board.c: getPiece(%d, %d) out of bounds\n", row, col);
        return NULL;
    }
    return board[row][col];
}

void setPiece(int row, int col, Piece *piece)
{
    if (!isInBounds(row, col))
    {
        fprintf(stderr, "board.c: setPiece(%d, %d) out of bounds\n", row, col);
        return;
    }
    board[row][col] = piece;
}

int isInBounds(int row, int col)
{
    return row >= 0 && row < NUM_ROWS && col >= 0 && col < NUM_COLS;
}

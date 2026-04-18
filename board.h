#ifndef BOARD_H
#define BOARD_H

#define NUM_ROWS 8
#define NUM_COLS 10

extern Piece *board[NUM_ROWS][NUM_COLS];

void initBoard();
void displayBoard();
Piece *getPiece(int row, int col);
void setPiece(int row, int col, Piece *piece);
int isInBounds(int row, int col);

#endif
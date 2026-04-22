/*
 * test_boarddisplay.c
 *
 * Minimal test for the board display.  Builds a board with the initial
 * Anteater Chess setup, applies one modification (white pawn F2 -> F4),
 * and prints the result in the ASCII format shown in the project spec.
 */

#include <stdio.h>
#include <string.h>

#include "pieces.h"

/* Symbol used when printing a piece (e.g. "wR", "bP", "  "). */
static void pieceSymbol(const Piece *p, char out[3])
{
    if (p == NULL || p->type == EMPTY) {
        out[0] = ' '; out[1] = ' '; out[2] = '\0';
        return;
    }
    out[0] = (p->color == WHITE) ? 'w' : 'b';
    switch (p->type) {
        case KING:     out[1] = 'K'; break;
        case QUEEN:    out[1] = 'Q'; break;
        case ANTEATER: out[1] = 'A'; break;
        case BISHOP:   out[1] = 'B'; break;
        case KNIGHT:   out[1] = 'N'; break;
        case ROOK:     out[1] = 'R'; break;
        case PAWN:     out[1] = 'P'; break;
        default:       out[1] = '?'; break;
    }
    out[2] = '\0';
}

static void displayBoardAscii(Piece board[8][10])
{
    for (int r = 7; r >= 0; r--) {
        printf("    +");
        for (int c = 0; c < 10; c++) printf("----+");
        printf("\n  %d |", r + 1);
        for (int c = 0; c < 10; c++) {
            char sym[3];
            pieceSymbol(&board[r][c], sym);
            printf(" %s |", sym);
        }
        printf("\n");
    }
    printf("    +");
    for (int c = 0; c < 10; c++) printf("----+");
    printf("\n      ");
    for (int c = 0; c < 10; c++) printf("  %c  ", 'A' + c);
    printf("\n");
}

static void clearBoard(Piece board[8][10])
{
    memset(board, 0, sizeof(Piece) * 8 * 10);
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 10; c++)
            board[r][c].type = EMPTY;
}

static void placePiece(Piece board[8][10], int r, int c, Color color, Rank type)
{
    board[r][c].color  = color;
    board[r][c].type   = type;
    board[r][c].value  = 0;
    board[r][c].img    = NULL;
    board[r][c].vtable = NULL;
}

static void setupInitial(Piece board[8][10])
{
    /* Back ranks: R N B A Q K A B N R */
    Rank order[10] = { ROOK, KNIGHT, BISHOP, ANTEATER, QUEEN,
                       KING, ANTEATER, BISHOP, KNIGHT, ROOK };
    for (int c = 0; c < 10; c++) {
        placePiece(board, 0, c, WHITE, order[c]);
        placePiece(board, 7, c, BLACK, order[c]);
        placePiece(board, 1, c, WHITE, PAWN);
        placePiece(board, 6, c, BLACK, PAWN);
    }
}

int main(void)
{
    Piece board[8][10];
    clearBoard(board);
    setupInitial(board);

    printf("Initial Anteater Chess board:\n\n");
    displayBoardAscii(board);

    /* Modify: move white pawn from F2 (row 1, col 5) to F4 (row 3, col 5). */
    board[3][5] = board[1][5];
    board[1][5].type  = EMPTY;
    board[1][5].color = WHITE;

    printf("\nAfter moving white pawn F2 -> F4:\n\n");
    displayBoardAscii(board);

    return 0;
}

/*
 * test_rulecheck.c
 *
 * Minimal unit test for the rules checker.  Sets up a near-empty board,
 * places a single piece, and verifies that the rule-check function
 * returns the expected value for a known legal and a known illegal move.
 */

#include <stdio.h>
#include <string.h>

#include "pieces.h"

static int tests_run    = 0;
static int tests_passed = 0;

static void check(const char *name, int expected, int actual)
{
    tests_run++;
    if (expected == actual) {
        tests_passed++;
        printf("  PASS: %s (expected=%d, actual=%d)\n",
               name, expected, actual);
    } else {
        printf("  FAIL: %s (expected=%d, actual=%d)\n",
               name, expected, actual);
    }
}

static void clearBoard(Piece board[8][10])
{
    memset(board, 0, sizeof(Piece) * 8 * 10);
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 10; c++) {
            board[r][c].type = EMPTY;
        }
    }
}

int main(void)
{
    Piece board[8][10];
    clearBoard(board);

    /* Place a white rook at (0,0) */
    Piece rook;
    rook.color = White;
    rook.type  = ROOK;
    rook.value = 5;
    rook.img   = NULL;
    rook.vtable = NULL;
    board[0][0] = rook;

    printf("Running rule-check tests...\n");

    /* Rook should be able to slide along row 0 to an empty square. */
    check("rook: legal horizontal slide",
          1, rookCanMove(board, &rook, 0, 0, 0, 5));

    /* Rook should not be allowed to move diagonally. */
    check("rook: illegal diagonal move",
          0, rookCanMove(board, &rook, 0, 0, 3, 3));

    printf("\nSummary: %d/%d tests passed\n", tests_passed, tests_run);

    return (tests_passed == tests_run) ? 0 : 1;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "pieces.h"
#include "board.h"
#include "move.h"
#include "input.h"
#include "ai.h"
#include "undo.h"
#include "log.h"

void displayBoard(Piece board[8][10]);
int isInCheckmate(Piece board[8][10], Color player);
int isInStalemate(Piece board[8][10], Color player);

#define LOG_FILENAME "chess_game.log"
#define INPUT_BUF 128

static Color chooseHumanColor(void);
static const char *colorName(Color c);
static void initializeRawBoard(Piece board[8][10]);
static Move makeMoveFromInput(Pos start, Pos end);
static void printPrompt(Color turn, int isHumanTurn);

int main(void) {
    Piece board[8][10];
    Color humanColor;
    Color cpuColor;
    Color currentTurn;
    int moveNumber = 1;
    int running = 1;

    initializeRawBoard(board);
    newLog(LOG_FILENAME);

    humanColor = chooseHumanColor();
    cpuColor = (humanColor == WHITE) ? BLACK : WHITE;
    currentTurn = WHITE;

    printf("\nAnteater Chess started.\n");
    printf("Human: %s\n", colorName(humanColor));
    printf("CPU:   %s\n\n", colorName(cpuColor));

    displayBoard(board);

    while (running) {
        if (currentTurn == humanColor) {
            char line[INPUT_BUF];

            printPrompt(currentTurn, 1);

            if (!fgets(line, sizeof(line), stdin)) {
                printf("Input error. Exiting.\n");
                break;
            }

            if (!isValid(line)) {
                printf("Invalid input. Try again.\n");
                continue;
            }

            if (inputIsQuit()) {
                printf("Quitting game.\n");
                break;
            }

            if (inputIsUndo()) {
                /*
                 * This assumes your undo module is finished and can reverse
                 * the most recent move on the board.
                 */
                printf("Undo requested.\n");
                /* undoLast(board); */
                displayBoard(board);
                currentTurn = (currentTurn == WHITE) ? BLACK : WHITE;
                continue;
            }

            if (inputHasMove()) {
                Pos start = inputGetStartPos();
                Pos end = inputGetEndPos();
                Move move = makeMoveFromInput(start, end);

                /*
                 * executeMove currently works on Piece board[8][10].
                 * After your headers are cleaned, this call is fine.
                 */
                executeMove(board, &move);

                addMove(
                    LOG_FILENAME,
                    moveNumber,
                    colorName(currentTurn),
                    "Move",
                    "from",
                    "to"
                );

                moveNumber++;
                displayBoard(board);
            } else if (inputIsCastleKingSide()) {
                printf("Kingside castle requested.\n");
                addCastle(LOG_FILENAME, moveNumber, colorName(currentTurn), 1);
                moveNumber++;
                displayBoard(board);
            } else if (inputIsCastleQueenSide()) {
                printf("Queenside castle requested.\n");
                addCastle(LOG_FILENAME, moveNumber, colorName(currentTurn), 0);
                moveNumber++;
                displayBoard(board);
            } else {
                printf("No recognized action from input.\n");
                continue;
            }
        } else {
            Move cpuMove;

            printPrompt(currentTurn, 0);

            if (!aiSelectMove(currentTurn, &cpuMove)) {
                printf("CPU has no legal move.\n");
                break;
            }

            executeMove(board, &cpuMove);

            addMove(
                LOG_FILENAME,
                moveNumber,
                colorName(currentTurn),
                "CPU Move",
                "from",
                "to"
            );

            moveNumber++;
            displayBoard(board);
        }

        {
            Color nextPlayer = (currentTurn == WHITE) ? BLACK : WHITE;

            if (isInCheckmate(board, nextPlayer)) {
                printf("\nCheckmate. %s wins.\n", colorName(currentTurn));
                running = 0;
            } else if (isInStalemate(board, nextPlayer)) {
                printf("\nStalemate.\n");
                running = 0;
            } else {
                currentTurn = nextPlayer;
            }
        }
    }

    printf("Game over.\n");
    return 0;
}

static Color chooseHumanColor(void) {
    char line[32];

    while (1) {
        printf("Choose your color (W/B): ");

        if (!fgets(line, sizeof(line), stdin)) {
            return WHITE;
        }

        if (toupper((unsigned char)line[0]) == 'W') {
            return WHITE;
        }
        if (toupper((unsigned char)line[0]) == 'B') {
            return BLACK;
        }

        printf("Please enter W or B.\n");
    }
}

static const char *colorName(Color c) {
    return (c == WHITE) ? "White" : "Black";
}

static void initializeRawBoard(Piece board[8][10]) {
    int r, c;

    for (r = 0; r < 8; r++) {
        for (c = 0; c < 10; c++) {
            board[r][c].img = NULL;
            board[r][c].color = WHITE;
            board[r][c].type = EMPTY;
            board[r][c].vtable = NULL;
            board[r][c].value = 0;
            board[r][c].pos.row = r;
            board[r][c].pos.col = c;
        }
    }

    }

static Move makeMoveFromInput(Pos start, Pos end) {
    Move m;
    m.startRow = start.row;
    m.startCol = start.col;
    m.endRow = end.row;
    m.endCol = end.col;
    m.capture = 0;
    return m;
}

static void printPrompt(Color turn, int isHumanTurn) {
    if (isHumanTurn) {
        printf("\n%s to move. Enter move: ", colorName(turn));
    } else {
        printf("\n%s CPU is thinking...\n", colorName(turn));
    }
}
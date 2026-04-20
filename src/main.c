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

static void displayBoard(Piece board[8][10]);
static int isInCheckmate(Piece board[8][10], Color player);
static int isInStalemate(Piece board[8][10], Color player);
static void setupInitialBoard(Piece board[8][10]);

#define LOG_FILENAME "chess_game.log"
#define INPUT_BUF 128

static Color chooseHumanColor(void);
static const char *colorName(Color c);
static Move makeMoveFromInput(Pos start, Pos end);
static void printPrompt(Color turn, int isHumanTurn);
static void squareName(int row, int col, char out[4]);

int main(void) {
    Piece board[8][10];
    Color humanColor;
    Color cpuColor;
    Color currentTurn;
    int moveNumber = 1;
    int running = 1;

    setupInitialBoard(board);
    undoSet();
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
                printf("Invalid input. Try again. (example: E2 E4)\n");
                continue;
            }

            if (inputIsQuit()) {
                printf("Quitting game.\n");
                logQuit(LOG_FILENAME, moveNumber, colorName(currentTurn));
                break;
            }

            if (inputIsUndo()) {
                list top;
                if (undoPop(&top)) {
                    undo(&top, board);
                    list top2;
                    if (undoPop(&top2)) {
                        undo(&top2, board);
                        printf("Undid last two moves (yours and CPU's).\n");
                    } else {
                        printf("Undid last move.\n");
                        currentTurn = (currentTurn == WHITE) ? BLACK : WHITE;
                    }
                    displayBoard(board);
                } else {
                    printf("Nothing to undo.\n");
                }
                continue;
            }

            if (inputHasMove()) {
                Pos start = inputGetStartPos();
                Pos end = inputGetEndPos();
                Move move = makeMoveFromInput(start, end);

                if (!isLegalMove(board, &move, currentTurn)) {
                    printf("Illegal move for that piece. Try again.\n");
                    continue;
                }

                list rec;
                rec.moveNUm = moveNumber;
                rec.move = move;
                rec.movPiece = board[move.startRow][move.startCol];
                rec.capturedPiece = board[move.endRow][move.endCol];

                executeMove(board, &move);

                if (isInCheck(board, currentTurn)) {
                    /* Reverted: leaves own king in check — undo. */
                    board[move.startRow][move.startCol] = rec.movPiece;
                    board[move.endRow][move.endCol]   = rec.capturedPiece;
                    printf("That move leaves your king in check. Try again.\n");
                    continue;
                }

                undoPush(&rec);

                char from[4], to[4];
                squareName(move.startRow, move.startCol, from);
                squareName(move.endRow,   move.endCol,   to);
                if (move.capture) {
                    addCapture(LOG_FILENAME, moveNumber, colorName(currentTurn),
                               "piece", from, "piece", to);
                } else {
                    addMove(LOG_FILENAME, moveNumber, colorName(currentTurn),
                            "Move", from, to);
                }

                moveNumber++;
                displayBoard(board);
            } else if (inputIsCastleKingSide() || inputIsCastleQueenSide()) {
                printf("Castling not yet implemented. Try a normal move.\n");
                continue;
            } else {
                printf("No recognized action from input.\n");
                continue;
            }
        } else {
            Move cpuMove;

            printPrompt(currentTurn, 0);

            if (!aiSelectMove(board, currentTurn, &cpuMove)) {
                printf("CPU has no legal move.\n");
                break;
            }

            list rec;
            rec.moveNUm = moveNumber;
            rec.move = cpuMove;
            rec.movPiece = board[cpuMove.startRow][cpuMove.startCol];
            rec.capturedPiece = board[cpuMove.endRow][cpuMove.endCol];

            executeMove(board, &cpuMove);
            undoPush(&rec);

            char from[4], to[4];
            squareName(cpuMove.startRow, cpuMove.startCol, from);
            squareName(cpuMove.endRow,   cpuMove.endCol,   to);
            printf("CPU plays %s -> %s\n", from, to);
            addMove(LOG_FILENAME, moveNumber, colorName(currentTurn),
                    "CPU Move", from, to);

            moveNumber++;
            displayBoard(board);
        }

        {
            Color nextPlayer = (currentTurn == WHITE) ? BLACK : WHITE;

            if (isInCheckmate(board, nextPlayer)) {
                printf("\nCheckmate. %s wins.\n", colorName(currentTurn));
                logCheckmate(LOG_FILENAME, moveNumber,
                             colorName(currentTurn), colorName(nextPlayer));
                running = 0;
            } else if (isInStalemate(board, nextPlayer)) {
                printf("\nStalemate.\n");
                logDraw(LOG_FILENAME, moveNumber);
                running = 0;
            } else {
                if (isInCheck(board, nextPlayer)) {
                    printf("%s is in check.\n", colorName(nextPlayer));
                    logCheck(LOG_FILENAME, moveNumber, colorName(nextPlayer));
                }
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

static void placePiece(Piece board[8][10], int r, int c, Color color, Rank type) {
    board[r][c] = createPiece(color, type);
    board[r][c].pos.row = r;
    board[r][c].pos.col = c;
    board[r][c].img = NULL;
}

static void setupInitialBoard(Piece board[8][10]) {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 10; c++) {
            board[r][c].img    = NULL;
            board[r][c].color  = WHITE;
            board[r][c].type   = EMPTY;
            board[r][c].vtable = NULL;
            board[r][c].value  = 0;
            board[r][c].pos.row = r;
            board[r][c].pos.col = c;
        }
    }

    /* Back ranks (Anteater Chess 10-wide): R N B A Q K A B N R */
    Rank order[10] = { ROOK, KNIGHT, BISHOP, ANTEATER, QUEEN,
                       KING, ANTEATER, BISHOP, KNIGHT, ROOK };
    for (int c = 0; c < 10; c++) {
        placePiece(board, 0, c, WHITE, order[c]);
        placePiece(board, 7, c, BLACK, order[c]);
        placePiece(board, 1, c, WHITE, PAWN);
        placePiece(board, 6, c, BLACK, PAWN);
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
        printf("\n%s to move. Enter move (e.g. E2 E4, UNDO, QUIT): ", colorName(turn));
    } else {
        printf("\n%s CPU is thinking...\n", colorName(turn));
    }
}

static void squareName(int row, int col, char out[4]) {
    out[0] = (char)('A' + col);
    out[1] = (char)('1' + row);
    out[2] = '\0';
    out[3] = '\0';
}

static char pieceSymbol(const Piece *p) {
    if (p == NULL || p->type == EMPTY) return '.';
    switch (p->type) {
        case KING:     return (p->color == White) ? 'K' : 'k';
        case QUEEN:    return (p->color == White) ? 'Q' : 'q';
        case ANTEATER: return (p->color == White) ? 'A' : 'a';
        case BISHOP:   return (p->color == White) ? 'B' : 'b';
        case KNIGHT:   return (p->color == White) ? 'N' : 'n';
        case ROOK:     return (p->color == White) ? 'R' : 'r';
        case PAWN:     return (p->color == White) ? 'P' : 'p';
        default:       return '?';
    }
}

static void displayBoard(Piece board[8][10]) {
    int r, c;
    printf("\n");
    for (r = 7; r >= 0; r--) {
        printf("  %d ", r + 1);
        for (c = 0; c < 10; c++) {
            printf(" %c", pieceSymbol(&board[r][c]));
        }
        printf("\n");
    }
    printf("    ");
    for (c = 0; c < 10; c++) printf(" %c", 'A' + c);
    printf("\n");
}

static int isInCheckmate(Piece board[8][10], Color player) {
    return isInCheck(board, player) && !hasAnyLegalMove(board, player);
}

static int isInStalemate(Piece board[8][10], Color player) {
    return !isInCheck(board, player) && !hasAnyLegalMove(board, player);
}

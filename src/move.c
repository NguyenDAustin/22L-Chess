#include <stdio.h>
#include "move.h"

void executeMove(Board* board, Move *move, Move lastMove)
{
    Piece moving = board->board->board->board[move->startRow][move->startCol];

    if (moving.type == EMPTY || moving.vtable == NULL) {
        return;
    }

    move->capture = 0;
    move->enPassant = 0;
    move->castle = 0;

    // castling
    if (moving.type == KING && kingCanCastle(board->board, &moving,
        move->startRow, move->startCol, move->endRow, move->endCol)) {
        executeCastle(board->board, move);
        return;
    }

    // en passant
    if (moving.type == PAWN && pawnCanEnPassant(board->board, &moving, move->startRow, move->startCol, move->endRow, move->endCol, &lastMove)) {
        executeEnPassant(board->board, move);
        return;

    }

    if (moving.vtable->canCapture(board->board, &moving, move->startRow, move->startCol, move->endRow, move->endCol)) {
        if (moving.type == PAWN) {
            executePawnCapture(board->board, move);
        }
        else if (moving.type == ANTEATER) {
            executeAnteaterCapture(board->board, move);
        }
        else {
            executeCapture(board->board, move);
        }

        board->board[move->endRow][move->endCol].moved = 1;

        return;
    }

    if (moving.vtable->canMove(board->board, &moving, move->startRow, move->startCol, move->endRow, move->endCol)) {
        board->board[move->endRow][move->endCol] = moving;
        board->board[move->endRow][move->endCol].pos.row = move->endRow;
        board->board[move->endRow][move->endCol].pos.col = move->endCol;
        board->board[move->endRow][move->endCol].moved = 1;

        board->board[move->startRow][move->startCol].img = NULL;
        board->board[move->startRow][move->startCol].type = EMPTY;
        board->board[move->startRow][move->startCol].vtable = NULL;
        board->board[move->startRow][move->startCol].pos.row = move->startRow;
        board->board[move->startRow][move->startCol].pos.col = move->startCol;
        board->board[move->startRow][move->startCol].moved = 0;

        return;
    }

    if (moving.vtable->canMove(board->board, &moving, move->startRow, move->startCol, move->endRow, move->endCol)) { //promotion

    // move the piece first
    board->board[move->endRow][move->endCol] = moving;
    board->board[move->endRow][move->endCol].pos.row = move->endRow;
    board->board[move->endRow][move->endCol].pos.col = move->endCol;
    board->board[move->endRow][move->endCol].moved = 1;

    // clear old square
    board->board[move->startRow][move->startCol].type = EMPTY;
    board->board[move->startRow][move->startCol].vtable = NULL;

    //check promotion
    if (moving.type == PAWN) {
        if ((moving.color == WHITE && move->endRow == BOARD->board_HEIGHT - 1) ||
            (moving.color == BLACK && move->endRow == 0)) {

            promotion(board->board, (Pos){move->endRow, move->endCol}, QUEEN);
        }
    }

    return;
}
}

void recordMove (Move move, const char *filename){
    FILE *fp = fopen(filename, "a");
    if (!fp) return;

    fprintf(fp, "%c%d %c%d\n", 'A' + move.startCol, move.startRow + 1, 'A' + move.endCol, move.endRow + 1);

    fclose(fp);
}

void executeCapture(Board* board, Move *move) {
    move->capture = 1;

    board->board[move->endRow][move->endCol] = board->board[move->startRow][move->startCol];
    board->board[move->endRow][move->endCol].pos.row = move->endRow;
    board->board[move->endRow][move->endCol].pos.col = move->endCol;

    board->board[move->startRow][move->startCol].img = NULL;
    board->board[move->startRow][move->startCol].type = EMPTY;
    board->board[move->startRow][move->startCol].vtable = NULL;
    board->board[move->startRow][move->startCol].pos.row = move->startRow;
    board->board[move->startRow][move->startCol].pos.col = move->startCol;
}

void executePawnCapture(Board* board, Move *move) {
    move->capture = 1;

    board->board[move->endRow][move->endCol] = board->board[move->startRow][move->startCol];
    board->board[move->endRow][move->endCol].pos.row = move->endRow;
    board->board[move->endRow][move->endCol].pos.col = move->endCol;

    board->board[move->startRow][move->startCol].img = NULL;
    board->board[move->startRow][move->startCol].type = EMPTY;
    board->board[move->startRow][move->startCol].vtable = NULL;
    board->board[move->startRow][move->startCol].pos.row = move->startRow;
    board->board[move->startRow][move->startCol].pos.col = move->startCol;
}

void executeAnteaterCapture(Board* board, Move *move) {
    Piece moving = board->board[move->startRow][move->startCol];

    int mr = move->endRow - move->startRow;
    int mc = move->endCol - move->startCol;

    int rowStep = 0;
    int colStep = 0;

    if (mr != 0) {
        rowStep = (mr > 0) ? 1 : -1;
    }
    if (mc != 0) {
        colStep = (mc > 0) ? 1 : -1;
    }

    int r = move->startRow + rowStep;
    int c = move->startCol + colStep;

    while (r != move->endRow || c != move->endCol) { //remove pawns
        board->board[r][c].type = EMPTY;
        board->board[r][c].vtable = NULL;
        r += rowStep;
        c += colStep;
    }

    board->board[move->endRow][move->endCol].img = NULL;
    board->board[move->endRow][move->endCol].type = EMPTY;
    board->board[move->endRow][move->endCol].vtable = NULL;
    board->board[move->endRow][move->endCol].pos.row = move->endRow;
    board->board[move->endRow][move->endCol].pos.col = move->endCol;

    board->board[move->endRow][move->endCol] = moving;
    board->board[move->endRow][move->endCol].pos.row = move->endRow;
    board->board[move->endRow][move->endCol].pos.col = move->endCol;

    board->board[move->startRow][move->startCol].img = NULL;
    board->board[move->startRow][move->startCol].type = EMPTY;
    board->board[move->startRow][move->startCol].vtable = NULL;
    board->board[move->startRow][move->startCol].pos.row = move->startRow;
    board->board[move->startRow][move->startCol].pos.col = move->startCol;

    move->capture = 1;
}

void executeEnPassant(Board* board, Move *move)
{
    Piece moving = board->board[move->startRow][move->startCol];

    board->board[move->endRow][move->endCol] = moving;
    board->board[move->endRow][move->endCol].pos.row = move->endRow;
    board->board[move->endRow][move->endCol].pos.col = move->endCol;
    board->board[move->endRow][move->endCol].moved = 1;

    board->board[move->startRow][move->endCol].img = NULL; //removes pawn
    board->board[move->startRow][move->endCol].type = EMPTY;
    board->board[move->startRow][move->endCol].vtable = NULL;
    board->board[move->startRow][move->endCol].pos.row = move->startRow;
    board->board[move->startRow][move->endCol].pos.col = move->endCol;
    board->board[move->startRow][move->endCol].moved = 0;

    // clears square
    board->board[move->startRow][move->startCol].img = NULL;
    board->board[move->startRow][move->startCol].type = EMPTY;
    board->board[move->startRow][move->startCol].vtable = NULL;
    board->board[move->startRow][move->startCol].pos.row = move->startRow;
    board->board[move->startRow][move->startCol].pos.col = move->startCol;
    board->board[move->startRow][move->startCol].moved = 0;

    move->capture = 1;
    move->enPassant = 1;
}

void executeCastle(Board* board, Move *move)
{
    Piece king = board->board[move->startRow][move->startCol];
    int row = move->startRow;

    board->board[move->endRow][move->endCol] = king;
    board->board[move->endRow][move->endCol].pos.row = move->endRow;
    board->board[move->endRow][move->endCol].pos.col = move->endCol;
    board->board[move->endRow][move->endCol].moved = 1;

    board->board[move->startRow][move->startCol].img = NULL;
    board->board[move->startRow][move->startCol].type = EMPTY;
    board->board[move->startRow][move->startCol].vtable = NULL;
    board->board[move->startRow][move->startCol].pos.row = move->startRow;
    board->board[move->startRow][move->startCol].pos.col = move->startCol;
    board->board[move->startRow][move->startCol].moved = 0;

    if (move->endCol > move->startCol) {
        // kingside
        int rookStartCol = BOARD->board_WIDTH - 1;
        int rookEndCol = move->endCol - 1;

        board->board[row][rookEndCol] = board->board[row][rookStartCol];
        board->board[row][rookEndCol].pos.row = row;
        board->board[row][rookEndCol].pos.col = rookEndCol;
        board->board[row][rookEndCol].moved = 1;

        board->board[row][rookStartCol].img = NULL;
        board->board[row][rookStartCol].type = EMPTY;
        board->board[row][rookStartCol].vtable = NULL;
        board->board[row][rookStartCol].pos.row = row;
        board->board[row][rookStartCol].pos.col = rookStartCol;
        board->board[row][rookStartCol].moved = 0;
    } else {
        // queenside
        int rookStartCol = 0;
        int rookEndCol = move->endCol + 1;

        board->board[row][rookEndCol] = board->board[row][rookStartCol];
        board->board[row][rookEndCol].pos.row = row;
        board->board[row][rookEndCol].pos.col = rookEndCol;
        board->board[row][rookEndCol].moved = 1;

        board->board[row][rookStartCol].img = NULL;
        board->board[row][rookStartCol].type = EMPTY;
        board->board[row][rookStartCol].vtable = NULL;
        board->board[row][rookStartCol].pos.row = row;
        board->board[row][rookStartCol].pos.col = rookStartCol;
        board->board[row][rookStartCol].moved = 0;
    }

    move->capture = 0;
    move->castle = 1;
}

void copyBoard->board(Piece new[8][10], Piece og[8][10])
{
    for (int r = 0; r < BOARD->board_HEIGHT; r++) {
        for (int c = 0; c < BOARD->board_WIDTH; c++) {
            new[r][c] = og[r][c];
        }
    }
}

int legalMove(Board->board *board->board, Move *move, Color turn, Move lastMove)
{
    Piece moving = board->board[move->startRow][move->startCol];

    if (moving.type == EMPTY || moving.vtable == NULL){
        return 0;
    }

    if (moving.color != turn){
        return 0;
    }

    Piece testBoard->board[8][10];
    copyBoard->board(testBoard->board, board->board);

    Move testMove = *move;

    executeMove(testBoard->board, &testMove, lastMove);

    if (testBoard->board[move->endRow][move->endCol].type == EMPTY || testBoard->board[move->endRow][move->endCol].color != turn){ //check to see if move didn't happen
        return 0;
    }

    // if the original square hasn't change
    if (move->startRow != move->endRow || move->startCol != move->endCol) {
        if (testBoard->board[move->startRow][move->startCol].type == moving.type && testBoard->board[move->startRow][move->startCol].color == moving.color) {
            return 0;
        }
    }

    if (kingCheck(testBoard->board, turn)) { //if the king will be in check
        return 0;
    }

    return 1;
}

int possibleMove(Board* board, Color turn, Move lastMove)
{
    for (int sr = 0; sr < BOARD->board_HEIGHT; sr++){
        for (int sc = 0; sc < BOARD->board_WIDTH; sc++){
            if (board->board[sr][sc].type == EMPTY || board->board[sr][sc].color != turn){
                continue;
            }
            for (int er = 0; er < BOARD->board_HEIGHT; er++){
                for (int ec = 0; ec < BOARD->board_WIDTH; ec++){
                    Move move;
                    move.startRow = sr;
                    move.startCol = sc;
                    move.endRow = er;
                    move.endCol = ec;
                    move.capture = 0;
                    move.enPassant = 0;
                    move.castle = 0;

                    if (legalMove(board->board, &move, turn, lastMove)){return 1;}

                }
            }
        }
    }
    return 0;
}

/* already made
int checkCheckmate(Board->board *board->board, Color turn, Move lastMove)
{
    if (!kingCheck(board->board, turn)) {return 0;}

    i;:qf (possibleMove(board->board, turn, lastMove)) {return 0;}

    return 1;
}

int checkStalemate(Board->board *board->board, Color turn, Move lastMove){
    if (kingCheck(board->board, turn)) {return 0;}

    if (possibleMove(board->board, turn, lastMove)) {return 0;}

    return 1;
    
}
*/


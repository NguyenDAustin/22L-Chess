#include <stdio.h>
#include "move.h"
#include "board.h"   // for promotion / Board
#include "pieces.h"  // for piece helpers / kingCheck prototype if declared here

void executeMove(Board *board, Move *move, Move lastMove)
{
    Piece *moving = board->board[move->startRow][move->startCol];

    if (moving == NULL || moving->type == EMPTY || moving->vtable == NULL) {
        return;
    }

    move->capture = 0;
    move->enPassant = 0;
    move->castle = 0;

    // castling
    if (moving->type == KING && kingCanCastle(board, moving,
        move->startRow, move->startCol, move->endRow, move->endCol)) {
        executeCastle(board, move);
        return;
    }

    // en passant
    if (moving->type == PAWN &&
        pawnCanEnPassant(board, moving,
                         move->startRow, move->startCol,
                         move->endRow, move->endCol, &lastMove)) {
        executeEnPassant(board, move);
        return;
    }

    if (moving->vtable->canCapture(board, moving,
                                   move->startRow, move->startCol,
                                   move->endRow, move->endCol)) {
        if (moving->type == PAWN) {
            executePawnCapture(board, move);
        }
        else if (moving->type == ANTEATER) {
            executeAnteaterCapture(board, move);
        }
        else {
            executeCapture(board, move);
        }

        if (board->board[move->endRow][move->endCol] != NULL) {
            board->board[move->endRow][move->endCol]->moved = 1;
        }

        return;
    }

    if (moving->vtable->canMove(board, moving,
                                move->startRow, move->startCol,
                                move->endRow, move->endCol)) {
        board->board[move->endRow][move->endCol] = moving;
        board->board[move->endRow][move->endCol]->pos.row = move->endRow;
        board->board[move->endRow][move->endCol]->pos.col = move->endCol;
        board->board[move->endRow][move->endCol]->moved = 1;

        board->board[move->startRow][move->startCol] = NULL;

        //check promotion
        if (moving->type == PAWN) {
            if ((moving->color == WHITE && move->endRow == 7) ||
                (moving->color == BLACK && move->endRow == 0)) {
                promotion(board, (Pos){move->endRow, move->endCol}, QUEEN);
            }
        }

        return;
    }
}

void recordMove(Move move, const char *filename) {
    FILE *fp = fopen(filename, "a");
    if (!fp) return;

    fprintf(fp, "%c%d %c%d\n",
            'A' + move.startCol, move.startRow + 1,
            'A' + move.endCol, move.endRow + 1);

    fclose(fp);
}

void executeCapture(Board *board, Move *move) {
    move->capture = 1;

    board->board[move->endRow][move->endCol] =
        board->board[move->startRow][move->startCol];

    if (board->board[move->endRow][move->endCol] != NULL) {
        board->board[move->endRow][move->endCol]->pos.row = move->endRow;
        board->board[move->endRow][move->endCol]->pos.col = move->endCol;
        board->board[move->endRow][move->endCol]->moved = 1;
    }

    board->board[move->startRow][move->startCol] = NULL;
}

void executePawnCapture(Board *board, Move *move) {
    move->capture = 1;

    board->board[move->endRow][move->endCol] =
        board->board[move->startRow][move->startCol];

    if (board->board[move->endRow][move->endCol] != NULL) {
        board->board[move->endRow][move->endCol]->pos.row = move->endRow;
        board->board[move->endRow][move->endCol]->pos.col = move->endCol;
        board->board[move->endRow][move->endCol]->moved = 1;
    }

    board->board[move->startRow][move->startCol] = NULL;
}

void executeAnteaterCapture(Board *board, Move *move) 
{
    Piece *moving = board->board[move->startRow][move->startCol];

<<<<<<< HEAD
    // Move anteater onto captured pawn's square
=======
    int mr = move->endRow - move->startRow;
    int mc = move->endCol - move->startCol;

    int rowStep = 0;
    int colStep = 0;

    if (mr != 0)
    {
        rowStep = (mr > 0) ? 1 : -1;
    }
    if (mc != 0)
    {
        colStep = (mc > 0) ? 1 : -1;
    }

    int r = move->startRow + rowStep;
    int c = move->startCol + colStep;

    while (r != move->endRow || c != move->endCol)
    { // remove pawns
        board->board[r][c]->type = EMPTY;
        board->board[r][c]->vtable = NULL;
        r += rowStep;
        c += colStep;
    }

>>>>>>> a6adb5723f847994783f2624c0bc2ebc71500f20
    board->board[move->endRow][move->endCol] = moving;

    if (moving != NULL) {
        moving->pos.row = move->endRow;
        moving->pos.col = move->endCol;
        moving->moved = 1;
    }

    // Clear old square
    board->board[move->startRow][move->startCol] = NULL;

    move->capture = 1;
}

void executeEnPassant(Board *board, Move *move)
{
    Piece *moving = board->board[move->startRow][move->startCol];

    board->board[move->endRow][move->endCol] = moving;
    if (board->board[move->endRow][move->endCol] != NULL) {
        board->board[move->endRow][move->endCol]->pos.row = move->endRow;
        board->board[move->endRow][move->endCol]->pos.col = move->endCol;
        board->board[move->endRow][move->endCol]->moved = 1;
    }

    board->board[move->startRow][move->endCol] = NULL; //removes pawn

    // clears square
    board->board[move->startRow][move->startCol] = NULL;

    move->capture = 1;
    move->enPassant = 1;
}

void executeCastle(Board *board, Move *move)
{
    Piece *king = board->board[move->startRow][move->startCol];
    int row = move->startRow;

    board->board[move->endRow][move->endCol] = king;
    if (board->board[move->endRow][move->endCol] != NULL) {
        board->board[move->endRow][move->endCol]->pos.row = move->endRow;
        board->board[move->endRow][move->endCol]->pos.col = move->endCol;
        board->board[move->endRow][move->endCol]->moved = 1;
    }

    board->board[move->startRow][move->startCol] = NULL;

    if (move->endCol > move->startCol) {
        // kingside
        int rookStartCol = 9;
        int rookEndCol = move->endCol - 1;

        board->board[row][rookEndCol] = board->board[row][rookStartCol];
<<<<<<< HEAD
        if (board->board[row][rookEndCol] != NULL) {
            board->board[row][rookEndCol]->pos.row = row;
            board->board[row][rookEndCol]->pos.col = rookEndCol;
            board->board[row][rookEndCol]->moved = 1;
        }

        board->board[row][rookStartCol] = NULL;
    } else {
=======
        board->board[row][rookEndCol]->pos.row = row;
        board->board[row][rookEndCol]->pos.col = rookEndCol;
        board->board[row][rookEndCol]->moved = 1;

        board->board[row][rookStartCol]->img = NULL;
        board->board[row][rookStartCol]->type = EMPTY;
        board->board[row][rookStartCol]->vtable = NULL;
        board->board[row][rookStartCol]->pos.row = row;
        board->board[row][rookStartCol]->pos.col = rookStartCol;
        board->board[row][rookStartCol]->moved = 0;
    }
    else
    {
>>>>>>> a6adb5723f847994783f2624c0bc2ebc71500f20
        // queenside
        int rookStartCol = 0;
        int rookEndCol = move->endCol + 1;

        board->board[row][rookEndCol] = board->board[row][rookStartCol];
        if (board->board[row][rookEndCol] != NULL) {
            board->board[row][rookEndCol]->pos.row = row;
            board->board[row][rookEndCol]->pos.col = rookEndCol;
            board->board[row][rookEndCol]->moved = 1;
        }

        board->board[row][rookStartCol] = NULL;
    }

    move->capture = 0;
    move->castle = 1;
}

void copyBoard(Board *dest, Board *src)
{
<<<<<<< HEAD
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 10; c++) {
=======
    for (int r = 0; r < BOARD_HEIGHT; r++)
    {
        for (int c = 0; c < BOARD_WIDTH; c++)
        {
>>>>>>> a6adb5723f847994783f2624c0bc2ebc71500f20
            dest->board[r][c] = src->board[r][c];
        }
    }
}

int legalMove(Board *board, Move *move, Color turn, Move lastMove)
{
    Piece *moving = board->board[move->startRow][move->startCol];

    if (moving == NULL || moving->type == EMPTY || moving->vtable == NULL){
        return 0;
    }

    if (moving->color != turn){
        return 0;
    }

    Board testBoard;
    copyBoard(&testBoard, board);

    Move testMove = *move;

    executeMove(&testBoard, &testMove, lastMove);

    if (testBoard.board[move->endRow][move->endCol] == NULL ||
        testBoard.board[move->endRow][move->endCol]->color != turn) { //check to see if move didn't happen
        return 0;
    }

    // if the original square hasn't change
    if (move->startRow != move->endRow || move->startCol != move->endCol) {
        if (testBoard.board[move->startRow][move->startCol] != NULL &&
            testBoard.board[move->startRow][move->startCol]->type == moving->type &&
            testBoard.board[move->startRow][move->startCol]->color == moving->color) {
            return 0;
        }
    }

    if (kingCheck(&testBoard, turn)) { //if the king will be in check
        return 0;
    }

    return 1;
}

int possibleMove(Board *board, Color turn, Move lastMove)
{
    for (int sr = 0; sr < 8; sr++){
        for (int sc = 0; sc < 10; sc++){
            if (board->board[sr][sc] == NULL ||
                board->board[sr][sc]->type == EMPTY ||
                board->board[sr][sc]->color != turn){
                continue;
            }
            for (int er = 0; er < 8; er++){
                for (int ec = 0; ec < 10; ec++){
                    Move move;
                    move.startRow = sr;
                    move.startCol = sc;
                    move.endRow = er;
                    move.endCol = ec;
                    move.capture = 0;
                    move.enPassant = 0;
                    move.castle = 0;

                    if (legalMove(board, &move, turn, lastMove)) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

 
int checkCheckmate(Board *board, Color turn, Move lastMove)
{
    if (!kingCheck(board, turn)) {return 0;}

    if (possibleMove(board, turn, lastMove)) {return 0;}

    return 1;
}

int checkStalemate(Board *board, Color turn, Move lastMove){
    if (kingCheck(board, turn)) {return 0;}

    if (possibleMove(board, turn, lastMove)) {return 0;}

    return 1;
}
/* already made*/
/* already made
int checkCheckmate(Board *board, Color turn, Move lastMove)
{
    if (!kingCheck(board, turn)) {return 0;}

    if (possibleMove(board, turn, lastMove)) {return 0;}

    return 1;
}

int checkStalemate(Board *board, Color turn, Move lastMove){
    if (kingCheck(board, turn)) {return 0;}

    if (possibleMove(board, turn, lastMove)) {return 0;}

    return 1;

}
*/

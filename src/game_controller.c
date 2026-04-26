#include "game_controller.h"
#include <stdio.h>
#include <string.h>

static Color currentTurn(const Board_State* boardState)
{
    return getMovesMade(boardState) % 2 == 0 ? WHITE : BLACK;
}

static void setMoveMessage(Board_Bundle* boardData, const char* message)
{
    snprintf(boardData->moveText, sizeof(boardData->moveText), "%s", message);
    boardData->move = boardData->moveText;
}

static void appendMoveMessage(Board_Bundle* boardData, const char* message)
{
    size_t used = strlen(boardData->moveText);

    if(used >= sizeof(boardData->moveText) - 1)
        return;

    snprintf(boardData->moveText + used, sizeof(boardData->moveText) - used, "%s", message);
    boardData->move = boardData->moveText;
}

static bool isUsersTurn(const Board_Bundle* boardData)
{
    return currentTurn(boardData->boardState) == boardData->userColor;
}

static void clearPiece(Piece* piece, int row, int col)
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

static void copyBoardForAi(const Board* board, Piece aiBoard[BOARD_HEIGHT][BOARD_WIDTH])
{
    for(int row = 0; row < BOARD_HEIGHT; row++){
        for(int col = 0; col < BOARD_WIDTH; col++){
            Piece* piece = board->board[row][col];

            if(piece){
                aiBoard[row][col] = *piece;
                aiBoard[row][col].pos.row = row;
                aiBoard[row][col].pos.col = col;
            }
            else {
                clearPiece(&aiBoard[row][col], row, col);
            }
        }
    }
}

bool makeCpuMove(Board_Bundle* boardData)
{
    Board* board = boardData->board;
    Board_State* boardState = boardData->boardState;
    Piece aiBoard[BOARD_HEIGHT][BOARD_WIDTH];
    Move move;

    if(currentTurn(boardState) != boardData->cpuColor)
        return false;

    copyBoardForAi(board, aiBoard);

    if(!aiSelectMoveAtDifficulty(aiBoard, boardData->cpuColor, boardData->cpuDifficulty, &move)){
        appendMoveMessage(boardData, "CPU has no legal move.\n");
        boardState->moveSuccess = true;
        return false;
    }

    executeMove(board, &move, boardState->lastMove);
    boardState->lastMove = move;
    incrementMovesMade(boardState);
    resetClickedPiece(boardState);
    resetLegalMoveCount(boardState);
    setUpdate(boardState, true);

    appendMoveMessage(boardData, "CPU move was made\n");
    boardState->moveSuccess = true;

    return true;
}

void sendInput(Board_Bundle* boardData, Pos clickPos)
{
    Board* board = boardData->board; 
    Board_State* boardState = boardData->boardState; 
    boardState->moveSuccess = false; 
    boardData->move = NULL;

    if (!isUsersTurn(boardData))
    {
        printf("It is the CPU's turn.\n");
        return;
    }
    
    if (!isPosValid(clickPos))
    {
        printf("invalid pos! -> row: %d col: %d\n", clickPos.row, clickPos.col);
        return;
    }

    int row = clickPos.row, col = clickPos.col;

    Piece *clicked = getSquare(board, row, col);

    if (hasPiece(board, row, col) && getColor(clicked) == boardData->userColor && !isLegalMoveSquare(boardState, clickPos)) {
        printf("new piece was clicked\n"); 
        setClickedPiece(boardState, clicked);
        generateLegalMoves(boardState, board, clicked, getPos(clicked), &boardState->lastMove);
        setUpdate(boardState, true);
    }
    else if (aPieceWasClicked(boardState) && isLegalMoveSquare(boardState, clickPos)) {
        printf("executing move\n");

        Piece *clickedPiece = getClickedPiece(boardState);

        Move move;
        move.startRow = clickedPiece->pos.row;
        move.startCol = clickedPiece->pos.col;
        move.endRow = clickPos.row;
        move.endCol = clickPos.col;
        move.capture = 0;
        move.enPassant = 0;
        move.castle = 0;

        executeMove(board, &move, boardState->lastMove);
        boardState->lastMove = move;

        incrementMovesMade(boardState);

        if (move.capture) {
            setMoveMessage(boardData, "capture was made\n");
        }
        else if (move.castle) {
            setMoveMessage(boardData, "castle was made\n");
        }
        else {
            setMoveMessage(boardData, "move was made\n");
        }

        boardState->moveSuccess = true;

        resetClickedPiece(boardState);
        resetLegalMoveCount(boardState);
        setUpdate(boardState, true);

        if(currentTurn(boardState) == boardData->cpuColor){
            makeCpuMove(boardData);
        }
    } 
    else {
        resetClickedPiece(boardState); 
        resetLegalMoveCount(boardState); 
        setUpdate(boardState, true);
    }
}

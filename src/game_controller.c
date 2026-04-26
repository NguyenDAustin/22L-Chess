#include "game_controller.h"

void sendInput(Board_Bundle* boardData, Pos clickPos)
{
    Board* board = boardData->board; 
    Board_State* boardState = boardData->boardState; 
    Color turn = (getMovesMade(boardState) % 2 == 0) ? WHITE : BLACK;
    boardState->moveSuccess = false; 
    
    if (!isPosValid(clickPos))
    {
        printf("invalid pos! -> row: %d col: %d\n", clickPos.row, clickPos.col);
        return;
    }

    int row = clickPos.row, col = clickPos.col;

    Piece *clicked = getSquare(board, row, col);

    if (hasPiece(board, row, col) && !isLegalMoveSquare(boardState, clickPos)) {
        if (clicked == NULL || clicked->color != turn) {
            resetClickedPiece(boardState);
            resetLegalMoveCount(boardState);
            setUpdate(boardState, true);
            return;
        }

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
            boardData->move = "capture was made\n";
            Pos old = getPos(clickedPiece); 
            Piece* captured = getSquare(board, clickPos.row, clickPos.col); 
            pushCapture(rec, old, clickPos, clickedPiece, captured); 
        }
        else if (move.castle) {
            boardData->move = "castle was made\n";
        }
        else {
            boardData->move = "move was made\n";
            Pos old = getPos(clickedPiece); 
            pushMove(rec, old, clickPos, clickedPiece);  
        }

        boardState->moveSuccess = true;

        resetClickedPiece(boardState);
        resetLegalMoveCount(boardState);
        setUpdate(boardState, true);
    } 
    else {
        resetClickedPiece(boardState); 
        resetLegalMoveCount(boardState); 
        setUpdate(boardState, true);
    }
}

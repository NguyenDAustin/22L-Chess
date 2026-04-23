#include "game_controller.h"

void sendInput(Board *board, Board_State *boardState, Pos clickPos)
{

    if (!isPosValid(clickPos))
    {
        printf("invalid pos! -> row: %d col: %d\n", clickPos.row, clickPos.col);
        return;
    } // making sure input is valid

    int row = clickPos.row, col = clickPos.col;

    Piece *clicked = getSquare(board, row, col);

    if(hasPiece(board, row, col) && !isLegalMoveSquare(boardState, clickPos)){ //if square has piece and we havent clicked a piece before --> a newPiece was clicked
        printf("new piece was clicked\n"); 
        setClickedPiece(boardState, clicked);
        generateLegalMoves(boardState, board, clicked, getPos(clicked));  //highlight
        setUpdate(boardState, true);
    }
    else if(hasPiece(board, row, col) && aPieceWasClicked(boardState) && isLegalMoveSquare(boardState, clickPos)){ //we clicked a piece before and its legal to move there
        printf("capture\n");
        capturePiece(board, boardState, getClickedPiece(boardState), getPos(clicked)); 
        setUpdate(boardState, true);
    }
    else if(aPieceWasClicked(boardState) && isLegalMoveSquare(boardState, clickPos)){
        printf("moving\n"); 
        Piece *clickedPiece = getClickedPiece(boardState);
        movePiece(board, boardState, clickedPiece, clickPos); 
        setUpdate(boardState, true);
    } 
    //if not legal move --> just cancel move --> reset clicked piece
}
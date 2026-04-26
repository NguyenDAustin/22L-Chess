#include "game_controller.h"

/*
void sendInput(Board_Bundle* boardData, Pos clickPos)
{
    Board* board = boardData->board; 
    Board_State* boardState = boardData->boardState; 
    boardState->moveSuccess = false; 
    
    if (!isPosValid(clickPos))
    {
        printf("invalid pos! -> row: %d col: %d\n", clickPos.row, clickPos.col);
        return;
    }

    int row = clickPos.row, col = clickPos.col;

    Piece *clicked = getSquare(board, row, col);

    if (hasPiece(board, row, col) && !isLegalMoveSquare(boardState, clickPos)) {
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
        }
        else if (move.castle) {
            boardData->move = "castle was made\n";
        }
        else {
            boardData->move = "move was made\n";
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
    */



void sendInput(Board_Bundle* boardData, Pos clickPos)
{
    Board* board = boardData->board; 
    Board_State* boardState = boardData->boardState; 
    boardState->moveSuccess = false; 
    
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
        Piece *movingPiece = getClickedPiece(boardState);
        capturePiece(board, boardState, movingPiece, getPos(clicked));
        if(getType(movingPiece) == PAWN && isRowEdge(clickPos.row))
            setClickedPiece(boardState, movingPiece);
        boardData->move = "capture was made\n";  
        boardState->moveSuccess = true; 
        //addCapture(log function)
        setUpdate(boardState, true);
    }
    else if(aPieceWasClicked(boardState) && isLegalMoveSquare(boardState, clickPos)){
        printf("moving\n"); 
        Piece *clickedPiece = getClickedPiece(boardState);
        movePiece(board, boardState, clickedPiece, clickPos); 
        if(getType(clickedPiece) == PAWN && isRowEdge(clickPos.row))
            setClickedPiece(boardState, clickedPiece);
        incrementMovesMade(boardState);  
        boardData->move = "move was made\n"; 
        boardState->moveSuccess = true; 
        //addMove(LOG_FILE, getMovesMade(boardState), "color - pro", "piece - pro", "fr", "to"); 
        setUpdate(boardState, true);
    } 
    else{
        resetClickedPiece(boardState); 
        resetLegalMoveCount(boardState); 
        setUpdate(boardState, true); //cancel move
    }
    //if not legal move --> just cancel move --> reset clicked piece
}

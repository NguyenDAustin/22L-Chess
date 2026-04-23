#include "board.h"
#include "board_state.h"

const int MAX_LEGAL_MOVES = BOARD_WIDTH * BOARD_HEIGHT; 


//Board Ctor
void initializeBoardState(Board_State* boardState){
  boardState->clickedPiece = NULL; 
  boardState->hasUpdate = false; 
  boardState->legalMoveCount = 0; 
}

//BOARD STATE - boolean state functions 

bool aPieceWasClicked(const Board_State* boardState){
    return boardState->clickedPiece;
}

bool newPieceWasClicked(const Board_State* boardState, Piece *clickedPiece){
    return (clickedPiece != boardState->clickedPiece && clickedPiece != NULL);
}

bool hasUpdate(const Board_State* boardState){
    return boardState->hasUpdate;
}

bool isLegalMoveSquare(const Board_State* boardState, Pos pos){
    const Pos* legalMoves = boardState->legalMoves; 
    int size = boardState->legalMoveCount; 

    for(int i = 0; i < size; i++){ 
        if(isSamePos(legalMoves[i], pos))
            return true; 
    }
    return false; 
}

//BOARD STATE - general getters + setters 

void setUpdate(Board_State *boardState, bool update){
    boardState->hasUpdate = update;
}

Piece *getClickedPiece(const Board_State *boardState){
    return boardState->clickedPiece;
}

void setClickedPiece(Board_State *boardState, Piece *piece){
    boardState->clickedPiece = piece;
}

void resetClickedPiece(Board_State *boardState){
    setClickedPiece(boardState, NULL);
}


//BOARD STATE - legal moves getter + setters 

int getLegalMoveCount(const Board_State* boardState){ 
    return boardState->legalMoveCount; 
}
void setLegalMoveCount(Board_State* boardState, int count){
     boardState->legalMoveCount = count; 
}

void resetLegalMoveCount(Board_State* boardState){
    setLegalMoveCount(boardState, 0); 
}


//BOARD STATE - LEGAL MOVE FUNCTIONS 

void addLegalMove(Board_State* boardState, Pos pos){ 
    int size = boardState->legalMoveCount; 

    if(size < MAX_LEGAL_MOVES){
        boardState->legalMoves[size] = pos; 
        boardState->legalMoveCount++; 
    }
}


//maybe move to its own file for legal move generation ...

bool canPieceGoTo(Board* board, Piece* piece, Pos start, Pos end) {
    if (!board || !piece || !piece->vtable){
        printf("ERROR: BOARD, PIECE, OR PIECE VTABLE IS NULL!\n");
        printf("board is null: %d", (!board));
        printf("piece is null: %d", !piece); 
        printf("piece vtable is null: %d", !piece->vtable); 
        return false;
    }

    if(!isPosValid(start) || !isPosValid(end)){
        printf("ERROR: INVALID START OR END POS!");
        return false;  
    }
        
    if (isSamePos(start,end)) 
        return false;
    

    Piece* target = getSquare(board, end.row, end.col); //square we want to go to

    return target ? piece->vtable->canCapture(board, piece, start.row, start.col, end.row, end.col) 
                  : piece->vtable->canMove(board, piece, start.row, start.col, end.row, end.col); 
}

void generateLegalMoves(Board_State* boardState, Board* board, Piece* piece, Pos start){ //might change to make more efficient in future
    resetLegalMoveCount(boardState); 

    Pos end; 
    for(int row = 0; row < BOARD_HEIGHT; row++){
        for(int col = 0; col < BOARD_WIDTH; col++){
            posCtor(&end, row, col);
            if(canPieceGoTo(board, piece, start, end)){
                //printf("legalMove: [%d][%d]\n", end.row, end.col);
                addLegalMove(boardState, end); 
            }
              
        }
    }
}


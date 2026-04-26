#include "board.h"
#include "board_state.h"

const int MAX_LEGAL_MOVES = BOARD_WIDTH * BOARD_HEIGHT; 


//Board Ctor
void initializeBoardState(Board_State* boardState){
  boardState->clickedPiece = NULL; 
  boardState->hasUpdate = false; 
  boardState->moveSuccess = false; 
  boardState->gameOver = false;
  boardState->legalMoveCount = 0; 
  boardState->movesMade = 0; 


  boardState->lastMove.startRow = -1;
  boardState->lastMove.startCol = -1;
  boardState->lastMove.endRow = -1; 
  boardState->lastMove.endCol = -1;
  boardState->lastMove.capture = 0;
  boardState->lastMove.enPassant = 0;
  boardState->lastMove.castle = 0;


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

bool moveSucces(const Board_State* boardState){
    return boardState->moveSuccess; 
}

bool isGameOver(const Board_State* boardState){
    return boardState->gameOver;
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

bool isPromotion(const Board_State* boardState){ 
    Piece* clickedPiece = boardState->clickedPiece; 
    if(clickedPiece == NULL)
        return false;

    Rank type = getType(clickedPiece); 
    return (isRowEdge(getPosRow(clickedPiece)) && type == PAWN); 
}


//BOARD STATE - general functions 

void incrementMovesMade(Board_State* boardState){
    boardState->movesMade++; 
}

//BOARD STATE - general getters + setters 

void setUpdate(Board_State *boardState, bool update){ //sets update flag. is true if board has changed
    boardState->hasUpdate = update;
}

void setGameOver(Board_State *boardState, bool gameOver){
    boardState->gameOver = gameOver;
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

void setMovesMade(Board_State* boardState, int moves){
    boardState->movesMade = moves; 
}

int getMovesMade(const Board_State* boardState){
    return boardState->movesMade; 
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
//adds the given move (pos) to the legalmoves array
void addLegalMove(Board_State* boardState, Pos pos){ 
    int size = boardState->legalMoveCount; 

    if(size < MAX_LEGAL_MOVES){
        boardState->legalMoves[size] = pos; 
        boardState->legalMoveCount++; 
    }
}

//takes in a start pos (piece's position) and an end position (target position (user click))
//if the piece can perform a capture or legally move to that target position --> return true 
bool canPieceGoTo(Board* board, Piece* piece, Pos start, Pos end) {
    if (!board || !piece || !piece->vtable){
        printf("ERROR: BOARD, PIECE, OR PIECE VTABLE IS NULL!\n");
        return false;
    }

    if(!isPosValid(start) || !isPosValid(end)){
        printf("ERROR: INVALID START OR END POS!");
        return false;  
    }
        
    if (isSamePos(start,end)) 
        return false;
    

    Piece* target = getSquare(board, end.row, end.col); //square we want to go to
    if (target != NULL && target->type == KING) {
        return false;
    }

    return target ? piece->vtable->canCapture(board, piece, start.row, start.col, end.row, end.col) 
                  : piece->vtable->canMove(board, piece, start.row, start.col, end.row, end.col); 
}

//given a piece and its position --> generate all the legal moves that piece can make
void generateLegalMoves(Board_State* boardState, Board* board, Piece* piece, Pos start, Move *lastMove){ //might change to make more efficient in future
    resetLegalMoveCount(boardState); 

    Pos end; 
    for(int row = 0; row < BOARD_HEIGHT; row++){
        for(int col = 0; col < BOARD_WIDTH; col++){
            posCtor(&end, row, col);
            if(canPieceGoTo(board, piece, start, end)){
                //printf("legalMove: [%d][%d]\n", end.row, end.col);
                addLegalMove(boardState, end); 
            }
            else if(piece->type == PAWN && pawnCanEnPassant(board, piece, start.row, start.col, end.row, end.col, lastMove)) {
                addLegalMove(boardState, end);
            }
        }
    }
}

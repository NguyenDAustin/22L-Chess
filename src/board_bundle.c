#include "board_bundle.h"



void initializeBoardBundle(Board_Bundle* boardData, Board* board, Icon** images, GtkWidget* boardWidget, GtkWidget* promotionPopUp,  GtkTextView* log, GtkTextBuffer* buffer){ 
  boardData->board = board; 
  boardData->boardState = malloc(sizeof(Board_State)); 
  boardData->boardWidget = boardWidget; 
  boardData->promotionPopUp = promotionPopUp; 
  boardData->logBuffer = buffer; 
  boardData->log = log; 
  boardData->move = NULL; 
  boardData->images = images; 
  initializeBoardState(boardData->boardState); 
}


//SETTERS 
void setBoard(Board_Bundle* boardData, Board* board){ 
    boardData->board = board; 
}

void setImages(Board_Bundle* boardData, Icon** images){ 
    boardData->images = images; 
}

void setBoardWidget(Board_Bundle* boardData, GtkWidget* boardWidget){
    boardData->boardWidget = boardWidget; 
} 

void setPromotionPop(Board_Bundle* boardData, GtkWidget* popUp){
    boardData->promotionPopUp = popUp; 
}

void setLogTextView(Board_Bundle* boardData, GtkTextView* logText){
    boardData->log = logText; 
}

void setLogBuffer(Board_Bundle* boardData, GtkTextBuffer* buffer){
    boardData->logBuffer = buffer; 
}

void setBoardState(Board_Bundle* boardData, Board_State* boardState){
    boardData->boardState = boardState; 
}


//GETTERS
Board* getBoard(const Board_Bundle* boardData){
    return boardData->board; 
}

Icon** getImages(const Board_Bundle* boardData){ 
    return boardData->images; 
}

GtkWidget* getBoardWidget(const Board_Bundle* boardData){
    return boardData->boardWidget; 
}

GtkWidget* getPromotionPop(const Board_Bundle* boardData){ 
    return boardData->promotionPopUp; 
}

GtkTextView* getLogTextView(const Board_Bundle* boardData){
    return boardData->log; 
}

GtkTextBuffer* getLogBuffer(const Board_Bundle* boardData){
    return boardData->logBuffer; 
}

Board_State* getBoardState(const Board_Bundle* boardData){ 
    return boardData->boardState; 
}
#include "board_bundle.h"

void initializeBoardBundle(Board_Bundle* boardData, Board* board, GtkWidget* boardWidget, GtkTextView* log, GtkTextBuffer* buffer){ 
  boardData->board = board; 
  boardData->boardState = malloc(sizeof(Board_State)); 
  boardData->boardWidget = boardWidget; 
  boardData->logBuffer = buffer; 
  boardData->log = log; 
  boardData->move = NULL; 
  initializeBoardState(boardData->boardState); 
   
}
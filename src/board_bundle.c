#include "board_bundle.h"

void initializeBoardBundle(Board_Bundle* boardData, Board* board, GtkWidget* boardWidget){ 
  boardData->board = board; 
  boardData->boardState = malloc(sizeof(Board_State)); 
  initializeBoardState(boardData->boardState); 
  boardData->boardWidget = boardWidget;  
}
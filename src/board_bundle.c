#include "board_bundle.h"



void initializeBoardBundle(Board_Bundle* boardData, Board* board, GtkWidget* boardWidget, GtkWidget* promotionPopUp, 
    GtkWidget* whitePromotionGrid, GtkWidget* blackPromotionGrid, GtkTextView* log, GtkTextBuffer* buffer){ 
  boardData->board = board; 
  boardData->boardState = malloc(sizeof(Board_State)); 
  boardData->boardWidget = boardWidget; 
  boardData->promotionPopUp = promotionPopUp; 
  boardData->whitePromotionGrid = whitePromotionGrid; 
  boardData->blackPromotionGrid = blackPromotionGrid; 
  boardData->logBuffer = buffer; 
  boardData->log = log; 
  boardData->move = NULL; 
  initializeBoardState(boardData->boardState); 
   
}
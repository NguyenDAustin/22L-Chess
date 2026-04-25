#include "board_bundle.h"



void initializeBoardBundle(Board_Bundle* boardData, Board* board, Icon** images, GtkWidget* boardWidget, GtkWidget* promotionPopUp,  GtkTextView* log, GtkTextBuffer* buffer){ 
void initializeBoardBundle(Board_Bundle* boardData, Board* board, GtkWidget* boardWidget, GtkTextView* log, GtkTextBuffer* buffer, GtkLabel* whiteTimerLabel, GtkLabel* blackTimerLabel){ 
  boardData->board = board; 
  boardData->boardState = malloc(sizeof(Board_State)); 
  boardData->boardWidget = boardWidget; 
  boardData->promotionPopUp = promotionPopUp; 
  boardData->logBuffer = buffer; 
  boardData->log = log; 
  boardData->whiteTimerLabel = whiteTimerLabel;
  boardData->blackTimerLabel = blackTimerLabel;
  boardData->timerSourceId = 0;
  boardData->whiteSeconds = 0;
  boardData->blackSeconds = 0;
  boardData->move = NULL; 
  boardData->images = images; 
  initializeBoardState(boardData->boardState); 
   
}

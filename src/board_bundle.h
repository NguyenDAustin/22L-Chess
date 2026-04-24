#ifndef BOARD_BUNDLE_H 
#define BOARD_BUNDLE_H 

#include "board.h"
#include "board_state.h"

typedef struct {
  Board* board; 
  Board_State* boardState; 
  GtkWidget* boardWidget; 
  GtkTextView* log; 
  GtkTextBuffer* logBuffer; 

  GtkWidget* promotionPopUp; 
  GtkWidget* whitePromotionGrid;
  GtkWidget* blackPromotionGrid;  
  
  char* move;
} Board_Bundle; 


void initializeBoardBundle(Board_Bundle* boardData, Board* board, GtkWidget* boardWidget, GtkWidget* promotionPopUp, GtkWidget* promotionGrid, GtkWidget* blackPromotionGrid, GtkTextView* log, GtkTextBuffer* buffer);

#endif 
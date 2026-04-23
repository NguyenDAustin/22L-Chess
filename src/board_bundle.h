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
  char* move;
} Board_Bundle; 


void initializeBoardBundle(Board_Bundle* boardData, Board* board, GtkWidget* boardWidget, GtkTextView* log, GtkTextBuffer* buffer);

#endif 
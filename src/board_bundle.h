#ifndef BOARD_BUNDLE_H
#define BOARD_BUNDLE_H

#include "board.h"
#include "board_state.h"
#include "ai.h"

<<<<<<< HEAD
typedef struct Board_Bundle {
  Board* board; 
  Board_State* boardState; 
  GtkWidget* boardWidget; 
  GtkTextView* log; 
  GtkTextBuffer* logBuffer; 
  GtkWidget* promotionPopUp; 
  Icon** images; 
  char* move; //move string that needs to be appended to UI
  char moveText[256];
=======
typedef struct Board_Bundle
{
  Board *board;
  Board_State *boardState;
  GtkWidget *boardWidget;
  GtkTextView *log;
  GtkTextBuffer *logBuffer;
  GtkWidget *promotionPopUp;
  Icon **images;
  char *move; // move string that needs to be appended to UI
>>>>>>> 162f63c1ea3de2f0975d79e2f8eff2231c3a6d09

  // Timer data
  GtkLabel *whiteTimerLabel;
  GtkLabel *blackTimerLabel;
  guint timerSourceId;
  int whiteSeconds;
  int blackSeconds;

  Color userColor;
  Color cpuColor;
  bool userStarts;
<<<<<<< HEAD
  AIDifficulty cpuDifficulty;
} Board_Bundle; 
=======
  int difficulty;
} Board_Bundle;
>>>>>>> 162f63c1ea3de2f0975d79e2f8eff2231c3a6d09

void initializeBoardBundle(Board_Bundle *boardData, Board *board, Icon **images, GtkWidget *boardWidget, GtkWidget *promotionPopUp, GtkTextView *log, GtkTextBuffer *buffer, GtkLabel *whiteTimerLabel, GtkLabel *blackTimerLabel);

// getters + setters
void setBoard(Board_Bundle *boardData, Board *board);
void setImages(Board_Bundle *boardData, Icon **images);
void setBoardWidget(Board_Bundle *boardData, GtkWidget *boardWidget);
void setPromotionPop(Board_Bundle *boardData, GtkWidget *popUp);
void setLogTextView(Board_Bundle *boardData, GtkTextView *logText);
void setLogBuffer(Board_Bundle *boardData, GtkTextBuffer *buffer);
void setBoardState(Board_Bundle *boardData, Board_State *boardState);
void setWhiteTimerLabel(Board_Bundle *boardData, GtkLabel *whiteTimerLabel);
void setBlackTimerLabel(Board_Bundle *boardData, GtkLabel *blackTimerLabel);
void setWhiteSeconds(Board_Bundle *boardData, int seconds);
void setBlackSeconds(Board_Bundle *boardData, int seconds);

Board *getBoard(const Board_Bundle *boardData);
Icon **getImages(const Board_Bundle *boardData);
GtkWidget *getBoardWidget(const Board_Bundle *boardData);
GtkWidget *getPromotionPop(const Board_Bundle *boardData);
GtkTextView *getLogTextView(const Board_Bundle *boardData);
GtkTextBuffer *getLogBuffer(const Board_Bundle *boardData);
Board_State *getBoardState(const Board_Bundle *boardData);
GtkLabel *getWhiteTimerLabel(const Board_Bundle *boardData);
GtkLabel *getBlackTImerLabel(const Board_Bundle *boardData);
int getWhiteSeconds(const Board_Bundle *boardData);
int getBlackSeconds(const Board_Bundle *boardData);

#endif

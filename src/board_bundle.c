#include "board_bundle.h"

void initializeBoardBundle(Board_Bundle *boardData, Board *board, Icon **images, GtkWidget *boardWidget, GtkWidget *promotionPopUp, GtkTextView *log, GtkTextBuffer *buffer, GtkLabel *whiteTimerLabel, GtkLabel *blackTimerLabel)
{

    setBoard(boardData, board);
    setBoardWidget(boardData, boardWidget);
    setPromotionPop(boardData, promotionPopUp);
    setLogTextView(boardData, log);
    setLogBuffer(boardData, buffer);
    setWhiteTimerLabel(boardData, whiteTimerLabel);
    setBlackTimerLabel(boardData, blackTimerLabel);
    setImages(boardData, images);

    boardData->timerSourceId = 0;
    boardData->whiteSeconds = 0;
    boardData->blackSeconds = 0;
    boardData->userColor = WHITE;
    boardData->cpuColor = BLACK;
    boardData->userStarts = true;
    boardData->move = NULL;
    boardData->difficulty = 1;

<<<<<<< HEAD
  boardData->timerSourceId = 0;
  boardData->whiteSeconds = 0;
  boardData->blackSeconds = 0;
  boardData->userColor = WHITE;
  boardData->cpuColor = BLACK;
  boardData->userStarts = true;
  boardData->cpuDifficulty = AI_EASY;
  boardData->moveText[0] = '\0';
  boardData->move = NULL; 

  initializeBoardState(boardData->boardState); 
=======
    initializeBoardState(boardData->boardState);
>>>>>>> 162f63c1ea3de2f0975d79e2f8eff2231c3a6d09
}

void setBoard(Board_Bundle *boardData, Board *board)
{
    boardData->board = board;
}

void setImages(Board_Bundle *boardData, Icon **images)
{
    boardData->images = images;
}

void setBoardWidget(Board_Bundle *boardData, GtkWidget *boardWidget)
{
    boardData->boardWidget = boardWidget;
}

void setPromotionPop(Board_Bundle *boardData, GtkWidget *popUp)
{
    boardData->promotionPopUp = popUp;
}

void setLogTextView(Board_Bundle *boardData, GtkTextView *logText)
{
    boardData->log = logText;
}

void setLogBuffer(Board_Bundle *boardData, GtkTextBuffer *buffer)
{
    boardData->logBuffer = buffer;
}

void setBoardState(Board_Bundle *boardData, Board_State *boardState)
{
    boardData->boardState = boardState;
}

void setWhiteTimerLabel(Board_Bundle *boardData, GtkLabel *whiteTimerLabel)
{
    boardData->whiteTimerLabel = whiteTimerLabel;
}

void setBlackTimerLabel(Board_Bundle *boardData, GtkLabel *blackTimerLabel)
{
    boardData->blackTimerLabel = blackTimerLabel;
}

void setWhiteSeconds(Board_Bundle *boardData, int seconds)
{
    boardData->whiteSeconds = seconds;
}
void setBlackSeconds(Board_Bundle *boardData, int seconds)
{
    boardData->blackSeconds = seconds;
}

void setDifficulty(Board_Bundle *boardData, int difficulty)
{
    boardData->difficulty = difficulty;
}

Board *getBoard(const Board_Bundle *boardData)
{
    return boardData->board;
}

Icon **getImages(const Board_Bundle *boardData)
{
    return boardData->images;
}

GtkWidget *getBoardWidget(const Board_Bundle *boardData)
{
    return boardData->boardWidget;
}

GtkWidget *getPromotionPop(const Board_Bundle *boardData)
{
    return boardData->promotionPopUp;
}

GtkTextView *getLogTextView(const Board_Bundle *boardData)
{
    return boardData->log;
}

GtkTextBuffer *getLogBuffer(const Board_Bundle *boardData)
{
    return boardData->logBuffer;
}

Board_State *getBoardState(const Board_Bundle *boardData)
{
    return boardData->boardState;
}

GtkLabel *getWhiteTimerLabel(const Board_Bundle *boardData)
{
    return boardData->whiteTimerLabel;
}
GtkLabel *getBlackTimerLabel(const Board_Bundle *boardData)
{
    return boardData->blackTimerLabel;
}

int getWhiteSeconds(const Board_Bundle *boardData)
{
    return boardData->whiteSeconds;
}
int getBlackSeconds(const Board_Bundle *boardData)
{
    return boardData->blackSeconds;
}

int getDifficulty(const Board_Bundle *boardData)
{
    return boardData->difficulty;
}

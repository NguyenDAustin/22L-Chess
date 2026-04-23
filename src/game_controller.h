#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H 

#include "board.h"
#include "board_state.h"

void sendInput(Board *board, Board_State *boardState, Pos clickPos); 

#endif
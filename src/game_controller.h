#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H 

#include "board_bundle.h"
#include "log.h"


/*
typedef enum{  
    //gui to board 
    PIECE_WAS_CLICKED = 0,
    PIECE_WAS_MOVED = 1,

    //board to gui
    PIECE_WAS_CAPTURED = 2
} FLAG; 
 */ //perhaps will have use in future


void sendInput(Board_Bundle* boardData, Pos clickPos); 

#endif
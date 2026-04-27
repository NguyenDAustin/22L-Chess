#ifndef SAVE_H
#define SAVE_H

#include "board.h"
#include "log.h"
#include "gui/render.h"
#include "gui/gui.h"

#define SAVE_FILENAME "save_chess.txt"

typedef struct {
	char name[64];
	char filname[128];
	char turn[8];
	char timestamp[64];
}SaveEntry


/*Save game state*/
const char *saveGame(const char *saveName, Board *board, Board_State *boardState, Board_Bundle *bundle, Color currentTurn);


/*Load game*/
int loadGame(const char *saveName, Board *board, Board_State *boardState, Board_Bundle *bundle, Piece_Icon **images, Color *currentTurn);

#endif

#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include "pieces.h"


bool isValid(const char *input);

bool inputHasMove(void);
bool inputIsCastleKingSide(void);
bool inputIsCastleQueenSide(void);
bool inputIsUndo(void);
bool inputIsQuit(void);

Pos inputGetStartPos(void);
Pos inputGetEndPos(void);

#endif
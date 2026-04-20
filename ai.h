/*ai.h*/
#ifndef AI_H
#define AI_H

#include "pieces.h"     
#include "board.h" 


#ifndef AI_MOVE_STUB_DEFINED
#define AI_MOVE_STUB_DEFINED

typedef struct {
    int sr, sc;     /* start row, start col */
    int er, ec;     /* end row, end col     */
} Move;

#endif

int aiSelectMove(Color player, Move *out);


#endif
/*(easy mode)*/
#include <stdio.h>
#include <stdlib.h>     
#include <time.h>  
     
#include "ai.h"
#include "pieces.h"
#include "board.h"


#define MAX_MOVES 512

static int generateLegalMoves(Color player, Move out[], int maxMoves)
{
    (void)player; (void)out; (void)maxMoves;
    return 0;
}

int aiSelectMove(Color player, Move *out)
{
    /* Seed rand() once per program run. */
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned)time(NULL));
        seeded = 1;
    }

    /* Get all legal moves from the rules engine. */
    Move moves[MAX_MOVES];
    int n = generateLegalMoves(player, moves, MAX_MOVES);

    if (n <= 0) return 0;   /* no legal moves */

    /* Pick one at random and hand it back. */
    *out = moves[rand() % n];
    return 1;
}
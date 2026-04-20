//Made by MIa Ness
//Draft1 : 4/19/26
//undo.c

#include "undo.h"
#include <stdio.h>
#include <string.h>

static list stack[512];
static int top=-1;

/*reset stack*/
void undoSet(void){
	top=-1;
}


int undoPush(const list *record){
	if (top>=511){
		fprintf(stderr, "Undo list is full\n");
		return 0;
	}
	stack[++top] = *record;
	return 1;
}

int undoPop(list *pop){
	if (top<0){
		fprintf(stderr, "No moves to undo\n");
		return 0;
	}
	*pop = stack[top--];
	return 1;
}

int undo(list *l, Piece board[10][10]){
	if (!l) return 0;
	board[l->from_row][l->from_col] = l->piece;
	board[l->to_row][l->to_col] = 0;
	return 1;
}

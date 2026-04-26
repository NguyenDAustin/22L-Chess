//Made by MIa Ness
//Draft1 : 4/19/26
//undo.c

#include "undo.h"
#include "board.h"
#include <stdio.h>
#include <string.h>


static Undo_Record stack[512];
static int top=-1;



void undoRecordCtor(Undo_Record* rec, Pos from, Pos to, Piece* moved, Piece* captured){
	rec->from = from; 
	rec->to = to; 
	rec->movedPiece = moved; 
	rec->capturedPiece = captured; 
}

//pushes a move onto our undo stack
void pushMove(Undo_Record* rec, Pos from, Pos to, Piece* moved){
	undoRecordCtor(rec, from, to, moved, NULL); 
	undoPush(rec); 
}

//pushes a capture onto our undo stack
void pushCapture(Undo_Record* rec, Pos from, Pos to, Piece* moved, Piece* captured){ 
	undoRecordCtor(rec, from, to, moved, captured); 
	undoPush(rec); 
}


/*reset stack*/
void undoSet(void){
	top=-1;
}


int undoPush(const Undo_Record *record){
	if (top>=511){
		fprintf(stderr, "Undo list is full\n");
		return 0;
	}
	stack[++top] = *record;
	return 1;
}

int undoPop(Undo_Record* pop){
	if (top<0){
		fprintf(stderr, "No moves to undo\n");
		return 0;
	}
	*pop = stack[top--];
	return 1;
}

int undo(Undo_Record* rec, Board* board){
	if (!rec) return 0;
	board->board[rec->from.row][rec->from.col] = rec->movedPiece;
	board->board[rec->to.row][rec->to.col] = rec->capturedPiece;
	return 1;
}

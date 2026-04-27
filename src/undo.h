//Made by MIa Ness
//Draft1 : 4/19/26
//undo.h

#ifndef UNDO_H
#define UNDO_H

#include "pieces.h"
#include "move.h"

/*
typedef struct {
	int moveNUm;
	Move move;
	Piece movPiece;
	Piece capturedPiece;
} list;
 */

typedef struct {
	//regular undo
    Move move; 
    Piece *movedPiece;
    Piece *capturedPiece;

	//castle handling 
	Piece* movedRook; 
	Move rookMove;  
} Undo_Record;


void pushMoveForUndo(Board *board, Move *move); 

void printUndoMove(Undo_Record* rec); 
void undoRecordCtor(Undo_Record* rec, Move move, Piece* moved, Piece* captured);
void pushMove(Undo_Record* rec, Move move, Piece* moved); 
void pushCapture(Undo_Record* rec, Move move, Piece* moved, Piece* captured); 


void undoSet(void);

int undoPush(const Undo_Record* record);

int undoPop(Undo_Record *pop);

int undo(Undo_Record* rec, Board* board); //had to modify to board* style for code to run

#endif

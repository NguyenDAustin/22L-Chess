// Made by MIa Ness
// Draft1 : 4/19/26
// undo.c

#include "undo.h"
#include "board.h"
#include <stdio.h>
#include <string.h>

static Undo_Record stack[512];
static int top = -1;

void printUndoMove(Undo_Record *rec)
{
	printf("From: [%d][%d] To: [%d][%d]\n", rec->move.startRow, rec->move.startCol, rec->move.endRow, rec->move.endCol);
}

void undoRecordCtor(Undo_Record *rec, Move move, Piece *moved, Piece *captured)
{
	rec->move = move;
	rec->movedPiece = moved;
	rec->capturedPiece = captured;
}

void pushMoveForUndo(Board *board, Move *move)
{
	Undo_Record rec;

	rec.move = *move;
	rec.movedPiece = board->board[move->startRow][move->startCol];
	rec.capturedPiece = board->board[move->endRow][move->endCol];

	undoPush(&rec);
}

// pushes a move onto our undo stack
void pushMove(Undo_Record *rec, Move move, Piece *moved)
{
	undoRecordCtor(rec, move, moved, NULL);
	undoPush(rec);
}

// pushes a capture onto our undo stack
void pushCapture(Undo_Record *rec, Move move, Piece *moved, Piece *captured)
{
	undoRecordCtor(rec, move, moved, captured);
	undoPush(rec);
}

/*reset stack*/
void undoSet(void)
{
	top = -1;
}

int undoPush(const Undo_Record *record)
{
	if (top >= 511)
	{
		fprintf(stderr, "Undo list is full\n");
		return 0;
	}
	stack[++top] = *record;
	return 1;
}

int undoPop(Undo_Record *pop)
{
	if (top < 0)
	{
		fprintf(stderr, "No moves to undo\n");
		return 0;
	}
	*pop = stack[top--];
	return 1;
}

int undo(Undo_Record *rec, Board *board)
{
	if (!rec)
		return 0;

	Piece *movedPiece = rec->movedPiece;
	Piece *capturedPiece = rec->capturedPiece;
	Pos pos;

	board->board[rec->move.startRow][rec->move.startCol] = rec->movedPiece;
	posCtor(&pos, rec->move.startRow, rec->move.startCol);

	if (movedPiece)
		setPos(movedPiece, pos);

	board->board[rec->move.endRow][rec->move.endCol] = rec->capturedPiece;
	posCtor(&pos, rec->move.endRow, rec->move.endCol);

	if (capturedPiece)
		setPos(capturedPiece, pos);

	return 1;
}

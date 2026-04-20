//Made by MIa Ness
//Draft1 : 4/19/26
//undo.h

#ifndef UNDO_H
#define UNDO_H


void undoSet(void);

int undoPush(const list *record);

int undoPop(list *pop);

int undo(list *l, Piece board[10][10]);

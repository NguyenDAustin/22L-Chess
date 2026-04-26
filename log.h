//Made by Mia Ness
//Draft1 4/19/26
//log.h

#ifndef LOG_H
#define LOG_H

#include "pieces.h"

#define LOG_FILE "chess_game.log"

void newLog(const char *filename);

void addMove(const char *filename, int moveNum, Piece *piece, Pos to){

void addCapture (const char *filename, int moveNum, Piece *piece, Pos from, Pos capturedAt);

void addCastle (const char *filename, int moveNum, Piece *piece,  int sideNum);

void logEnPassant(const char *filename, int moveNum, Piece *piece, Pos pos);

void logPromotion(const char *filename, int moveNum, Piece *piece, Pos to, Piece *newPiece);

void logCheck(const char *filename, int moveNum, Piece *piece);

void logCheckmate(const char *filename, int moveNum, Piece *winner){

void logDraw(const char *filename, int moveNum);

void logQuit(const char *filename, int moveNum, Piece *quitter);


#endif

//Made by Mia Ness
//Draft1 4/19/26
//log.h

#ifndef LOG_H
#define LOG_H

#include "pieces.h"

#define LOG_FILE "chess_game.log"

const char newLog(const char *filename);

const char addMove(const char *filename, int moveNum, Piece *piece, Pos to){

const char addCapture (const char *filename, int moveNum, Piece *piece, Pos from, Pos capturedAt);

const char addCastle (const char *filename, int moveNum, Piece *piece,  int sideNum);

const char logEnPassant(const char *filename, int moveNum, Piece *piece, Pos pos);

const char logPromotion(const char *filename, int moveNum, Piece *piece, Pos to, Piece *newPiece);

const char logCheck(const char *filename, int moveNum, Piece *piece);

const char logCheckmate(const char *filename, int moveNum, Piece *winner){

const char logDraw(const char *filename, int moveNum);

const char logQuit(const char *filename, int moveNum, Piece *quitter);


#endif

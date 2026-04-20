//Made by Mia Ness
//Draft1 4/19/26
//log.h

#ifndef LOG_H
#define LOG_H

#define LOG_FILE "chess_game.log"

void newLog(const char *filename);

void addMove(const char *filename, int moveNum, const char *color, const char *piece, const char *from, const char *to);

void addCapture (const char *filename, int moveNum, const char *color, const char *attacker, const char *from, const char *captured, const char *at);

void addCastle (const char *filename, int moveNum, const char *color, int sideNum);

void logEnPassant(const char *filename, int moveNum, const char *color, const char *capturedPlace, const char *from, const char *to);

void logPromotion(const char *filename, int moveNum, const char *color, const char *promotion, const char *from, const char *to);

void logCheck(const char *filename, int moveNum, const char *color);

void logCheckmate(const char *filename, int moveNum, const char *winner, const char *loser);

void logDraw(const char *filename, int moveNum);

void logQuit(const char *filename, int moveNum, const char *color);
#endif

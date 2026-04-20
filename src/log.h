//Made by Mia Ness
//Draft1 4/19/26
//log.h

#ifndef LOG_H
#define LOG_H

#define LOG_FILE "chess_game.log"

void newLog(const char *filename);

void addMove(const char *filename, int moveNum, const char *color, const char *piece, const char *from, const char *to);

void addCapture (const char *filename, int moveNum, const char *color, const char *attacker, const char *from, const char *captured, const char *);

void addCastle (const char *filename, int moveNum, const char *color, int );

#endif

//Made by Mia Ness
//Draft1 4/19/26

#include "log.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
/*creates or opens log file*/
void newLog(const char *filename){
	FILE *f = fopen(filename, "a");
	if (!f){
		fprintf(stderr, "Log file '%s' could not be opened\n", filename);
		return;
	}
	/* get current time*/
	time_t current = time(NULL);
	/*convert time to readable format*/
	char *timetxt = ctime(&current);

	fprintf(f, "\n Chess Game Log : %s \n", timetxt);
	fprintf(f, "%-2s %-6s %-20s %s\n", "#", "Color", "Action", "Move");
	fprintf(f, "-------------------------------------------\n");
	fclose(f);
}

/*add a move to the log*/
void addMove(const char *filename, int moveNum, const char *color, const char *piece, const char *from, const char *to){
	FILE *f = fopen(filename, "a");
	if (!f) return;
	fprintf(f,"%-2d %-6s %-20s {%s, %s}\n", moveNum, color, piece, from, to);
	fclose(f);
}

/*add a capture to the log*/
void addCapture (const char *filename, int moveNum, const char *color, const char *attacker, const char *from, const char *captured, const char *at){
	FILE *f = fopen(filename, "a");
	if (!f) return;
	fprintf(f, "%-2d %-6s %-20s %s captures %s {%s, %s}\n", moveNum, color, "Capture", attacker, captured, at, from);
        fclose(f);
}
/*add castle move to the log*/
void addCastle (const char *filename, int moveNum, const char *color, int sideNum){
	FILE *f = fopen(filename, "a");
	if (!f) return;
	const char *side = (sideNum == 0) ? "King" : "Queen";
	fprintf(f, "%-2d %-6s %-20s %s castling\n", moveNum, color, "Castle", side);
        fclose(f);
}
/*add en passant to the log*/
void logEnPassant(const char *filename, int moveNum, const char *color, const char *capturedPlace, const char *from, const char *to){
	FILE *f = fopen(filename, "a");
	if (!f) return;
	fprintf(f, "%-6d %-6s %-20s pawn captured on %s {%s, %s}\n", moveNum, color, "En Passant", capturedPlace, from, to);
	fclose(f);
}
/*add promotion to the log*/
void logPromotion(const char *filename, int moveNum, const char *color, const char *promotion, const char *from, const char *to){
	FILE *f = fopen(filename, "a");
	if (!f) return;
	fprintf(f, "%-6d %-6s %-20s Pawn promoted to %s {%s, %s}\n", moveNum, color, "Promotion", promotion, from, to);
	fclose(f);
}
/*add check to the log*/
void logCheck(const char *filename, int moveNum, const char *color){
	FILE *f = fopen(filename, "a");
	if (!f) return;
	fprintf(f, "%-6d %-6s %-20s %s is in check\n", moveNum, color, "Check", color);
	fclose(f);
}
/*add checkmate to the log*/
void logCheckmate(const char *filename, int moveNum, const char *winner, const char *loser){
	FILE *f = fopen(filename, "a");
	if (!f) return;
	fprintf(f, "%-6d %-6s %-20s %s puts %s in checkmate\n", moveNum, winner, "Checkmate", winner, loser);
	fprintf(f, "End of game. The winner is %s\n", winner);
	fclose(f);
}
/*add draw to the log*/
void logDraw(const char *filename, int moveNum){
	FILE *f = fopen(filename, "a");
	if (!f) return;
	fprintf(f, "%-6d %-6s %-20s\n", moveNum, "Draw", "Draw");
	fprintf(f, "End of game. The game was a draw.\n");
	fclose(f);
}
/*add a player quitting to the log*/
void logQuit(const char *filename, int moveNum, const char *color){
	FILE *f = fopen(filename, "a");
	if (!f) return;
	fprintf(f, "%-6d %-6s %-20s %s quits\n", moveNum, color, "Quit", color);
	fprintf(f, "End of game. %s quits.\n", color);
	fclose(f);
}


//Made by Mia Ness
//Draft1 4/19/26

#include "log.h"
#include <std.io.h>
#include <string.h>
#include <time.h>
/*creates or opens log file*/
void newLog(const char *filename){
	File *f = fopen(filename, "a");
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
	fprintf(f,"%-2d %-6s %-20s %s to %s\n", moveNum, color, piece, from, to);
	fclose(f);
}

/*add a capture to the log*/
void addCapture (const char *filename, int moveNum, const char *color, const char *attacker, const char *from, const char *captured, const char *){
	FILE *f = fopen(filename, "a");
	if (!f) return;
	fprintf(f, "%-2d %-6s %-20s %s captures %s at %s from %s\n", moveNum. color, "Capture", attacker, captured, at, from);
        fclose(f);
}
/*add castle move to the log*/
void addCastle (const char *filename, int moveNum, const char *color, int ){
	FILE *f = fopen(filename, "a");
	if (!f) return;
	fprintf(f, "%-2d %-6s %-20s %s castling\n", moveNum. color, "Castle", side);
        fclose(f);
}


//test

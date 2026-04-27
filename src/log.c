// Made by Mia Ness
// Draft1 4/19/26
#include "log.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
/*Returns a label for the position of a piece*/
static void label(Pos pos, char square[4])
{
	square[0] = (char)('a' + pos.col);
	square[1] = (char)('1' + pos.row);
	square[2] = '\0'; /*terminated*/
}

/*transforms rank enum into a string*/
static const char *rankConv(Rank rank)
{
	switch (rank)
	{
	case KING:
		return "K";
	case QUEEN:
		return "Q";
	case BISHOP:
		return "B";
	case KNIGHT:
		return "N";
	case ROOK:
		return "R";
	case ANTEATER:
		return "A";
	case PAWN:
		return " ";
	case EMPTY:
		return "E";
	default:
		return "N/a";
		;
	}
}

/*Convert color enum to string*/
static const char *colorConv(Color color)
{
	if (color == WHITE)
	{
		return "White";
	}
	else
		return "Black";
}

/*creates or opens log file*/
const char *newLog(const char *filename)
{
	FILE *f = fopen(filename, "a");
	if (!f)
	{
		fprintf(stderr, "Log file '%s' could not be opened\n", filename);
		return NULL;
	}
	/* get current time*/
	time_t current = time(NULL);
	/*convert time to readable format*/
	char *timetxt = ctime(&current);
	static char output[150];
	snprintf(output, 150, "\n Chess Game Log : %s \n%-2s %-6s %-20s %s\n-------------------------------------------\n", timetxt, "#", "Color", "Action", "Move");

	fprintf(f, "\n Chess Game Log : %s \n", timetxt);
	fprintf(f, "%-2s %-6s %s\n", "#", "Color", "Move");
	fprintf(f, "-------------------------------------------\n");
	fclose(f);
	return output;
}

/*add a move to the log*/
const char *addMove(const char *filename, int moveNum, Piece *piece, Pos to)
{
	FILE *f = fopen(filename, "a");
	if (!f)
		return NULL;
	char sq[4];
	label(to, sq);

	static char output[150];
	snprintf(output, 150, "%-2d %-6s %s%s\n", moveNum, colorConv(piece->color), rankConv(piece->type), sq);
	fprintf(f, "%-2d %-6s %s%s\n", moveNum, colorConv(piece->color), rankConv(piece->type), sq);
	fclose(f);
	return output;
}

/*add a capture to the log*/
const char *addCapture(const char *filename, int moveNum, Piece *piece, Pos from, Pos capturedAt)
{
	FILE *f = fopen(filename, "a");
	if (!f)
		return NULL;
	char sq[4];
	const char *pce;
	label(capturedAt, sq);
	static char col_str[2] = {0};

	if (piece->type == PAWN)
	{
		col_str[0] = (char)('a' + from.col);
		col_str[1] = '\0';
		pce = col_str;
	}
	else
	{
		pce = rankConv(piece->type);
	}

	pce = rankConv(piece->type);

	static char output[150];
	snprintf(output, 150, "%-2d %-6s %s%s%s\n", moveNum, colorConv(piece->color), pce, "x", sq);
	fprintf(f, "%s", output);
	fclose(f);
	return output;
}
/*add castle move to the log*/
const char *addCastle(const char *filename, int moveNum, Piece *piece, int sideNum)
{
	FILE *f = fopen(filename, "a");
	if (!f)
		return NULL;
	const char *lbl = (sideNum == 0) ? "0-0" : "0-0-0"; /*if king side, sideNum=0, if queen side, sideNum=1*/
	static char output[150];
	snprintf(output, 150, "%-2d %-6s %s\n", moveNum, colorConv(piece->color), lbl);
	fprintf(f, "%-2d %-6s %s\n", moveNum, colorConv(piece->color), lbl);
	fclose(f);
	return output;
}
/*add en passant to the log*/
const char *logEnPassant(const char *filename, int moveNum, Piece *piece, Pos pos)
{
	FILE *f = fopen(filename, "a");
	if (!f)
		return NULL;
	char sq[4];
	label(pos, sq);
	static char output[150];
	snprintf(output, 150, "%-2d %-6s %cx%se.p.\n", moveNum, colorConv(piece->color), (char)('a' + pos.col), sq);
	fprintf(f, "%s", output);
	fclose(f);
	return output;
}
/*add promotion to the log*/
const char *logPromotion(const char *filename, int moveNum, Piece *piece, Pos to, Piece *newPiece)
{
	FILE *f = fopen(filename, "a");
	if (!f)
		return NULL;
	char sq[4];
	label(to, sq);
	static char output[150];
	snprintf(output, 150, "%-2d %-6s %s%s\n", moveNum, colorConv(piece->color), sq, rankConv(newPiece->type));
	fprintf(f, "%-2d %-6s %s%s\n", moveNum, colorConv(piece->color), sq, rankConv(newPiece->type));
	fclose(f);
	return output;
}
/*add check to the log*/
const char *logCheck(const char *filename, int moveNum, Piece *piece)
{
	FILE *f = fopen(filename, "a");
	if (!f)
		return NULL;
	static char output[150];
	snprintf(output, 150, "%-2d %-6s +\n", moveNum, colorConv(piece->color));
	fprintf(f, "%-2d %-6s +\n", moveNum, colorConv(piece->color));
	fclose(f);
	return output;
}
/*add checkmate to the log*/
const char *logCheckmate(const char *filename, int moveNum, Piece *winner)
{
	FILE *f = fopen(filename, "a");
	if (!f)
		return NULL;
	static char output[150];
	snprintf(output, 150, "%-2d %-6s ++\nEnd of game. The winner is %s\n", moveNum, colorConv(winner->color), colorConv(winner->color));
	fprintf(f, "%-2d %-6s ++\n", moveNum, colorConv(winner->color));
	fprintf(f, "End of game. The winner is %s\n", colorConv(winner->color));
	fclose(f);
	return output;
}
/*add draw to the log*/
const char *logDraw(const char *filename, int moveNum)
{
	FILE *f = fopen(filename, "a");
	if (!f)
		return NULL;
	static char output[150];
	snprintf(output, 150, "%-2d %-6s (=)\nEnd of game. The game was a draw.\n", moveNum, " ");
	fprintf(f, "%-2d %-6s (=)\n", moveNum, " ");
	fprintf(f, "End of game. The game was a draw.\n");
	fclose(f);
	return output;
}
/*add a player quitting to the log*/
const char *logQuit(const char *filename, int moveNum, Piece *quitter)
{
	FILE *f = fopen(filename, "a");
	if (!f)
		return NULL;
	static char output[150];
	snprintf(output, 150, "%-2d %-6s resigns\nEnd of game. %s quits.\n", moveNum, colorConv(quitter->color), colorConv(quitter->color));
	fprintf(f, "%-2d %-6s resigns\n", moveNum, colorConv(quitter->color));
	fprintf(f, "End of game. %s quits.\n", colorConv(quitter->color));
	fclose(f);
	return output;
}

#include "input.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    INPUT_NONE = 0,
    INPUT_MOVE,
    INPUT_CASTLE_KINGSIDE,
    INPUT_CASTLE_QUEENSIDE,
    INPUT_UNDO,
    INPUT_QUIT
} InputKind;

static InputKind g_lastKind = INPUT_NONE;
static Pos g_lastStart = { 0, 0 };
static Pos g_lastEnd   = { 0, 0 };

static void clearState(void) {
    g_lastKind = INPUT_NONE;
    g_lastStart.row = 0;
    g_lastStart.col = 0;
    g_lastEnd.row = 0;
    g_lastEnd.col = 0;
}

static void trimWhitespace(const char *src, char *dst, size_t dstSize) {
    size_t i = 0;
    size_t j = 0;

    if (!src || !dst || dstSize == 0) {
        return;
    }

    while (src[i] != '\0' && isspace((unsigned char)src[i])) {
        i++;
    }

    while (src[i] != '\0' && j + 1 < dstSize) {
        dst[j++] = src[i++];
    }
    dst[j] = '\0';

    while (j > 0 && isspace((unsigned char)dst[j - 1])) {
        dst[j - 1] = '\0';
        j--;
    }
}

static void toUpperString(const char *src, char *dst, size_t dstSize) {
    size_t i = 0;

    if (!src || !dst || dstSize == 0) {
        return;
    }

    while (src[i] != '\0' && i + 1 < dstSize) {
        dst[i] = (char)toupper((unsigned char)src[i]);
        i++;
    }
    dst[i] = '\0';
}

static bool parseSquare(const char *s, Pos *p) {
    if (!s || !p) {
        return false;
    }

    if (strlen(s) != 2) {
        return false;
    }

    if (s[0] < 'A' || s[0] > 'J') {
        return false;
    }

    if (s[1] < '1' || s[1] > '8') {
        return false;
    }

    p->col = s[0] - 'A';
    p->row = s[1] - '1';
    return true;
}

static bool parseMoveTokens(const char *a, const char *b) {
    Pos start;
    Pos end;

    if (!parseSquare(a, &start)) {
        return false;
    }
    if (!parseSquare(b, &end)) {
        return false;
    }

    g_lastKind = INPUT_MOVE;
    g_lastStart = start;
    g_lastEnd = end;
    return true;
}

static bool parseCoordinateInput(const char *input) {
    char work[128];
    char a[16];
    char b[16];

    if (!input) {
        return false;
    }

    size_t i, j = 0;
    for (i = 0; input[i] != '\0' && j + 1 < sizeof(work); i++) {
        char c = input[i];

        if (c == '{' || c == '}' || c == ',') {
            work[j++] = ' ';
        } else {
            work[j++] = c;
        }
    }
    work[j] = '\0';

    if (sscanf(work, " %15s %15s", a, b) != 2) {
        return false;
    }

    return parseMoveTokens(a, b);
}

bool isValid(const char *input) {
    char trimmed[128];
    char upper[128];

    clearState();

    if (!input) {
        return false;
    }

    trimWhitespace(input, trimmed, sizeof(trimmed));
    toUpperString(trimmed, upper, sizeof(upper));

    if (upper[0] == '\0') {
        return false;
    }

    if (strcmp(upper, "O-O") == 0) {
        g_lastKind = INPUT_CASTLE_KINGSIDE;
        return true;
    }

    if (strcmp(upper, "O-O-O") == 0) {
        g_lastKind = INPUT_CASTLE_QUEENSIDE;
        return true;
    }

    if (strcmp(upper, "UNDO") == 0) {
        g_lastKind = INPUT_UNDO;
        return true;
    }

    if (strcmp(upper, "QUIT") == 0) {
        g_lastKind = INPUT_QUIT;
        return true;
    }

    if (parseCoordinateInput(upper)) {
        return true;
    }

    clearState();
    return false;
}

bool inputHasMove(void) {
    return g_lastKind == INPUT_MOVE;
}

bool inputIsCastleKingSide(void) {
    return g_lastKind == INPUT_CASTLE_KINGSIDE;
}

bool inputIsCastleQueenSide(void) {
    return g_lastKind == INPUT_CASTLE_QUEENSIDE;
}

bool inputIsUndo(void) {
    return g_lastKind == INPUT_UNDO;
}

bool inputIsQuit(void) {
    return g_lastKind == INPUT_QUIT;
}

Pos inputGetStartPos(void) {
    return g_lastStart;
}

Pos inputGetEndPos(void) {
    return g_lastEnd;
}

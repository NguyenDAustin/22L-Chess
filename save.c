#include "save.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


static void nameToPath(const char *saveName, char *path, int pathLen) {
    char safe[64];
    int  i = 0;

    while (*saveName && i < 63) {
        char c = *saveName++;
        safe[i++] = (c == ' ' || c == '/' || c == '\\') ? '_' : c;
    }
    safe[i] = '\0';

    snprintf(path, pathLen, "%s%s.txt", "save/", safe);
}

/*Save game state*/
const char *saveGame(const char *saveName, Board *board, Board_State *boardState, Board_Bundle *bundle, Color currentTurn){
    static char status[128];
    mkdir("save/", 0755);

    char filepath[128];
    nameToPath(saveName, path, sizeof(path));

    FILE *f = fopen(path, "w");
    if (!f) {
        snprintf(status, sizeof(status),
                 "Error: could not create save '%s'", saveName);
        return status; 
    }
    time_t current = time(NULL);
	char *timetxt = ctime(&current);

    fprintf(f, "Saved Anteater Chess Game: %s\n", saveName);
    fprintf(f, "Saved at: %s", timetxt);
    fprintf(f, "Current Turn: %s\n", currentTurn == WHITE ? "White" : "Black");
    fprintf(f, "Board State:\n");
    for (int row = 0; row < BOARD_HEIGHT; row++) {
        for (int col = 0; col < BOARD_WIDTH; col++) {
            Piece *piece = board->board[row][col];
            if (piece && piece->type != EMPTY) {
                fprintf(f, "%d %d %d %d\n", row, col, piece->color, piece->type);
            }
            else {
                fprintf(f, "%d %d EMPTY EMPTY\n", row, col); 
            }
        }
    }
    /*save log*/
    fprintf(f, "Log:\n");
    FILE *logFile = fopen(LOG_FILENAME, "r");
    if (logFile) {
        char line[128];
        while (fgets(line, sizeof(line), logFile)) {
            fprintf(f, "%s", line);
        }
    fclose(logFile);
    } else {
        fprintf(f, "No log available\n");
    }
    fprintf(f, "END\n");

    fclose(f);
    snprintf(status, sizeof(status), "Game saved as '%s'", saveName);
    return status;
} 


/*Load game*/
int loadGame(const char *saveName, Board *board, Board_State *boardState, Board_Bundle *bundle, Piece_Icon **images, Color *currentTurn){
    
    SaveEntry entry;
    if(findSave(saveName, &entry)){
        fprintf(stderr, "Save '%s' not found\n", saveName);
        return 0;
    }
    FILE *f = fopen(entry.filename, "r");
    if (!f){
        fprintf(stderr, "Save file '%s' could not be opened\n", entry.filename);
        return 0;
    }
    char line[128];

    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);
    char turnStr[8];
    fscanf(f, "TURN %s\n", turnStr);
    *currentTurn = (strcmp(turnStr, "WHITE") == 0) ? WHITE : BLACK;

    for(int row = 0; row < BOARD_HEIGHT; row++){
        for(int col = 0; col < BOARD_WIDTH; col++){
            Piece *piece = board->board[row][col];
            if(piece){
                free(piece);
                board->board[row][col] = NULL;
            }
        }
    }
    fscanf(f, "Board State:\n");
    for (int i = 0; i < BOARD_HEIGHT * BOARD_WIDTH; i++) {
        int  row, col, moved, value;
        char colorStr[8], rankStr[16];

        fscanf(f, "%d %d %s %s\n",
               &row, &col, colorStr, rankStr);
        if (strcmp(rankStr, "EMPTY") == 0) {
            setSquare(board, NULL, row, col);
        } else {
            Color color = (strcmp(colorStr, "WHITE") == 0) ? WHITE : BLACK;
            Rank rank = parseRank(rankStr);
            Piece *base = createPiece(color, rank);
            posCtor(&base.pos, row, col);
            base.img = getImagePiece(images, color, rank);
            Piece *p = createPiecePtr(p.img, color, rank, base.pos);

            addPiece(board, p, base.pos);
        }
    }
    fscanf(f, "Log:\n");
    FILE *logFile = fopen(entry.logFilename, "w");
    if (logFile) {
        while (fgets(line, sizeof(line), f)) {
            if (strcmp(line, "END\n", 3) == 0) {
                break;
            }
            fprintf(logFile, "%s", line);
        }
        fclose(logFile);
    }

    fclose(f);
    newLog(entry.logFilename);
    if (bundle && bundle->boardWidget) {
        gtk_widget_queue_draw(bundle->boardWidget);
    }
    return 1;
}


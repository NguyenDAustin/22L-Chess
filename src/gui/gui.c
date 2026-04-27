#include "gui.h"
#include "ai.h"

// global variables
const char FILES[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
const char *TITLE = "CHESS APP";
const char *CSS_CLASS = "chess-bg";
const int MIN_LOG_WIDTH = 300;
const int MIN_LOG_HEIGHT = 500;
const int GRID_COLUMN_SPACING = 50;
const int GRID_ROW_SPACING = 10;
const int PROMOTION_COLUMN_SPACING = 10;
const int PROMOTION_BUTTON_SIZE = 72;
const int LOG_SPACING = 10;

typedef struct
{
  Board_Bundle *boardData;
  GtkComboBoxText *gameModeCombo;
  GtkComboBoxText *colorCombo;
  GtkComboBoxText *difficultyCombo;
} StartupDialogData;

static bool finishGameIfNeeded(Board_Bundle *boardData, Color playerWhoMoved);
static void showEndGameDialog(Board_Bundle *boardData, const char *resultMsg);
static void onEndGameDialogResponse(GtkDialog *dialog, int responseId, gpointer user_data);
static void showStartupDialog(GtkWindow *parent, Board_Bundle *boardData);
static void onQuitConfirmed(GtkDialog *dialog, int responseId, gpointer user_data);

static GtkWindow *parentWindowForWidget(GtkWidget *widget)
{
  GtkWidget *toplevel = gtk_widget_get_toplevel(widget);
  return GTK_IS_WINDOW(toplevel) ? GTK_WINDOW(toplevel) : NULL;
}

static void addCssClass(GtkWidget *widget, const char *cssClass)
{
  gtk_style_context_add_class(gtk_widget_get_style_context(widget), cssClass);
}

static void removeCssClass(GtkWidget *widget, const char *cssClass)
{
  gtk_style_context_remove_class(gtk_widget_get_style_context(widget), cssClass);
}

void whichSquare(float x, float y)
{ // just for debug purposes
  int file = (x - LABEL_MARGIN) / SQUARE_SIZE;
  int rank = (y - LABEL_MARGIN) / SQUARE_SIZE;
  printf("square: %c%d\n", FILES[file], rank);
}

static const char *colorLabel(Color color)
{
  return color == WHITE ? "White" : "Black";
}

static Color oppositeColor(Color color)
{
  return color == WHITE ? BLACK : WHITE;
}

void appendTextToLogUI(Board_Bundle *boardData, const char *text) // Queency changed from static
{
  GtkTextIter end;
  gtk_text_buffer_get_end_iter(boardData->logBuffer, &end);
  gtk_text_buffer_insert(boardData->logBuffer, &end, text, -1);

  gtk_text_buffer_get_end_iter(boardData->logBuffer, &end);
  GtkTextMark *mark = gtk_text_buffer_create_mark(boardData->logBuffer, NULL, &end, FALSE);
  gtk_text_view_scroll_mark_onscreen(boardData->log, mark);
  gtk_text_buffer_delete_mark(boardData->logBuffer, mark);
}

static bool boardHasKing(Board *board, Color color)
{
  Pos kingPos = findKing(board, color);
  return kingPos.row >= 0 && kingPos.col >= 0;
}

static bool hasAnyMove(Board_Bundle *boardData, Color color)
{
  Board_State testState = *boardData->boardState;
  return possibleMove(&testState, boardData->board, color) != 0;
}

static bool finishGameIfNeeded(Board_Bundle *boardData, Color playerWhoMoved)
{
  Board_State *boardState = boardData->boardState;
  Color nextPlayer = oppositeColor(playerWhoMoved);

  if (!boardHasKing(boardData->board, nextPlayer))
  {
    char msg[80];
    snprintf(msg, sizeof(msg), "%s wins.\n", colorLabel(playerWhoMoved));
    appendTextToLogUI(boardData, msg);
    setGameOver(boardState, true);
    showEndGameDialog(boardData, msg);
    return true;
  }

  if (kingCheck(boardData->board, nextPlayer))
  {
    if (!hasAnyMove(boardData, nextPlayer))
    {
      char msg[96];
      snprintf(msg, sizeof(msg), "Checkmate. %s wins.\n", colorLabel(playerWhoMoved));
      appendTextToLogUI(boardData, msg);
      setGameOver(boardState, true);
      showEndGameDialog(boardData, msg);
      return true;
    }

    char msg[64];
    snprintf(msg, sizeof(msg), "%s is in check.\n", colorLabel(nextPlayer));
    appendTextToLogUI(boardData, msg);
  }
  else if (!hasAnyMove(boardData, nextPlayer))
  {
    appendTextToLogUI(boardData, "Stalemate.\n");
    setGameOver(boardState, true);
    showEndGameDialog(boardData, "Stalemate.");
    return true;
  }

  return false;
}

static void onEndGameDialogResponse(GtkDialog *dialog, int responseId, gpointer user_data)
{
  Board_Bundle *boardData = user_data;

  gtk_widget_destroy(GTK_WIDGET(dialog));

  if (responseId == GTK_RESPONSE_ACCEPT)
  {
    defaultInitializeBoard(boardData->board);
    initializeBoard(boardData->board, boardData->images);

    initializeBoardState(boardData->boardState);

    setWhiteSeconds(boardData, 60);
    setBlackSeconds(boardData, 60);
    updateTimerLabels(boardData);

    gtk_text_buffer_set_text(boardData->logBuffer, "", -1);

    gtk_widget_queue_draw(boardData->boardWidget);

    showStartupDialog(parentWindowForWidget(boardData->boardWidget), boardData);
  }
  else
  {
    GtkApplication *app = GTK_APPLICATION(g_application_get_default());
    g_application_quit(G_APPLICATION(app));
  }
}

static void showEndGameDialog(Board_Bundle *boardData, const char *resultMsg)
{
  GtkWidget *dialog = gtk_dialog_new();
  gtk_window_set_title(GTK_WINDOW(dialog), "Game Over");
  gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
  gtk_window_set_transient_for(GTK_WINDOW(dialog),
                               parentWindowForWidget(boardData->boardWidget));

  gtk_dialog_add_button(GTK_DIALOG(dialog), "New Game", GTK_RESPONSE_ACCEPT);
  gtk_dialog_add_button(GTK_DIALOG(dialog), "Quit", GTK_RESPONSE_REJECT);

  GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, LOG_SPACING);

  gtk_widget_set_margin_top(box, LOG_SPACING);
  gtk_widget_set_margin_bottom(box, LOG_SPACING);
  gtk_widget_set_margin_start(box, LOG_SPACING);
  gtk_widget_set_margin_end(box, LOG_SPACING);

  GtkWidget *resultLabel = gtk_label_new(resultMsg);

  char movesText[64];
  snprintf(movesText, sizeof(movesText), "Total moves: %d",
           getMovesMade(boardData->boardState));
  GtkWidget *movesLabel = gtk_label_new(movesText);

  gtk_box_pack_start(GTK_BOX(box), resultLabel, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), movesLabel, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(content), box, FALSE, FALSE, 0);

  g_signal_connect(dialog, "response",
                   G_CALLBACK(onEndGameDialogResponse), boardData);
  gtk_widget_show_all(dialog);
  gtk_window_present(GTK_WINDOW(dialog));
}

static void boardToPieceArray(Board *board, Piece state[8][10])
{
  for (int r = 0; r < BOARD_HEIGHT; r++)
  {
    for (int c = 0; c < BOARD_WIDTH; c++)
    {
      Piece *p = board->board[r][c];
      if (p != NULL)
      {
        state[r][c] = *p;
      }
      else
      {
        state[r][c].img = NULL;
        state[r][c].color = WHITE;
        state[r][c].type = EMPTY;
        state[r][c].vtable = NULL;
        state[r][c].value = 0;
        state[r][c].pos.row = r;
        state[r][c].pos.col = c;
        state[r][c].moved = 0;
      }
    }
  }
}

static void resetTurnTimer(Board_Bundle *boardData)
{
  bool whiteTurnNext = getMovesMade(boardData->boardState) % 2 == 0;
  if (whiteTurnNext)
    boardData->whiteSeconds = 60;
  else
    boardData->blackSeconds = 60;
}

static void triggerCpuMove(Board_Bundle *boardData)
{
  Board_State *boardState = boardData->boardState;
  Board *board = boardData->board;
  int movesMade = getMovesMade(boardState);
  Color cpuColor = boardData->cpuColor;

  if (isGameOver(boardState))
    return;

  /* White plays on even move counts, black on odd */
  int cpuParity = (cpuColor == BLACK) ? 1 : 0;
  if (movesMade % 2 != cpuParity)
    return;

  Piece state[8][10];
  boardToPieceArray(board, state);

  Move cpuMove;
  if (!aiSelectMoveAtDifficulty(state, cpuColor, boardData->cpuDifficulty, &cpuMove))
  {
    if (kingCheck(boardData->board, cpuColor))
    {
      char msg[96];
      snprintf(msg, sizeof(msg), "Checkmate. %s wins.\n", colorLabel(boardData->userColor));
      appendTextToLogUI(boardData, msg);
      setGameOver(boardState, true);
      showEndGameDialog(boardData, msg);
    }
    else
    {
      appendTextToLogUI(boardData, "Stalemate.\n");
      setGameOver(boardState, true);
      showEndGameDialog(boardData, "Stalemate.");
    }
    return;
  }

  // pushMoveForUndo(board, &cpuMove);

  executeMove(board, &cpuMove, boardState->lastMove);
  boardState->lastMove = cpuMove;
  incrementMovesMade(boardState);
  resetTurnTimer(boardData);
  setUpdate(boardState, true);

  // char msg[64];
  /* snprintf(msg, sizeof(msg), "CPU: %c%d -> %c%d\n",
            'A' + cpuMove.startCol, cpuMove.startRow + 1,
            'A' + cpuMove.endCol, cpuMove.endRow + 1); */
  // queency commented out

  // appendTextToLogUI(boardData, msg);

  if (cpuMove.capture)
  {
    printf("cpu capture was made\n");
    Pos capturePos;
    Pos start;
    posCtor(&capturePos, cpuMove.endRow, cpuMove.endCol);
    posCtor(&start, cpuMove.startRow, cpuMove.startCol);
    Piece *movedPiece = board->board[cpuMove.endRow][cpuMove.endCol];
    boardData->move = addCapture(LOG_FILE, getMovesMade(boardState), movedPiece, getPos(movedPiece), capturePos);
  }
  else if (cpuMove.castle)
  {
    printf("cpu castle was made\n");
    int sideNum = (cpuMove.endCol > cpuMove.startCol) ? 0 : 1;
    Piece *movedPiece = board->board[cpuMove.endRow][cpuMove.endCol];
    boardData->move = addCastle(LOG_FILE, getMovesMade(boardState), movedPiece, sideNum);
  }
  else if (cpuMove.enPassant)
  {
    printf("cpu en passant capture was made\n");
    Pos enPassantPos;
    posCtor(&enPassantPos, cpuMove.endRow, cpuMove.endCol);
    Piece *movedPiece = board->board[cpuMove.endRow][cpuMove.endCol];
    boardData->move = logEnPassant(LOG_FILE, getMovesMade(boardState), movedPiece, enPassantPos);
  }
  else
  {
    printf("cpu move was made\n");
    Pos pos;
    posCtor(&pos, cpuMove.endRow, cpuMove.endCol);
    Piece *movedPiece = board->board[cpuMove.endRow][cpuMove.endCol];
    boardData->move = addMove(LOG_FILE, getMovesMade(boardState), movedPiece, pos);
  }
  appendTextToLogUI(boardData, boardData->move);

  finishGameIfNeeded(boardData, cpuColor);
}

static gboolean cpuMoveCallback(gpointer user_data)
{
  Board_Bundle *boardData = user_data;
  triggerCpuMove(boardData);
  gtk_widget_queue_draw(boardData->boardWidget);
  updateTimerLabels(boardData);
  return G_SOURCE_REMOVE;
}

static const char *difficultyLabel(AIDifficulty d)
{
  switch (d)
  {
  case AI_EASY:
    return "Easy";
  case AI_HARD:
    return "Hard";
  default:
    return "Medium";
  }
}

static void onGameModeChanged(GtkComboBoxText *combo, gpointer user_data)
{
  GtkWidget *difficultyBox = user_data;
  int modeIndex = gtk_combo_box_get_active(GTK_COMBO_BOX(combo));
  gtk_widget_set_sensitive(difficultyBox, modeIndex == 0);
}

static void onStartupDialogResponse(GtkDialog *dialog, int responseId, gpointer user_data)
{
  StartupDialogData *dialogData = user_data;
  Board_Bundle *boardData = dialogData->boardData;

  if (responseId == GTK_RESPONSE_ACCEPT)
  {
    int modeIndex = gtk_combo_box_get_active(GTK_COMBO_BOX(dialogData->gameModeCombo));
    boardData->humanVsHuman = (modeIndex == 1);

    /* White always moves first; timer offset tracks whose turn it is. */
    setMovesMade(boardData->boardState, 0);
    updateTimerLabels(boardData);

    if (boardData->humanVsHuman)
    {
      boardData->userColor = WHITE;
      boardData->cpuColor = BLACK;
      boardData->userStarts = true;
      appendTextToLogUI(boardData, "Game setup: Human vs Human. White starts.\n");
    }
    else
    {
      int colorIndex = gtk_combo_box_get_active(GTK_COMBO_BOX(dialogData->colorCombo));
      int difficultyIndex = gtk_combo_box_get_active(GTK_COMBO_BOX(dialogData->difficultyCombo));

      boardData->userColor = colorIndex == 0 ? WHITE : BLACK;
      boardData->cpuColor = boardData->userColor == WHITE ? BLACK : WHITE;
      boardData->userStarts = boardData->userColor == WHITE;

      const AIDifficulty difficulties[3] = {AI_EASY, AI_MEDIUM, AI_HARD};
      boardData->cpuDifficulty = difficulties[difficultyIndex];

      char message[160];
      snprintf(message, sizeof(message),
               "Game setup: You are %s. CPU is %s (%s difficulty). White starts.\n",
               colorLabel(boardData->userColor),
               colorLabel(boardData->cpuColor),
               difficultyLabel(boardData->cpuDifficulty));
      appendTextToLogUI(boardData, message);
    }
  }

  if (boardData->timerSourceId == 0)
  {
    boardData->timerSourceId = g_timeout_add_seconds(1, onTimerTick, boardData);
  }

  gtk_widget_destroy(GTK_WIDGET(dialog));
  g_free(dialogData);

  /* If user chose black in CPU mode, CPU (white) must move first */
  if (!boardData->humanVsHuman && !boardData->userStarts && !isGameOver(boardData->boardState))
  {
    triggerCpuMove(boardData);
    gtk_widget_queue_draw(boardData->boardWidget);
    updateTimerLabels(boardData);
  }
}

static void showStartupDialog(GtkWindow *parent, Board_Bundle *boardData)
{
  GtkWidget *dialog = gtk_dialog_new();
  gtk_window_set_title(GTK_WINDOW(dialog), "Game Setup");
  gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
  gtk_window_set_transient_for(GTK_WINDOW(dialog), parent);
  gtk_dialog_add_button(GTK_DIALOG(dialog), "Start Game", GTK_RESPONSE_ACCEPT);

  GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, LOG_SPACING);

  GtkWidget *modeLabelWidget = gtk_label_new("Game Mode");
  GtkWidget *gameModeCombo = gtk_combo_box_text_new();
  GtkWidget *colorLabelWidget = gtk_label_new("Your color (White always moves first)");
  GtkWidget *colorCombo = gtk_combo_box_text_new();
  GtkWidget *difficultyLabelWidget = gtk_label_new("CPU Difficulty");
  GtkWidget *difficultyCombo = gtk_combo_box_text_new();
  GtkWidget *difficultyBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);

  gtk_widget_set_margin_top(box, LOG_SPACING);
  gtk_widget_set_margin_bottom(box, LOG_SPACING);
  gtk_widget_set_margin_start(box, LOG_SPACING);
  gtk_widget_set_margin_end(box, LOG_SPACING);

  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gameModeCombo), "Human vs CPU");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gameModeCombo), "Human vs Human");
  gtk_combo_box_set_active(GTK_COMBO_BOX(gameModeCombo), 0);

  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(colorCombo), "White");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(colorCombo), "Black");
  gtk_combo_box_set_active(GTK_COMBO_BOX(colorCombo), 0);

  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(difficultyCombo), "Easy");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(difficultyCombo), "Medium");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(difficultyCombo), "Hard");
  gtk_combo_box_set_active(GTK_COMBO_BOX(difficultyCombo), 1);

  gtk_box_pack_start(GTK_BOX(difficultyBox), difficultyLabelWidget, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(difficultyBox), difficultyCombo, FALSE, FALSE, 0);

  g_signal_connect(gameModeCombo, "changed", G_CALLBACK(onGameModeChanged), difficultyBox);

  gtk_box_pack_start(GTK_BOX(box), modeLabelWidget, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), gameModeCombo, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), colorLabelWidget, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), colorCombo, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), difficultyBox, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(content), box, FALSE, FALSE, 0);

  StartupDialogData *dialogData = g_new0(StartupDialogData, 1);
  dialogData->boardData = boardData;
  dialogData->gameModeCombo = GTK_COMBO_BOX_TEXT(gameModeCombo);
  dialogData->colorCombo = GTK_COMBO_BOX_TEXT(colorCombo);
  dialogData->difficultyCombo = GTK_COMBO_BOX_TEXT(difficultyCombo);

  g_signal_connect(dialog, "response", G_CALLBACK(onStartupDialogResponse), dialogData);
  gtk_widget_show_all(dialog);
  gtk_window_present(GTK_WINDOW(dialog));
}

void setBackground(GdkScreen *screen, GtkCssProvider *provider, const char *BG_CSS)
{
  gtk_css_provider_load_from_data(provider, BG_CSS, -1, NULL);
  gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

GtkWidget *createWindow(GtkApplication *app, const char *title, const char *cssClass)
{
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), title);
  gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_HEIGHT, WINDOW_WIDTH);
  addCssClass(window, cssClass);
  return window;
}

GtkWidget *createPromotionGrid(GtkWidget **buttons)
{
  GtkWidget *grid = createGrid(PROMOTION_COLUMN_SPACING);
  gridAttacher(grid, buttons, NUMBER_OF_PROMOTION_BUTTONS);
  return grid;
}

GtkWidget *createMainGrid()
{
  return createGrid(GRID_COLUMN_SPACING);
}

GtkWidget *createGrid(int colSpacing)
{
  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_column_spacing(GTK_GRID(grid), colSpacing);
  gtk_grid_set_row_spacing(GTK_GRID(grid), GRID_ROW_SPACING);
  gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
  return grid;
}

GtkWidget *createTimerBox(GtkWidget *whiteTimer, GtkWidget *blackTimer)
{
  GtkWidget *timerBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, LOG_SPACING);
  addCssClass(timerBox, "timer-box");
  addCssClass(whiteTimer, "timer-label");
  addCssClass(blackTimer, "timer-label");

  gtk_widget_set_vexpand(timerBox, FALSE);
  gtk_widget_set_valign(timerBox, GTK_ALIGN_START);
  gtk_widget_set_halign(timerBox, GTK_ALIGN_CENTER);

  gtk_label_set_xalign(GTK_LABEL(whiteTimer), 0.5);
  gtk_label_set_xalign(GTK_LABEL(blackTimer), 0.5);
  gtk_box_pack_start(GTK_BOX(timerBox), whiteTimer, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(timerBox), blackTimer, FALSE, FALSE, 0);
  return timerBox;
}

void createBoard(GtkWidget *board, Board_Bundle *boardData)
{
  gtk_widget_set_size_request(board, SQUARE_SIZE * BOARD_WIDTH + LABEL_MARGIN + 20, SQUARE_SIZE * BOARD_HEIGHT + LABEL_MARGIN + 20);
  g_signal_connect(board, "draw", G_CALLBACK(drawBoard), boardData);
  gtk_widget_set_hexpand(board, FALSE);
  gtk_widget_set_halign(board, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(board, GTK_ALIGN_CENTER);
}

void updateTimerLabels(Board_Bundle *boardData)
{
  int whiteMinutes = boardData->whiteSeconds / 60;
  int whiteSeconds = boardData->whiteSeconds % 60;
  int blackMinutes = boardData->blackSeconds / 60;
  int blackSeconds = boardData->blackSeconds % 60;
  bool whiteTurn = getMovesMade(boardData->boardState) % 2 == 0;
  char whiteText[32];
  char blackText[32];

  snprintf(whiteText, sizeof(whiteText), "White: %02d:%02d", whiteMinutes, whiteSeconds);
  snprintf(blackText, sizeof(blackText), "Black: %02d:%02d", blackMinutes, blackSeconds);

  gtk_label_set_text(boardData->whiteTimerLabel, whiteText);
  gtk_label_set_text(boardData->blackTimerLabel, blackText);

  if (whiteTurn)
  {
    addCssClass(GTK_WIDGET(boardData->whiteTimerLabel), "timer-active");
    removeCssClass(GTK_WIDGET(boardData->blackTimerLabel), "timer-active");
  }
  else
  {
    removeCssClass(GTK_WIDGET(boardData->whiteTimerLabel), "timer-active");
    addCssClass(GTK_WIDGET(boardData->blackTimerLabel), "timer-active");
  }
}

gboolean onTimerTick(gpointer user_data)
{
  Board_Bundle *boardData = user_data;

  if (isGameOver(boardData->boardState))
    return G_SOURCE_CONTINUE;

  bool whiteTurn = getMovesMade(boardData->boardState) % 2 == 0;

  if (whiteTurn)
  {
    if (boardData->whiteSeconds > 0)
      boardData->whiteSeconds--;
    if (boardData->whiteSeconds == 0)
    {
      appendTextToLogUI(boardData, "White ran out of time. Black wins.\n");
      setGameOver(boardData->boardState, true);
      showEndGameDialog(boardData, "White ran out of time. Black wins.");
    }
  }
  else
  {
    if (boardData->blackSeconds > 0)
      boardData->blackSeconds--;
    if (boardData->blackSeconds == 0)
    {
      appendTextToLogUI(boardData, "Black ran out of time. White wins.\n");
      setGameOver(boardData->boardState, true);
      showEndGameDialog(boardData, "Black ran out of time. White wins.");
    }
  }

  updateTimerLabels(boardData);
  return G_SOURCE_CONTINUE;
}

void createLogScroller(GtkWidget *logScroller)
{
  gtk_widget_set_size_request(logScroller, MIN_LOG_WIDTH, -1);
}

void createLog(GtkWidget *logScroller, GtkWidget *log)
{
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(log), LOG_SPACING);
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(log), LOG_SPACING);
  gtk_text_view_set_top_margin(GTK_TEXT_VIEW(log), LOG_SPACING);
  gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(log), LOG_SPACING);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(log), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(log), FALSE);
  gtk_container_add(GTK_CONTAINER(logScroller), log);
  gtk_widget_set_size_request(logScroller, MIN_LOG_WIDTH, 700);
}

void createBlackButtons(Board_Bundle *boardData, GtkWidget **buttons)
{
  const char *blackImages[NUMBER_OF_PROMOTION_BUTTONS] = {PIECE_RESOURCES[BLACK_QUEEN], PIECE_RESOURCES[BLACK_ROOK], PIECE_RESOURCES[BLACK_BISHOP], PIECE_RESOURCES[BLACK_KNIGHT], PIECE_RESOURCES[BLACK_ANTEATER]};
  initializePromotionButtonArr(boardData, buttons, blackImages);
}

void createWhiteButtons(Board_Bundle *boardData, GtkWidget **buttons)
{
  const char *whiteImages[NUMBER_OF_PROMOTION_BUTTONS] = {PIECE_RESOURCES[WHITE_QUEEN], PIECE_RESOURCES[WHITE_ROOK], PIECE_RESOURCES[WHITE_BISHOP], PIECE_RESOURCES[WHITE_KNIGHT], PIECE_RESOURCES[WHITE_ANTEATER]};
  initializePromotionButtonArr(boardData, buttons, whiteImages);
}

void createPopUp(Board_Bundle *boardData)
{
  Board_State *boardState = boardData->boardState;
  Piece *promoting = boardState->clickedPiece;
  GtkWidget *boardWidget = boardData->boardWidget;
  Color pieceColor = getColor(promoting);
  GtkWidget *popUp = boardData->promotionPopUp;
  GtkWidget *promotionGrid;
  Pos promotingPos;
  GdkRectangle rect;

  if (promoting == NULL)
    return;

  promotingPos = getPos(promoting);
  rect.x = indexToPix(promotingPos.col);
  rect.y = indexToPix(promotingPos.row);
  rect.width = SQUARE_SIZE;
  rect.height = SQUARE_SIZE;

  GtkWidget *buttons[NUMBER_OF_PROMOTION_BUTTONS];
  (pieceColor == WHITE) ? createWhiteButtons(boardData, buttons) : createBlackButtons(boardData, buttons);
  (pieceColor == WHITE) ? gtk_popover_set_position(GTK_POPOVER(popUp), GTK_POS_TOP) : gtk_popover_set_position(GTK_POPOVER(popUp), GTK_POS_BOTTOM);
  gtk_popover_set_relative_to(GTK_POPOVER(popUp), boardWidget);
  gtk_popover_set_pointing_to(GTK_POPOVER(popUp), &rect);
  promotionGrid = createPromotionGrid(buttons);
  GtkWidget *oldChild = gtk_bin_get_child(GTK_BIN(popUp));
  if (oldChild != NULL)
    gtk_container_remove(GTK_CONTAINER(popUp), oldChild);
  gtk_container_add(GTK_CONTAINER(popUp), promotionGrid);
  gtk_widget_set_sensitive(boardWidget, TRUE);
  gtk_widget_show_all(popUp);
  gtk_popover_popup(GTK_POPOVER(popUp));
}

gboolean onClick(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
  double x = event->x;
  double y = event->y;

  if (x < LABEL_MARGIN || y < LABEL_MARGIN) // clicked on label, not square
    return FALSE;

  Board_Bundle *boardData = user_data;
  GtkWidget *boardWidget = boardData->boardWidget;
  Board_State *boardState = boardData->boardState;

  if (isGameOver(boardState))
    return TRUE;

  if (isPromotion(boardState))
    return TRUE;

  int movesMade = getMovesMade(boardState);
  Color toMove = (movesMade % 2 == 0) ? WHITE : BLACK;
  if (!boardData->humanVsHuman && toMove == boardData->cpuColor)
    return TRUE;

  whichSquare(x, y);

  int row = pixToIndex(y);
  int col = pixToIndex(x);

  if (col < 0 || col >= BOARD_WIDTH || row < 0 || row >= BOARD_HEIGHT)
    return FALSE;

  Pos clickPos;
  posCtor(&clickPos, row, col);

  sendInput(boardData, clickPos);

  if (hasUpdate(boardState))
    gtk_widget_queue_draw(boardWidget);

  if (moveSucces(boardState))
  {
    resetTurnTimer(boardData);
    updateTimerLabels(boardData);
    appendToLogUI(boardData);

    if (isPromotion(boardState))
    {
      gtk_widget_queue_draw(boardWidget);
      createPopUp(boardData);
      return TRUE;
    }

    if (finishGameIfNeeded(boardData, toMove))
    {
      if (hasUpdate(boardState))
        gtk_widget_queue_draw(boardWidget);
      updateTimerLabels(boardData);
      return TRUE;
    }

    if (!boardData->humanVsHuman)
      g_timeout_add_seconds(1, cpuMoveCallback, boardData);
    if (hasUpdate(boardState))
      gtk_widget_queue_draw(boardWidget);
    updateTimerLabels(boardData);
  }

  return TRUE;
}

void onPromotionClicked(GtkButton *button, gpointer user_data)
{
  Rank promoteTo = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "promoteRank"));
  Board_Bundle *boardData = user_data;
  Board_State *boardState = getBoardState(boardData);
  Pos clickPiecePos = getPos(boardState->clickedPiece);
  GtkWidget *popUp = boardData->promotionPopUp;

  int movesMadeBeforePromo = getMovesMade(boardState);
  Color promotingColor = (movesMadeBeforePromo % 2 == 0) ? WHITE : BLACK;

  promotePiece(boardData, clickPiecePos, promoteTo);
  incrementMovesMade(boardState);
  resetTurnTimer(boardData);
  resetClickedPiece(boardState);
  gtk_widget_queue_draw(boardData->boardWidget);
  gtk_popover_popdown(GTK_POPOVER(popUp));

  Color movedColor = boardData->humanVsHuman ? promotingColor : boardData->userColor;
  if (!finishGameIfNeeded(boardData, movedColor))
  {
    if (!boardData->humanVsHuman)
      g_timeout_add_seconds(1, cpuMoveCallback, boardData);
  }
  updateTimerLabels(boardData);
}

void onUndoClicked(GtkButton *button, gpointer user_data)
{
  Board_Bundle *boardData = user_data;
  Board *board = getBoard(boardData);
  Board_State *boardState = getBoardState(boardData);
  GtkWidget *boardWidget = getBoardWidget(boardData);

  printf("undo was clicked\n");
  Undo_Record undoMove;

  if (undoPop(&undoMove))
  {
    printUndoMove(&undoMove);
    undo(&undoMove, board);
    setMovesMade(boardState, getMovesMade(boardState) - 1); //reverts turn
    setUpdate(boardState, true);
    gtk_widget_queue_draw(boardWidget);
  }
}

GtkWidget *createPromotionButton(Board_Bundle *boardData, const char *imagePath, Rank type)
{
  GtkWidget *button = gtk_button_new();
  GtkWidget *picture = gtk_image_new_from_file(imagePath);
  gtk_widget_set_size_request(picture, PROMOTION_BUTTON_SIZE, PROMOTION_BUTTON_SIZE);
  gtk_button_set_image(GTK_BUTTON(button), picture);
  gtk_button_set_always_show_image(GTK_BUTTON(button), TRUE);
  g_object_set_data(G_OBJECT(button), "promoteRank", GINT_TO_POINTER(type));
  g_signal_connect(button, "clicked", G_CALLBACK(onPromotionClicked), boardData);
  return button;
}

void appendToLogUI(Board_Bundle *boardData)
{
  /* Special messages that aren't coordinate-based (e.g. Anteater extra turn) */
  if (boardData->move && boardData->move[0] == 'A')
  {
    appendTextToLogUI(boardData, boardData->move);
    return;
  }

  /*char msg[96];
  snprintf(msg, sizeof(msg), "%s: %c%d -> %c%d%s\n",
           colorLabel(movedColor),
           'A' + last.startCol, last.startRow + 1,
           'A' + last.endCol, last.endRow + 1,
           suffix);*/
  // appendTextToLogUI(boardData, msg); //QUEENCY

  printf("HERE IN APPENDING TO LOG UI\n");

  appendTextToLogUI(boardData, boardData->move);
}

void gridAttacher(GtkWidget *grid, GtkWidget **attachments, int size)
{
  int row = 0;
  for (int col = 0; col < size; col++)
  {
    gtk_grid_attach(GTK_GRID(grid), attachments[col], col, row, 1, 1);
  }
}

void initializePromotionButtonArr(Board_Bundle *boardData, GtkWidget **buttons, const char **images)
{
  Rank type[NUMBER_OF_PROMOTION_BUTTONS] = {QUEEN, ROOK, BISHOP, KNIGHT, ANTEATER};
  for (int i = 0; i < NUMBER_OF_PROMOTION_BUTTONS; i++)
  {
    buttons[i] = createPromotionButton(boardData, images[i], type[i]);
  }
}

static void onQuitClicked(GtkButton *button, gpointer user_data)
{
  Board_Bundle *boardData = user_data;
  GtkWidget *dialog = gtk_dialog_new();
  gtk_window_set_title(GTK_WINDOW(dialog), "Quit");
  gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
  gtk_window_set_transient_for(GTK_WINDOW(dialog),
                               parentWindowForWidget(boardData->boardWidget));

  gtk_dialog_add_button(GTK_DIALOG(dialog), "Yes", GTK_RESPONSE_ACCEPT);
  gtk_dialog_add_button(GTK_DIALOG(dialog), "No", GTK_RESPONSE_REJECT);

  GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  GtkWidget *label = gtk_label_new("Are you sure you want to quit?");
  gtk_widget_set_margin_top(label, LOG_SPACING);
  gtk_widget_set_margin_bottom(label, LOG_SPACING);
  gtk_widget_set_margin_start(label, LOG_SPACING);
  gtk_widget_set_margin_end(label, LOG_SPACING);
  gtk_box_pack_start(GTK_BOX(content), label, FALSE, FALSE, 0);

  g_signal_connect(dialog, "response", G_CALLBACK(onQuitConfirmed), boardData);
  gtk_widget_show_all(dialog);
  gtk_window_present(GTK_WINDOW(dialog));
}

static void onQuitConfirmed(GtkDialog *dialog, int responseId, gpointer user_data)
{
  gtk_widget_destroy(GTK_WIDGET(dialog));
  if (responseId == GTK_RESPONSE_ACCEPT)
  {
    GtkApplication *app = GTK_APPLICATION(g_application_get_default());
    g_application_quit(G_APPLICATION(app));
  }
}

static void activate(GtkApplication *app, gpointer user_data)
{
  GtkWidget *grid = createMainGrid();

  GtkWidget *window = createWindow(app, TITLE, CSS_CLASS);
  GtkWidget *board = gtk_drawing_area_new();
  GtkWidget *logScroller = gtk_scrolled_window_new(NULL, NULL);
  GtkWidget *log = gtk_text_view_new();
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(log));

  GtkWidget *promotionPop = gtk_popover_new(board);

  Board_Bundle *boardData = user_data;
  setBoardWidget(boardData, board);
  setLogTextView(boardData, GTK_TEXT_VIEW(log));
  setLogBuffer(boardData, buffer);
  setPromotionPop(boardData, promotionPop);

  // creation of timer
  GtkWidget *whiteTimer = gtk_label_new(NULL);
  GtkWidget *blackTimer = gtk_label_new(NULL);
  GtkWidget *timerBox = createTimerBox(whiteTimer, blackTimer);

  setWhiteTimerLabel(boardData, GTK_LABEL(whiteTimer));
  setBlackTimerLabel(boardData, GTK_LABEL(blackTimer));
  setBlackSeconds(boardData, 60);
  setWhiteSeconds(boardData, 60);
  updateTimerLabels(boardData);

  // setting background
  GdkScreen *screen = gdk_screen_get_default();
  GtkCssProvider *provider = gtk_css_provider_new();
  setBackground(screen, provider, CHESS_BG);

  // creating board
  createBoard(board, boardData);

  // creating log scroller container
  createLogScroller(logScroller);

  // creating log
  createLog(logScroller, log);

  // create undo button
  GtkWidget *undoButton = gtk_button_new();
  GtkWidget *picture = gtk_image_new_from_file("src/gui/resources/undo.png");
  gtk_widget_set_size_request(picture, 20, 20);
  gtk_button_set_image(GTK_BUTTON(undoButton), picture);
  gtk_button_set_always_show_image(GTK_BUTTON(undoButton), TRUE);
  g_signal_connect(undoButton, "clicked", G_CALLBACK(onUndoClicked), boardData);
  // gtk_widget_set_halign(undoButton, GTK_ALIGN_END);
  // gtk_widget_set_valign(undoButton, GTK_ALIGN_START);

  GtkWidget *quitButton = gtk_button_new();
  GtkWidget *quitPicture = gtk_image_new_from_file("src/gui/resources/quit.png");
  gtk_widget_set_size_request(quitPicture, 20, 20);
  gtk_button_set_image(GTK_BUTTON(quitButton), quitPicture);
  gtk_button_set_always_show_image(GTK_BUTTON(quitButton), TRUE);
  g_signal_connect(quitButton, "clicked", G_CALLBACK(onQuitClicked), boardData);
  // gtk_widget_set_halign(quitButton, GTK_ALIGN_END);
  // gtk_widget_set_valign(quitButton, GTK_ALIGN_START);

  gtk_widget_add_events(board, GDK_BUTTON_PRESS_MASK);
  g_signal_connect(board, "button-press-event", G_CALLBACK(onClick), boardData);

  g_object_unref(provider);

  GtkWidget *buttonBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, LOG_SPACING);
  gtk_widget_set_halign(buttonBox, GTK_ALIGN_END);
  gtk_widget_set_valign(buttonBox, GTK_ALIGN_START);
  gtk_box_pack_start(GTK_BOX(buttonBox), undoButton, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(buttonBox), quitButton, FALSE, FALSE, 0);
  gtk_widget_set_margin_bottom(buttonBox, 10);

  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_grid_attach(GTK_GRID(grid), board, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), timerBox, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), logScroller, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), buttonBox, 1, 1, 1, 1);

  gtk_widget_show_all(window);
  gtk_window_present(GTK_WINDOW(window));
  showStartupDialog(GTK_WINDOW(window), boardData);
}

int main(int argc, char **argv)
{
  GtkApplication *app;
  int status;

  int numOfImages = 14;

  Board_Bundle boardData;
  Board board;
  boardData.timerSourceId = 0;
  boardData.whiteSeconds = 0;
  boardData.blackSeconds = 0;
  boardData.userColor = WHITE;
  boardData.cpuColor = BLACK;
  boardData.userStarts = true;
  boardData.humanVsHuman = false;
  boardData.cpuDifficulty = AI_MEDIUM;
  boardData.move = NULL;

  // set images
  cairo_surface_t *images[numOfImages];
  createImages(images, numOfImages);
  setImages(&boardData, images);

  // set board
  defaultInitializeBoard(&board);
  initializeBoard(&board, images);
  setBoard(&boardData, &board);

  // set board state
  Board_State boardState;
  initializeBoardState(&boardState);
  setBoardState(&boardData, &boardState);

  app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), &boardData);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}

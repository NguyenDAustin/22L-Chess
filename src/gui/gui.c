
#include "gui.h"



//global variables 
const char FILES[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'}; 
const char* TITLE = "CHESS APP"; 
const char* CSS_CLASS = "chess-bg"; 
const int MIN_LOG_WIDTH = 300; 
const int MIN_LOG_HEIGHT = 500;
const int GRID_COLUMN_SPACING = 50;  
const int GRID_ROW_SPACING = 10;
const int LOG_SPACING = 10; 

typedef struct {
  Board_Bundle* boardData;
  GtkComboBoxText* firstMoveCombo;
  GtkComboBoxText* colorCombo;
} StartupDialogData;

void whichSquare(float x, float y){ //just for debug purposes
  int file = x / SQUARE_SIZE; 
  int rank =  y / SQUARE_SIZE; 
  printf("square: %c%d\n", FILES[file], rank); 
}

static const char* colorLabel(Color color){
  return color == WHITE ? "White" : "Black";
}

static void appendTextToLogUI(Board_Bundle* boardData, const char* text){
  GtkTextIter end;
  gtk_text_buffer_get_end_iter(boardData->logBuffer, &end);
  gtk_text_buffer_insert(boardData->logBuffer, &end, text, -1);

  gtk_text_buffer_get_end_iter(boardData->logBuffer, &end);
  GtkTextMark *mark = gtk_text_buffer_create_mark(boardData->logBuffer, NULL, &end, FALSE);
  gtk_text_view_scroll_mark_onscreen(boardData->log, mark);
  gtk_text_buffer_delete_mark(boardData->logBuffer, mark);
}

static void onStartupDialogResponse(GtkDialog* dialog, int responseId, gpointer user_data){
  StartupDialogData* dialogData = user_data;
  Board_Bundle* boardData = dialogData->boardData;

  if(responseId == GTK_RESPONSE_ACCEPT){
    int firstMoveIndex = gtk_combo_box_get_active(GTK_COMBO_BOX(dialogData->firstMoveCombo));
    int colorIndex = gtk_combo_box_get_active(GTK_COMBO_BOX(dialogData->colorCombo));

    boardData->userStarts = firstMoveIndex == 0;
    boardData->userColor = colorIndex == 0 ? WHITE : BLACK;
    boardData->cpuColor = boardData->userColor == WHITE ? BLACK : WHITE;

    Color firstColor = boardData->userStarts ? boardData->userColor : boardData->cpuColor;
    setMovesMade(boardData->boardState, firstColor == WHITE ? 0 : 1);
    updateTimerLabels(boardData);

    char message[160];
    snprintf(message, sizeof(message), "Game setup: %s starts. You are %s, CPU is %s.\n",
             boardData->userStarts ? "User" : "CPU",
             colorLabel(boardData->userColor),
             colorLabel(boardData->cpuColor));
    appendTextToLogUI(boardData, message);

    if(!boardData->userStarts){
      appendTextToLogUI(boardData, "CPU start selected. CPU moves are not implemented in the GUI yet.\n");
    }
  }

  if(boardData->timerSourceId == 0){
    boardData->timerSourceId = g_timeout_add_seconds(1, onTimerTick, boardData);
  }

  gtk_window_destroy(GTK_WINDOW(dialog));
  g_free(dialogData);
}

static void showStartupDialog(GtkWindow* parent, Board_Bundle* boardData){
  GtkWidget* dialog = gtk_dialog_new();
  gtk_window_set_title(GTK_WINDOW(dialog), "Game Setup");
  gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
  gtk_window_set_transient_for(GTK_WINDOW(dialog), parent);
  gtk_dialog_add_button(GTK_DIALOG(dialog), "Start Game", GTK_RESPONSE_ACCEPT);

  GtkWidget* content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, LOG_SPACING);
  GtkWidget* firstMoveLabel = gtk_label_new("Who starts?");
  GtkWidget* firstMoveCombo = gtk_combo_box_text_new();
  GtkWidget* colorLabelWidget = gtk_label_new("Your color");
  GtkWidget* colorCombo = gtk_combo_box_text_new();

  gtk_widget_set_margin_top(box, LOG_SPACING);
  gtk_widget_set_margin_bottom(box, LOG_SPACING);
  gtk_widget_set_margin_start(box, LOG_SPACING);
  gtk_widget_set_margin_end(box, LOG_SPACING);

  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(firstMoveCombo), "User starts");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(firstMoveCombo), "CPU starts");
  gtk_combo_box_set_active(GTK_COMBO_BOX(firstMoveCombo), 0);

  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(colorCombo), "White");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(colorCombo), "Black");
  gtk_combo_box_set_active(GTK_COMBO_BOX(colorCombo), 0);

  gtk_box_append(GTK_BOX(box), firstMoveLabel);
  gtk_box_append(GTK_BOX(box), firstMoveCombo);
  gtk_box_append(GTK_BOX(box), colorLabelWidget);
  gtk_box_append(GTK_BOX(box), colorCombo);
  gtk_box_append(GTK_BOX(content), box);

  StartupDialogData* dialogData = g_new0(StartupDialogData, 1);
  dialogData->boardData = boardData;
  dialogData->firstMoveCombo = GTK_COMBO_BOX_TEXT(firstMoveCombo);
  dialogData->colorCombo = GTK_COMBO_BOX_TEXT(colorCombo);

  g_signal_connect(dialog, "response", G_CALLBACK(onStartupDialogResponse), dialogData);
  gtk_window_present(GTK_WINDOW(dialog));
}

void setBackground(GdkDisplay* display, GtkCssProvider* provider, const char* BG_CSS){ 
  gtk_css_provider_load_from_string(provider, BG_CSS);
  gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

GtkWidget* createWindow(GtkApplication* app, const char* title, const char* cssClass){ 
  GtkWidget* window = gtk_application_window_new(app);
  gtk_window_set_title (GTK_WINDOW (window), title);
  gtk_window_set_default_size (GTK_WINDOW (window), WINDOW_HEIGHT, WINDOW_WIDTH);
  gtk_widget_add_css_class(window, cssClass); 
  return window; 
}

GtkWidget* createGrid(){
  GtkWidget* grid = gtk_grid_new(); 
  gtk_grid_set_column_spacing(GTK_GRID(grid), GRID_COLUMN_SPACING);
  gtk_grid_set_row_spacing(GTK_GRID(grid), GRID_ROW_SPACING);
  gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
  return grid; 
}

GtkWidget* createTimerBox(GtkWidget* whiteTimer, GtkWidget* blackTimer){
  GtkWidget* timerBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, LOG_SPACING);
  gtk_widget_add_css_class(timerBox, "timer-box");
  gtk_widget_add_css_class(whiteTimer, "timer-label");
  gtk_widget_add_css_class(blackTimer, "timer-label");
  gtk_widget_set_halign(timerBox, GTK_ALIGN_CENTER);
  gtk_widget_set_hexpand(timerBox, TRUE);
  gtk_widget_set_hexpand(whiteTimer, TRUE);
  gtk_widget_set_hexpand(blackTimer, TRUE);
  gtk_label_set_xalign(GTK_LABEL(whiteTimer), 0.5);
  gtk_label_set_xalign(GTK_LABEL(blackTimer), 0.5);
  gtk_box_append(GTK_BOX(timerBox), whiteTimer);
  gtk_box_append(GTK_BOX(timerBox), blackTimer);
  return timerBox;
}

void createBoard(GtkWidget* board, Board_Bundle* boardData){ 
  gtk_drawing_area_set_content_width (GTK_DRAWING_AREA (board), SQUARE_SIZE * BOARD_WIDTH);
  gtk_drawing_area_set_content_height (GTK_DRAWING_AREA (board), SQUARE_SIZE * BOARD_HEIGHT);
  gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (board), drawBoard, boardData, NULL);
  gtk_widget_set_hexpand(board, FALSE); 
  gtk_widget_set_halign(board, GTK_ALIGN_CENTER); 
  gtk_widget_set_valign(board, GTK_ALIGN_CENTER); 
}

void updateTimerLabels(Board_Bundle* boardData){
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

  if(whiteTurn){
    gtk_widget_add_css_class(GTK_WIDGET(boardData->whiteTimerLabel), "timer-active");
    gtk_widget_remove_css_class(GTK_WIDGET(boardData->blackTimerLabel), "timer-active");
  }
  else {
    gtk_widget_remove_css_class(GTK_WIDGET(boardData->whiteTimerLabel), "timer-active");
    gtk_widget_add_css_class(GTK_WIDGET(boardData->blackTimerLabel), "timer-active");
  }
}

gboolean onTimerTick(gpointer user_data){
  Board_Bundle* boardData = user_data;

  if(getMovesMade(boardData->boardState) % 2 == 0)
    boardData->whiteSeconds++;
  else
    boardData->blackSeconds++;

  updateTimerLabels(boardData);
  return G_SOURCE_CONTINUE;
}


void createLogScroller(GtkWidget* logScroller){
  gtk_widget_set_size_request(logScroller, MIN_LOG_WIDTH, -1);
}

void createLog(GtkWidget* logScroller, GtkWidget* log){ 
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(log), LOG_SPACING);
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(log), LOG_SPACING);
  gtk_text_view_set_top_margin(GTK_TEXT_VIEW(log), LOG_SPACING);
  gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(log), LOG_SPACING);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(log), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(log), FALSE);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(logScroller), log);
}

//get user click 
//send click data to board 
//board will update  
//i draw if things have changed 

void onClick(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data){  
  Board_Bundle* boardData = user_data; 
  GtkWidget* boardWidget = boardData->boardWidget; 
  Board_State* boardState = boardData->boardState; 

  whichSquare(x,y); //just for debug purposes 

  int row = pixToIndex(y); 
  int col = pixToIndex(x); 

  Pos clickPos;  
  posCtor(&clickPos, row, col);  

  sendInput(boardData, clickPos); 

  if(hasUpdate(boardState))
     gtk_widget_queue_draw(boardWidget); 

  if(moveSucces(boardState)){
    updateTimerLabels(boardData);
    appendToLogUI(boardData); 
  }
   
}

void appendToLogUI(Board_Bundle* boardData) {
  char* text = boardData->move; 

  if(text){
    appendTextToLogUI(boardData, text);
  }
}


static void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget* grid = createGrid(); 
  GtkWidget* window = createWindow(app, TITLE, CSS_CLASS); 
  GtkWidget* board = gtk_drawing_area_new(); 
  GtkWidget* logScroller = gtk_scrolled_window_new(); 
  GtkWidget* log = gtk_text_view_new();
  GtkWidget* whiteTimer = gtk_label_new(NULL);
  GtkWidget* blackTimer = gtk_label_new(NULL);
  GtkWidget* timerBox = createTimerBox(whiteTimer, blackTimer);
  GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(log)); 
 
  Board_Bundle* boardData = malloc(sizeof(Board_Bundle)); 
  initializeBoardBundle(boardData, user_data, board, GTK_TEXT_VIEW(log), buffer, GTK_LABEL(whiteTimer), GTK_LABEL(blackTimer)); 
  updateTimerLabels(boardData);

  //setting background
  GdkDisplay *display = gdk_display_get_default();
  GtkCssProvider *provider = gtk_css_provider_new();
  setBackground(display, provider, CHESS_BG); 

  //creating board
  createBoard(board, boardData); 

  //creating log scroller container 
  createLogScroller(logScroller); 

  //creating log 
  createLog(logScroller, log);
  
  //event controller
  GtkGesture *click = gtk_gesture_click_new(); 
  gtk_widget_add_controller(board, GTK_EVENT_CONTROLLER(click)); 
  g_signal_connect(click, "pressed", G_CALLBACK(onClick), boardData); 


  g_object_unref(provider);

  gtk_window_set_child(GTK_WINDOW(window), grid);
  gtk_grid_attach(GTK_GRID(grid), board,  0, 0, 1, 2); //col row colspan rowspan
  gtk_grid_attach(GTK_GRID(grid), timerBox,  1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), logScroller,  1, 1, 1, 1);
  gtk_window_present (GTK_WINDOW (window));
  showStartupDialog(GTK_WINDOW(window), boardData);

}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  int numOfImages = 14; 

  Board board; 
  cairo_surface_t* images[numOfImages]; 
  createImages(images, numOfImages); 
  defaultInitializeBoard(&board); 
  initializeBoard(&board, images); 

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), &board);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

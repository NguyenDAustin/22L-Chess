
#include "gui.h"



//global variables 
const char FILES[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'}; 
const char* TITLE = "CHESS APP"; 
const char* CSS_CLASS = "chess-bg"; 
const int MIN_LOG_WIDTH = 300; 
const int MIN_LOG_HEIGHT = 500;
const int GRID_COLUMN_SPACING = 50;  
const int PROMOTION_COLUMN_SPACING = 5; 
const int LOG_SPACING = 10; 
const int PROMOTION_BUTTON_SIZE = 80;

void whichSquare(float x, float y){ //just for debug purposes
  int file = x / SQUARE_SIZE; 
  int rank =  y / SQUARE_SIZE; 
  printf("square: %c%d\n", FILES[file], rank); 
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

GtkWidget* createGrid(int columnSpacing){
  GtkWidget* grid = gtk_grid_new(); 
  gtk_grid_set_column_spacing(GTK_GRID(grid), columnSpacing);
  gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
  return grid; 
}

GtkWidget* createPromotionGrid(GtkWidget** buttons){ 
  GtkWidget* grid = createGrid(PROMOTION_COLUMN_SPACING);  
  gridAttacher(grid, buttons, NUMBER_OF_PROMOTION_BUTTONS); 
  return grid; 
}

GtkWidget* createMainGrid(){ 
  return createGrid(GRID_COLUMN_SPACING); 
}

void createBoard(GtkWidget* board, Board_Bundle* boardData){ 
  gtk_drawing_area_set_content_width (GTK_DRAWING_AREA (board), SQUARE_SIZE * BOARD_WIDTH);
  gtk_drawing_area_set_content_height (GTK_DRAWING_AREA (board), SQUARE_SIZE * BOARD_HEIGHT);
  gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (board), drawBoard, boardData, NULL);
  gtk_widget_set_hexpand(board, FALSE); 
  gtk_widget_set_halign(board, GTK_ALIGN_CENTER); 
  gtk_widget_set_valign(board, GTK_ALIGN_CENTER); 
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


void createBlackButtons(Board_Bundle* boardData, GtkWidget** buttons){ 
  const char* blackImages[NUMBER_OF_PROMOTION_BUTTONS] = {PIECE_RESOURCES[BLACK_QUEEN], PIECE_RESOURCES[BLACK_ROOK], PIECE_RESOURCES[BLACK_BISHOP], PIECE_RESOURCES[BLACK_KNIGHT], PIECE_RESOURCES[BLACK_ANTEATER]};
  initializePromotionButtonArr(boardData, buttons, blackImages); 
}

void createWhiteButtons(Board_Bundle* boardData, GtkWidget** buttons){ 
  const char* whiteImages[NUMBER_OF_PROMOTION_BUTTONS] = {PIECE_RESOURCES[WHITE_QUEEN], PIECE_RESOURCES[WHITE_ROOK], PIECE_RESOURCES[WHITE_BISHOP], PIECE_RESOURCES[WHITE_KNIGHT], PIECE_RESOURCES[WHITE_ANTEATER]};
  initializePromotionButtonArr(boardData, buttons, whiteImages); 
}

void createPopUp(Board_Bundle* boardData){ 
  Board_State* boardState = boardData->boardState; 
  Piece* promoting = boardState->clickedPiece; 
  Color pieceColor = getColor(promoting); 
  GtkWidget* popUp = boardData->promotionPopUp; 
  GtkWidget* promotionGrid; 

  GtkWidget* buttons[NUMBER_OF_PROMOTION_BUTTONS]; 
  (pieceColor == WHITE) ? createWhiteButtons(boardData, buttons) : createBlackButtons(boardData, buttons); 
  (pieceColor == WHITE) ? gtk_popover_set_position(GTK_POPOVER(popUp), GTK_POS_TOP) : gtk_popover_set_position(GTK_POPOVER(popUp), GTK_POS_BOTTOM); 
  
  promotionGrid = createPromotionGrid(buttons);
  gtk_popover_set_child(GTK_POPOVER(popUp), promotionGrid); 
  gtk_popover_popup(GTK_POPOVER(popUp)); 
}

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

  if(moveSucces(boardState)) 
    appendToLogUI(boardData);  

  if(isPromotion(boardState)){
    printf("is promotion\n");  
    createPopUp(boardData); 
  }
    
}


void onPromotionClicked(GtkButton *button, gpointer user_data) {
    Rank promoteTo = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "promoteRank"));
    Board_Bundle* boardData = user_data; 
    Board_State* boardState = getBoardState(boardData); 
    Pos clickPiecePos = getPos(boardState->clickedPiece); 
    GtkWidget* popUp = boardData->promotionPopUp; 

   // printf("Promote to rank: %d\n", promoteTo);

    promotePiece(boardData, clickPiecePos, promoteTo);
    gtk_popover_popdown(GTK_POPOVER(popUp));
}

GtkWidget* createPromotionButton(Board_Bundle* boardData, const char *imagePath, Rank type) {
    GtkWidget *button = gtk_button_new();
    GtkWidget *picture = gtk_picture_new_for_filename(imagePath);
    gtk_widget_set_size_request(picture, PROMOTION_BUTTON_SIZE, PROMOTION_BUTTON_SIZE);
    gtk_button_set_child(GTK_BUTTON(button), picture);
    g_object_set_data(G_OBJECT(button), "promoteRank", GINT_TO_POINTER(type)); 
    g_signal_connect(button, "clicked", G_CALLBACK(onPromotionClicked), boardData); 
    return button;
}

void appendToLogUI(Board_Bundle* boardData) {
  GtkTextBuffer* logBuffer = boardData->logBuffer; 
  GtkTextView* logView = boardData->log; 
  char* text = boardData->move; 

  GtkTextIter end;
  gtk_text_buffer_get_end_iter(logBuffer, &end);
  gtk_text_buffer_insert(logBuffer, &end, text, -1);

  gtk_text_buffer_get_end_iter(logBuffer, &end);
  GtkTextMark *mark = gtk_text_buffer_create_mark(logBuffer, NULL, &end, FALSE);
  gtk_text_view_scroll_mark_onscreen(logView, mark);
  gtk_text_buffer_delete_mark(logBuffer, mark);
}

void gridAttacher(GtkWidget* grid, GtkWidget** attachments, int size){
  int row = 0; 
  for(int col = 0; col < size; col++){
    gtk_grid_attach(GTK_GRID(grid), attachments[col], col, row, 1, 1); 
  }
}

void initializePromotionButtonArr(Board_Bundle* boardData, GtkWidget** buttons, const char** images){
  Rank type[NUMBER_OF_PROMOTION_BUTTONS] = {QUEEN, ROOK, BISHOP, KNIGHT, ANTEATER}; 
  for(int i = 0; i < NUMBER_OF_PROMOTION_BUTTONS; i++){ 
    buttons[i] = createPromotionButton(boardData, images[i], type[i]); 
  } 
}

static void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget* grid = createMainGrid(); 
  GtkWidget* window = createWindow(app, TITLE, CSS_CLASS); 
  GtkWidget* board = gtk_drawing_area_new(); 
  GtkWidget* logScroller = gtk_scrolled_window_new(); 
  GtkWidget* log = gtk_text_view_new();
  GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(log)); 

  GtkWidget* promotionPop = gtk_popover_new(); 
  gtk_widget_set_parent(promotionPop, board); //promotion Pop needs to be handle memory

  Board_Bundle* boardData = user_data;  
  setBoardWidget(boardData, board); 
  setLogTextView(boardData, GTK_TEXT_VIEW(log)); 
  setLogBuffer(boardData, buffer); 
  setPromotionPop(boardData, promotionPop); 

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
  gtk_grid_attach(GTK_GRID(grid), board,  0, 0, 1, 1); //col row colspan rowspan
  gtk_grid_attach(GTK_GRID(grid), logScroller,  1, 0, 1, 1);
  gtk_window_present (GTK_WINDOW (window));
}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  int numOfImages = 14; 

  Board_Bundle boardData;  
  Board board; 

  //set images
  cairo_surface_t* images[numOfImages]; 
  createImages(images, numOfImages);
  setImages(&boardData, images); 
  
  
  //set board
  defaultInitializeBoard(&board); 
  initializeBoard(&board, images); 
  setBoard(&boardData, &board); 


  //set board state 
  Board_State boardState; 
  initializeBoardState(&boardState);
  setBoardState(&boardData, &boardState); 

  
  //initializeBoardBundle(boardData, board, images, NULL, NULL, NULL, NULL); 


  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), &boardData);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

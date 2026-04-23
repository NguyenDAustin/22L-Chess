
#include "gui.h"



//global variables 
const char FILES[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'}; 
const char* TITLE = "CHESS APP"; 
const char* CSS_CLASS = "chess-bg"; 
const int MIN_LOG_WIDTH = 300; 
const int MIN_LOG_HEIGHT = 500;
const int GRID_COLUMN_SPACING = 50;  

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

GtkWidget* createGrid(){
  GtkWidget* grid = gtk_grid_new(); 
  gtk_grid_set_column_spacing(GTK_GRID(grid), GRID_COLUMN_SPACING);
  gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
  return grid; 
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
  Board* board = boardData->board; 
  whichSquare(x,y); //just for debug purposes 

  int row = pixToIndex(y); 
  int col = pixToIndex(x); 

  Pos clickPos;  
  posCtor(&clickPos, row, col);  

  sendInput(board, boardState, clickPos); 

  if(hasUpdate(boardState))
    gtk_widget_queue_draw(boardWidget); 
}


static void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget* grid = createGrid(); 
  GtkWidget* window = createWindow(app, TITLE, CSS_CLASS); 
  GtkWidget* board = gtk_drawing_area_new(); 
  GtkWidget* logScroller = gtk_scrolled_window_new(); 
  GtkWidget* log = gtk_text_view_new();
 
  Board_Bundle* boardData = malloc(sizeof(Board_Bundle)); 
  initializeBoardBundle(boardData, user_data, board); 

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

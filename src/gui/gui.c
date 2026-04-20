
#include ".vscode/gui.h"

//global variables 
const char FILES[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'}; 
const char* TITLE = "CHESS APP"; 
const char* CSS_CLASS = "chess-bg"; 


void whichSquare(float x, float y){ //just for debug purposes
  int file = x / SQUARE_SIZE; 
  int rank =  y / SQUARE_SIZE; 
  printf("square: %c%d\n", FILES[file], rank); 
}

void setBackground(GdkDisplay* display, GtkCssProvider* provider, const char* BG_CSS){ 
  gtk_css_provider_load_from_string(provider, BG_CSS);
  gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void createWindow(GtkWidget* window, const char* title, const char* cssClass){ 
  gtk_window_set_title (GTK_WINDOW (window), title);
  gtk_window_set_default_size (GTK_WINDOW (window), WINDOW_HEIGHT, WINDOW_WIDTH);
  gtk_widget_add_css_class(window, cssClass); 
}

void createBoard(GtkWidget* board, Board_Bundle* boardData){ 
  gtk_drawing_area_set_content_width (GTK_DRAWING_AREA (board), SQUARE_SIZE * BOARD_WIDTH);
  gtk_drawing_area_set_content_height (GTK_DRAWING_AREA (board), SQUARE_SIZE * BOARD_HEIGHT);
  gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (board), drawBoard, boardData, NULL);
  gtk_widget_set_halign(board, GTK_ALIGN_CENTER); 
  gtk_widget_set_valign(board, GTK_ALIGN_CENTER); 
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

  /*
  Pos legalMoves[30];  //for now --> replace with linked list or something later //30 is max number of moves a piece can possible have avail
  int size = 0; 

  if(aPieceWasClicked(boardState)){ 
    Piece* clickedPiece = getClickedPiece(boardState); 
    calculateForwardMoves(board, clickPos, legalMoves, &size); 
    printMoves(legalMoves, size); 
  }
    */

  //////

  if(hasUpdate(boardState))
    gtk_widget_queue_draw(boardWidget); 
}


static void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *window = gtk_application_window_new(app);
  GtkWidget* board = gtk_drawing_area_new(); 

  //setting up data
  Board_Bundle* boardData = malloc(sizeof(Board_Bundle)); 
  boardData->boardState = malloc(sizeof(Board_State)); 
  boardData->boardState->clickedPiece = NULL; 
  boardData->boardWidget = board; 
  boardData->board = user_data;  

  //setting background
  GdkDisplay *display = gdk_display_get_default();
  GtkCssProvider *provider = gtk_css_provider_new();
  setBackground(display, provider, CHESS_BG); 

  //creating window
  createWindow(window, TITLE, CSS_CLASS); 

  //creating board
  createBoard(board, boardData); 
  
  //event controller
  GtkGesture *click = gtk_gesture_click_new(); 
  gtk_widget_add_controller(board, GTK_EVENT_CONTROLLER(click)); 
  g_signal_connect(click, "pressed", G_CALLBACK(onClick), boardData); 


  g_object_unref(provider);
  gtk_window_set_child(GTK_WINDOW(window), board);
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

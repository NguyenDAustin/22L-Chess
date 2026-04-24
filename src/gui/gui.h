#ifndef GUI_H 
#define GUI_H 

#include "render.h"
#include "pieces.h"
#include "board_bundle.h"
#include "game_controller.h"

//global variables 
extern const char FILES[10]; 
extern const char* TITLE; 
extern const char* CSS_CLASS; 
extern const int MIN_LOG_WIDTH; 
extern const int MIN_LOG_HEIGHT; 
extern const int GRID_COLUMN_SPACING; 
extern const int LOG_SPACING; 
extern const int PROMOTION_COLUMN_SPACING; 
extern const int PROMOTION_BUTTON_SIZE;

void whichSquare(float x, float y); //just for debug processes 

void setBackground(GdkDisplay* display, GtkCssProvider* provider, const char* BG_CSS);


GtkWidget* createWindow(GtkApplication* app, const char* title, const char* cssClass);
GtkWidget* createGrid(int columnSpacing);
GtkWidget* createPromotionButton(const char *imagePath); 
GtkWidget* createPromotionGrid(GtkWidget** buttons); 

void createBoard(GtkWidget* board, Board_Bundle* boardData);
void createLogScroller(GtkWidget* log); 
void createLog(GtkWidget* logScroller, GtkWidget* log); 

void gridAttacher(GtkWidget* grid, GtkWidget** attachments, int size); 

void onClick(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data);
static void activate (GtkApplication *app, gpointer user_data);
void appendToLogUI(Board_Bundle* boardData);
void createPopUp(const Board_Bundle* boardData); 

#endif 
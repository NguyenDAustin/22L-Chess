#ifndef GUI_H 
#define GUI_H 

#include "render.h"
#include "pieces.h"

//global variables 
extern const char FILES[10]; 
extern const char* TITLE; 
extern const char* CSS_CLASS; 

void whichSquare(float x, float y); //just for debug processes 

void setBackground(GdkDisplay* display, GtkCssProvider* provider, const char* BG_CSS);
void createWindow(GtkWidget* window, const char* title, const char* cssClass);
void createBoard(GtkWidget* board, Board_Bundle* boardData);

void onClick(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data);
static void activate (GtkApplication *app, gpointer user_data);

#endif 
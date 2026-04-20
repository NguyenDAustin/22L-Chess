#ifndef RENDER_H 
#define RENDER_H 

//contains the functions we need to render the gui (drawing scaling etc)

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "color_gui.h"
#include "enum.h"
#include "board.h"


extern const char *PIECE_RESOURCES[14]; 

typedef struct {
  Board* board; 
  Board_State* boardState; 
  GtkWidget* boardWidget; 
} Board_Bundle; 


void createImages(cairo_surface_t** imgs, int numOfImgs); 

//helper functions 
Piece_Icon* textureToSurface(GdkTexture* texture, guchar **pixel_data_out); 
int pixToIndex(int pix); 
int indexToPix(int i); 
void scale(cairo_t* cr, Piece_Icon* img, float targetSize); 


//draw functions 
void drawSquare(GtkDrawingArea *area, cairo_t *cr, int row, int col); 
void drawPiece(cairo_t *cr, Piece_Icon* img, int row, int col);
void drawBoard(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data);

#endif 
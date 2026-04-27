#ifndef RENDER_H
#define RENDER_H

// contains the functions we need to render the gui (drawing scaling etc)

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "color_gui.h"
#include "board_bundle.h"
#include "enum.h"

#define LABEL_MARGIN 24

extern const char *PIECE_RESOURCES[14];

void createImages(cairo_surface_t **imgs, int numOfImgs);

// helper functions
Icon *pixbufToSurface(GdkPixbuf *pixbuf);
int pixToIndex(int pix);
int indexToPix(int i);
void scale(cairo_t *cr, Icon *img, float targetSize);

// draw functions
void drawImg(cairo_t *cr, Icon *img, float targetSize, int row, int col);
void drawSquare(GtkWidget *area, cairo_t *cr, int row, int col);
void drawPiece(cairo_t *cr, Icon *img, int row, int col);
gboolean drawBoard(GtkWidget *area, cairo_t *cr, gpointer data);

#endif

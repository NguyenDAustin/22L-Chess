#ifndef ENUM_H 
#define ENUM_H 


#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

typedef enum{ 
  KING = 0, 
  QUEEN = 1, 
  ANTEATER = 2, 
  BISHOP = 3, 
  KNIGHT = 4, 
  ROOK = 5, 
  ANT = 6 
} Rank; 

typedef enum{ 
    WHITE_KING = 0,
    BLACK_KING = 1,
    WHITE_QUEEN = 2, 
    BLACK_QUEEN = 3, 
    WHITE_ANTEATER = 4, 
    BLACK_ANTEATER = 5, 
    WHITE_BISHOP = 6, 
    BLACK_BISHOP = 7, 
    WHITE_KNIGHT = 8, 
    BLACK_KNIGHT = 9, 
    WHITE_ROOK = 10, 
    BLACK_ROOK = 11, 
    WHITE_ANT = 12, 
    BLACK_ANT = 13
} Piece_Type; 


typedef enum {
    BLACK = 0,
    WHITE = 1,
} Color; 


enum {
    BOARD_HEIGHT = 8, 
    BOARD_WIDTH = 10,
    SQUARE_SIZE = 100,
    WINDOW_HEIGHT = 1500, 
    WINDOW_WIDTH = 1500
}; 

typedef cairo_surface_t Piece_Icon; 

#endif 
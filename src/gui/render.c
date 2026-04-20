#include ".vscode/render.h"

const char *PIECE_RESOURCES[14] = {"resources/white_king.png", "resources/black_king.png", "resources/white_queen.png", 
  "resources/black_queen.png",  "resources/white_anteater.png", "resources/black_anteater.png","resources/white_bishop.png", 
  "resources/black_bishop.png","resources/white_knight.png", "resources/black_knight.png","resources/white_rook.png",  
  "resources/black_rook.png", "resources/white_ant.png", "resources/black_ant.png"}; 

Piece_Icon* textureToSurface(GdkTexture* texture, guchar **pixel_data_out){ 
  int width = gdk_texture_get_width(texture); 
  int height = gdk_texture_get_height(texture); 

  int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width);
  guchar *pixels = g_malloc(stride * height);

  gdk_texture_download(texture, pixels, stride);

  Piece_Icon *surface = cairo_image_surface_create_for_data(pixels, CAIRO_FORMAT_ARGB32, width, height, stride);
  *pixel_data_out = pixels;

  return surface;
}

void createImages(cairo_surface_t** imgs, int numOfImgs){ 
  GError* error = NULL; 
  
  GdkTexture* pieceTexture; 
  cairo_surface_t* img; 
   
  for(int i = 0; i < numOfImgs; i++){ 
    pieceTexture = gdk_texture_new_from_filename(PIECE_RESOURCES[i], &error); 
    if(error != NULL)
      printf("FAILED TO LOAD RESOURCE!"); 
    error = NULL; 
    guchar* piecePixels; 
    img = textureToSurface(pieceTexture, &piecePixels); //might need to call destroy on piece pixels later --> memory management
    imgs[i] = img; 

    g_object_unref(pieceTexture); 
    pieceTexture = NULL; 
  }
}

int pixToIndex(int pix){ 
  return pix / SQUARE_SIZE; 
}

int indexToPix(int i){ 
  return i * SQUARE_SIZE; 
}

void scale(cairo_t* cr, Piece_Icon* img, float targetSize){ //scales piece to the desired chess square size
  int imgWidth = cairo_image_surface_get_width(img); 
  int imgHeight = cairo_image_surface_get_height(img); 
  double yScaleFactor = targetSize/(float)imgHeight; 
  double xScaleFactor = targetSize/(float)imgWidth; 
  cairo_scale(cr, xScaleFactor, yScaleFactor); 
}

void drawSquare(GtkDrawingArea *area, cairo_t *cr, int row, int col){ 
  GdkRGBA color = ((row + col) % 2 == 0) ? WHITE_SQUARE() : BLACK_SQUARE();
  cairo_rectangle(cr, indexToPix(row), indexToPix(col), SQUARE_SIZE, SQUARE_SIZE); 
  gdk_cairo_set_source_rgba (cr, &color);
  cairo_fill (cr);
}


void drawPiece(cairo_t *cr, Piece_Icon* img, int row, int col){ 
  float targetSize = SQUARE_SIZE * 0.8; 
  cairo_save(cr);
  cairo_translate(cr, indexToPix(col) + (SQUARE_SIZE - targetSize)/2.0, indexToPix(row) + (SQUARE_SIZE - targetSize)/2.0); //center
  scale(cr, img, targetSize);  
  cairo_set_source_surface(cr, img, 0, 0); 
  cairo_paint(cr); 
  cairo_restore(cr); 
}


void drawBoard(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data){ 
  Board_Bundle* boardInfo = data; 
  Board* mBoard = boardInfo->board; 
  Piece* piece; 
  for(int row = 0; row < BOARD_HEIGHT; row++){ 
    for(int col = 0; col < BOARD_WIDTH; col++){ 
      drawSquare(area, cr, col, row); 
      if(hasPiece(mBoard, row, col)){ 
        piece = getSquare(mBoard, row, col); 
        drawPiece(cr, getImage(piece), row, col); 
      }
    }
  }
}
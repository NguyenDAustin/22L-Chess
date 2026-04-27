#include "render.h"
#include "board_state.h"

const char *PIECE_RESOURCES[14] = {
    "src/gui/resources/white_king.png",
    "src/gui/resources/black_king.png",
    "src/gui/resources/white_queen.png",
    "src/gui/resources/black_queen.png",
    "src/gui/resources/white_anteater.png",
    "src/gui/resources/black_anteater.png",
    "src/gui/resources/white_bishop.png",
    "src/gui/resources/black_bishop.png",
    "src/gui/resources/white_knight.png",
    "src/gui/resources/black_knight.png",
    "src/gui/resources/white_rook.png",
    "src/gui/resources/black_rook.png",
    "src/gui/resources/white_ant.png",
    "src/gui/resources/black_ant.png"};

Icon *pixbufToSurface(GdkPixbuf *pixbuf)
{
  int width = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);
  int channels = gdk_pixbuf_get_n_channels(pixbuf);
  int pixbufStride = gdk_pixbuf_get_rowstride(pixbuf);
  guchar *srcPixels = gdk_pixbuf_get_pixels(pixbuf);

  Icon *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  unsigned char *dstPixels = cairo_image_surface_get_data(surface);
  int surfaceStride = cairo_image_surface_get_stride(surface);

  for (int y = 0; y < height; y++)
  {
    guchar *srcRow = srcPixels + y * pixbufStride;
    unsigned int *dstRow = (unsigned int *)(dstPixels + y * surfaceStride);

    for (int x = 0; x < width; x++)
    {
      guchar *src = srcRow + x * channels;
      unsigned int red = src[0];
      unsigned int green = src[1];
      unsigned int blue = src[2];
      unsigned int alpha = channels == 4 ? src[3] : 255;

      red = (red * alpha + 127) / 255;
      green = (green * alpha + 127) / 255;
      blue = (blue * alpha + 127) / 255;

      dstRow[x] = (alpha << 24) | (red << 16) | (green << 8) | blue;
    }
  }

  cairo_surface_mark_dirty(surface);
  return surface;
}

void createImages(cairo_surface_t **imgs, int numOfImgs)
{
  GError *error = NULL;

  GdkPixbuf *piecePixbuf;
  cairo_surface_t *img;

  for (int i = 0; i < numOfImgs; i++)
  {
    piecePixbuf = gdk_pixbuf_new_from_file(PIECE_RESOURCES[i], &error);
    if (error != NULL)
    {
      printf("FAILED TO LOAD RESOURCE!");
      g_error_free(error);
      error = NULL;
      imgs[i] = NULL;
      continue;
    }
    error = NULL;
    img = pixbufToSurface(piecePixbuf);
    imgs[i] = img;

    g_object_unref(piecePixbuf);
    piecePixbuf = NULL;
  }
}

int pixToIndex(int pix)
{
  return (pix - LABEL_MARGIN) / SQUARE_SIZE;
}

int indexToPix(int i)
{
  return LABEL_MARGIN + i * SQUARE_SIZE;
}

void scale(cairo_t *cr, Icon *img, float targetSize)
{ // scales piece to the desired chess square size
  int imgWidth = cairo_image_surface_get_width(img);
  int imgHeight = cairo_image_surface_get_height(img);
  double yScaleFactor = targetSize / (float)imgHeight;
  double xScaleFactor = targetSize / (float)imgWidth;
  cairo_scale(cr, xScaleFactor, yScaleFactor);
}

void drawSquare(GtkWidget *area, cairo_t *cr, int row, int col)
{
  GdkRGBA color = ((row + col) % 2 == 0) ? WHITE_SQUARE() : BLACK_SQUARE();
  cairo_rectangle(cr, indexToPix(col), indexToPix(row), SQUARE_SIZE, SQUARE_SIZE);
  gdk_cairo_set_source_rgba(cr, &color);
  cairo_fill(cr);
}

void drawImg(cairo_t *cr, Icon *img, float targetSize, int row, int col)
{
  cairo_save(cr);
  cairo_translate(cr, indexToPix(col) + (SQUARE_SIZE - targetSize) / 2.0, indexToPix(row) + (SQUARE_SIZE - targetSize) / 2.0); // center
  scale(cr, img, targetSize);
  cairo_set_source_surface(cr, img, 0, 0);
  cairo_paint(cr);
  cairo_restore(cr);
}

void drawPiece(cairo_t *cr, Icon *img, int row, int col)
{
  float targetSize = SQUARE_SIZE * 0.8;
  drawImg(cr, img, targetSize, row, col);
}

void drawHighlight(cairo_t *cr, int row, int col)
{
  double x = indexToPix(col) + (SQUARE_SIZE / 2.0);
  double y = indexToPix(row) + (SQUARE_SIZE / 2.0);
  double r = SQUARE_SIZE * 0.1; // controls size
  GdkRGBA color = HIGHLIGHT_COLOR();

  cairo_save(cr);

  gdk_cairo_set_source_rgba(cr, &color);

  cairo_move_to(cr, x, y - r); // top
  cairo_line_to(cr, x + r, y); // right
  cairo_line_to(cr, x, y + r); // bottom
  cairo_line_to(cr, x - r, y); // left
  cairo_close_path(cr);

  cairo_fill(cr);
  cairo_restore(cr);
}

gboolean drawBoard(GtkWidget *area, cairo_t *cr, gpointer data)
{
  Board_Bundle *boardInfo = data;
  Board *mBoard = boardInfo->board;
  Board_State *boardState = boardInfo->boardState;
  Piece *piece;
  Pos pos;
  for (int row = 0; row < BOARD_HEIGHT; row++)
  {
    for (int col = 0; col < BOARD_WIDTH; col++)
    {

      posCtor(&pos, row, col);

      drawSquare(area, cr, row, col);

      if (hasPiece(mBoard, row, col))
      {
        piece = getSquare(mBoard, row, col);
        drawPiece(cr, getImage(piece), row, col);
      }

      if (isLegalMoveSquare(boardState, pos))
        drawHighlight(cr, row, col);
    }
  }

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_select_font_face(cr, "Sans",
                         CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, 14);

  // FILES (A, B, C...)
  for (int col = 0; col < BOARD_WIDTH; col++)
  {
    char label[2] = {'A' + col, '\0'};

    double x = indexToPix(col) + SQUARE_SIZE / 2.0 - 5;
    double y = indexToPix(BOARD_HEIGHT - 1) + SQUARE_SIZE + 16;

    cairo_move_to(cr, x, y);
    cairo_show_text(cr, label);
  }

  // RANKS (1–8)
  for (int row = 0; row < BOARD_HEIGHT; row++)
  {
    char label[3];
    snprintf(label, sizeof(label), "%d", BOARD_HEIGHT - row);

    double x = 6;
    double y = indexToPix(row) + SQUARE_SIZE / 2.0 + 5;

    cairo_move_to(cr, x, y);
    cairo_show_text(cr, label);
  }

  return FALSE;
}

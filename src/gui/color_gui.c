#include "color_gui.h"

const char* CHESS_BG =
  ".chess-bg { background-color: rgb(253, 246, 224); }"
  ".timer-box { margin-bottom: 8px; }"
  ".timer-label {"
  "  padding: 10px 14px;"
  "  border-radius: 6px;"
  "  background-color: rgb(244, 235, 205);"
  "  color: rgb(42, 37, 41);"
  "  font-size: 18px;"
  "  font-weight: 700;"
  "}"
  ".timer-active {"
  "  background-color: rgb(42, 37, 41);"
  "  color: rgb(253, 246, 224);"
  "}"
  "textview.log-box, textview.log-box text {"
  "  background-color: rgb(209, 206, 184);"
  "}";

  

float RGBtoGTK(float RGB){ 
  return (float) RGB / 255.0; 
}

void colorCtor(GdkRGBA* color, int R, int B, int G){ //given RGB values, set the color, and covert to GTK vals
  color->red = RGBtoGTK(R); 
  color->blue = RGBtoGTK(B); 
  color->green = RGBtoGTK (G); 
}

// Color Scheme 
GdkRGBA BLACK_SQUARE(){ //chess square 
  GdkRGBA color; 
  colorCtor(&color, 42, 37, 41); 
  color.alpha = 1;
  return color; 
}

GdkRGBA WHITE_SQUARE(){ //chess square
   GdkRGBA color; 
   colorCtor(&color, 209, 184, 206); 
   color.alpha = 1; 
   return color; 
}

GdkRGBA HIGHLIGHT_COLOR(){
  GdkRGBA color; 
  colorCtor(&color, 255, 255, 255); 
  color.alpha = 1; 
  return color; 
}

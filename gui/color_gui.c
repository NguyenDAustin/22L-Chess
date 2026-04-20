#include ".vscode/colors_gui.h"

const char* CHESS_BG = ".chess-bg { background-color: rgb(253, 246, 224);}"; 

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
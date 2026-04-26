#ifndef COLORS_GUI_H 
#define COLORS_GUI_H 

#include <gtk/gtk.h>

float RGBtoGTK(float RGB);
void colorCtor(GdkRGBA* color, int R, int B, int G);


// Color Scheme 
GdkRGBA BLACK_SQUARE();
GdkRGBA WHITE_SQUARE(); 
GdkRGBA HIGHLIGHT_COLOR(); 

//CSS CLASS
extern const char* CHESS_BG; 

#endif 
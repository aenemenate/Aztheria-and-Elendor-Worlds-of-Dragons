#include "../include/BearLibTerminal.h"
#include "draw_funcs.h"
#include "base.h"

#include <sstream>

void DrawBorder(Rectangle rect, string fgcolor, string bgcolor) {
  terminal_color(fgcolor.c_str());
  terminal_bkcolor(bgcolor.c_str());
  for (int i = rect.left; i <= rect.right; i ++) {
    terminal_print(i, rect.top, "═"); 
    terminal_print(i, rect.bottom, "═");
  }
  for (int i = rect.top; i <= rect.bottom; i ++) {
    terminal_print(rect.left, i, "║");
    terminal_print(rect.right, i, "║");
  }
  terminal_print(rect.left, rect.top, "╔"); 
  terminal_print(rect.right, rect.top, "╗"); 
  terminal_print(rect.left, rect.bottom, "╚"); 
  terminal_print(rect.right, rect.bottom, "╝"); 
}

void PrintCh(int x, int y, Graphic gr) {
  terminal_color(gr.fgcolor.c_str());
  terminal_bkcolor(gr.bgcolor.c_str());
  terminal_print(x, y, gr.ch.c_str());
}
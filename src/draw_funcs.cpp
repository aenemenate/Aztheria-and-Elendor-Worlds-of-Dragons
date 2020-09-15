#include "draw_funcs.h"
#include "base.h"

#include "../include/BearLibTerminal.h"

#include <sstream>

void PrintCh(int x, int y, Graphic gr)
{
  terminal_color(color_from_name(gr.fgcolor.c_str()));
  terminal_bkcolor(color_from_name(gr.bgcolor.c_str()));
  std::stringstream sstr;
  sstr << gr.ch;
  terminal_print(x, y, sstr.str().c_str());
}

void DrawBorder(int  left, int right, int top, int bottom, string fgcolor, string bgcolor)
{
  terminal_color(fgcolor.c_str());
  terminal_bkcolor(bgcolor.c_str());
  for (int i = left; i <= right; i ++)
  {
    terminal_print(i, top, "═"); 
    terminal_print(i, bottom, "═");
  }
  for (int i = top; i <= bottom; i ++)
  {
    terminal_print(left, i, "║");
    terminal_print(right, i, "║");
  }
  terminal_print(left, top, "╔"); 
  terminal_print(right, top, "╗"); 
  terminal_print(left, bottom, "╚"); 
  terminal_print(right, bottom, "╝"); 
}
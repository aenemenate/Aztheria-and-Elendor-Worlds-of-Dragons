#include "draw_funcs.h"
#include "base.h"
#include <BearLibTerminal.h>
#include <sstream>


color_t term_fgcolor = color_from_name("white");
color_t term_bkcolor = color_from_name("black");


void SetTerminal(std::string set_text) {
  terminal_open();
  terminal_set(set_text.c_str());
}

void ClearTerminal() {
  terminal_clear();
}

void RefreshTerminal() {
  terminal_refresh();
}

void PrintGraphic(int x, int y, Graphic gr) {
  if (term_fgcolor != color_from_name(gr.fgcolor.c_str()))
    terminal_color(term_fgcolor = color_from_name(gr.fgcolor.c_str()));
  if (term_bkcolor != color_from_name(gr.bgcolor.c_str()))
    terminal_bkcolor(term_bkcolor = color_from_name(gr.bgcolor.c_str()));
  terminal_print(x, y, gr.ch.c_str());
}

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

int GetTermWidth() {
  return terminal_state(TK_WIDTH);
}

int GetTermHeight() {
  return terminal_state(TK_HEIGHT);
}
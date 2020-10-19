#include <BearLibTerminal.h>
#include "draw_funcs.h"
#include "base.h"

#include <sstream>

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
  terminal_color(gr.fgcolor.c_str());
  terminal_bkcolor(gr.bgcolor.c_str());
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
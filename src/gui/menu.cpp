#include "menu.h"
#include "../game.h"
#include "../base.h"
#include "../draw_funcs.h"
#include "../input_funcs.h"

void Menu::Draw(Game *game) {
  if (!show)
    return;
  DrawBorder({xpos-1, xpos+width, ypos, ypos+height+1}, "white", "black");
  for (int i = 0; i < width; i++) {
    if (title.length() > i)
      PrintGraphic(xpos + i, ypos, {title.substr(i,1),"white", "black"});
    else
      PrintGraphic(xpos + i, ypos, {"═", "white", "black"});
  }
  PrintGraphic(xpos + width - 1, ypos, {"x","red", "black"});
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++)
        PrintGraphic(xpos + i, ypos + 1 + j, {" ","white", "black"});
  CustomDraw(game);
}

void Menu::Update(Game *game) {
  if (!show)
    return;
  if (!dragging && isclicked()) {
    click_offset = TerminalGetMouseX() - xpos;
    if (click_offset == width - 1) {
      show = false;
      return;
    }
  }
  if (isclicked() || dragging) {
    int mouse_x = TerminalGetMouseX(), mouse_y = TerminalGetMouseY();
    xpos = mouse_x - click_offset; ypos = mouse_y;
    dragging = true;
  }
  if (TerminalGetKey() == (MTK_MOUSE_LEFT|MTK_KEY_RELEASED))
    dragging = false;
  CustomUpdate(game);
}

bool Menu::isclicked() {
  bool ret_val = false;
  if (TerminalGetKey() == MTK_MOUSE_LEFT)
    if (ishovered())
      ret_val = true;
  return ret_val;
}

bool Menu::ishovered() {
  bool ret_val = false;
  int mouse_x = TerminalGetMouseX();
  if ((TerminalGetMouseY() == this->ypos) && (mouse_x >= this->xpos) && (mouse_x < this->xpos + width))
    ret_val = true;
  return ret_val;
}

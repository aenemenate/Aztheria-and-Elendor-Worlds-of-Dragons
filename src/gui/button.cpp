#include "button.h"
#include "../game.h"
#include "../draw_funcs.h"
#include "../base.h"
#include "../input_funcs.h"

void Button::Activate(Game *game) { callback_f(game); }

void Button::Update(Game *game) { if (isclicked()) callback_f(game); }

void Button::Render() {
  std::string bkcolor = "black";
  std::string fgcolor = (ishovered() ? "green" : "white");
  PrintGraphic(this->x, this->y, {text, fgcolor, bkcolor});
}

bool Button::isclicked() {
  bool ret_val = false;
  if (TerminalGetKey() == (MTK_MOUSE_LEFT|MTK_KEY_RELEASED))
    if (ishovered())
      ret_val = true;
  return ret_val;
}

bool Button::ishovered() {
  bool ret_val = false;
  int mouse_x = TerminalGetMouseX();
  if ((TerminalGetMouseY() == this->y) && (mouse_x >= this->x) && (mouse_x < this->x + this->text.length()))
    ret_val = true;
  return ret_val;
}

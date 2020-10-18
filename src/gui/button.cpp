#include "button.h"
#include "../game.h"
#include "../draw_funcs.h"
#include "../base.h"
#include <BearLibTerminal.h>

void Button::Activate(Game *game) { callback_f(game); }

void Button::Update(Game *game) { if (isclicked(game)) callback_f(game); }

void Button::Render(Game *game) {
  std::string bkcolor = "black";
  std::string fgcolor = (ishovered() ? "green" : "white");
  PrintGraphic(this->x, this->y, {text, fgcolor, bkcolor});
}

bool Button::isclicked(Game *game) {
  bool ret_val = false;
  if (game->key == (TK_MOUSE_LEFT|TK_KEY_RELEASED))
    if (ishovered())
      ret_val = true;
  return ret_val;
}

bool Button::ishovered() {
  bool ret_val = false;
  int mouse_x = terminal_state(TK_MOUSE_X);
  if ((terminal_state(TK_MOUSE_Y) == this->y) && (mouse_x >= this->x) && (mouse_x < this->x + this->text.length()))
    ret_val = true;
  return ret_val;
}

#include "panel_status.h"

#include "../../include/BearLibTerminal.h"
#include "../draw_funcs.h"

#include "../game.h"

#include "../world.h"
#include "../map/area.h"
#include "../entity.h"

void StatusPanel::Update(Game *game)
{
}

void StatusPanel::Draw(Game *game)
{
  int term_height = terminal_state(TK_HEIGHT), term_width = terminal_state(TK_WIDTH);
  terminal_color("white");
  terminal_bkcolor("black");
  for (int j = 0; j < term_height; ++j)
    for (int i = start_x(term_width); i < term_width; ++i)
      terminal_put(i, j, ' ');
  DrawBorder(start_x(term_width),term_width-1,0,term_height-1,"white","black");
  terminal_print(start_x(term_width) + 1, 1, game->world->entities[0].name.c_str());
}
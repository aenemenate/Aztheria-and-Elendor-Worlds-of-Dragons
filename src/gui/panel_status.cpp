#include <BearLibTerminal.h>
#include "panel_status.h"

#include "../draw_funcs.h"
#include "../base.h"

#include "../game.h"

#include "../world.h"
#include "../map/area.h"
#include "../ecs/entity.h"

void StatusPanel::Update(Game *game) { }

void StatusPanel::Draw(Game *game, int map_startx, int map_starty) {
  int term_height = terminal_state(TK_HEIGHT), term_width = terminal_state(TK_WIDTH);
  terminal_color("white");
  terminal_bkcolor("black");
  for (int j = 0; j < term_height; ++j)
    for (int i = start_x(term_width); i < term_width; ++i)
      terminal_put(i, j, ' ');
  DrawBorder({start_x(term_width),term_width-1,0,term_height-1},"white","black");
  std::shared_ptr<Name> name_c = std::dynamic_pointer_cast<Name>(game->world->entities[0].GetComponent(EC_NAME_ID));
  terminal_print(start_x(term_width) + 1, 1, name_c->name.c_str());
// look func
  if (game->world->GetArea(0,0)->PointWithinBounds(terminal_state(TK_MOUSE_X) + map_startx, terminal_state(TK_MOUSE_Y) + map_starty)) {
    std::shared_ptr<EntPosition> pos_c = std::dynamic_pointer_cast<EntPosition>(game->world->entities[0].GetComponent(EC_POSITION_ID));
    Position pos = pos_c->position;
    std::string block_name = game->world->GetArea(pos.wx, pos.wy)->GetBlock(terminal_state(TK_MOUSE_X) + map_startx, terminal_state(TK_MOUSE_Y) + map_starty, pos.z)->name;
    if (block_name == "air") block_name = game->world->GetArea(pos.wx, pos.wy)->GetTile(terminal_state(TK_MOUSE_X) + map_startx, terminal_state(TK_MOUSE_Y) + map_starty, pos.z)->name;
    terminal_print(start_x(term_width) + 1, term_height - 3, block_name.c_str());
  }
}

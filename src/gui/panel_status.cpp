#include "panel_status.h"
#include "../game.h"
#include "../world.h"
#include "../map/area.h"
#include "../ecs/entity.h"
#include "../draw_funcs.h"
#include "../base.h"
#include <BearLibTerminal.h>

void StatusPanel::Update(Game *game) { }

void StatusPanel::Draw(Game *game, int map_startx, int map_starty) {
// draw background
  int term_height = terminal_state(TK_HEIGHT), term_width = terminal_state(TK_WIDTH);
  for (int j = 0; j < term_height; ++j)
    for (int i = start_x(term_width); i < term_width; ++i)
      PrintGraphic(i, j, {" ", "white", "black"});
// draw border
  DrawBorder({start_x(term_width),term_width-1,0,term_height-1},"white","black");
// print player name
  std::shared_ptr<Name> name_c = std::dynamic_pointer_cast<Name>(game->world->entities[0].GetComponent(EC_NAME_ID));
  PrintGraphic(start_x(term_width) + 1, 1, {name_c->name, "white", "black"});
// print look func
  if (game->world->GetArea(0,0)->PointWithinBounds(terminal_state(TK_MOUSE_X) + map_startx, terminal_state(TK_MOUSE_Y) + map_starty)) {
    std::shared_ptr<EntPosition> pos_c = std::dynamic_pointer_cast<EntPosition>(game->world->entities[0].GetComponent(EC_POSITION_ID));
    Position pos = pos_c->position;
    std::string block_name = game->world->GetArea(pos.wx, pos.wy)->GetBlock(terminal_state(TK_MOUSE_X) + map_startx, terminal_state(TK_MOUSE_Y) + map_starty, pos.z)->name;
    if (block_name == "air") block_name = game->world->GetArea(pos.wx, pos.wy)->GetTile(terminal_state(TK_MOUSE_X) + map_startx, terminal_state(TK_MOUSE_Y) + map_starty, pos.z)->name;
    PrintGraphic(start_x(term_width) + 1, term_height - 3, {block_name, "white", "black"});
  }
}

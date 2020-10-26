#include "panel_status.h"
#include "../game.h"
#include "../world/world.h"
#include "../map/area.h"
#include "../ecs/entity.h"
#include "../draw_funcs.h"
#include "../base.h"
#include "../input_funcs.h"

#include <sstream>

void StatusPanel::Update(Game *game) { }

void StatusPanel::Draw(Game *game, int map_startx, int map_starty) {
// draw background
  int term_height = GetTermHeight(), term_width = GetTermWidth();
  for (int j = 0; j < term_height; ++j)
    for (int i = start_x(term_width); i < term_width; ++i)
      PrintGraphic(i, j, {" ", "white", "black"});
// draw border
  DrawBorder({start_x(term_width),term_width-1,0,term_height-1},"white","black");
// print time
  std::stringstream time_ss;
  Time time = game->world->time;
  time_ss << time.hour << ":" << time.minute << " " << time.second << "." << time.ms;
  time_ss << std::endl << time.month << "/" << time.day << "/" << time.year;
  PrintGraphic(start_x(term_width) + 1, 1, {time_ss.str(), "white", "black"});
// print look func
  if (game->world->GetArea(0,0)->PointWithinBounds(TerminalGetMouseX() + map_startx, TerminalGetMouseY() + map_starty)) {
    std::shared_ptr<EntPosition> pos_c = dynamic_pointer_cast<EntPosition>(game->world->entities[0].GetComponent(EC_POSITION_ID));
    Position pos = pos_c->position;
    Block *block = game->world->GetArea(pos.wx, pos.wy)->GetBlock(TerminalGetMouseX() + map_startx, TerminalGetMouseY() + map_starty, pos.z);
    if (block->explored) {
      Tile  *tile  = game->world->GetArea(pos.wx, pos.wy)->GetTile(TerminalGetMouseX() + map_startx, TerminalGetMouseY() + map_starty, pos.z);
      Entity *ent = game->world->GetArea(pos.wx, pos.wy)->GetEntity(TerminalGetMouseX() + map_startx, TerminalGetMouseY() + map_starty, pos.z);
      std::string block_name = block->name;
      if (block_name == "air") 
	block_name = tile->name;
      if (ent != nullptr)
        if (ent->HasComponent(EC_NAME_ID))
          block_name = dynamic_pointer_cast<Name>(ent->GetComponent(EC_NAME_ID))->name;
      PrintGraphic(start_x(term_width) + 1, term_height - 3, {block_name, "white", "black"});
    }
  }
}

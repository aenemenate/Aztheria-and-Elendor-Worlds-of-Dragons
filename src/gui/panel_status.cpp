#include "panel_status.h"
#include "../game.h"
#include "../world/world.h"
#include "../map/area.h"
#include "../ecs/entity.h"
#include "../draw_funcs.h"
#include "../base.h"
#include "../input_funcs.h"

#include <sstream>

void PrintHealthBar(int x, int y, int length, double percent, std::string color) {
  int full_percent = length * percent;
  for (int i = 0; i < length; ++i) {
    std::string bg_color = color;
    if (i >= full_percent)
      bg_color = "gray";
    PrintGraphic(x + i, y, {" ", "white", bg_color});
  }
}

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
  std::shared_ptr<Name> playername_c = dynamic_pointer_cast<Name>(game->world->entities[0].GetComponent(EC_NAME_ID));
  PrintGraphic(start_x(term_width) + 1, 4, {playername_c->name, "white", "black"});
  std::shared_ptr<Stats> playerstats_c = dynamic_pointer_cast<Stats>(game->world->entities[0].GetComponent(EC_STATS_ID));
  double health_bar_percent = (double)(playerstats_c->resources[Health]) / (double)(playerstats_c->resources[MaxHealth]);
  double hunger_bar_percent = (double)(playerstats_c->resources[Hunger]) / (double)(playerstats_c->resources[MaxHunger]);
  double magicka_bar_percent = (double)(playerstats_c->resources[Magicka]) / (double)(playerstats_c->resources[MaxMagicka]);
  double stamina_bar_percent = (double)(playerstats_c->resources[Stamina]) / (double)(playerstats_c->resources[MaxStamina]);
  PrintHealthBar(start_x(term_width) + 1, 5, width - 2, health_bar_percent, "dark red");
  PrintHealthBar(start_x(term_width) + 1, 6, width - 2, hunger_bar_percent, "dark green");
  PrintHealthBar(start_x(term_width) + 1, 7, width - 2, magicka_bar_percent, "dark blue");
  PrintHealthBar(start_x(term_width) + 1, 8, width - 2, stamina_bar_percent, "dark yellow");
  if (game->world->entities[0].HasComponent(EC_FOV_ID)) {
    std::shared_ptr<Fov> fov_c = dynamic_pointer_cast<Fov>(game->world->entities[0].GetComponent(EC_FOV_ID));
    int y = 0;
    for (int i = 0; i < fov_c->visibleEntities.size(); ++i) {
      if (fov_c->visibleEntities[i].HasComponent(EC_STATS_ID)) {
	std::shared_ptr<Stats> entstats_c = dynamic_pointer_cast<Stats>(fov_c->visibleEntities[i].GetComponent(EC_STATS_ID));
	health_bar_percent = (double)(entstats_c->resources[Health]) / (double)(entstats_c->resources[MaxHealth]);
	PrintHealthBar(start_x(term_width) + 1, 10 + y * 3 + 1, width - 2, health_bar_percent, "dark red");
      }
      else continue;
      if (fov_c->visibleEntities[i].HasComponent(EC_NAME_ID)) {
        std::shared_ptr<Name> name_c = dynamic_pointer_cast<Name>(fov_c->visibleEntities[i].GetComponent(EC_NAME_ID));
        PrintGraphic(start_x(term_width) + 1, 10 + y * 3, {name_c->name, "white", "black"});
      }
      ++y;
    }
  }
// print look func
  if (game->world->GetArea(0,0)->PointWithinBounds(TerminalGetMouseX() + map_startx, TerminalGetMouseY() + map_starty)) {
    std::shared_ptr<EntPosition> pos_c = dynamic_pointer_cast<EntPosition>(game->world->entities[0].GetComponent(EC_POSITION_ID));
    Position pos = pos_c->position;
    Block *block = game->world->GetArea(pos.wx, pos.wy)->GetBlock(TerminalGetMouseX() + map_startx, TerminalGetMouseY() + map_starty, pos.z);
    if (block->explored) {
      Tile  *tile  = game->world->GetArea(pos.wx, pos.wy)->GetTile(TerminalGetMouseX() + map_startx, TerminalGetMouseY() + map_starty, pos.z);
      Entity *ent = nullptr;
      if (game->world->GetArea(pos.wx, pos.wy)->GetEntity(TerminalGetMouseX() + map_startx, TerminalGetMouseY() + map_starty, pos.z) != -1)
        ent = &(game->world->entities[game->world->GetArea(pos.wx, pos.wy)->GetEntity(TerminalGetMouseX() + map_startx, TerminalGetMouseY() + map_starty, pos.z)]);
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

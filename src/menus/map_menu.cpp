#include "map_menu.h"
#include "../game.h"
#include "../base.h"
#include "../draw_funcs.h"
#include "../../include/BearLibTerminal.h"
#include "../world.h"
#include "../map.h"
#include "../entity.h"


void MapMenu::CustomDraw(Game* game)
{
// draw world map
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++)
    {
      Map *map = game->world->GetMap(i, j);
        Graphic gset = GetMapGraphic(map->biome_type, map->terrain_type);
        PrintCh(xpos + i, ypos + 1 + j, gset);
    }
    PrintCh(xpos + game->world->entities[0].pos.wx, ypos + 1 + game->world->entities[0].pos.wy, game->world->entities[0].gset);
}

void MapMenu::CustomUpdate(Game* game)
{
}
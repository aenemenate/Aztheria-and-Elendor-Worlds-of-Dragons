#include <BearLibTerminal.h>
#include "map_menu.h"
#include "../game.h"
#include "../draw_funcs.h"
#include "../world.h"
#include "../map/area.h"
#include "../entity/entity.h"


void MapMenu::CustomDraw(Game* game) {
// draw world map
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++) {
      Graphic gset = GetAreaGraphic(game->world->GetArea(i, j));
      PrintCh(xpos + i, ypos + 1 + j, gset);
    }
    PrintCh(xpos + game->world->entities[0].pos.wx, ypos + 1 + game->world->entities[0].pos.wy, game->world->entities[0].gset);
}

void MapMenu::CustomUpdate(Game* game) { }

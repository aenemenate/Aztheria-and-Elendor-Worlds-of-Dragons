#include "map_menu.h"
#include "../game.h"
#include "../draw_funcs.h"
#include "../world.h"
#include "../map/area.h"
#include "../ecs/entity.h"


void MapMenu::CustomDraw(Game* game) {
// draw world map
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++) {
      Graphic gset = GetAreaGraphic(game->world->GetArea(i, j));
      PrintGraphic(xpos + i, ypos + 1 + j, gset);
    }
// draw player position on world map
  std::shared_ptr<EntPosition> pos_c = std::dynamic_pointer_cast<EntPosition>(game->world->entities[0].GetComponent(EC_POSITION_ID));
  Position pos = pos_c->position;
  std::shared_ptr<Renderable> rend_c = std::dynamic_pointer_cast<Renderable>(game->world->entities[0].GetComponent(EC_RENDERABLE_ID));
  PrintGraphic(xpos + pos.wx, ypos + 1 + pos.wy, rend_c->graphic);
}

void MapMenu::CustomUpdate(Game* game) { }

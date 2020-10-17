#include "entity_components.h"
#include "../game.h"
#include "../world.h"
#include "../map/area.h"
#include "entity.h"

bool Fov::Tick(Entity *src, Game *game) {
  visiblepoints.clear();
  if (src->HasComponent(EC_POSITION_ID)) {
    World *world = game->world;
    std::shared_ptr<EntPosition> pos_c = std::dynamic_pointer_cast<EntPosition>(src->GetComponent(EC_POSITION_ID));
    Position *pos = &(pos_c->position);
    Area* map_ptr = world->GetArea(pos->wx, pos->wy);
    fov_circle(game->settings.get_fov_settings(), map_ptr, &visiblepoints, pos->x, pos->y, pos->z, viewradius);
    if (src->HasComponent(EC_PLAYER_ID))
      for (int vp = 0; vp < visiblepoints.size(); vp++) {
        Position *temp_pos = &(visiblepoints[vp]);
        map_ptr->GetTile(temp_pos->x, temp_pos->y, temp_pos->z)->explored = true;
        map_ptr->GetBlock(temp_pos->x, temp_pos->y, temp_pos->z)->explored = true;
      }
  }
  return false;
}

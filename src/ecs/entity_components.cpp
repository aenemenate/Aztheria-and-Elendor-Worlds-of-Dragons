#include "entity_components.h"
#include "../game.h"
#include "../world.h"
#include "../map/area.h"
#include "entity.h"
#include "../input_funcs.h"

bool Fov::Tick(Entity *src, Game *game) {
  visiblepoints.clear();
  if (src->HasComponent(EC_POSITION_ID)) {
    World *world = game->world;
    std::shared_ptr<EntPosition> pos_c = dynamic_pointer_cast<EntPosition>(src->GetComponent(EC_POSITION_ID));
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

bool Player::Tick(Entity *src, Game *game) {
  Position plyr_pos = (dynamic_pointer_cast<EntPosition>(src->GetComponent(EC_POSITION_ID)))->position;
  switch (TerminalGetKey()) {
    case MTK_KP_8:
    case MTK_UP:
      src->actions.push_back(std::shared_ptr<EntityAction>(new Move(0,-1,0))); break;
    case MTK_KP_9:
      src->actions.push_back(std::shared_ptr<EntityAction>(new Move(1,-1,0))); break;
    case MTK_KP_6:
    case MTK_RIGHT:
      src->actions.push_back(std::shared_ptr<EntityAction>(new Move(1,0,0))); break;
    case MTK_KP_3:
      src->actions.push_back(std::shared_ptr<EntityAction>(new Move(1,1,0))); break;
    case MTK_KP_2:
    case MTK_DOWN:
      src->actions.push_back(std::shared_ptr<EntityAction>(new Move(0,1,0))); break;
    case MTK_KP_1:
      src->actions.push_back(std::shared_ptr<EntityAction>(new Move(-1,1,0))); break;
    case MTK_KP_4:
    case MTK_LEFT:
      src->actions.push_back(std::shared_ptr<EntityAction>(new Move(-1,0,0))); break;
    case MTK_KP_7:
      src->actions.push_back(std::shared_ptr<EntityAction>(new Move(-1,-1,0))); break;
    case MTK_KP_ENTER:
      src->actions.push_back(std::shared_ptr<EntityAction>(new ActivateBlock(0, 0))); break;
  }
  return false;
}

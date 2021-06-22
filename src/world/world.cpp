#include "world.h"
#include "../map/area.h"
#include "../ecs/entity.h"
#include "../game.h"
#include "../map_objects/block_systems.h"

World::World(uint8_t width, uint8_t height, uint16_t map_w, uint16_t map_h, int slot) {
  this->width = width;
  this->height = height;
  areas.resize(width*height, Area(map_w,map_h));
  seed = 0;
  this->slot = slot;
  time = Time();
  lastUpdateHour = 0;
}

void World::Update(Game *game) {
  Entity *plyr = &(entities[0]);
  Position plyr_pos = (dynamic_pointer_cast<EntPosition>(plyr->GetComponent(EC_POSITION_ID)))->position;
// tick pre-action components
  for (int e = 0; e < entities.size(); ++e) {
    Position pos = (dynamic_pointer_cast<EntPosition>(entities[e].GetComponent(EC_POSITION_ID)))->position;
    if (pos.wx >= plyr_pos.wx - 2 && pos.wx <= plyr_pos.wx + 2
    &&  pos.wy >= plyr_pos.wy - 2 && pos.wy <= plyr_pos.wy + 2)
      entities[e].Tick(game, EC_PRIO_PRE);
  }
// act, only allowing other entities to act if the player does
  if (entities[0].Act(this))
    for (int e = 1; e < entities.size(); ++e)
      entities[e].Act(this);
// tick post-action components
  for (int e = 0; e < entities.size(); ++e) {
    Position pos = (dynamic_pointer_cast<EntPosition>(entities[e].GetComponent(EC_POSITION_ID)))->position;
    if (pos.wx >= plyr_pos.wx - 1 && pos.wx <= plyr_pos.wx + 1
    &&  pos.wy >= plyr_pos.wy - 1 && pos.wy <= plyr_pos.wy + 1
    && pos.z >= plyr_pos.z -  1 && pos.z <= plyr_pos.z + 1)
      entities[e].Tick(game, EC_PRIO_POST);
  }
  std::shared_ptr<ActionTime> plyrActionTime = dynamic_pointer_cast<ActionTime>(plyr->GetComponent(EC_ACTIONTIME_ID));
// if the player performed an action, jump to their time
  if (time < plyrActionTime->time)
    time = Time(plyrActionTime->time);
// update world once per 3 hours
  if (time.hour % 3 == 0 && lastUpdateHour != time.hour) {
    UpdatePlants(this);
    lastUpdateHour = time.hour;
  }
}

bool World::PointWithinBounds(int x, int y) {
  bool ret_val = false;
  if (x >= 0 && y >= 0 && x < width && y < height)
    ret_val = true;
  return ret_val;
}

Area *World::GetArea(int x, int y) {
    Area *area = nullptr;
    if (PointWithinBounds(x, y))
      area = &(areas[x*width+y]);
    return area;
}

void World::AddEntity(Entity entity) {
  entities.push_back(entity);
  if (entity.HasComponent(EC_POSITION_ID)) {
    Position pos = (dynamic_pointer_cast<EntPosition>(entity.GetComponent(EC_POSITION_ID)))->position;
    SetEnts();
  }
}

void World::SetEnts() {
  for (int i = 0; i < width; ++i)
    for (int j = 0; j < height; ++j)
      GetArea(i, j)->ClearEnts();
  for (int e = 0; e < entities.size(); ++e) {
    Entity *entity = &(entities[e]);
    if (entity->HasComponent(EC_POSITION_ID)) {
      Position pos = (dynamic_pointer_cast<EntPosition>(entity->GetComponent(EC_POSITION_ID)))->position;
      int map_idx = pos.wx * width + pos.wy;
      areas[map_idx].SetEntity(pos.x, pos.y, pos.z, entity);
    }
  }
}

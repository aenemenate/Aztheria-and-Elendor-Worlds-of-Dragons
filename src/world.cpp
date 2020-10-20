#include "world.h"
#include "map/area.h"
#include "ecs/entity.h"
#include "game.h"


World::World(uint8_t width, uint8_t height, uint16_t map_w, uint16_t map_h, int slot) {
  this->width = width;
  this->height = height;
  areas.resize(width*height, Area(map_w,map_h));
  seed = 0;
  this->slot = slot;
}

World::~World() {
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
  for (int e = 0; e < entities.size(); ++e) {
    Entity *entity = &(entities[e]);
    if (entity->HasComponent(EC_POSITION_ID)) {
      Position pos = (dynamic_pointer_cast<EntPosition>(entity->GetComponent(EC_POSITION_ID)))->position;
      int map_idx = pos.wx * width + pos.wy;
      areas[map_idx].SetEntity(pos.x, pos.y, pos.z, entity);
    }
  }
}

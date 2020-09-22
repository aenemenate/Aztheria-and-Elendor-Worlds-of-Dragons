#include "world.h"
#include "map/area.h"
#include "entity.h"
#include "game.h"


World::World(uint8_t width, uint8_t height, uint16_t map_w, uint16_t map_h, int slot)
{
  this->width = width;
  this->height = height;
  areas.resize(width*height, Area(map_w,map_h));
  seed = 0;
  this->slot = slot;
}

bool World::PointWithinBounds(int x, int y)
{
  bool ret_val = false;
  if (x >= 0 && y >= 0 && x < width && y < height)
    ret_val = true;
  return ret_val;
}

Area *World::GetArea(int x, int y)
{
    Area *area = nullptr;
    if (PointWithinBounds(x, y))
      area = &(areas[x*width+y]);
    return area;
}

void World::AddEntity(Entity entity)
{
  entities.push_back(entity);
  int map_idx = entity.pos.wx * width + entity.pos.wy;
  areas[map_idx].SetEntity(entity.pos.x, entity.pos.y, entity.pos.z, &(entities[entities.size()-1]));
}

void World::SetEnts()
{
  for (int e = 0; e < entities.size(); ++e) {
    Entity *ent = &(entities[e]);
    int map_idx = ent->pos.wx * width + ent->pos.wy;
    areas[map_idx].SetEntity(ent->pos.x, ent->pos.y, ent->pos.z, ent);
  }
}

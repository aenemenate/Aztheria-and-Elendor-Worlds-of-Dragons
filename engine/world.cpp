#include "world.h"
#include "map.h"
#include "entity.h"
#include "game.h"


World::World(uint8_t width, uint8_t height, uint16_t map_w, uint16_t map_h, int slot)
{
  this->width = width;
  this->height = height;
  maps.resize(width*height, Map(map_w,map_h));
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

Map *World::GetMap(int x, int y)
{
    Map *map = nullptr;
    if (PointWithinBounds(x, y))
      map = &(maps[x*width+y]);
    return map;
}

void World::AddEntity(Entity entity)
{
  entities.push_back(entity);
  int map_idx = entity.pos.wx * width + entity.pos.wy,
      ent_idx = entity.pos.x * maps[0].width + entity.pos.y;
  maps[map_idx].ent_map[ent_idx] = &(entities[entities.size()-1]);
}

void World::SetEnts()
{
  for (int e = 0; e < entities.size(); ++e)
  {
    Entity *ent = &(entities[e]);
    int map_idx = ent->pos.wx * width + ent->pos.wy,
        ent_idx = ent->pos.x * maps[0].width + ent->pos.y;
    maps[map_idx].ent_map[ent_idx] = ent;
  }
}

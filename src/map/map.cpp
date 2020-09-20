#include "map.h"

Map::Map() 
{ 
  width = 0;
  height = 0;
  this->name = "";
  tiles.clear();
  ent_map.clear();
}

Map::Map(uint16_t width, uint16_t height)
{
  this->width = width;
  this->height = height;
  this->name = "";
  tiles.resize(width*height);
  ent_map.resize(width*height, nullptr);
}

bool Map::PointWithinBounds(int x, int y)
{
  return (x >= 0 && y >= 0 && x < width && y < height);
}

Entity *Map::GetEntity(int x, int y)
{
  return PointWithinBounds(x, y) ? ent_map[x*width+y] : nullptr;
}

void Map::SetEntity(int x, int y, Entity *entity)
{
  if (PointWithinBounds(x, y))
    ent_map[x*width+y] = entity;
}

Tile *Map::GetTile(int x, int y)
{
  return PointWithinBounds(x, y) ? &(tiles[x*width+y]) : nullptr;
}

void Map::SetTile(int x, int y, Tile tile)
{
  if (PointWithinBounds(x, y))
    tiles[x*width+y] = tile;
}
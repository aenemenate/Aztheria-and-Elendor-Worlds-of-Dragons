#include "map.h"

Map::Map() 
{ 
  width = 0;
  height = 0;
  this->name = "";
  tiles.clear();
  height_map.clear();
  ent_map.clear();
}

Map::Map(uint16_t width, uint16_t height)
{
  this->width = width;
  this->height = height;
  this->name = "";
  tiles.resize(width*height);
  height_map.resize(width*height, 0);
  ent_map.resize(width*height, nullptr);
}

bool Map::PointWithinBounds(int x, int y)
{
  bool ret_val = false;
  if (x >= 0 && y >= 0 && x < width && y < height)
    ret_val = true;
  return ret_val;
}

Tile *Map::GetTile(int x, int y)
{
  Tile *tile = nullptr;
  if (PointWithinBounds(x, y))
    tile = &(tiles[x*width+y]);
  return tile;
}

void Map::SetTile(int x, int y, Tile tile)
{
  if (PointWithinBounds(x, y))
    tiles[x*width+y] = tile;
}
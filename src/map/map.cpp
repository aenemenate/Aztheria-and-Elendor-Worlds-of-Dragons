#include "map.h"
#include "../map_objects/block.h"
#include "../map_objects/tile.h"
#include "../ecs/entity.h"

Map::Map() { 
  width = 0;
  height = 0;
  this->name = "";
  tiles.clear();
  blocks.clear();
  ent_map.clear();
}

Map::Map(uint16_t width, uint16_t height) {
  this->width = width;
  this->height = height;
  this->name = "";
  tiles.resize(width*height);
  blocks.resize(width*height);
  ent_map.resize(width*height, -1);
}

bool Map::PointWithinBounds(int x, int y) { return (x >= 0 && y >= 0 && x < width && y < height); }

int Map::GetEntity(int x, int y) { return PointWithinBounds(x, y) ? ent_map[x*height+y] : -1; }
void Map::SetEntity(int x, int y, int entId) { if (PointWithinBounds(x, y)) ent_map[x*height+y] = entId; }

Tile *Map::GetTile(int x, int y) { return PointWithinBounds(x, y) ? &(tiles[x*width+y]) : nullptr; }
void Map::SetTile(int x, int y, Tile tile) { if (PointWithinBounds(x, y)) tiles[x*height+y] = tile; }

Block *Map::GetBlock(int x, int y) { return PointWithinBounds(x, y) ? &(blocks[x*width+y]) : nullptr; }
void Map::SetBlock(int x, int y, Block block) { if (PointWithinBounds(x, y)) blocks[x*height+y] = block; }
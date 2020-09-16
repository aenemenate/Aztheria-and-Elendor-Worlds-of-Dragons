#include "map.h"

// gets the graphic for an area (map) using its biome type and terrain type
Graphic GetMapGraphic(BiomeType biome_type, TerrainType terrain_type)
{
  switch (biome_type)
  {
    case BiomeType::Desert:
      return {"≈", "237,201,175", "black"};
    case BiomeType::Mesa:
      return {"=", "149,85,90", "black"};
    case BiomeType::Taiga:
      return {"↑", "darker green", "black"};
    case BiomeType::Tundra:
      return {"▒", "white", "blue"};
    case BiomeType::BorealForest:
      return {"♠", "dark green", "black"};
    case BiomeType::GrassyPlain:
      return {"\"", "green", "black"};
    case BiomeType::SnowyPlain:
      return {"\"", "white", "black"};
    case BiomeType::MagicalForest:
      return {"♠", "dark pink", "black"};
    case BiomeType::HauntedForest:
      return {"♠", "purple", "black"};
    case BiomeType::TropicalBeach:
      return {"≈", "237,201,175", "black"};
    case BiomeType::TropicalForest:
      return {"⌠", "green", "black"};
    case BiomeType::BambooForest:
      return {"║", "214,180,80", "black"};
    case BiomeType::Swamp:
      return {"√", "65,104,37", "black"};
    case BiomeType::SnowyMountain:
        return {"▲", "white", "black"};
    case BiomeType::Barren:
      if (terrain_type == TerrainType::Mountain)
        return {"▲", "gray", "black"};
      if (terrain_type == TerrainType::Hill)
        return {"▲", "dark gray", "black"};
      if (terrain_type == TerrainType::Plain)
        return {".", "165,42,42", "black"};
      if (terrain_type == TerrainType::Ocean)
        return {"≈", "blue", "black"};
  }
  return {" ", "black", "black"};
}

Map::Map() 
{ 
  width = 0; 
  height = 0;
  tiles.clear();
  height_map.clear();
  ent_map.clear();
}

Map::Map(uint16_t width, uint16_t height)
{
  this->width = width;
  this->height = height;
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
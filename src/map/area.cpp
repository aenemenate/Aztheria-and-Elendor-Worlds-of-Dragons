#include "area.h"
#include "../ecs/entity.h"

string GetAreaName(BiomeType biome_type, TerrainType terrain_type) {
  switch (biome_type) {
    case Desert:
      return "desert";
    case Mesa:
      return "mesa";
    case Taiga:
      return "taiga";
    case Tundra:
      return "tundra";
    case BorealForest:
      return "boreal forest";
    case GrassyPlain:
      return "grassy plain";
    case SnowyPlain:
      return "snowy plain";
    case MagicalForest:
      return "magical forest";
    case HauntedForest:
      return "haunted forest";
    case TropicalBeach:
      return "tropical beach";
    case TropicalForest:
      return "tropical forest";
    case BambooForest:
      return "bamboo forest";
    case Swamp:
      return "swamp";
    case SnowyMountain:
      return "snowy mountain";
    case Barren:
      if (terrain_type == TerrainType::Mountain)
        return "mountain";
      if (terrain_type == TerrainType::Hill)
        return "hill";
      if (terrain_type == TerrainType::Plain)
        return "plain";
      if (terrain_type == TerrainType::Ocean)
        return "ocean";
  }
  return "";
}

// gets the graphic for an area (map) using its biome type and terrain type
Graphic GetAreaGraphic(Area *area) {
  BiomeType biome_type = area->biome_type;
  TerrainType terrain_type = area->terrain_type;
  if (area->GetDungeonFloors()->size() > 0)
    return {">", "light gray", "black"};
  switch (biome_type) {
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

void Area::ClearEnts() {
  ent_map.clear();
  ent_map.resize(width * height, nullptr);
  for (int i = 0; i < dungeon_floors.size(); ++i) {
    dungeon_floors[i].ClearEnts();
  }
}

Tile *Area::GetTile(int x, int y, int z) {
  if (PointWithinBounds(x, y)) {
    if (z == 0)
      return &(tiles[x * height + y]);
    else if (z > 0 && z <= dungeon_floors.size())
      return dungeon_floors[z-1].GetTile(x, y);
  }
  return nullptr;
}

void Area::SetTile(int x, int y, int z, Tile tile) {
  if (PointWithinBounds(x, y)) {
    if (z == 0)
      tiles[x * height + y] = tile;
    else if (z > 0 && z <= dungeon_floors.size())
      dungeon_floors[z-1].SetTile(x, y, tile);
  }
}

Block *Area::GetBlock(int x, int y, int z) {
  if (PointWithinBounds(x, y)) {
    if (z == 0)
      return &(blocks[x * height + y]);
    else if (z > 0 && z <= dungeon_floors.size())
      return dungeon_floors[z-1].GetBlock(x, y);
  }
  return nullptr;
}

void Area::SetBlock(int x, int y, int z, Block block) {
  if (PointWithinBounds(x, y)) {
    if (z == 0)
      blocks[x * height + y] = block;
    else if (z > 0 && z <= dungeon_floors.size())
      dungeon_floors[z-1].SetBlock(x, y, block);
  }
}

Entity *Area::GetEntity(int x, int y, int z) {
  if (PointWithinBounds(x, y)) {
    if (z == 0)
      return ent_map[x * height + y];
    else if (z > 0 && z <= dungeon_floors.size())
      return dungeon_floors[z-1].GetEntity(x, y);
  }
  return nullptr;
}

void Area::SetEntity(int x, int y, int z, Entity *entity) {
  if (PointWithinBounds(x, y)) {
    if (z == 0)
      ent_map[x*height+y] = entity;
    else
      dungeon_floors[z-1].SetEntity(x, y, entity);
  }
}

float Area::GetHeightMap(int x, int y) { return PointWithinBounds(x, y) ? height_map[x * height + y] : 0.0f; } 
void Area::SetHeightMap(int x, int y, float v) { if (PointWithinBounds(x, y)) height_map[x * height + y] = v; }

string Area::GetName() { return name + GetAreaName(this->biome_type, this->terrain_type); }

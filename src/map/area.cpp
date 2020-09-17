#include "area.h"


// gets the graphic for an area (map) using its biome type and terrain type
Graphic GetAreaGraphic(BiomeType biome_type, TerrainType terrain_type)
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

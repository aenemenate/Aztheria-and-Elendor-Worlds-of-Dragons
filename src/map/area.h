#pragma once

#include "map.h"
#include "dungeon.h"

#include <boost/serialization/base_object.hpp>

enum TerrainType {
  Plain,
  Hill, 
  Mountain, 
  Ocean,
  Beach
};

enum BiomeType {
  Desert,
  Mesa,
  Taiga, 
  Tundra,
  BorealForest,
  GrassyPlain,
  SnowyPlain,
  MagicalForest,
  HauntedForest,
  TropicalBeach,
  TropicalForest,
  BambooForest,
  Swamp,
  SnowyMountain,
  Barren
};

Graphic GetAreaGraphic(BiomeType, TerrainType);

class Area : public Map
{
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & boost::serialization::base_object<Map>(*this);
    ar & height_map;
    ar & temperature;
    ar & humidity;
    ar & terrain_type;
    ar & biome_type;
    ent_map.resize(width*height, nullptr);
  }
  vector<float> height_map;
  vector<Dungeon> dungeons;
public:
// temperature measures daytime summer temperature
// humidity measures chance of rain on any given day
  int temperature;
  float humidity;
  TerrainType terrain_type;
  BiomeType biome_type;

  Area() : Map() { height_map.clear(); }
  Area(uint16_t width,uint16_t height) : Map(width,height) { height_map.resize(width*height, 0); }
  
  float GetHeightMap(int x,int y) { return height_map[x * width + y]; }
  void SetHeightMap(int x,int y,float v) { height_map[x * width + y] = v; }

  string GetName();
};
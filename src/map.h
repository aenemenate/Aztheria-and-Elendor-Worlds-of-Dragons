#pragma once

#include "base.h"

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <vector>
using namespace std;

class Entity;

struct Tile
{
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & gset;
    ar & walkable;
    ar & opaque;
    ar & explored;
  }
  Graphic gset;
  bool walkable, opaque, explored;
};

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

Graphic GetMapGraphic(BiomeType, TerrainType);

class Map
{
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & width;
    ar & height;
    ar & tiles;
    ar & height_map;
    ar & temperature;
    ar & humidity;
    ar & terrain_type;
    ar & biome_type;
    ent_map.resize(width*height, nullptr);
  }
  vector<Tile> tiles;
  vector<float> height_map;
public:
  vector<Entity*> ent_map;
  uint16_t width, height;
// temperature measures daytime summer temperature
// humidity measures chance of rain on any given day
  int temperature;
  float humidity;
  TerrainType terrain_type;
  BiomeType biome_type;

  Map();
  Map(uint16_t,uint16_t);

  bool PointWithinBounds(int,int);

  Tile *GetTile(int,int);
  void SetTile(int,int,Tile);
  
  float GetHeightMap(int x,int y) { return height_map[x * width + y]; }
  void SetHeightMap(int x,int y,float v) { height_map[x * width + y] = v; }

  std::string GetName();
};
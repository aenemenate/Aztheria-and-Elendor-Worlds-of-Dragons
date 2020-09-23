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

class Area : public Map
{
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & boost::serialization::base_object<Map>(*this);
    ar & height_map;
    ar & dungeon_floors;
    ar & temperature;
    ar & humidity;
    ar & terrain_type;
    ar & biome_type;
    ent_map.resize(width*height, nullptr);
  }
  vector<float> height_map;
  vector<Dungeon> dungeon_floors;
public:
// temperature measures daytime summer temperature
// humidity measures chance of rain on any given day
  int temperature;
  float humidity;
  TerrainType terrain_type;
  BiomeType biome_type;

  Area() : Map() { height_map.clear(); dungeon_floors.clear(); }
  Area(uint16_t width,uint16_t height) : Map(width,height) { height_map.resize(width*height, 0); dungeon_floors.clear(); }
  
  bool HasDungeon() { return dungeon_floors.size() > 0;}

  Tile *GetTile(int,int,int);
  void SetTile(int,int,int,Tile);
  Block *GetBlock(int,int,int);
  void SetBlock(int,int,int,Block);
  Entity *GetEntity(int,int,int);
  void SetEntity(int,int,int,Entity*);
  float GetHeightMap(int,int);
  void SetHeightMap(int,int,float);

  vector<Dungeon> *GetDungeonFloors() { return &dungeon_floors; }

  string GetName();
};

Graphic GetAreaGraphic(Area*);

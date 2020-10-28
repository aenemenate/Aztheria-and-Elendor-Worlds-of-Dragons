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
  Desert	  = 0,
  Mesa		  = 1,
  Taiga		  = 2, 
  Tundra	  = 3,
  DeciduousForest = 4,
  GrassyPlain	  = 5,
  SnowyPlain	  = 6,
  MagicalForest	  = 7,
  HauntedForest	  = 8,
  TropicalBeach	  = 9,
  TropicalForest  = 10,
  BambooForest	  = 11,
  Swamp		  = 12,
  SnowyMountain	  = 13,
  Barren	  = 14
};

class Area : public Map {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
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
/* temperature measures daytime summer temperature
 * humidity measures chance of rain on any given day
 */
  int temperature;
  float humidity;
  TerrainType terrain_type;
  BiomeType biome_type;

  Area() : Map() { height_map.clear(); dungeon_floors.clear(); }
  Area(uint16_t width,uint16_t height) : Map(width,height) { height_map.resize(width*height, 0); dungeon_floors.clear(); }

  void ClearEnts();
/* Check if there are any dungeon floors */
  bool HasDungeon() { return dungeon_floors.size() > 0;}
/* 3-dimensional varients of the map getters/setters, z of 0 gets world map, every subsequent (1,2,etc.) z gets dungeon floor */
  Tile *GetTile(int,int,int);
  void SetTile(int,int,int,Tile);
  Block *GetBlock(int,int,int);
  void SetBlock(int,int,int,Block);
  Entity *GetEntity(int,int,int);
  void SetEntity(int,int,int,Entity*);
/* Get/set the height map value */
  float GetHeightMap(int,int);
  void SetHeightMap(int,int,float);
/* Get the dungeon floors */
  vector<Dungeon> *GetDungeonFloors() { return &dungeon_floors; }
/* Get the name, based on biome, terrain and name */
  string GetName();
};

/* Get the graphic for the world map, based on terrain and biome */
Graphic GetAreaGraphic(Area*);

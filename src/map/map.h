#pragma once
#include "../base.h"
#include "../map_objects/block.h"
#include "../map_objects/tile.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
using namespace std;

class Entity;

/* This is a virtual class which is inherited by other classes, it is not able to be used on its own */

class Map {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & width;
    ar & height;
    ar & tiles;
    ar & blocks;
    ar & name;
    ent_map.resize(width*height, -1);
  }
protected:
  vector<int> ent_map;
  vector<Tile> tiles;
  vector<Block> blocks;
public:
  uint16_t width, height;
  std::string name;

/* Empy allocator for boost serialization */
  Map();
/* Allocator takes width and height */
  Map(uint16_t,uint16_t);

/* Returns true if the point is within the limit of the map */
  bool PointWithinBounds(int,int);
/* Get the entity at the specified x and y. !note: this can return null (indicates no entity at given position) */
  int GetEntity(int,int); 
/* Set the entity at the specified x and y */
  void SetEntity(int,int,int);
/* Get/set the tile/block at the specified x and y */
  Tile *GetTile(int,int);
  void SetTile(int,int,Tile);
  Block *GetBlock(int,int);
  void SetBlock(int,int,Block);
/* Virtual: Get the name (may need to be computed) */
  virtual string GetName() = 0;
};
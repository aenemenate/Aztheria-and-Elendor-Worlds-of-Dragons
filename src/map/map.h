#pragma once
#include "../base.h"
#include "../map_objects/block.h"
#include "../map_objects/tile.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
using namespace std;

class Entity;

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
    ent_map.resize(width*height, nullptr);
  }
protected:
  vector<Entity*> ent_map;
  vector<Tile> tiles;
  vector<Block> blocks;
public:
  uint16_t width, height;
  std::string name;

  Map();
  Map(uint16_t,uint16_t);

  bool PointWithinBounds(int,int);

  Entity *GetEntity(int,int); // note: this can return null (indicates no entity at given position)
  void SetEntity(int,int,Entity*);
  Tile *GetTile(int,int);
  void SetTile(int,int,Tile);
  Block *GetBlock(int,int);
  void SetBlock(int,int,Block);
  virtual string GetName() = 0;
};
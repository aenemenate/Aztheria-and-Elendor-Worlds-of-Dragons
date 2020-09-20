#pragma once
#include "../base.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
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

class Map
{
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & width;
    ar & height;
    ar & tiles;
    ar & name;
    ent_map.resize(width*height, nullptr);
  }
protected:
  vector<Entity*> ent_map;
  vector<Tile> tiles;
public:
  uint16_t width, height;
  std::string name;

  Map();
  Map(uint16_t,uint16_t);

  bool PointWithinBounds(int,int);


  Entity *GetEntity(int,int);// note: this can return null (indicates no entity at given position)
  void SetEntity(int,int,Entity*);
  Tile *GetTile(int,int);
  void SetTile(int,int,Tile);
  virtual string GetName() = 0;
};
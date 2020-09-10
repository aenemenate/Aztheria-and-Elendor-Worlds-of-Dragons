#pragma once

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <vector>
using namespace std;

class Map;
class Entity;
class Game;

/* World has a width, height, vector of maps, and vector of entities
 * entities[0] is always the player. no exceptions.
 * 
 */

class World
{
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & width;
    ar & height;
    ar & seed;
    ar & slot;
    ar & maps;
    ar & entities;
  }
  vector<Map> maps;
public:
  vector<Entity> entities;
  uint8_t width, height;
  int seed;
  int slot;
  
  World() { }
  World(uint8_t,uint8_t,uint16_t,uint16_t,int);
  
  bool PointWithinBounds(int,int);
  
  Map *GetMap(int,int);
  
  void AddEntity(Entity entity);
  void SetEnts();
};
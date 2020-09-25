#pragma once

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <vector>
using namespace std;

class Area;
class Entity;

/* World has a width, height, vector of maps, and vector of entities
 * entities[0] is always the player. no exceptions.
 * 
 */

class World {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & width;
    ar & height;
    ar & seed;
    ar & slot;
    ar & areas;
    ar & entities;
  }
  vector<Area> areas;
public:
  vector<Entity> entities;
  uint8_t width, height;
  int seed;
  int slot;
  
  World() { }
  World(uint8_t,uint8_t,uint16_t,uint16_t,int);
  
  bool PointWithinBounds(int,int);
  
  Area *GetArea(int,int);
  
  void AddEntity(Entity entity);
  void SetEnts();
};
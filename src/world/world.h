#pragma once

#include "time_system.h"
#include "../msg_console.h"
#include "../ecs/entity.h"

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <vector>
using namespace std;

class Area;
class Entity;

/* World has a width, height, vector of maps, and vector of entities
 * entities[0] is always the player. No exceptions. (Unless you choose
 * to modify the source in such a way)
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
    ar & time;
    ar & lastUpdateHour;
  }
  vector<Area> areas;
public:
  int lastUpdateHour;
  Time time;
  MsgConsole msgConsole;
  vector<Entity> entities;
  uint8_t width, height;
  int seed;
  int slot;

  World() { }
  World(uint8_t,uint8_t,uint16_t,uint16_t,int);
  ~World() { }

  /* Update the world */
  void Update(Game *game);

  /* Check if the given point is within bounds of the area array */
  bool PointWithinBounds(int,int);

  /* Get a pointer to an area*/
  Area *GetArea(int,int);

  /* Add an entity to the entity queue */
  void AddEntity(Entity entity);

  /* Set the ent_map of every map in the game */
  void SetEnts();
};
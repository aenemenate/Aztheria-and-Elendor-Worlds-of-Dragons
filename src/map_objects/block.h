#pragma once

#include "../base.h"
#include "block_components.h"
#include <string>
#include <vector>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

class Entity;
class World;

struct Block
{
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & gr;
    ar & explored;
    ar & opaque;
    ar & solid;
    ar & enterable;
    ar & name;
    ar.template register_type<Stair>();
    ar & blockcomponents;
  }
  Graphic gr;
  bool explored, opaque, solid, enterable;
  std::string name;
  std::vector<BlockComponent*> blockcomponents;
  void Activate(Entity *ent, World *world);
};
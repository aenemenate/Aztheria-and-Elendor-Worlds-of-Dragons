#pragma once

#include "map.h"

#include <boost/serialization/base_object.hpp>

/* Basically a blatant map wrapper, can be used for any non-world maps */

class Dungeon : public Map {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<Map>(*this);
    ent_map.resize(width*height, -1);
  }
public:
  Dungeon() : Map() {}
  Dungeon(uint16_t width,uint16_t height) : Map(width,height) {}
  
  void ClearEnts();
  string GetName() { return this->name; }
};
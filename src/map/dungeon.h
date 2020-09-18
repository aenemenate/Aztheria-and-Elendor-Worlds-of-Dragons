#pragma once

#include "map.h"

#include <boost/serialization/base_object.hpp>

class Dungeon : public Map
{
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & boost::serialization::base_object<Map>(*this);
    ent_map.resize(width*height, nullptr);
  }
public:
  Dungeon() : Map() {}
  Dungeon(uint16_t width,uint16_t height) : Map(width,height) {}
  
  string GetName() { return this->name; }
};
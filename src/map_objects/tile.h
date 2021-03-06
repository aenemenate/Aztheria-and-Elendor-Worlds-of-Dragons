#pragma once

#include "../base.h"
#include <string>
#include <boost/serialization/access.hpp>

struct Tile {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & gr;
    ar & explored;
    ar & walkable;
    ar & isSnowy;
    ar & name;
  }
  Graphic gr;
  bool explored, walkable, isSnowy;
  std::string name;
};

#define TILE_WATER {{"~", "blue", "black"},       false,false,false,"water"}
#define TILE_DIRT  {{".", "165,42,42", "black"},  false,true, false,"dirt" }
#define TILE_SAND  {{"~", "light brown", "black"},false,true, false,"sand" }

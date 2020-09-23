#pragma once

#include "../base.h"
#include <string>
#include <boost/serialization/access.hpp>

struct Block
{
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & gr;
    ar & explored;
    ar & opaque;
    ar & solid;
    ar & name;
  }
  Graphic gr;
  bool explored, opaque, solid;
  std::string name;
};

#define BLOCK_AIR {{" ", "black", "black"}, false, false, false, "air"}
#define BLOCK_STONE {{"#", "gray", "black"}, false, true, true, "stone"}
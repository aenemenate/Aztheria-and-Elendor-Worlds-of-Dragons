#pragma once

#include "../base.h"
#include "block_components.h"
#include <string>
#include <vector>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

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
    ar.template register_type<Stair>();
    ar & blockcomponents;
  }
  Graphic gr;
  bool explored, opaque, solid;
  std::string name;
  std::vector<BlockComponent*> blockcomponents;
};
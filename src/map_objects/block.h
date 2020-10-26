#pragma once

#include "../base.h"
#include "block_components.h"
#include <string>
#include <vector>
#include <memory>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

class World;

struct Block {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar & gr;
    ar & explored;
    ar & opaque;
    ar & solid;
    ar & enterable;
    ar & name;
    RegisterBlockComponentTypes(ar);
    ar & blockcomponents;
  }
  Graphic gr;
  bool explored, opaque, solid, enterable;
  std::string name;
  std::vector<std::shared_ptr<BlockComponent>> blockcomponents;
  void Activate(void *src, World *world);
  void Update(void *src, World *world);
  bool HasComponent(int component_type) { for (auto c : blockcomponents) if (c->ID == component_type) return true; return false; }
  std::shared_ptr<BlockComponent> GetComponent(int component_type) { for (auto c : blockcomponents) if (c->ID == component_type) return c; return nullptr; }
};
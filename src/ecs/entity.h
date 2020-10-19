#pragma once

// TODO (aenem):
//   add tick function
//   add component list
//   add component function
//   remove component function

#include "../base.h"
#include "entity_components.h"
#include "entity_actions.h"

#include <string>
#include <vector>
#include <memory>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

class Game;

class Entity {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    RegisterEntityComponentTypes(ar);
    ar & components;
  }
public:
  std::vector<std::shared_ptr<EntityComponent>> components;
  std::vector<std::shared_ptr<EntityAction>> actions;
// (De-)Initializers
  Entity() {}
// Component handles
  bool HasComponent(int component_type) { for (auto c : components) if (c->ID == component_type) return true; return false; }
  std::shared_ptr<EntityComponent> GetComponent(int component_type) { for (auto c : components) if (c->ID == component_type) return c; return nullptr; }
  void AddComponent(std::shared_ptr<EntityComponent> component) { components.push_back(component); }
  void RemoveComponent(int component_type);
// Tick systems
  void Tick(Game *game);
  void Act(World *world);
};
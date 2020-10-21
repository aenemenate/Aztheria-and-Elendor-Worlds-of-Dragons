#pragma once

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

  /* Initializer, for boost serialization */
  Entity() {}

  /* Check if the entity has a given component, based on Component ID. */
  bool HasComponent(int component_type) { for (auto c : components) if (c->ID == component_type) return true; return false; }

  /* Get the entity\'s given component, based on Component ID. */
  std::shared_ptr<EntityComponent> GetComponent(int component_type) { for (auto c : components) if (c->ID == component_type) return c; return nullptr; }

  /* Add a component. */
  void AddComponent(std::shared_ptr<EntityComponent> component) { components.push_back(component); }

  /* Remove a component. */
  void RemoveComponent(int component_type);

  /* Tick components. */
  void Tick(Game *game, int prio);

  /* Perform actions. */
  bool Act(World *world);
};

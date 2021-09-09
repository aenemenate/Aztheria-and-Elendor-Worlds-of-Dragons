#pragma once

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/base_object.hpp>

#include <memory>

#include "../base.h"

class World;

/* Block component base */

class BlockComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & ID;
  }

public:
  int ID;
  BlockComponent() {}
  BlockComponent(int ID) : ID(ID) {}
  virtual ~BlockComponent() {}
  virtual bool Update(void *src, World *world) = 0;
  virtual bool Activate(void *src, World *world) = 0;
  virtual inline std::shared_ptr<BlockComponent> GetCopy() = 0;
};


/* Defined block components */

#define STAIR_ID 0
#define PLANT_ID 1

class Stair : public BlockComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<BlockComponent>(*this);
    ar & zdirection;
  }

public:
  ZDirection zdirection;
  Stair() : BlockComponent() {}
  Stair(ZDirection _zdirection) : zdirection(_zdirection), BlockComponent(STAIR_ID) {}
  bool Update(void *src, World *world) { return false; }
  bool Activate(void *src, World *world);
  inline std::shared_ptr<BlockComponent> GetCopy() { return std::make_shared<Stair>(Stair(zdirection)); }
};


class Plant : public BlockComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
  /* serialize base class information */
    ar & boost::serialization::base_object<BlockComponent>(*this);
    ar & stage;
    ar & spawn_chance;
    ar & biome;
    ar & seed_tile;
    ar & tickstostage;
    ar & seedradius;
    ar & required_space;
    ar & stages;
  }
public:
  int stage;
  int spawn_chance;
  int biome;
  std::string seed_tile;
  int tickstostage, seedradius;
  int required_space;
  std::vector<char> stages;

  Plant() : BlockComponent() {}
  Plant(int spawn_chance, int biome, std::string seed_tile, int tickstostage, int seedradius, int required_space, std::vector<char> stages) : stage(0),
    spawn_chance(spawn_chance), biome(biome), seed_tile(seed_tile), tickstostage(tickstostage), seedradius(seedradius), required_space(required_space), stages(stages), 
      BlockComponent(PLANT_ID) {}
  bool Update(void *src, World *world);
  bool Activate(void *src, World *world) { return false; }
  inline std::shared_ptr<BlockComponent> GetCopy() { return std::make_shared<Plant>(Plant(spawn_chance, biome, seed_tile, tickstostage, seedradius, required_space, stages)); }

  char GetCurrentStage() { return stages[stage]; }
  void ResetGrowth() { stage = 0; }
};


/* Register the inherited block components with boost serialization */
template<class Archive>
void RegisterBlockComponentTypes(Archive &ar) {
    ar.template register_type<Stair>();
    ar.template register_type<Plant>();
}

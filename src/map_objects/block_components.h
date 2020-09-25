#pragma once

#include "../base.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>

class World;

// Base class

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
};


// Defined components

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
  virtual ~Stair() {}
  bool Update(void *src, World *world) { return false; }
  bool Activate(void *src, World *world);
};


class Plant : public BlockComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    // serialize base class information
    ar& boost::serialization::base_object<BlockComponent>(*this);
    ar& stage;
    ar& numstages;
    ar& tickstostage;
    ar& seedradius;
    ar& required_space;
    ar& stages;
  }
  int stage, numstages;
  int tickstostage, seedradius;
  int required_space;
  std::vector<char> stages;

public:
  Plant() : BlockComponent() {}
  Plant(int numstages, int tickstostage, int seedradius, int required_space, std::vector<char> stages) : stage(0), 
    numstages(numstages), tickstostage(tickstostage), seedradius(seedradius), required_space(required_space), stages(stages), 
      BlockComponent(PLANT_ID) {}
  bool Update(void *src, World *world);
  bool Activate(void *src, World *world) { return false; }

  int GetSeedRadius() { return seedradius; }
  char GetCurrentStage() { return stages[stage]; }
  int GetRequiredSpace() { return required_space; }
  void ResetGrowth() { stage = 0; }
};


// Functions

template<class Archive>
void RegisterBlockComponentTypes(Archive &ar) {
    ar.template register_type<Stair>();
    ar.template register_type<Plant>();
}
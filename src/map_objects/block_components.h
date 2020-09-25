#pragma once

#include "../base.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

class World;

#define STAIR_ID 0
#define PLANT_ID 1

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
  virtual void Update(void *src, World *world) = 0;
  virtual void Activate(void *src, World *world) = 0;
};

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
  void Update(void *src, World *world) {}
  void Activate(void *src, World *world);
};

class Plant : public BlockComponent {
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    // serialize base class information
    ar& boost::serialization::base_object<BlockComponent>(*this);
    ar& stage;
    ar& numstages;
    ar& tickstostage;
    ar& seedradius;
    ar& stages;
  }
  int stage, numstages;
  int tickstostage, seedradius;
  std::vector<char> stages;
public:
  Plant() : BlockComponent() {}
  Plant(int numstages, int tickstostage, int seedradius, std::vector<char> stages) : stage(0), 
    numstages(numstages), tickstostage(tickstostage), seedradius(seedradius), stages(stages), BlockComponent(PLANT_ID) {}
  void Update(void *src, World *world) {}
  void Activate(void *src, World *world) {}
  int GetSeedRadius() { return seedradius; }
  char GetCurrentStage() { return stages[stage]; }
  void ResetGrowth() { stage = 0; }
};

template<class Archive>
void RegisterBlockComponentTypes(Archive &ar) {
    ar.template register_type<Stair>();
    ar.template register_type<Plant>();
}
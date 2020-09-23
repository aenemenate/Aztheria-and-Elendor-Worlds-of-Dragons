#pragma once

#include "../base.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

class World;

#define STAIR_ID 0

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
  void Update(void *src, World *world);
};
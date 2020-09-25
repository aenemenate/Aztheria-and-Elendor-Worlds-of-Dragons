#pragma once

#include "base.h"

#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>

class Game;
class World;

class Entity {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & gset;
    ar & name;
    ar & pos;
    ar & viewradius;
  }
  int viewradius;
public:
  Graphic gset;
  std::string name;
  Position pos;
  std::vector<Position> visiblepoints;
  
  Entity() { }
  Entity(Graphic,std::string,Position,int);
  
  void Update(Game*, bool);
  void Move(int, int, int, World*);
};
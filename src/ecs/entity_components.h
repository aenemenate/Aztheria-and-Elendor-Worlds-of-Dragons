#pragma once

#include "../base.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/base_object.hpp>

class Game;
class Entity;

class EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & ID;
  }
public:
  int ID;
  EntityComponent() {}
  EntityComponent(int ID) : ID(ID) {}
  virtual ~EntityComponent() {}
  virtual bool Tick(Entity *ent, Game *game) = 0;
};


// Defined components

#define EC_RENDERABLE_ID	0
#define EC_POSITION_ID		1
#define EC_NAME_ID		2
#define EC_FOV_ID		3
#define EC_PLAYER_ID		4

class Renderable : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
    ar & graphic;
  }
public:
  Graphic graphic;
  Renderable() : EntityComponent() {}
  Renderable(Graphic _graphic) : graphic(_graphic), EntityComponent(EC_RENDERABLE_ID) {}
  bool Tick(Entity *src, Game *game) { return false; }
};

class EntPosition : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
    ar & position;
  }
public:
  Position position;
  EntPosition() : EntityComponent() {}
  EntPosition(Position _position) : position(_position), EntityComponent(EC_POSITION_ID) {}
  bool Tick(Entity *src, Game *game) { return false; }
};

class Name : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
    ar & name;
  }
public:
  std::string name;
  Name() : EntityComponent() {}
  Name(std::string _name) : name(_name), EntityComponent(EC_NAME_ID) {}
  bool Tick(Entity *src, Game *game) { return false; }
};

class Fov : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
    ar & viewradius;
    ar & visiblepoints;
  }
public:
  int viewradius;
  std::vector<Position> visiblepoints;
  Fov() : EntityComponent() {}
  Fov(int _viewradius) : viewradius(_viewradius), EntityComponent(EC_FOV_ID) {}
  bool Tick(Entity *src, Game *game);
};

class Player : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
  }
public:
  Player() : EntityComponent() {}
  Player(bool empty_val) : EntityComponent(EC_PLAYER_ID) {}
  bool Tick(Entity *src, Game *game) { return false; }
};


// Functions

template<class Archive>
void RegisterEntityComponentTypes(Archive &ar) {
    ar.template register_type<Renderable>();
    ar.template register_type<EntPosition>();
    ar.template register_type<Name>();
    ar.template register_type<Fov>();
    ar.template register_type<Player>();
}
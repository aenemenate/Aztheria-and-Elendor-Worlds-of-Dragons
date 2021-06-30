#pragma once

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/base_object.hpp>
#include <memory>

#include "../base.h"
#include "../map/area.h"
#include "../world/time_system.h"

class World;
class Game;
class Entity;

class EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & ID;
    ar & prio;
  }
public:
  int ID;
  int prio;
  EntityComponent() {}
  EntityComponent(int ID, int prio) : ID(ID), prio(prio) {}
  virtual ~EntityComponent() {}
  virtual void Tick(Entity *ent, Game *game) = 0;
  virtual inline std::shared_ptr<EntityComponent> GetCopy() = 0;
};


// Defined components

#define EC_PRIO_NULL		0
#define EC_PRIO_PRE		1
#define EC_PRIO_POST		2

#define EC_RENDERABLE_ID	0
#define EC_POSITION_ID		1
#define EC_NAME_ID		2
#define EC_FOV_ID		3
#define EC_PLAYER_ID		4
#define EC_ANIMALAI_ID		5
#define EC_ACTIONTIME_ID	6
#define EC_MONSTERAI_ID		7
#define EC_STATS_ID		8

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
  Renderable(Graphic _graphic) : graphic(_graphic), EntityComponent(EC_RENDERABLE_ID, EC_PRIO_NULL) {}
  void Tick(Entity *src, Game *game) {}
  inline std::shared_ptr<EntityComponent> GetCopy() { return std::make_shared<Renderable>(Renderable(graphic)); }
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
  EntPosition(Position _position) : position(_position), EntityComponent(EC_POSITION_ID, EC_PRIO_NULL) {}
  void Tick(Entity *src, Game *game) {}
  inline std::shared_ptr<EntityComponent> GetCopy() { return std::make_shared<EntPosition>(EntPosition(position)); }
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
  Name(std::string _name) : name(_name), EntityComponent(EC_NAME_ID, EC_PRIO_NULL) {}
  void Tick(Entity *src, Game *game) {}
  inline std::shared_ptr<EntityComponent> GetCopy() { return std::make_shared<Name>(Name(name)); }
};

class Fov : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
    ar & viewradius;
  }
public:
  int viewradius;
  std::vector<Position> visiblepoints;
  Fov() : EntityComponent() {}
  Fov(int _viewradius) : viewradius(_viewradius), EntityComponent(EC_FOV_ID, EC_PRIO_POST) {}
  /* Fov helper functions */
  Entity *ClosestVisibleEnemy(World *world, Position pos);
  /* Update fov and update the map if this entity has EC_PLAYER */
  void Tick(Entity *src, Game *game);
  inline std::shared_ptr<EntityComponent> GetCopy() { return std::make_shared<Fov>(Fov(viewradius)); }
};

class Player : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
  }
public:
  Player() : EntityComponent() {}
  Player(bool empty_val) : EntityComponent(EC_PLAYER_ID, EC_PRIO_PRE) {}
  void Tick(Entity *src, Game *game);
  inline std::shared_ptr<EntityComponent> GetCopy() { return std::make_shared<Player>(Player(false)); }
};

class AnimalAi : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
    ar & biome;
  }
public:
  BiomeType biome;
  AnimalAi() : EntityComponent() {}
  AnimalAi(BiomeType biome) : biome(biome), EntityComponent(EC_ANIMALAI_ID, EC_PRIO_PRE) {}
  void Tick(Entity *src, Game *game);
  inline std::shared_ptr<EntityComponent> GetCopy() { return std::make_shared<AnimalAi>(AnimalAi(biome)); }
};

class ActionTime : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
    ar & time;
  }
public:
  Time time;
  ActionTime() : EntityComponent() {}
  ActionTime(Time time) : time(time), EntityComponent(EC_ACTIONTIME_ID, EC_PRIO_NULL) {}
  void Tick(Entity *src, Game *game) { }
  inline std::shared_ptr<EntityComponent> GetCopy() { return std::make_shared<ActionTime>(ActionTime(time)); }
};

class MonsterAi : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
    ar & type;
  }
public:
  std::string type;
  MonsterAi() : EntityComponent() {}
  MonsterAi(std::string type) : type(type), EntityComponent(EC_MONSTERAI_ID, EC_PRIO_PRE) {}
  void Tick(Entity *src, Game *game);
  inline std::shared_ptr<EntityComponent> GetCopy() { return std::make_shared<MonsterAi>(MonsterAi(type)); }
};

class Stats : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
    ar & atk;
    ar & def;
    ar & spd;
    ar & hp;
    ar & maxhp;
  }
public:
  int atk;
  int def;
  int spd;
  int hp;
  int maxhp;
  Stats() : EntityComponent() {}
  Stats(int atk, int def, int spd, int hp) : atk(atk), def(def), spd(spd), hp(hp), maxhp(hp), EntityComponent(EC_STATS_ID, EC_PRIO_NULL) {}
  void Tick(Entity *src, Game *game) { }
  inline std::shared_ptr<EntityComponent> GetCopy() { return std::make_shared<Stats>(Stats(atk, def, spd, hp)); }
};

// Functions

template<class Archive>
inline void RegisterEntityComponentTypes(Archive &ar) {
    ar.template register_type<Renderable>();
    ar.template register_type<EntPosition>();
    ar.template register_type<Name>();
    ar.template register_type<Fov>();
    ar.template register_type<Player>();
    ar.template register_type<AnimalAi>();
    ar.template register_type<ActionTime>();
    ar.template register_type<MonsterAi>();
    ar.template register_type<Stats>();
}

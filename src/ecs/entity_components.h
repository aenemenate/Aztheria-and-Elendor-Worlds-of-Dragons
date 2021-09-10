#pragma once

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/base_object.hpp>
#include <memory>
#include <map>

#include "../base.h"
#include "../map/area.h"
#include "../world/time_system.h"
#include "../draw_funcs.h"

enum Material;

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
#define EC_CLASS_ID		8
#define EC_STATS_ID		9
#define EC_NOTSOLID_ID		10
#define EC_INVENTORY_ID		11
#define EC_EQUIPMENT_ID		12
#define EC_PICKABLE_ID		13
#define EC_POTION_ID		14
#define EC_MWEAPON_ID		15
#define EC_ARMOR_ID		16

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
  std::vector<Entity> visibleEntities;
  Fov() : EntityComponent() {}
  Fov(int _viewradius) : viewradius(_viewradius), EntityComponent(EC_FOV_ID, EC_PRIO_POST) {}
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

enum Attribute {
  Strength = 0,
  Intelligence = 1,
  Willpower = 2,
  Dexterity = 3,
  Speed = 4,
  Endurance = 5,
  Personality = 6,
  Luck = 7
};

enum Resource {
  Health,
  MaxHealth,
  Hunger,
  MaxHunger,
  Magicka,
  MaxMagicka,
  Stamina,
  MaxStamina
};

enum Skill {
  Forging = 0,
  HeavyWeapons,
  LongBlades,
  Blocking,

  Alchemy,
  Conjuration,
  Enchant,
  Lockpick,

  Destruction,
  Restoration,
  Illusion,

  LightArmor,
  Marksmanship,
  Sneaking,
  Crafting,

  Acrobatics,
  Brawling,
  ShortBlade,
  Unarmored,

  HeavyArmor,
  Spear,
  Swimming,

  Mercantile,
  Speech
};

Attribute AttributeFromName(std::string name);

Resource ResourceFromName(std::string name);

Skill SkillFromName(std::string name);

class Class : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
    ar & majorAttributes;
    ar & majorSkills;
    ar & minorSkills;
  }
public:
  std::vector<Attribute> majorAttributes;
  std::vector<Skill> majorSkills;
  std::vector<Skill> minorSkills;
  Class() : EntityComponent() {}
  Class(std::vector<Attribute> majorAttributes, std::vector<Skill> majorSkills, 
	std::vector<Skill> minorSkills) : majorAttributes(majorAttributes), 
	majorSkills(majorSkills), minorSkills(minorSkills), 
	EntityComponent(EC_CLASS_ID, EC_PRIO_NULL) {}
  void Tick(Entity *src, Game *game) { }
  inline std::shared_ptr<EntityComponent> GetCopy() { 
    return std::make_shared<Class>(Class(majorAttributes, majorSkills, minorSkills)); 
  }
};

class Stats : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
    ar & attributes;
    ar & resources;
    ar & skills;
  }
public:
  std::map<Attribute, int> attributes;
  std::map<Resource, int> resources;
  std::map<Skill, int> skills;
  Stats() : EntityComponent() {}
  Stats(std::shared_ptr<Class> uClass) : EntityComponent(EC_STATS_ID, EC_PRIO_NULL) {
    SetAttributes(uClass);
    SetResources();
    SetSkills(uClass);
  }
  Stats(std::map<Attribute, int> attributes, std::map<Resource, int> resources, std::map<Skill, int> skills) : 
	attributes(attributes), resources(resources), skills(skills), EntityComponent(EC_STATS_ID, EC_PRIO_NULL) {}
  void Tick(Entity *src, Game *game) { }
  inline std::shared_ptr<EntityComponent> GetCopy() { return std::make_shared<Stats>(Stats(attributes, resources, skills)); }
private:
  void SetAttributes(std::shared_ptr<Class> uClass);
  void SetResources();
  void SetSkills(std::shared_ptr<Class> uClass);
};

class NotSolid : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
  }
public:
  NotSolid() : EntityComponent(EC_NOTSOLID_ID, EC_PRIO_NULL) {}
  void Tick(Entity *src, Game *game) { }
  inline std::shared_ptr<EntityComponent> GetCopy() { return std::make_shared<NotSolid>(NotSolid()); }
};

class Inventory : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
    ar & inventory;
  }
public:
  std::vector<Entity*> inventory;
  Inventory() : EntityComponent(EC_INVENTORY_ID, EC_PRIO_NULL) {}
  Inventory(std::vector<Entity*> inventory) : inventory(inventory), EntityComponent(EC_INVENTORY_ID, EC_PRIO_NULL) {}
  void Tick(Entity *src, Game *game) { }
  inline std::shared_ptr<EntityComponent> GetCopy() { return std::make_shared<Inventory>(Inventory(inventory)); }
};

enum BodyPartType {
  PHead,
  PBody,
  PHand,
  PLegs,
  PFeet
};

class BodyPart {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & bodyPartType;
    ar & equippedEntity;
    ar & name;
  }
public:
  BodyPartType bodyPartType;
  Entity* equippedEntity;
  std::string name;
  BodyPart(BodyPartType _bodyPartType, std::string _name) {
    bodyPartType = _bodyPartType;
    equippedEntity = nullptr;
    name = _name;
  }
  BodyPart() {}
};

class Equipment : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
    ar & bodyParts;
  }
public:
  std::vector<BodyPart> bodyParts;
  Equipment() : EntityComponent(EC_EQUIPMENT_ID, EC_PRIO_NULL) {}
  Equipment(std::vector<BodyPart> bodyParts) : bodyParts(bodyParts), EntityComponent(EC_EQUIPMENT_ID, EC_PRIO_NULL) {}
  void Tick(Entity *src, Game *game) { }
  inline std::shared_ptr<EntityComponent> GetCopy() { return std::make_shared<Equipment>(Equipment(bodyParts)); }
};

class Pickable : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
  }
public:
  Pickable() : EntityComponent(EC_PICKABLE_ID, EC_PRIO_NULL) {}
  void Tick(Entity *src, Game *game) { }
  inline std::shared_ptr<EntityComponent> GetCopy() { return std::make_shared<Pickable>(Pickable()); }
};

class Potion : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
    ar & healthValue;
    ar & magickaValue;
    ar & staminaValue;
  }
public:
  int healthValue;
  int magickaValue;
  int staminaValue;
  Potion() : EntityComponent() {}
  Potion(int healthValue, int magickaValue, int staminaValue) : healthValue(healthValue), 
	magickaValue(magickaValue), staminaValue(staminaValue), EntityComponent(EC_POTION_ID, EC_PRIO_NULL) {}
  void Tick(Entity *src, Game *game) { }
  inline std::shared_ptr<EntityComponent> GetCopy() { return std::make_shared<Potion>(Potion(healthValue, magickaValue, staminaValue)); }
};

enum MWeaponType {
  MAxe = 0,
  MMace,
  MSword,
  MDagger,
  MSpear
};

std::string NameFromWeaponType(MWeaponType weaponType);

class MeleeWeapon : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
    ar & material;
    ar & weaponType;
  }
public:
  Material material;
  MWeaponType weaponType;
  MeleeWeapon() : EntityComponent() {}
  MeleeWeapon(Material material, MWeaponType weaponType) : material(material), 
	weaponType(weaponType), EntityComponent(EC_MWEAPON_ID, EC_PRIO_NULL) {}
  void Tick(Entity *src, Game *game) { }
  inline std::shared_ptr<EntityComponent> GetCopy() { return std::make_shared<MeleeWeapon>(MeleeWeapon(material, weaponType)); }
};

class Armor : public EntityComponent {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<EntityComponent>(*this);
    ar & material;
    ar & bodyPartType;
  }
public:
  Material material;
  BodyPartType bodyPartType;
  Armor() : EntityComponent() {}
  Armor(Material material, BodyPartType bodyPartType) : material(material), 
	bodyPartType(bodyPartType), EntityComponent(EC_ARMOR_ID, EC_PRIO_NULL) {}
  void Tick(Entity *src, Game *game) { }
  inline std::shared_ptr<EntityComponent> GetCopy() { return std::make_shared<Armor>(Armor(material, bodyPartType)); }
};

std::string ArmorNameFromInfo(BodyPartType bodyPartType, Material material);

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
    ar.template register_type<Class>();
    ar.template register_type<Stats>();
    ar.template register_type<NotSolid>();
    ar.template register_type<Inventory>();
    ar.template register_type<Equipment>();
    ar.template register_type<Pickable>();
    ar.template register_type<Potion>();
    ar.template register_type<MeleeWeapon>();
    ar.template register_type<Armor>();
}

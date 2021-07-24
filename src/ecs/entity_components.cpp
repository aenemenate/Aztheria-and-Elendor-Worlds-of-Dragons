#include "entity.h"
#include "entity_components.h"
#include "../game.h"
#include "../world/world.h"
#include "../map/area.h"
#include "../input_funcs.h"

#include <iostream>

Entity *Fov::ClosestVisibleEnemy(World *world, Position pos) {
  return nullptr;
}

void Fov::Tick(Entity *src, Game *game) {
  visiblepoints.clear();
  if (src->HasComponent(EC_POSITION_ID)) {
    World *world = game->world;
    std::shared_ptr<EntPosition> pos_c = dynamic_pointer_cast<EntPosition>(src->GetComponent(EC_POSITION_ID));
    Position *pos = &(pos_c->position);
    Area* map_ptr = world->GetArea(pos->wx, pos->wy);
    fov_circle(game->settings.get_fov_settings(), map_ptr, &visiblepoints, pos->x, pos->y, pos->z, viewradius);
    if (src->HasComponent(EC_PLAYER_ID))
      for (int vp = 0; vp < visiblepoints.size(); vp++) {
        Position *temp_pos = &(visiblepoints[vp]);
        map_ptr->GetTile(temp_pos->x, temp_pos->y, temp_pos->z)->explored = true;
        map_ptr->GetBlock(temp_pos->x, temp_pos->y, temp_pos->z)->explored = true;
      }
  }
}

void Player::Tick(Entity *src, Game *game) {
  Position plyr_pos = (dynamic_pointer_cast<EntPosition>(src->GetComponent(EC_POSITION_ID)))->position;
  switch (TerminalGetKey()) {
    case MTK_KP_8:
    case MTK_UP:
      src->actions.push_back(std::make_shared<Move>(Move(0,-1,0))); break;
    case MTK_KP_9:
      src->actions.push_back(std::make_shared<Move>(Move(1,-1,0))); break;
    case MTK_KP_6:
    case MTK_RIGHT:
      src->actions.push_back(std::make_shared<Move>(Move(1,0,0))); break;
    case MTK_KP_3:
      src->actions.push_back(std::make_shared<Move>(Move(1,1,0))); break;
    case MTK_KP_2:
    case MTK_DOWN:
      src->actions.push_back(std::make_shared<Move>(Move(0,1,0))); break;
    case MTK_KP_1:
      src->actions.push_back(std::make_shared<Move>(Move(-1,1,0))); break;
    case MTK_KP_4:
    case MTK_LEFT:
      src->actions.push_back(std::make_shared<Move>(Move(-1,0,0))); break;
    case MTK_KP_7:
      src->actions.push_back(std::make_shared<Move>(Move(-1,-1,0))); break;
    case MTK_KP_ENTER:
    case MTK_ENTER:
      src->actions.push_back(std::make_shared<ActivateBlock>(ActivateBlock(0, 0))); break;
  }
}

void AnimalAi::Tick(Entity *src, Game *game) {
  if (src->HasComponent(EC_POSITION_ID) && src->actions.size() == 0) {
    Position pos = (dynamic_pointer_cast<EntPosition>(src->GetComponent(EC_POSITION_ID)))->position;
    if (src->HasComponent(EC_FOV_ID)) {
      std::shared_ptr<Fov> fov = dynamic_pointer_cast<Fov>(src->GetComponent(EC_FOV_ID));
      Entity *ent;
      if ((ent = fov->ClosestVisibleEnemy(game->world, pos)) != nullptr) {
	// move away from entity
      }
      else {
	// random move
	int dir = rand()%8;
	switch (dir) {
	  case (0):
            src->actions.push_back(std::make_shared<Move>(Move(-1,0,0))); break;
	  case (1): 
	    src->actions.push_back(std::make_shared<Move>(Move(1,0,0))); break;
	  case (2):
	    src->actions.push_back(std::make_shared<Move>(Move(0,1,0))); break;
	  case (3):
	    src->actions.push_back(std::make_shared<Move>(Move(0,-1,0))); break;
	  case (4):
            src->actions.push_back(std::make_shared<Move>(Move(1,1,0))); break;
	  case (5): 
	    src->actions.push_back(std::make_shared<Move>(Move(1,-1,0))); break;
	  case (6):
	    src->actions.push_back(std::make_shared<Move>(Move(-1,1,0))); break;
	  case (7):
	    src->actions.push_back(std::make_shared<Move>(Move(-1,-1,0))); break;
	}
      }
    }
  }
}

void MonsterAi::Tick(Entity *src, Game *game) {
  if (src->HasComponent(EC_POSITION_ID) && src->actions.size() == 0) {
    Position pos = (dynamic_pointer_cast<EntPosition>(src->GetComponent(EC_POSITION_ID)))->position;
    if (src->HasComponent(EC_FOV_ID)) {
      std::shared_ptr<Fov> fov = dynamic_pointer_cast<Fov>(src->GetComponent(EC_FOV_ID));
      Entity *ent;
      if ((ent = fov->ClosestVisibleEnemy(game->world, pos)) != nullptr) {
	// move away from entity
      }
      else {
	// random move
	int dir = rand()%8;
	switch (dir) {
	  case (0):
            src->actions.push_back(std::make_shared<Move>(Move(-1,0,0))); break;
	  case (1): 
	    src->actions.push_back(std::make_shared<Move>(Move(1,0,0))); break;
	  case (2):
	    src->actions.push_back(std::make_shared<Move>(Move(0,1,0))); break;
	  case (3):
	    src->actions.push_back(std::make_shared<Move>(Move(0,-1,0))); break;
	  case (4):
            src->actions.push_back(std::make_shared<Move>(Move(1,1,0))); break;
	  case (5): 
	    src->actions.push_back(std::make_shared<Move>(Move(1,-1,0))); break;
	  case (6):
	    src->actions.push_back(std::make_shared<Move>(Move(-1,1,0))); break;
	  case (7):
	    src->actions.push_back(std::make_shared<Move>(Move(-1,-1,0))); break;
	}
      }
    }
  }
}

Attribute AttributeFromName(std::string name) {
    if (name == "Strength") return Strength;
    if (name == "Intelligence") return Intelligence;
    if (name == "Willpower") return Willpower;
    if (name == "Dexterity") return Dexterity;
    if (name =="Speed") return Speed;
    if (name == "Endurance") return Endurance;
    if (name =="Personality") return Personality;
    if (name == "Luck") return Luck;
    return Strength;
}

Resource ResourceFromName(std::string name) {
    if (name == "Health") return Health;
    if (name == "MaxHealth") return MaxHealth;
    if (name == "Hunger") return Hunger;
    if (name == "MaxHunger") return MaxHunger;
    if (name == "Magicka") return Magicka;
    if (name == "MaxMagicka") return MaxMagicka;
    if (name == "Stamina") return Stamina;
    if (name == "MaxStamina") return MaxStamina;
    return Health;
}

Skill SkillFromName(std::string name) {
    if (name == "HeavyArmor") return HeavyArmor;
    if (name == "Spear") return Spear;
    if (name == "Blocking") return Blocking;
    if (name == "Brawling") return Brawling;
    if (name == "Forging") return Forging;
    if (name == "HeavyWeapons") return HeavyWeapons;
    if (name == "LongBlades") return LongBlades;
    if (name == "LightArmor") return LightArmor;
    if (name == "Marksmanship") return Marksmanship;
    if (name == "Sneaking") return Sneaking;
    if (name == "Acrobatics") return Acrobatics;
    if (name == "Swimming") return Swimming;
    if (name == "ShortBlade") return ShortBlade;
    if (name == "Unarmored") return Unarmored;
    if (name == "Illusion") return Illusion;
    if (name == "Mercantile") return Mercantile;
    if (name == "Speech") return Speech;
    if (name == "Alchemy") return Alchemy;
    if (name == "Conjuration") return Conjuration;
    if (name == "Enchant") return Enchant;
    if (name == "Lockpick") return Lockpick;
    if (name == "Destruction") return Destruction;
    if (name == "Restoration") return Restoration;
    if (name == "Crafting") return Crafting;
    return HeavyArmor;
}


void Stats::SetAttributes(std::shared_ptr<Class> uClass) {
  attributes[Strength] = 40;
  attributes[Intelligence] = 40;
  attributes[Willpower] = 40;
  attributes[Dexterity] = 40;
  attributes[Speed] = 40;
  attributes[Endurance] = 40;
  attributes[Personality] = 40;
  attributes[Luck] = 40;
  for (int i = 0; i < uClass->majorAttributes.size(); ++i)
    attributes[uClass->majorAttributes[i]] += 10;
}

void Stats::SetResources() {
  int hp = (attributes[Strength] + attributes[Endurance]) / 2;
  int mp = attributes[Intelligence];
  int sp = attributes[Strength] + attributes[Willpower] + attributes[Speed] + attributes[Endurance];
  resources[Health] = hp;
  resources[MaxHealth] = hp;
  resources[Hunger] = hp + sp;
  resources[MaxHunger] = hp + sp;
  resources[Magicka] = mp;
  resources[MaxMagicka] = mp;
  resources[Stamina] = sp;
  resources[MaxStamina] = sp;
}

void Stats::SetSkills(std::shared_ptr<Class> uClass) {
  skills[HeavyArmor] = 5;
  skills[Spear] = 5;
  skills[Blocking] = 5;
  skills[Brawling] = 5;
  skills[Forging] = 5;
  skills[HeavyWeapons] = 5;
  skills[LongBlades] = 5;
  skills[LightArmor] = 5;
  skills[Marksmanship] = 5;
  skills[Sneaking] = 5;
  skills[Acrobatics] = 5;
  skills[Swimming] = 5;
  skills[ShortBlade] = 5;
  skills[Unarmored] = 5;
  skills[Illusion] = 5;
  skills[Mercantile] = 5;
  skills[Speech] = 5;
  skills[Alchemy] = 5;
  skills[Conjuration] = 5;
  skills[Enchant] = 5;
  skills[Lockpick] = 5;
  skills[Destruction] = 5;
  skills[Restoration] = 5;
  skills[Crafting] = 5;
  for (int i = 0; i < uClass->majorSkills.size(); ++i)
    skills[uClass->majorSkills[i]] += 25;
  for (int i = 0; i < uClass->minorSkills.size(); ++i)
    skills[uClass->minorSkills[i]] += 10;
}
#include "entity_components.h"
#include "../game.h"
#include "../world/world.h"
#include "../map/area.h"
#include "../input_funcs.h"
#include "../materials.h"

#include <iostream>

void Fov::Tick(Entity *src, Game *game) {
  visiblepoints.clear();
  visibleEntities.clear();
  if (src->HasComponent(EC_POSITION_ID)) {
    World *world = game->world;
    std::shared_ptr<EntPosition> pos_c = dynamic_pointer_cast<EntPosition>(src->GetComponent(EC_POSITION_ID));
    Position *pos = &(pos_c->position);
    Area* map_ptr = world->GetArea(pos->wx, pos->wy);
    fov_circle(game->settings.get_fov_settings(), map_ptr, world, &visiblepoints, pos->x, pos->y, pos->z, pos->wx, pos->wy, viewradius);
      for (int vp = 0; vp < visiblepoints.size(); vp++) {
        Position *temp_pos = &(visiblepoints[vp]);
	if (src->HasComponent(EC_PLAYER_ID)) {
          map_ptr->GetTile(temp_pos->x, temp_pos->y, temp_pos->z)->explored = true;
          map_ptr->GetBlock(temp_pos->x, temp_pos->y, temp_pos->z)->explored = true;
	}
        if (map_ptr->GetEntity(temp_pos->x,temp_pos->y, pos->z) != -1) {
	  visibleEntities.push_back(world->entities[map_ptr->GetEntity(temp_pos->x,temp_pos->y, pos->z)]);
	} 
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

void MonsterAi::Tick(Entity *src, Game *game) {
  if (src->HasComponent(EC_POSITION_ID) && src->actions.size() == 0) {
    Position pos = (dynamic_pointer_cast<EntPosition>(src->GetComponent(EC_POSITION_ID)))->position;
    if (src->HasComponent(EC_FOV_ID)) {
      std::shared_ptr<Fov> fov_c = dynamic_pointer_cast<Fov>(src->GetComponent(EC_FOV_ID));
      bool found_player = false;
      Position player_pos;
      for (int i = 0; i < fov_c->visibleEntities.size(); ++i) {
        if (fov_c->visibleEntities[i].HasComponent(EC_PLAYER_ID)) {
          std::shared_ptr<EntPosition> player_pos_c = dynamic_pointer_cast<EntPosition>(fov_c->visibleEntities[i].GetComponent(EC_POSITION_ID));
          player_pos = player_pos_c->position;
	  found_player = true;
      break;
        }
      }
      if (found_player) {
          Position closest_pos = { pos.x, pos.y, pos.z, pos.wx, pos.wy };
        for (uint16_t i = pos.x - 1; i <= pos.x + 1; ++i)
	  for (uint16_t j = pos.y - 1; j <= pos.y + 1; ++j) {
	    int prev_dist = ManhattanDistance(closest_pos.x, closest_pos.y, player_pos.x,  player_pos.y);
	    int cur_dist = ManhattanDistance(i, j, player_pos.x, player_pos.y);
	    if (prev_dist >= cur_dist) {
              closest_pos = {i, j, closest_pos.z, closest_pos.wx, closest_pos.wy};
            }
          }
	src->actions.push_back(std::make_shared<Move>(Move(closest_pos.x - pos.x, closest_pos.y - pos.y, 0)));
      }	// random move
      else {
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

std::string NameFromAttribute(Attribute att) {
    if (att == Strength) return "Strength";
    if (att == Intelligence) return "Intelligence";
    if (att == Willpower) return "Willpower";
    if (att == Dexterity) return "Dexterity";
    if (att == Speed) return "Speed";
    if (att == Endurance) return "Endurance";
    if (att == Personality) return "Personality";
    if (att == Luck) return "Luck";
    return "";
}

std::string NameFromSkill(Skill skill) {
    if (skill == HeavyArmor) return "Heavy Armor";
    if (skill == Spear) return "Spears";
    if (skill == Blocking) return "Blocking";
    if (skill == Brawling) return "Brawling";
    if (skill == Forging) return "Forging";
    if (skill == HeavyWeapons) return "Heavy Weapons";
    if (skill == LongBlades) return "Long Blades";
    if (skill == LightArmor) return "Light Armor";
    if (skill == Marksmanship) return "Marksmanship";
    if (skill == Sneaking) return "Sneaking";
    if (skill == Acrobatics) return "Acrobatics";
    if (skill == Swimming) return "Swimming";
    if (skill == ShortBlade) return "Short Blades";
    if (skill == Unarmored) return "Unarmored";
    if (skill == Illusion) return "Illusion";
    if (skill == Mercantile) return "Mercantile";
    if (skill == Speech) return "Speech";
    if (skill == Alchemy) return "Alchemy";
    if (skill == Conjuration) return "Conjuration";
    if (skill == Enchant) return "Enchanting";
    if (skill == Lockpick) return "Lockpick";
    if (skill == Destruction) return "Destruction";
    if (skill == Restoration) return "Restoration";
    if (skill == Crafting) return "Crafting";
    return "HeavyArmor";
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

BodyPartType BodyPartTypeFromName(std::string name) {
  if (name == "Hand")
    return PHand;
  if (name == "Head")
    return PHead;
  if (name == "Body")
    return PBody;
  if (name == "Legs")
    return PLegs;
  if (name == "Feet")
    return PFeet;
  return PHand;
}

std::string NameFromWeaponType(MWeaponType weaponType) {
  switch (weaponType) {
    case (MAxe):
      return "axe";
    case (MMace):
      return "mace";
    case (MSword):
      return "sword";
    case (MDagger):
      return "dagger";
    case (MSpear):
      return "spear";
  }
}

std::string ArmorNameFromInfo(BodyPartType bodyPartType, Material material) {
  switch (material) {
    case (Copper):
    case (Bronze):
    case (Brass):
    case (Iron):
    case (Steel):
    case (Platinum):
      switch (bodyPartType) {
        case (PHead):
          return "helmet";
        case (PBody):
          return "chestpiece";
        case (PLegs):
          return "chausses";
        case (PFeet):
          return "grieves";
      }
    break;
    case (Glass):
    case (Leather):
    case (Cloth):
    case (Silk):
    case (Bone):
      switch (bodyPartType) {
        case (PHead):
          return "hat";
        case (PBody):
          return "shirt";
        case (PLegs):
          return "pants";
        case (PFeet):
          return "shoes";
      }
    break;
  }
  return "";
}

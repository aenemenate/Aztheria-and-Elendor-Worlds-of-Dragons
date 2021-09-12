#include "entity_actions.h"
#include "../game.h"
#include "../world/world.h"
#include "../map/area.h"
#include "entity.h"
#include "../materials.h"

#include <random>
#include <string>
#include <iostream>

std::shared_ptr<MeleeWeapon> GetWeapon(Entity *src, World *world) {
  if (src->HasComponent(EC_EQUIPMENT_ID)) {
    std::shared_ptr<Equipment> equipment = dynamic_pointer_cast<Equipment>(src->GetComponent(EC_EQUIPMENT_ID));
    std::vector<std::shared_ptr<MeleeWeapon>> weapons;
    for (int i = 0; i < equipment->bodyParts.size(); ++i) {
      if (equipment->bodyParts[i].equippedEntity != -1) {
	Entity *equipped = &(world->entities[equipment->bodyParts[i].equippedEntity]);
	if (equipped->HasComponent(EC_MWEAPON_ID)) {
          weapons.push_back(dynamic_pointer_cast<MeleeWeapon>(equipped->GetComponent(EC_MWEAPON_ID)));
        }
      }
    }
    if (weapons.size() > 0) {
      int weapon = rand()%weapons.size();
      return weapons[weapon];
    }
  }
  return nullptr;
}

Skill GetArmorSkill(std::shared_ptr<Armor> armorPiece) {
  if (armorPiece == nullptr)
    return Unarmored;
  switch (armorPiece->material) {
    case (Copper):
    case (Bronze):
    case (Brass):
    case (Iron):
    case (Steel):
    case (Platinum):
      return HeavyArmor;
  }
  return LightArmor;
}

int GetArmorValue(Entity *src, World *world) {
  int armorValue = 0;
  if (src->HasComponent(EC_EQUIPMENT_ID)) {
    std::shared_ptr<Equipment> equipment = dynamic_pointer_cast<Equipment>(src->GetComponent(EC_EQUIPMENT_ID));
    std::shared_ptr<Stats> stats = dynamic_pointer_cast<Stats>(src->GetComponent(EC_STATS_ID));
    for (int i = 0; i < equipment->bodyParts.size(); ++i) {
      if (equipment->bodyParts[i].equippedEntity != -1) {
	Entity *equipped = &(world->entities[equipment->bodyParts[i].equippedEntity]);
	if (equipped->HasComponent(EC_ARMOR_ID)) {
          std::shared_ptr<Armor> armorPiece = dynamic_pointer_cast<Armor>(equipped->GetComponent(EC_ARMOR_ID));
          switch (armorPiece->bodyPartType) {
	    case (PHead):
              armorValue += .33 * GetImpactYields()[armorPiece->material] * stats->skills[GetArmorSkill(armorPiece)] / 30;
	      break;
	    case (PBody):
	    case (PLegs):
	    case (PFeet):
              armorValue += .22 * GetImpactYields()[armorPiece->material] * stats->skills[GetArmorSkill(armorPiece)] / 30;
	      break;
          }
        }
      }
    }
  }
  return armorValue;
}

int GetWeaponDamage(std::shared_ptr<MeleeWeapon> weapon) {
  if (weapon != nullptr) {
    switch (weapon->weaponType) {
      case(MAxe):
        return GetShearYields()[weapon->material] * 1.5;
      case(MSword):
        return GetShearYields()[weapon->material];
      case(MDagger):
        return GetShearYields()[weapon->material] * .5;
      case(MSpear):
        return GetShearYields()[weapon->material] * 1.25;
      case(MMace):
        return GetImpactYields()[weapon->material] * 1.5;
    }
  }
  return GetImpactYields()[Bone];
}

int GetWeaponSkill(Entity *src, std::shared_ptr<MeleeWeapon> weapon) {
  std::shared_ptr<Stats> src_stats = dynamic_pointer_cast<Stats>(src->GetComponent(EC_STATS_ID));
  if (weapon != nullptr) {
    switch (weapon->weaponType) {
      case(MMace):
      case(MAxe):
        return src_stats->skills[HeavyWeapons];
      case(MSword):
        return src_stats->skills[LongBlades];
      case(MDagger):
        return src_stats->skills[ShortBlade];
      case(MSpear):
        return src_stats->skills[Spear];
    }
  }
  return src_stats->skills[Brawling];
}

int GetWeaponStaminaCost(std::shared_ptr<MeleeWeapon> weapon) {
  if (weapon != nullptr) {
    switch (weapon->weaponType) {
      case(MAxe):
        return 10 * 1.5;
      case(MSword):
        return 10;
      case(MDagger):
        return 10 * .5;
      case(MSpear):
        return 10 * 1.25;
      case(MMace):
        return 10 * 1.5;
    }
  }
  return 10 * .5;
}

int Attack(Entity *src, Entity *def, World *world) {
  std::string message;
  srand(time(0));
  if (src->HasComponent(EC_STATS_ID) && def->HasComponent(EC_STATS_ID)) {
    std::shared_ptr<Stats> src_stats = dynamic_pointer_cast<Stats>(src->GetComponent(EC_STATS_ID));
    std::shared_ptr<Stats> def_stats = dynamic_pointer_cast<Stats>(def->GetComponent(EC_STATS_ID));
    std::shared_ptr<Name> src_name = dynamic_pointer_cast<Name>(src->GetComponent(EC_NAME_ID));
    std::shared_ptr<Name> def_name = dynamic_pointer_cast<Name>(def->GetComponent(EC_NAME_ID));
    if (def_stats->resources[Health] <= 0)
      return 0;
    std::shared_ptr<MeleeWeapon> weapon = GetWeapon(src, world);
    bool attackLanded = rand()%100 <= (GetWeaponSkill(src, weapon) + src_stats->attributes[Speed] / 5 + src_stats->attributes[Luck] / 10) 
			* (0.75 + 0.5 * src_stats->resources[Stamina] / src_stats->resources[MaxStamina]);
    if (attackLanded) {
      bool attackEvaded = rand()%100 <= (def_stats->attributes[Speed] / 5 + def_stats->attributes[Luck] / 10) 
			* (0.75 + 0.5 * def_stats->resources[Stamina] / def_stats->resources[MaxStamina]);
      if (!attackEvaded) {
        int weaponDamage = GetWeaponDamage(weapon);
        int damage = weaponDamage * ((double)(src_stats->attributes[Strength] + 50) / 100.0);
        damage /= (1 + GetArmorValue(def, world) / damage) > 4 ? 4 : 1 + GetArmorValue(def, world) / damage;
        message = src_name->name + " attacked " + def_name->name + " for " + std::to_string(damage) + " points.";
	def_stats->resources[Health] -= damage;
        if (def_stats->resources[Health] <= 0) {
          def_name->name = "dead " + def_name->name;
          std::shared_ptr<Renderable> def_renderable = dynamic_pointer_cast<Renderable>(def->GetComponent(EC_RENDERABLE_ID));
          def_renderable->graphic.fgcolor = "red";
        }
      }
      else {
        message = def_name->name + " evaded " + src_name->name + "\'s attack.";
      }
    }
    else {
      message = src_name->name + " attacked " + def_name->name + " and missed.";
    }
    if (src->HasComponent(EC_PLAYER_ID) || def->HasComponent(EC_PLAYER_ID)) {
      world->msgConsole.PushLine(message);
    }
    src_stats->resources[Stamina] -= GetWeaponStaminaCost(weapon);
    if (src_stats->resources[Stamina] < 0) src_stats->resources[Stamina] = 0;
    return 1000;
  }
  else
    return 0;
}

int GetItem(Entity *src, Position position, int item, World *world) {
  std::string message;
  if (src->HasComponent(EC_INVENTORY_ID)) {
    std::shared_ptr<Inventory> inventory = dynamic_pointer_cast<Inventory>(src->GetComponent(EC_INVENTORY_ID));
    inventory->inventory.push_back(item);
    Area *curmap = world->GetArea(position.wx,position.wy);
    curmap->SetEntity(position.x, position.y, position.z, -1);
    std::shared_ptr<Name> src_name = dynamic_pointer_cast<Name>(src->GetComponent(EC_NAME_ID));
    std::shared_ptr<Name> item_name = dynamic_pointer_cast<Name>(world->entities[item].GetComponent(EC_NAME_ID));
    message = src_name->name + " picked up " + item_name->name + ".";
    world->msgConsole.PushLine(message);
    return 1000;
  }
  return 0;
}

int Move::Do(Entity *src, World *world) {
  int cost = 0;
// clamp sign between -1 and 1
  int xsign = (xdir <= -1) ? -1 : ((xdir >= 1) ? 1 : 0);
  int ysign = (ydir <= -1) ? -1 : ((ydir >= 1) ? 1 : 0);
  int zsign = (zdir <= -1) ? -1 : ((zdir >= 1) ? 1 : 0);
// set variables
  if (src->HasComponent(EC_POSITION_ID)) {
    std::shared_ptr<EntPosition> pos_c = dynamic_pointer_cast<EntPosition>(src->GetComponent(EC_POSITION_ID));
    Position *pos = &(pos_c->position);
    uint16_t new_x = pos->x + xsign, 
        new_y = pos->y + ysign,
        new_z = pos->z + zsign;
    uint16_t new_wx = pos->wx, new_wy = pos->wy;
    Area *curmap = world->GetArea(pos->wx,pos->wy);
// if new position is not within map bounds
    if (!curmap->PointWithinBounds(new_x, new_y) && new_z == pos->z && pos->z == 0) {
// check in each direction and set the new_wx, new_wy and new_x and new_y
      new_x = pos->x;
      new_y = pos->y;
      if (new_x + xsign < 0 && xsign == -1 && pos->wx - 1 >= 0) {
        new_x = new_x + xsign + curmap->width;
        new_wx -= 1;
      }
      else if (new_x + xsign >= curmap->width && xsign == 1 && pos->wx + 1 < world->width) {
        new_x = new_x + xsign - curmap->width;
        new_wx += 1;
      }
      if (new_y + ysign < 0 && ysign == -1 && pos->wy - 1 >= 0) {
        new_y = new_y + ysign + curmap->height;
        new_wy -= 1;
      }
      else if (new_y + ysign >= curmap->height && ysign == 1 && pos->wy + 1 < world->height) {
        new_y = new_y + ysign - curmap->height;
        new_wy += 1;
      }
    }
// else if this z is not the same as before
    else if (new_z != pos->z && curmap->PointWithinBounds(new_x, new_y)) {
      if (!(new_z >= 0 && new_z <= curmap->GetDungeonFloors()->size())) {
        new_z = pos->z;
      }
    }
// else if it is on the current map, check if the new position is walkable
    else if (curmap->PointWithinBounds(new_x, new_y)) {
      if (new_z >= 0 && new_z <= curmap->GetDungeonFloors()->size()
      && !curmap->GetTile(new_x, new_y, new_z)->walkable
      || curmap->GetBlock(new_x, new_y, new_z)->solid
      || curmap->GetEntity(new_x, new_y, new_z) != -1) {
        if (curmap->GetEntity(new_x, new_y, new_z) != -1) {
          Entity *entity = &(world->entities[curmap->GetEntity(new_x, new_y, new_z)]);
          if (entity->HasComponent(EC_PICKABLE_ID))
	    cost = GetItem(src, {new_x, new_y, new_z, new_wx, new_wy}, curmap->GetEntity(new_x, new_y, new_z), world);
	  else
            cost = Attack(src, entity, world);
        }
        new_x = pos->x;
        new_y = pos->y;
        new_z = pos->z;
      }
    }
    else if (!curmap->PointWithinBounds(new_x, new_y)) {
      new_x = pos->x;
      new_y = pos->y;
    }
// if we decided to move maps, check if the desired position is actually walkable
    if (new_wx != pos->wx || new_wy != pos->wy || new_z != pos->z) {
      Area *newmap = world->GetArea(new_wx, new_wy);
      if (!newmap->GetTile(new_x,new_y,new_z)->walkable
      ||  newmap->GetBlock(new_x,new_y,new_z)->solid
      ||  newmap->GetEntity(new_x, new_y, new_z) != -1) {
        if (newmap->GetEntity(new_x, new_y, new_z) != -1) {
          Entity *entity = &(world->entities[newmap->GetEntity(new_x, new_y, new_z)]);
          if (entity->HasComponent(EC_PICKABLE_ID))
	    cost = GetItem(src, {new_x, new_y, new_z, new_wx, new_wy}, newmap->GetEntity(new_x, new_y, new_z), world);
	  else
            cost = Attack(src, entity, world);
        }
	new_x = pos->x;
	new_y = pos->y;
	new_z = pos->z;
	new_wx = pos->wx;
	new_wy = pos->wy;
      }
    }
    if (new_x != pos->x || new_y != pos->y || new_z != pos->z || new_wx != pos->wx || new_wy != pos->wy) {
      world->GetArea(pos->wx, pos->wy)->SetEntity(pos->x,pos->y,pos->z,-1);
      world->GetArea(new_wx, new_wy)->SetEntity(new_x,new_y,new_z,src->Id);
      cost = 1000;
    }
// set the new positions
    pos->x = new_x;
    pos->y = new_y;
    pos->z = new_z;
    pos->wx = new_wx;
    pos->wy = new_wy;
  }
  if (cost > 0) {
    std::shared_ptr<Stats> src_stats = dynamic_pointer_cast<Stats>(src->GetComponent(EC_STATS_ID));
    src_stats->resources[Stamina] += src_stats->resources[MaxStamina]/20;
    if (src_stats->resources[Stamina] >= src_stats->resources[MaxStamina])
      src_stats->resources[Stamina] = src_stats->resources[MaxStamina];
  }
  return cost;
}

int ActivateBlock::Do(Entity *src, World *world) {
  int cost = 0;
  if (src->HasComponent(EC_POSITION_ID)) {
    Position this_pos = ( dynamic_pointer_cast<EntPosition>( src->GetComponent(EC_POSITION_ID) ) )->position;
    Position block_pos = { this_pos.x + this->xdir, this_pos.y + this->ydir, this_pos.z, this_pos.wx, this_pos.wy };
    world->GetArea(block_pos.wx, block_pos.wy)->GetBlock(block_pos.x, block_pos.y, block_pos.z)->Activate((void*)src, world);
    cost = 1000;
  }
  return cost;
}

int UseItem::Do(Entity *src, World *world) {
  std::shared_ptr<Inventory> inventory = dynamic_pointer_cast<Inventory>(src->GetComponent(EC_INVENTORY_ID));
  if (itemIndex < inventory->inventory.size()) {
    Entity *item = &(world->entities[inventory->inventory[itemIndex]]);
    if (item->HasComponent(EC_MWEAPON_ID)) {
      // equip weapon
      std::shared_ptr<Equipment> equipment = dynamic_pointer_cast<Equipment>(src->GetComponent(EC_EQUIPMENT_ID));
      std::vector<BodyPart*> hands;
      for (int i = 0; i < equipment->bodyParts.size(); ++i) {
	if (equipment->bodyParts[i].bodyPartType == PHand) {
	  hands.push_back(&(equipment->bodyParts[i]));
        }
      }
      if (hands.size() > 0) {
        for (int i = 0; i < hands.size(); ++i) {
	  if (hands[i]->equippedEntity == -1) {
            hands[i]->equippedEntity = inventory->inventory[itemIndex];
            inventory->inventory.erase(inventory->inventory.begin() + itemIndex);
	    break;
	  }
        }
      }
    }
    if (item->HasComponent(EC_ARMOR_ID)) {
      std::shared_ptr<Equipment> equipment = dynamic_pointer_cast<Equipment>(src->GetComponent(EC_EQUIPMENT_ID));
      std::shared_ptr<Armor> armor_c = dynamic_pointer_cast<Armor>(item->GetComponent(EC_ARMOR_ID));
      for (int i = 0; i < equipment->bodyParts.size(); ++i) {
	if (equipment->bodyParts[i].bodyPartType == armor_c->bodyPartType) {
	  if (equipment->bodyParts[i].equippedEntity == -1) {
            equipment->bodyParts[i].equippedEntity = inventory->inventory[itemIndex];
            inventory->inventory.erase(inventory->inventory.begin() + itemIndex);
	  }
        }
      }
    }
    if (item->HasComponent(EC_POTION_ID)) {
      std::shared_ptr<Potion> potion_c = dynamic_pointer_cast<Potion>(item->GetComponent(EC_POTION_ID));
      std::shared_ptr<Stats> src_stats = dynamic_pointer_cast<Stats>(src->GetComponent(EC_STATS_ID));
      src_stats->resources[Health]  += potion_c->healthValue;
      src_stats->resources[Magicka] += potion_c->magickaValue;
      src_stats->resources[Stamina] += potion_c->staminaValue;
      if (src_stats->resources[Health] > src_stats->resources[MaxHealth])
        src_stats->resources[Health] = src_stats->resources[MaxHealth];
      if (src_stats->resources[Magicka] > src_stats->resources[MaxMagicka])
        src_stats->resources[Magicka] = src_stats->resources[MaxMagicka];
      if (src_stats->resources[Stamina] > src_stats->resources[MaxStamina])
        src_stats->resources[Stamina] = src_stats->resources[MaxStamina];
      inventory->inventory.erase(inventory->inventory.begin() + itemIndex);
      return 2000;
    }
  }
  return 0;
}

int Unequip::Do(Entity *src, World *world) {
  std::shared_ptr<Inventory> inventory = dynamic_pointer_cast<Inventory>(src->GetComponent(EC_INVENTORY_ID));
  std::shared_ptr<Equipment> equipment = dynamic_pointer_cast<Equipment>(src->GetComponent(EC_EQUIPMENT_ID));
  if (equipment->bodyParts[itemIndex].equippedEntity != -1) {
    inventory->inventory.push_back(equipment->bodyParts[itemIndex].equippedEntity);
    equipment->bodyParts[itemIndex].equippedEntity = -1;
  }
  return 0;
}
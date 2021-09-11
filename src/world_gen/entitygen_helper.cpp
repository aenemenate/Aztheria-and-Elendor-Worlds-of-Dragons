#include "entitygen_helper.h"
#include "../materials.h"
#include "../ecs/entity.h"
#include "../world/world.h"


Entity MakeWeaponOfType(MWeaponType weaponType, Material material) {
  Entity entity = Entity();
  entity.AddComponent(std::make_shared<Renderable>(Renderable({"/", GetMaterialColors()[material], "black"})));
  entity.AddComponent(std::make_shared<Name>(Name(std::string{GetMaterialNames()[material] + " " + NameFromWeaponType(weaponType)})));
  entity.AddComponent(std::make_shared<MeleeWeapon>(MeleeWeapon(material, weaponType)));
  entity.AddComponent(std::make_shared<NotSolid>(NotSolid()));
  entity.AddComponent(std::make_shared<Pickable>(Pickable()));
  return entity;
}

Entity MakeArmorOfType(BodyPartType bodyPartType, Material material) {
  Entity entity = Entity();
  entity.AddComponent(std::make_shared<Renderable>(Renderable({"$", GetMaterialColors()[material], "black"})));
  entity.AddComponent(std::make_shared<Name>(Name(std::string{GetMaterialNames()[material] + " " + ArmorNameFromInfo(bodyPartType, material)})));
  entity.AddComponent(std::make_shared<Armor>(Armor(material, bodyPartType)));
  entity.AddComponent(std::make_shared<NotSolid>(NotSolid()));
  entity.AddComponent(std::make_shared<Pickable>(Pickable()));
  return entity;
}

MWeaponType GetWeaponTypeFromSkills(Entity *src) {
  std::shared_ptr<Class> uclass = dynamic_pointer_cast<Class>(src->GetComponent(EC_CLASS_ID));
  Skill weaponSkill = Brawling;
  for (int i = 0; i < uclass->minorSkills.size(); ++i) {
    if (uclass->minorSkills[i] == HeavyWeapons || uclass->minorSkills[i] == LongBlades
    || uclass->minorSkills[i] == ShortBlade || uclass->minorSkills[i] == Spear)
      weaponSkill = uclass->minorSkills[i]; 
  }
  for (int i = 0; i < uclass->majorSkills.size(); ++i) {
    if (uclass->majorSkills[i] == HeavyWeapons || uclass->majorSkills[i] == LongBlades
    || uclass->majorSkills[i] == ShortBlade || uclass->majorSkills[i] == Spear)
      weaponSkill = uclass->majorSkills[i]; 
  }
  switch (weaponSkill) {
    case (HeavyWeapons):
      if (rand()%50 >= 50)
        return MAxe;
      else
        return MMace;
    case (LongBlades):
      return MSword;
    case (ShortBlade):
      return MDagger;
    case (Spear):
      return MSpear;
    default:
      return MDagger;
  }
}

Material GetWeaponMaterial() {
  switch (rand()%6) {
    case (0):
      return Copper;
    case (1):
      return Bronze;
    case (2):
      return Brass;
    case (3):
      return Iron;
    case (4):
      return Steel;
    case (5):
      return Platinum;
  }
}

Material GetArmorMaterialFromSkills(Entity *src) {
  std::shared_ptr<Class> uclass = dynamic_pointer_cast<Class>(src->GetComponent(EC_CLASS_ID));
  Skill armorSkill = Unarmored;
  for (int i = 0; i < uclass->minorSkills.size(); ++i) {
    if (uclass->minorSkills[i] == HeavyArmor || uclass->minorSkills[i] == LightArmor)
      armorSkill = uclass->minorSkills[i];
  }
  for (int i = 0; i < uclass->majorSkills.size(); ++i) {
    if (uclass->majorSkills[i] == HeavyArmor || uclass->majorSkills[i] == LightArmor)
      armorSkill = uclass->majorSkills[i];
  }
  if (armorSkill == HeavyArmor) {
    return GetWeaponMaterial();
  }
  if (armorSkill == LightArmor) {
    switch (rand()%5) {
      case (0):
        return Glass;
      case (1):
	return Leather;
      case (2):
	return Cloth;
      case (3):
	return Silk;
      case (4):
	return Bone;
    }
  }
  return Cloth;
}

void EquipEntity(int src_id, World *world) {
  Entity *src = &(world->entities[src_id]);
  std::shared_ptr<Equipment> equipment = dynamic_pointer_cast<Equipment>(src->GetComponent(EC_EQUIPMENT_ID));
  for (int i = 0; i < equipment->bodyParts.size(); ++i) {
    src = &(world->entities[src_id]);
    if (rand()%100 >= 50 || equipment->bodyParts[i].bodyPartType == PHand && i == 0) {
      Entity toEquip;
      switch (equipment->bodyParts[i].bodyPartType) {
	case (PHand):
          toEquip = MakeWeaponOfType(GetWeaponTypeFromSkills(src), GetWeaponMaterial());
	  break;
        case (PHead):
	case (PBody):
	case (PLegs):
	case (PFeet):
          toEquip = MakeArmorOfType(equipment->bodyParts[i].bodyPartType,  GetArmorMaterialFromSkills(src));
	  break;
      }
      toEquip.Id = world->entities.size();
      world->entities.push_back(toEquip);
      equipment->bodyParts[i].equippedEntity = world->entities.size() - 1;
    }
  }
}

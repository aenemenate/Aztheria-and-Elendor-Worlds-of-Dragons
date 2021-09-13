#include "../ecs/entity.h"

class World;

enum PotionType {
  Minor = 1,
  Normal = 2,
  Major = 3
};

Entity MakeWeaponOfType(MWeaponType weaponType, Material material);

Entity MakeArmorOfType(BodyPartType bodyPartType, Material material);

Entity MakePotionOfType(PotionType type, Resource resource);

MWeaponType GetWeaponTypeFromSkills(Entity *src);

Material GetWeaponMaterial();

Material GetArmorMaterialFromSkills(Entity *src);

void EquipEntity(int src_id, World *world);
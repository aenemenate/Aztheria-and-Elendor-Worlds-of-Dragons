#include "../ecs/entity.h"

class World;

Entity MakeWeaponOfType(MWeaponType weaponType, Material material);

Entity MakeArmorOfType(BodyPartType bodyPartType, Material material);

MWeaponType GetWeaponTypeFromSkills(Entity *src);

Material GetWeaponMaterial();

Material GetArmorMaterialFromSkills(Entity *src);

void EquipEntity(int src_id, World *world);
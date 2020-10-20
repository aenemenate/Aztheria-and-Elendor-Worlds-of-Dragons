#include "dungeon.h"
#include "../ecs/entity.h"

void Dungeon::ClearEnts() {
  ent_map.clear();
  ent_map.resize(width * height, nullptr);
}

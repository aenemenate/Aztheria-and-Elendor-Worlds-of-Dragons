#include "block_components.h"
#include "../entity.h"

void Stair::Update(void* src, World *world) {
  Entity *ent = (Entity*)src;
  ent->Move(0, 0, zdirection, world);
}

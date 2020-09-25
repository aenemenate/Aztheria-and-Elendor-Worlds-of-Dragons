#include "block_components.h"
#include "../entity/entity.h"

bool Stair::Activate(void *src, World *world) {
  Entity *ent = (Entity*)src;
  ent->Move(0, 0, zdirection, world);
  return true;
}

bool Plant::Update(void *src, World *world) {
  if (stage == numstages - 1)
    return true;
  stage++;
  return false;
}

#include "block_components.h"
#include "../ecs/entity.h"

bool Stair::Activate(void *src, World *world) {
  Entity *ent = (Entity*)src;
  ent->actions.push_back(std::shared_ptr<EntityAction>(new Move(0,0,zdirection)));
  return true;
}

bool Plant::Update(void *src, World *world) {
  if (stage == numstages - 1)
    return true;
  stage++;
  return false;
}

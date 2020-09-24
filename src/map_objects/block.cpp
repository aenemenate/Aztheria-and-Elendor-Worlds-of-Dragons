#include "block.h"

void Block::Activate(Entity *ent, World *world) {
    for (BlockComponent *bc : blockcomponents)
      bc->Update(ent, world);
}
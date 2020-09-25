#include "block.h"

void Block::Activate(void *src, World *world) { for (BlockComponent *bc : blockcomponents) bc->Activate(src, world); }
void Block::Update  (void *src, World *world) { for (BlockComponent *bc : blockcomponents)   bc->Update(src, world); }
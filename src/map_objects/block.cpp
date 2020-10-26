#include "block.h"

void Block::Activate(void *src, World *world) { for (auto bc : blockcomponents) bc->Activate(src, world); }
void Block::Update  (void *src, World *world) { for (auto bc : blockcomponents)   bc->Update(src, world); }
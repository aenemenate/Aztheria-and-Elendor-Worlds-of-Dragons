#include "block_systems.h"
#include "../world/world.h"
#include "../map/area.h"
#include "../map/map_helper.h"
#include "../ecs/entity.h"
#include "block.h"
#include "tile.h"

void UpdatePlants(World* world) {
  uint16_t tilewidth = world->GetArea(0,0)->width, 
      tileheight = world->GetArea(0,0)->height;
  for (int i = 0; i < world->width * tilewidth; i++)
    for (int j = 0; j < world->height * tileheight; j++) {
      uint16_t wx = i / tilewidth, 
               wy = j / tileheight, 
               x = i % tilewidth, 
               y = j % tileheight, 
               z = 0;
      Position pos = { x, y, z, wx, wy };
      Area *area = world->GetArea(wx, wy);
      Block *block = area->GetBlock(x, y, 0);
      if (block->HasComponent(PLANT_ID)) {
        std::shared_ptr<Plant> pc = std::dynamic_pointer_cast<Plant>(block->GetComponent(PLANT_ID));
        if (pc->Update(&pos, world)) {
          block->gr.ch = pc->GetCurrentStage();
          area->SetBlock(x, y, 0, *block);
          int seedr = pc->GetSeedRadius(),
              xoffs = (rand() % seedr - seedr / 2), yoffs = (rand() % seedr - seedr / 2);
          Point seed = { x + xoffs, y + yoffs };
          if (!area->PointWithinBounds(seed.x, seed.y)) {
            if ((i + xoffs) >= world->width * tilewidth || (j + yoffs) >= world->height * tileheight) continue;
            Area* area2 = world->GetArea((i + xoffs) / tilewidth, (j + yoffs) / tileheight);
            if (seed.x >= tilewidth) seed.x -= tilewidth;
            if (seed.x < 0) seed.x += tilewidth;
            if (seed.y >= tileheight) seed.y -= tileheight;
            if (seed.y < 0) seed.y += tileheight;
            if (area2->GetTile(seed.x, seed.y, 0)->name == "dirt" 
            &&  (area2->GetBlock(seed.x, seed.y, 0)->name == "air"
            ||  area2->GetBlock(seed.x, seed.y, 0)->HasComponent(PLANT_ID))
            &&  MapHelper::SpaceIsClear(area2, {seed.x, seed.y}, 0, pc->GetRequiredSpace())) {
              pc->ResetGrowth();
              block->gr.ch = pc->GetCurrentStage();
              area2->SetBlock(seed.x, seed.y, 0, *block);
            }
            continue;
          }
          if (area->GetTile(seed.x, seed.y, 0)->name == "dirt" 
          &&  (area->GetBlock(seed.x, seed.y, 0)->name == "air"
          ||  area->GetBlock(seed.x, seed.y, 0)->HasComponent(PLANT_ID))
          &&  MapHelper::SpaceIsClear(area, {seed.x, seed.y}, 0, pc->GetRequiredSpace())) {
            pc->ResetGrowth();
            block->gr.ch = pc->GetCurrentStage();
            area->SetBlock(seed.x, seed.y, 0, *block);
          }
        }
      }
    }
}
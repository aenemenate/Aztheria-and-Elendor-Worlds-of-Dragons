#include "map_helper.h"
#include "area.h"
#include "../base.h"

#include <algorithm>

bool MapHelper::PointAdjacentToTileOfType(Area *area, Point pos, int z, std::string tile_type) {
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++) {
            Point cur_pos = { pos.x + i, pos.y +j };
            if (area->PointWithinBounds(cur_pos.x, cur_pos.y)
            &&  area->GetTile(cur_pos.x, cur_pos.y, z)->name == tile_type)
              return true;
        }
    return false;
}

bool MapHelper::PointAdjacentToBlockOfType(Area *area, Point pos, int z, std::string block_type) {
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++) {
            Point cur_pos = { pos.x + i, pos.y +j };
            if (area->PointWithinBounds(cur_pos.x, cur_pos.y)
            &&  area->GetBlock(cur_pos.x, cur_pos.y, z)->name == block_type)
              return true;
        }
    return false;
}
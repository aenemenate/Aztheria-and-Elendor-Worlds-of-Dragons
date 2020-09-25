#pragma once

#include <string>

class Area;
class Point;

namespace MapHelper {
    bool PointAdjacentToTileOfType(Area *area, Point pos, int z, std::string tile_type);
    bool PointAdjacentToBlockOfType(Area *area, Point pos, int z, std::string block_type);
}
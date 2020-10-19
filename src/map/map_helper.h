#pragma once

#include <string>
#include <vector>

class Area;
class Map;
class Point;

namespace MapHelper {
    bool PointAdjacentToTileOfType(Area *area, Point pos, int z, std::string tile_type);
    bool PointAdjacentToBlockOfType(Area *area, Point pos, int z, std::string block_type);
    bool SpaceIsClear(Area *area, Point pos, int z, int space);

    std::vector<Point> GetWalkablePoints(Map *map);
}
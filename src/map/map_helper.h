#pragma once

#include <string>
#include <vector>

class Area;
class Map;
class Point;

namespace MapHelper {

/* Returns true if the current point has an adjacent tile with the specified name */
  bool PointAdjacentToTileOfType(Area *area, Point pos, int z, std::string tile_type);

/* Returns true if the current point has an adjacent block with the specified name */
  bool PointAdjacentToBlockOfType(Area *area, Point pos, int z, std::string block_type);

/* Returns true if there are no solid blocks within a square with the specified 'radius' centered
   at the specified point */
  bool SpaceIsClear(Area *area, Point pos, int z, int space);
}
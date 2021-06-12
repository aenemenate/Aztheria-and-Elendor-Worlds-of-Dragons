#pragma once

#include <vector>
#include "base.h"

class World;
class Area;

namespace Pathfinder {
  std::vector<Point> GetPath(World *world, int wx, int wy, int z, int startx, int starty, int endx, int endy);
  std::vector<Point> GetPath(Area *area, int z, int startx, int starty, int endx, int endy);
}
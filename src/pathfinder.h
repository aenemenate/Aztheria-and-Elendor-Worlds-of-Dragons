#pragma once

#include <vector>
#include "base.h"

class World;

namespace Pathfinder
{
  std::vector<Point> GetPath(World *world, int wx, int wy, int z, int startx, int starty, int endx, int endy);
}
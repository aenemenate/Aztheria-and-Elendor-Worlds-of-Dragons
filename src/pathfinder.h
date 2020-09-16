#pragma once

#include <vector>

class World;

namespace Pathfinder
{
  struct Point {
    int x, y;
  };

  std::vector<Point> GetPath(World *world, int wx, int wy, int startx, int starty, int endx, int endy);
}
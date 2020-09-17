#include "pathfinder.h"

#include "world.h"
#include "map/area.h"
#include "entity.h"

#include "./AStar/AStar.h"

using namespace Pathfinder;

int gwx, gwy;
Grid *grid;

void InitializeGrid(Area *area)
{
  int rows = area->width;
  int cols = area->height;
  Grid::movementType movement = Grid::movementType::EIGHT_DIRECTIONS;
  vector<Node> obstacles;
  for (int i = 0; i < area->width; i++)
    for (int j = 0; j < area->height; j++) {
      if (!area->GetTile(i, j)->walkable)
	      obstacles.push_back(Node(i, j));
    }
  if (grid != nullptr)
    delete grid;
  grid = new Grid(cols, rows, movement, obstacles);
}

std::vector<Point> Pathfinder::GetPath(World *world, int wx, int wy, int startx, int starty, int endx, int endy)
{
  if (grid == nullptr || wx != gwx || wy != gwy) {
    InitializeGrid(world->GetArea(wx, wy));
    gwx = wx;
    gwy = wy;
  }
  Node start = Node(startx, starty);
  Node goal = Node(endx, endy);
  AStar astar(grid);
  vector<Node> nodepath = astar.search(start, goal);
  vector<Point> path;
  for (Node node : nodepath)
    path.push_back({node.getX(), node.getY()});
  return path;
}

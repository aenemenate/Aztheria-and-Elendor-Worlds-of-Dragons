#include "base.h"

int ManhattanDistance(int x1, int y1, int x2, int y2) {
  int xdist = x1 - x2;
  int ydist = y1 - y2;
  if (xdist < 0) xdist = xdist * -1;
  if (ydist < 0) ydist = ydist * -1;
  return xdist + ydist;
}
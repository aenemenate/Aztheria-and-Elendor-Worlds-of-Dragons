#pragma once

#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <string>
#include <vector>

using namespace std;

struct Graphic {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & ch;
    ar & fgcolor;
    ar & bgcolor;
  }
  string ch, fgcolor, bgcolor;
};

struct Position {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & x;
    ar & y;
    ar & z;
    ar & wx;
    ar & wy;
  }
  uint16_t x, y, z, wx, wy;
};

struct Point {
  int x, y;
};

struct Rectangle {
  int left, right, top, bottom;
};

enum Direction { north, east, south, west, northeast, northwest, southeast, southwest };
enum ZDirection { up = -1, down = 1 };

int ManhattanDistance(int x1, int y1, int x2, int y2);
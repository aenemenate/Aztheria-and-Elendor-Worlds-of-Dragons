#pragma once

#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <string>
#include <vector>

using namespace std;

struct Graphic 
{
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & ch;
    ar & fgcolor;
    ar & bgcolor;
  }
  string ch, fgcolor, bgcolor;
};

struct Position
{
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & x;
    ar & y;
    ar & wx;
    ar & wy;
  }
  uint16_t x, y, wx, wy;
};

struct Point
{
  int x, y;
};

#pragma once

#include <ctime>
#include <string>
#include <vector>

class Game;

class Line  {
public:
  std::string value;
  time_t rawtime;
  Line(std::string value) : value(value) {
    time(&rawtime);
  }
};

class MsgConsole {
public:
  std::vector<Line> lines;
  void PushLine(std::string value);
  void PrintLines(Game *game);
};
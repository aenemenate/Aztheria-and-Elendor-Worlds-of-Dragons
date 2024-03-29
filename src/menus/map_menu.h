#pragma once

#include "../gui/menu.h"
#include <vector>

class MapMenu : public Menu {
public:
  static MapMenu mapMenu;
  MapMenu() {}
  MapMenu(int xpos, int ypos, int width, int height) : Menu{xpos, ypos, width, height, "map", false} {}

  void CustomDraw(Game*);
  void CustomUpdate(Game*);

  static MapMenu *Instance() { return &mapMenu; }
};
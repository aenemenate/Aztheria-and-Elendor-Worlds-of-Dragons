#pragma once

#include "../gui/menu.h"
#include <vector>

class InventoryMenu : public Menu {
public:
  InventoryMenu() {}
  InventoryMenu(int xpos, int ypos, int width, int height) : Menu{xpos, ypos, width, height, "inventory", false} {}

  void CustomDraw(Game*);
  void CustomUpdate(Game*);
};
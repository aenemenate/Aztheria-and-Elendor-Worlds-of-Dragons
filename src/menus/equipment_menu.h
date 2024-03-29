#pragma once

#include "../gui/menu.h"
#include <vector>

class EquipmentMenu : public Menu {
public:
  static EquipmentMenu equipmentMenu;
  EquipmentMenu() {}
  EquipmentMenu(int xpos, int ypos, int width, int height) : Menu{xpos, ypos, width, height, "equipment", false} {}

  void CustomDraw(Game*);
  void CustomUpdate(Game*);

  static EquipmentMenu *Instance() { return &equipmentMenu; }
};
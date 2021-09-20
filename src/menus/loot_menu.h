#pragma once

#include "../gui/menu.h"
#include <vector>

class LootMenu : public Menu {
public:
  static LootMenu lootMenu;
  LootMenu() {}
  LootMenu(int xpos, int ypos, int width, int height) : Menu{xpos, ypos, width, height, "loot", false} {}

  void CustomDraw(Game*);
  void CustomUpdate(Game*);

  static void SetSelectedEntity(int selectedEntity);

  static LootMenu *Instance() { return &lootMenu; }
};
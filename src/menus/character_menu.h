#pragma once

#include "../gui/menu.h"
#include <vector>

class CharacterMenu : public Menu {
public:
  static CharacterMenu characterMenu;
  CharacterMenu() {}
  CharacterMenu(int xpos, int ypos, int width, int height) : Menu{xpos, ypos, width, height, "character", false} {}

  void CustomDraw(Game*);
  void CustomUpdate(Game*);

  static void SetSelectedEntity(int selectedEntity);

  static CharacterMenu *Instance() { return &characterMenu; }
};
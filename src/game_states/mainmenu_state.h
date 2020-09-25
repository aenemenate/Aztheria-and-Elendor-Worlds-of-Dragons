#pragma once

#include "../game.h"
#include "../ui/button.h"

#include <string>
#include <vector>

class MainMenuState : public GameState {
  static MainMenuState mainMenuState;
  vector<Button> buttons;
  int menu_caret;
  std::string title;
protected:
  MainMenuState() { }
public:
  void Init(Game *game);
  void Cleanup();

  void HandleEvents(Game* game);
  void Update(Game* game);
  void Draw(Game* game);

  static MainMenuState* Instance() {
	  return &mainMenuState;
  }
};

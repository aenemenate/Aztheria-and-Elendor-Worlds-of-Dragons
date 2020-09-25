#pragma once

#include "../game.h"
#include "../gui/button.h"

class LoadWorldState : public GameState {
  static LoadWorldState load_world_state;
  vector<Button> buttons;
  int menu_caret;
protected:
  LoadWorldState() { }
public:
  void Init(Game *game);
  void Cleanup();

  void HandleEvents(Game* game);
  void Update(Game* game);
  void Draw(Game* game);

  static LoadWorldState* Instance() {
	return &load_world_state;
  }
};

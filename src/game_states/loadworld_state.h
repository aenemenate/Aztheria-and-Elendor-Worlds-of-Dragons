#pragma once

#include "../../engine/game.h"
#include "../../engine/ui/button.h"

class LoadWorldState : public GameState
{
public:
  void Init(Game *game);
  void Cleanup();

  void HandleEvents(Game* game);
  void Update(Game* game);
  void Draw(Game* game);

  static LoadWorldState* Instance() {
	return &load_world_state;
  }

protected:
  LoadWorldState() { }
private:
  static LoadWorldState load_world_state;
  vector<Button> buttons;
  int menu_caret;
};

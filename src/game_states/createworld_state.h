#pragma once

#include "../game.h"
#include "../gui/button.h"

class CreateWorldState : public GameState
{
  static CreateWorldState create_world_state;
  vector<Button> buttons;
protected:
  CreateWorldState() { }
public:
  void Init(Game *game);
  void Cleanup();

  void Resize(Game* game);
  void HandleEvents(Game* game);
  void Update(Game* game);
  void Draw(Game* game);

  static CreateWorldState* Instance() {
	return &create_world_state;
  }
};

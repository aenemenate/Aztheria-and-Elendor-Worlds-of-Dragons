#pragma once

#include "../game.h"
#include "../ui/button.h"

class CreateWorldState : public GameState
{
  static CreateWorldState create_world_state;
  vector<Button> buttons;
protected:
  CreateWorldState() { }
public:
  void Init(Game *game);
  void Cleanup();

  void HandleEvents(Game* game);
  void Update(Game* game);
  void Draw(Game* game);

  static CreateWorldState* Instance() {
	return &create_world_state;
  }
};

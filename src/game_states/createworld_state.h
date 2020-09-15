#pragma once

#include "../../engine/game.h"
#include "../../engine/ui/button.h"

class CreateWorldState : public GameState
{
public:
  void Init(Game *game);
  void Cleanup();

  void HandleEvents(Game* game);
  void Update(Game* game);
  void Draw(Game* game);

  static CreateWorldState* Instance() {
	return &create_world_state;
  }

protected:
  CreateWorldState() { }
private:
  static CreateWorldState create_world_state;
  vector<Button> buttons;
};

#pragma once

#include "../game.h"
#include "../gui/button.h"

class CreateCharacterState : public GameState
{
  static CreateCharacterState create_character_state;
protected:
  CreateCharacterState() { }
public:
  vector<Button> buttons;

  void Init(Game *game);
  void Cleanup();

  void Resize(Game* game);
  void HandleEvents(Game* game);
  void Update(Game* game);
  void Draw(Game* game);

  static CreateCharacterState* Instance() {
	return &create_character_state;
  }
};

#pragma once

#include "../game.h"
#include "../gui/button.h"
#include "../gui/panel_status.h"

class PlayState : public GameState
{
  static PlayState playState;
 protected:
  PlayState() { }
 public:
  void Init(Game *game);
  void Cleanup();

  void Resize(Game* game);
  void HandleEvents(Game* game);
  void Update(Game* game);
  void Draw(Game* game);

  static PlayState* Instance() {
	  return &playState;
  }
};

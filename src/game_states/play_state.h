#pragma once

#include "../game.h"
#include "../ui/button.h"
#include "../ui/panel_status.h"

class PlayState : public GameState
{
  static PlayState playState;
 protected:
  PlayState() { }
 public:
  void Init(Game *game);
  void Cleanup();

  void HandleEvents(Game* game);
  void Update(Game* game);
  void Draw(Game* game);

  static PlayState* Instance() {
	  return &playState;
  }
};

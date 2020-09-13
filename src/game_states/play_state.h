#pragma once

#include "../game.h"
#include "../ui/button.h"
#include "../ui/panel_status.h"

class PlayState : public GameState
{
 public:
  void Init(Game *game);
  void Cleanup();

  void HandleEvents(Game* game);
  void Update(Game* game);
  void Draw(Game* game);

  static PlayState* Instance() {
	  return &playState;
  }

 protected:
  PlayState() { paused = false; menu_caret = 0; }
 private:
  static PlayState playState;

  StatusPanel status_panel;

  vector<Button> pmenu_buttons;
  int menu_caret;
  bool paused;
};


#include "../include/BearLibTerminal.h"

#include "./game_states/mainmenu_state.h"

int WinMain()
{
  Game game;
  game.Init();
  game.ChangeState(MainMenuState::Instance());

  while (game.Running())
  {
    game.Draw();
    game.HandleEvents();
    game.Update();
  }

  game.CleanupAll();
}

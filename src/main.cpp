#include "./game_states/mainmenu_state.h"

#if defined(_WIN32) || defined(_WIN64)
#define MAIN WinMain
#elif defined(__APPLE__) || defined (linux)
#define MAIN main
#endif

int MAIN() {
  Game game;
  game.Init();
  game.ChangeState(MainMenuState::Instance());
  while (game.Running()) {
    game.Draw();
    game.HandleEvents();
    game.Update();
  }
  game.CleanupAll();

  return 0;
}

#pragma once

#include "gamestate.h"
#include "settings.h"
#include "world/world.h"
#include <vector>
using namespace std;

class Game {
  vector<GameState*> states;
  bool running;
public:
  Settings settings;
  World *world;

  void Init();
  void CleanupAll();
  void CleanupResources();

  void ChangeState(GameState *state);
  void PushState(GameState *state);
  void PopState();

  void HandleEvents();
  void Update();
  void Draw();

  bool Running() { return running; }
  void Quit() { running = false; }
};
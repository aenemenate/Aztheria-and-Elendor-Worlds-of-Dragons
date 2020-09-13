#pragma once

#include "gamestate.h"
#include "settings.h"
#include <vector>
using namespace std;

class World;

class Game
{
public:
  int key;
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

  void SetInputBlockMode(bool value) { input_block_mode = value;}

private:
  vector<GameState*> states;
  bool running, input_block_mode;
};
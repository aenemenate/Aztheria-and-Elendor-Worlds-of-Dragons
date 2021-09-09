#pragma once

class Game;

class GameState {
public:
  virtual void Init(Game *game) = 0;
  virtual void Cleanup() = 0;

  virtual void Resize(Game* game) = 0;
  virtual void HandleEvents(Game* game) = 0;
  virtual void Update(Game* game) = 0;
  virtual void Draw(Game* game) = 0;

  protected: GameState() { }
};
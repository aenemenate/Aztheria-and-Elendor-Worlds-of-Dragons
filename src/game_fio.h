#pragma once

class Game;

class GameFIO
{
  public:
    static void LoadWorld(Game*, int slot);
    static void SaveWorld(Game*);
};
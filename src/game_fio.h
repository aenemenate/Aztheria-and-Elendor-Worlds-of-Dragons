#pragma once

#include "game.h"

class GameFIO
{
  public:
    static void LoadWorld(Game*, int slot);
    static void SaveWorld(Game*);
};
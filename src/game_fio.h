#pragma once

class Game;

namespace GameFIO
{
    void LoadWorld(Game*, int slot);
    void SaveWorld(Game*);
};
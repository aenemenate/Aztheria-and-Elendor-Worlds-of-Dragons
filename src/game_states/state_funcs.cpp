#include "state_funcs.h"
#include "../../engine/game.h"

void GoBack(Game* game)
{
  game->PopState();
}
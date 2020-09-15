#include "createworld_state.h"
#include "play_state.h"
#include "state_funcs.h"

#include "../../include/BearLibTerminal.h"
#include "../../engine/draw_funcs.h"
#include "../../engine/base.h"

#include "../../engine/world.h"
#include "../../engine/map.h"
#include "../../engine/entity.h"

#include "../world_gen.h"
#include "../../engine/game_fio.h"

#include <filesystem>
#include <algorithm>
#include <sstream>

namespace fs = std::filesystem;

int worldsize;

void IncrementWorldSize(Game* game)
{
  worldsize = std::min(worldsize + 4, 40);
}

void DecrementWorldSize(Game* game)
{
  worldsize = std::max(worldsize - 4, 0);
}

void CreateWorld(Game* game)
{
  int slot = 1;
  for(const auto& entry : fs::directory_iterator("./saves"))
        if (entry.path().extension() == ".bin") {
          std::stringstream sstrslot;
          sstrslot << entry.path().string()[13];
          int i = 1;
          while (entry.path().string()[13 + i] != '.') {
            sstrslot << entry.path().string()[13+i];
          }
          int temp;
          sstrslot >> temp;
          if (temp >= slot)
          slot = temp + 1;
        }
  WorldGen::GenerateWorld(game, worldsize, slot);
  game->ChangeState(PlayState::Instance());
  game->Update();
}


CreateWorldState CreateWorldState::create_world_state;

void CreateWorldState::Init(Game *game)
{
  buttons.clear();
  int term_width  = terminal_state(TK_WIDTH), 
      term_height = terminal_state(TK_HEIGHT);
// push buttons
  buttons.push_back(Button(1,1, "<->", DecrementWorldSize));
  buttons.push_back(Button(8,1, "<+>", IncrementWorldSize));
  buttons.push_back(Button(term_width - 2 - 14,term_height-2, "[[c]]reate world", CreateWorld));
  buttons.push_back(Button(1,term_height-2, "go [[b]]ack", GoBack));
  worldsize = 12;
}

void CreateWorldState::Cleanup()
{
  buttons.clear();
}

void CreateWorldState::HandleEvents(Game *game)
{
  if (terminal_state(TK_EVENT) == TK_RESIZED)
    this->Init(game);
  switch (game->key)
  {
    case TK_KP_6:
    case TK_RIGHT:
      IncrementWorldSize(game);
      break;
    case TK_KP_4:
    case TK_LEFT:
      DecrementWorldSize(game);
      break;
    case TK_C:
      CreateWorld(game);
      break;
    case TK_ESCAPE:
    case TK_B:
      GoBack(game);
      break;
  }
}

void CreateWorldState::Update(Game *game)
{
  for (int b=0;b<buttons.size();b++)
    buttons[b].Update(game);
}

void CreateWorldState::Draw(Game *game)
{
  int term_width  = terminal_state(TK_WIDTH), 
      term_height = terminal_state(TK_HEIGHT);
  for (int b=0;b<buttons.size();b++)
    buttons[b].Render(game);
  std::stringstream sstream;
  sstream << worldsize;
  terminal_print(5, 1, sstream.str().c_str());
}
#include "createworld_state.h"
#include "play_state.h"
#include "state_funcs.h"

#include "../base.h"
#include "../draw_funcs.h"
#include <BearLibTerminal.h>

#include "../world_gen/world_gen.h"
#include "../util/filesystem.h"

#include <algorithm>
#include <sstream>

int worldsize;

void IncrementWorldSize(Game* game) { worldsize = std::min(worldsize + 4, 40); }

void DecrementWorldSize(Game* game) { worldsize = std::max(worldsize - 4, 0); }

void CreateWorld(Game* game) {
  int slot = 1;
  for(const auto& entry : fs::directory_iterator("./saves"))
        if (entry.path().extension() == ".bin") {
          std::stringstream sstrslot;
          sstrslot << entry.path().string()[13];
          int i = 1;
          while (entry.path().string()[13+i] != '.') {
            sstrslot << entry.path().string()[13+i];
            i++;
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

void CreateWorldState::Init(Game *game) {
  buttons.clear();
  int term_width  = terminal_state(TK_WIDTH), 
      term_height = terminal_state(TK_HEIGHT);
// change world size buttons
  buttons.push_back(Button(1,1, "<->", DecrementWorldSize));
  buttons.push_back(Button(8,1, "<+>", IncrementWorldSize));
// create world button
  buttons.push_back(Button(term_width - 2 - 14,term_height-2, "[[c]]reate world", CreateWorld));
// go back button
  buttons.push_back(Button(1,term_height-2, "go [[b]]ack", GoBack));
  worldsize = 12;
}

void CreateWorldState::Cleanup() {
  buttons.clear();
}

void CreateWorldState::HandleEvents(Game *game) {
// reset our graphical elements if terminal is resized
  if (terminal_state(TK_EVENT) == TK_RESIZED)
    this->Init(game);
// handle key input
  switch (game->key) {
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

void CreateWorldState::Update(Game *game) {
// update buttons
  for (int b=0;b<buttons.size();b++)
    buttons[b].Update(game);
}

void CreateWorldState::Draw(Game *game) {
  int term_width  = GetTermWidth(), 
      term_height = GetTermHeight();
// draw buttons
  for (int b=0;b<buttons.size();b++)
    buttons[b].Render(game);
// print world size
  std::stringstream sstream;
  sstream << worldsize;
  PrintGraphic(5, 1, {sstream.str(), "white", "black"});
}

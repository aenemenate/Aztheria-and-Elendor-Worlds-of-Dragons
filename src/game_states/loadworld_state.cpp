#include "loadworld_state.h"
#include "play_state.h"
#include "state_funcs.h"

#include "../../include/BearLibTerminal.h"
#include "../draw_funcs.h"
#include "../base.h"

#include "../game_fio.h"

#include <filesystem>
#include <algorithm>
#include <sstream>

namespace fs = std::filesystem;

std::string filepath;


void LoadWorld(Game* game)
{
  int slot;
  std::stringstream sstrslot;
  sstrslot << filepath[13];
  int i = 1;
  while (filepath[13 + i] != '.') {
    sstrslot << filepath[13+i];
  }
  sstrslot >> slot;
  GameFIO::LoadWorld(game, slot);
  game->ChangeState(PlayState::Instance());
  game->Update();
}

LoadWorldState LoadWorldState::load_world_state;

void LoadWorldState::Init(Game *game)
{
  buttons.clear();
  int term_width  = terminal_state(TK_WIDTH), 
      term_height = terminal_state(TK_HEIGHT);
// push buttons
  int y_offset = 0;
  for(const auto& entry : fs::directory_iterator(".\\saves"))
    if (entry.path().extension() == ".bin") {
      buttons.push_back(Button(term_width/2 - 10,term_height/2-15+y_offset*2, entry.path().string(), LoadWorld));
      y_offset++;
    }
  buttons.push_back(Button(1,term_height-2, "go [[b]]ack", GoBack));
  menu_caret = 0;
}

void LoadWorldState::Cleanup()
{
  buttons.clear();
}

void LoadWorldState::HandleEvents(Game *game)
{
  if (terminal_state(TK_EVENT) == TK_RESIZED)
    this->Init(game);
  switch (game->key)
  {
    case TK_KP_8:
    case TK_UP:
      menu_caret = (menu_caret > 0) ? --menu_caret : menu_caret; 
      break;
    case TK_KP_2:
    case TK_DOWN:
      menu_caret = (menu_caret < buttons.size() - 2) ? ++menu_caret : menu_caret;
      break;
    case TK_KP_ENTER:
    case TK_ENTER:
    case TK_SPACE:
      filepath = buttons[menu_caret].GetText();
      buttons[menu_caret].Activate(game);
      break;
    case TK_ESCAPE:
    case TK_B:
      GoBack(game);
      break;
  }
}

void LoadWorldState::Update(Game *game)
{
  for (int b=0;b<buttons.size();b++)
  {
    if (buttons[b].isclicked(game))
      filepath = buttons[b].GetText();
    buttons[b].Update(game);
  }
}

void LoadWorldState::Draw(Game *game)
{
  int term_width  = terminal_state(TK_WIDTH), 
      term_height = terminal_state(TK_HEIGHT);
  for (int b=0;b<buttons.size();b++)
    buttons[b].Render(game);
  PrintCh(term_width/2-12, term_height/2 - 15 + menu_caret*2, {">", "white", "black"});
}
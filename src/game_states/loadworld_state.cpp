#include "loadworld_state.h"
#include "play_state.h"
#include "state_funcs.h"

#include "../draw_funcs.h"
#include "../base.h"
#include "../input_funcs.h"

#include "../game_fio.h"
#include "../util/filesystem.h"

#include <algorithm>
#include <sstream>

std::string filepath;

#if defined(_WIN32) || defined(_WIN64)
  char file_delim = '\\';
#elif defined(__APPLE__) || defined(linux)
  char file_delim = '/';
#endif

void LoadWorld(Game* game) {
  int slot;
  std::stringstream sstrslot;
  sstrslot << filepath[13];
  int i = 1;
  while (filepath[13+i] != '.') {
    sstrslot << filepath[13+i];
    i++;
  }
  sstrslot >> slot;
  GameFIO::LoadWorld(game, slot);
  game->ChangeState(PlayState::Instance());
  game->Update();
}

LoadWorldState LoadWorldState::load_world_state;

void LoadWorldState::Init(Game *game) {
  buttons.clear();
  int term_width  = GetTermWidth(), 
      term_height = GetTermHeight();
// push buttons (based on available save files)
  int y_offset = 0;
  for(const auto& entry : fs::directory_iterator("." + std::string{file_delim} + "saves"))
    if (entry.path().extension() == ".bin") {
      buttons.push_back(Button(term_width/2 - 10,term_height/2-15+y_offset*2, entry.path().string(), LoadWorld));
      y_offset++;
    }
// push go back button
  buttons.push_back(Button(1,term_height-2, "go [[b]]ack", GoBack));
// init menu caret
  menu_caret = 0;
}

void LoadWorldState::Cleanup() {
  buttons.clear();
}

void LoadWorldState::HandleEvents(Game *game) {
// re-init if resized
  if (TerminalWasResized())
    this->Init(game);
// handle input
  switch (TerminalGetKey()) {
    case MTK_KP_8:
    case MTK_UP:
      menu_caret = (menu_caret > 0) ? --menu_caret : menu_caret; 
      break;
    case MTK_KP_2:
    case MTK_DOWN:
      menu_caret = (menu_caret < buttons.size() - 2) ? ++menu_caret : menu_caret;
      break;
    case MTK_KP_ENTER:
    case MTK_ENTER:
    case MTK_SPACE:
      filepath = buttons[menu_caret].GetText();
      buttons[menu_caret].Activate(game);
      break;
    case MTK_ESCAPE:
    case MTK_B:
      GoBack(game);
      break;
  }
}

void LoadWorldState::Update(Game *game) {
// update buttons
  for (int b=0;b<buttons.size();b++) {
    if (buttons[b].isclicked())
      filepath = buttons[b].GetText();
    buttons[b].Update(game);
  }
}

void LoadWorldState::Draw(Game *game) {
  int term_width  = GetTermWidth(), 
      term_height = GetTermHeight();
// draw buttons
  for (int b=0;b<buttons.size();b++)
    buttons[b].Render();
// print menu caret
  PrintGraphic(term_width/2-12, term_height/2 - 15 + menu_caret*2, {">", "white", "black"});
}

#include "mainmenu_state.h"
#include "createcharacter_state.h"
#include "loadworld_state.h"
#include "../draw_funcs.h"
#include "../base.h"
#include "../input_funcs.h"
#include "../util/filesystem.h"

#include <fstream>
#include <sstream>

void NewWorld(Game* game) { game->PushState(CreateCharacterState::Instance()); game->Update(); }

void PlayWorld(Game* game) { game->PushState(LoadWorldState::Instance()); game->Update(); }

void QuitGame(Game* game) { game->Quit(); }

MainMenuState MainMenuState::mainMenuState;

void MainMenuState::Init(Game *game) {
  int term_width  = GetTermWidth(), 
      term_height = GetTermHeight(),
      y_offset = 0;
  buttons.clear();
  bool save_exists = false;
  for(const auto& entry : fs::directory_iterator("./saves"))
    if (entry.path().extension() == ".bin")
      save_exists = true;
// check if a save file exists before putting the continue button
  if (save_exists) {
    buttons.push_back(Button(term_width/2-5,term_height/2-1+y_offset*2, "play world", PlayWorld));
    y_offset++;
  }
  buttons.push_back(Button(term_width/2-4,term_height/2-1+y_offset*2, "new world", NewWorld));
  y_offset++;
  buttons.push_back(Button(term_width/2-4,term_height/2-1+y_offset*2, "quit game", QuitGame));
  menu_caret = 0;
// load title
  ifstream f("./data/title.txt"); //taking file as inputstream
  if(f) {
    ostringstream ss;
    ss << f.rdbuf(); // reading data
    title = ss.str();
  }
}

void MainMenuState::Cleanup() {
  buttons.clear();
}

void MainMenuState::Resize(Game *game) {
  Init(game);
}

void MainMenuState::HandleEvents(Game *game) {
  if (TerminalWasResized())
    this->Init(game);
  switch (TerminalGetKey()) {
    case MTK_KP_8:
    case MTK_UP:
      menu_caret = (menu_caret > 0) ? --menu_caret : menu_caret; 
      break;
    case MTK_KP_2:
    case MTK_DOWN:
      menu_caret = (menu_caret < buttons.size() - 1) ? ++menu_caret : menu_caret;
      break;
    case MTK_KP_ENTER:
    case MTK_ENTER:
    case MTK_SPACE:
      buttons[menu_caret].Activate(game);
      break;
  }
}

void MainMenuState::Update(Game *game) {
  for (int b=0;b<buttons.size();b++)
    buttons[b].Update(game);
}

void MainMenuState::Draw(Game *game) {
  int term_width  = GetTermWidth(), 
      term_height = GetTermHeight();
  for (int b=0;b<buttons.size();b++)
    buttons[b].Render();
  PrintGraphic(buttons[menu_caret].GetX() - 2, term_height/2 - 1 + menu_caret*2, {">", "white", "black"});
  PrintGraphic(term_width/2 - 26, term_height/2 - 11, {title, "white", "black"});
}

#include "mainmenu_state.h"
#include "createworld_state.h"
#include "loadworld_state.h"
#include "../../include/BearLibTerminal.h"
#include "../draw_funcs.h"
#include "../base.h"

#include <filesystem>
#include <fstream>
#include <sstream>
namespace fs = std::filesystem;

void NewWorld(Game* game) { game->PushState(CreateWorldState::Instance()); game->Update(); }

void PlayWorld(Game* game) { game->PushState(LoadWorldState::Instance()); game->Update(); }

void QuitGame(Game* game) { game->Quit(); }

MainMenuState MainMenuState::mainMenuState;

void MainMenuState::Init(Game *game) {
  int term_width  = terminal_state(TK_WIDTH), 
      term_height = terminal_state(TK_HEIGHT),
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
  ifstream f(".\\data\\title.txt"); //taking file as inputstream
  if(f) {
    ostringstream ss;
    ss << f.rdbuf(); // reading data
    title = ss.str();
  }
}

void MainMenuState::Cleanup() {
  buttons.clear();
}

void MainMenuState::HandleEvents(Game *game) {
  if (terminal_state(TK_EVENT) == TK_RESIZED)
    this->Init(game);
  switch (game->key) {
    case TK_KP_8:
    case TK_UP:
      menu_caret = (menu_caret > 0) ? --menu_caret : menu_caret; 
      break;
    case TK_KP_2:
    case TK_DOWN:
      menu_caret = (menu_caret < buttons.size() - 1) ? ++menu_caret : menu_caret;
      break;
    case TK_KP_ENTER:
    case TK_ENTER:
    case TK_SPACE:
      buttons[menu_caret].Activate(game);
      break;
  }
}

void MainMenuState::Update(Game *game) {
  for (int b=0;b<buttons.size();b++)
    buttons[b].Update(game);
}

void MainMenuState::Draw(Game *game) {
  int term_width  = terminal_state(TK_WIDTH), 
      term_height = terminal_state(TK_HEIGHT);
  for (int b=0;b<buttons.size();b++)
    buttons[b].Render(game);
  PrintCh(buttons[menu_caret].GetX() - 2, term_height/2 - 1 + menu_caret*2, {">", "white", "black"});
  terminal_print(term_width/2 - 26, term_height/2 - 11, title.c_str());
}
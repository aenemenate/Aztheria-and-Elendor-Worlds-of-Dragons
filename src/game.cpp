#include "game.h"
#include "world.h"
#include "ecs/entity.h"
#include "map/area.h"
#include "draw_funcs.h"
#include <BearLibTerminal.h> // for input code

void Game::CleanupResources() {
  if (this->world != nullptr)
    delete this->world;
  this->world = nullptr;
}

void Game::Init() {
  key = 0;
  running = true;
  input_block_mode = true;
  SetTerminal("window.size=60x40");
// set values
  world = nullptr;
}

void Game::CleanupAll() {
	// cleanup the all states
  while ( !states.empty() ) {
	  states.back()->Cleanup();
	  states.pop_back();
  }
  CleanupResources();
  terminal_close();
}

void Game::ChangeState(GameState* state) {
// cleanup the current state
  if ( !states.empty() ) {
  	states.back()->Cleanup();
	  states.pop_back();
  }
// store and init the new state
  states.push_back(state);
  states.back()->Init(this);
}

void Game::PushState(GameState* state) {
// store and init the new state
  states.push_back(state);
  states.back()->Init(this);
}

void Game::PopState() {
// cleanup the current state
  if ( !states.empty() ) {
	  states.back()->Cleanup();
	  states.pop_back();
  }
  states.back()->Cleanup();
  states.back()->Init(this);
}

void Game::HandleEvents() {
  if (input_block_mode 
  || terminal_has_input())
    key = terminal_read();
  else { key = 0; }
// let the state handle events
  states.back()->HandleEvents(this);
  if (key==TK_CLOSE)
    Quit();
}

void Game::Update() {
// let the state update the game
  states.back()->Update(this);
}

void Game::Draw() {
// let the state draw the screen
  ClearTerminal();
  states.back()->Draw(this);
  RefreshTerminal();
}

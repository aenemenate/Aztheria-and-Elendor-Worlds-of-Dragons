#include "game.h"
#include "../include/BearLibTerminal.h"
#include "world.h"
#include "entity.h"
#include "map.h"

void Game::CleanupResources()
{
  if (this->world != nullptr)
    delete this->world;
}

void Game::Init()
{
  key = 0;
  running = true;
  input_block_mode = true;
  terminal_open();
  terminal_set("window.size=60x40");
  terminal_refresh();
  
// set values
  world = nullptr;
}

void Game::CleanupAll()
{
	// cleanup the all states
  while ( !states.empty() ) {
	  states.back()->Cleanup();
	  states.pop_back();
  }
  CleanupResources();
  terminal_close();
}

void Game::ChangeState(GameState* state)
{
// cleanup the current state
  if ( !states.empty() ) {
  	states.back()->Cleanup();
	  states.pop_back();
  }
// store and init the new state
  states.push_back(state);
  states.back()->Init(this);
}

void Game::PushState(GameState* state)
{
// store and init the new state
  states.push_back(state);
  states.back()->Init(this);
}

void Game::PopState()
{
// cleanup the current state
  if ( !states.empty() ) {
	  states.back()->Cleanup();
	  states.pop_back();
  }
  states.back()->Cleanup();
  states.back()->Init(this);
}

void Game::HandleEvents() 
{
  if (input_block_mode || terminal_has_input())
    key = terminal_read();
  else
    key = 0;
// let the state handle events
  states.back()->HandleEvents(this);
  if (key==TK_CLOSE)
    Quit();
}

void Game::Update() 
{
// let the state update the game
  states.back()->Update(this);
}

void Game::Draw() 
{
// let the state draw the screen
  terminal_clear();
  states.back()->Draw(this);
  terminal_refresh();
}

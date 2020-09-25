#include "game_fio.h"
#include "../include/BearLibTerminal.h"

#include "game.h"
#include "world.h"
#include "map/area.h"
#include "entity.h"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

void GameFIO::LoadWorld(Game *game, int slot)
{
  terminal_print(0, 0, "Loading . . .");
  terminal_refresh();
  game->CleanupResources();
  try
  {
    stringstream filepath;
    filepath << "./saves/world" << slot << ".bin";
    std::ifstream ifs(filepath.str(), std::ios::binary);
    boost::archive::binary_iarchive ia(ifs);
    ia >> game->world;
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  game->world->SetEnts();
}

void GameFIO::SaveWorld(Game *game)
{
  terminal_print(0, 0, "Saving . . .");
  terminal_refresh();
  try
  {
    stringstream filepath;
    filepath << "./saves/world" << game->world->slot << ".bin";
    std::ofstream ofs(filepath.str(), std::ios::binary);
    {
      boost::archive::binary_oarchive oa(ofs);
      oa << game->world;
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}

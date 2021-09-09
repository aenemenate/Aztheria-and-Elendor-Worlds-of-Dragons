#include "game_fio.h"

#include "game.h"
#include "world/world.h"
#include "map/area.h"
#include "ecs/entity.h"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

#include "draw_funcs.h"
#include "base.h"

void GameFIO::LoadWorld(Game *game, int slot) {
  PrintGraphic(0, 0, {"Loading . . .", "white", "black"});
  RefreshTerminal();
  game->CleanupResources();
  try {
    stringstream filepath;
    filepath << "./saves/world" << slot << ".bin";
    std::ifstream ifs(filepath.str(), std::ios::binary);
    boost::archive::binary_iarchive ia(ifs);
    ia >> game->world;
  }
  catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
  game->world->SetEnts();
}

void GameFIO::SaveWorld(Game *game) {
  PrintGraphic(0, 0, {"Saving . . .", "white", "black"});
  RefreshTerminal();
  try {
    stringstream filepath;
    filepath << "./saves/world" << game->world->slot << ".bin";
    std::ofstream ofs(filepath.str(), std::ios::binary);
    {
      boost::archive::binary_oarchive oa(ofs);
      oa << game->world;
    }
  }
  catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
}

#include "world_gen.h"

#include "./map_generators/perlin_generator.h"

#include "game.h"
#include "world.h"
#include "map.h"
#include "entity.h"

#include <chrono>
#include <random>

void WorldGen::GeneratePerlinMap(Map *map, int wx, int wy, float freq, int depth, int seed)
{
  PerlinGenerator perlinGenerator = PerlinGenerator(seed);

  int map_w = map->width, map_h = map->height;
  for (int i = 0; i < map_w; i++)
    for (int j = 0; j < map_h; j++)
    {
      float height = perlinGenerator.Perlin2d(wx * map_w + i, wy * map_h + j, freq, depth);
      float height2 = perlinGenerator.Perlin2d(wx * map_w + i, wy * map_h + j, freq*6, depth/2);
      float height3 = perlinGenerator.Perlin2d(wx * map_w + i, wy * map_h + j, freq/3, depth);
      if (height < .45)
        map->SetTile(i, j, {{'~', "blue", "black"},false,false,false});
      else if (height <.75 && height3 < .4)
        map->SetTile(i, j, {{'~', "blue", "black"},false,false,false});
      else if (height >= .75 || height2 > .65)
        map->SetTile(i, j, {{'#', "gray", "black"},false,true,false});
      else if (height < .75)
        map->SetTile(i, j, {{'.', "165,42,42", "black"},true,false,false});
    }
}

void WorldGen::GenerateWorld(Game *game, int size, int slot)
{
// pick random seed from clock
  chrono::high_resolution_clock::time_point d = chrono::high_resolution_clock::now();
  unsigned seed2 = d.time_since_epoch().count();
  default_random_engine generator(seed2);
  binomial_distribution<int> distribution(INT_MAX,.5);
  int dice_roll = distribution(generator);
// initialize the world with the seed
  game->CleanupResources();
  game->world = new World(size,size,128,128,slot);
  game->world->seed = dice_roll;
// generate map
  vector<int> walkable_positions;
  for (int i = 0; i < game->world->width; i++)
    for (int j = 0; j < game->world->height; j++) {
      GeneratePerlinMap(game->world->GetMap(i, j), i, j, 0.002, 16, dice_roll);
      game->world->GetMap(i, j)->DetermineGraphic();
      if (game->world->GetMap(i, j)->gset.ch == '.')
        walkable_positions.push_back(i * game->world->width + j);
    }
    srand(time(0));
  int position = walkable_positions[0+rand()%walkable_positions.size()];
// add some entities
  game->world->AddEntity(Entity({'@', "yellow", "black" }, "player", { 10, 10, static_cast<uint16_t>(position / game->world->width), static_cast<uint16_t>(position % game->world->width) }, 28));
  game->world->AddEntity(Entity({'g', "green", "black" }, "goblin", { 20, 10, 0, 0 }, 5));
}

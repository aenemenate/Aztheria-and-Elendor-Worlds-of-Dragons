#include "world_gen.h"

#include "perlin_generator.h"

#include "../game.h"
#include "../world.h"
#include "../map/area.h"
#include "../map/map_helper.h"
#include "../entity.h"
#include "../map_objects/block_builders.h"
#include "../map_objects/block_systems.h"

#include "biome_gen.h"
#include "dung_gen.h"

#include <string>
#include <chrono>
#include <random>
#include <stdlib.h>
#include <algorithm>

void WorldGen::GeneratePerlinMap(Area *area, int wx, int wy, float freq, int depth, int seed) {
  PerlinGenerator perlinGenerator = PerlinGenerator(seed);
  int map_w = area->width, map_h = area->height;
  for (int i = 0; i < map_w; i++)
    for (int j = 0; j < map_h; j++) {
    // TODO(Tristan): add sand to the map generator
      float height = perlinGenerator.Perlin2d(wx * map_w + i, wy * map_h + j, freq, depth);
      float height2 = perlinGenerator.Perlin2d(wx * map_w + i, wy * map_h + j, freq*6, depth/2);
      float height3 = perlinGenerator.Perlin2d(wx * map_w + i, wy * map_h + j, freq/3, depth);
      if (height < .45) {
        area->SetTile(i, j, 0, TILE_WATER);
        area->SetBlock(i, j, 0, BuildAirBlock());
        area->SetHeightMap(i, j, height);
      }
      else if (height <.77 && height3 < .4) {
        area->SetTile(i, j, 0, TILE_WATER);
        area->SetBlock(i, j, 0, BuildAirBlock());
        area->SetHeightMap(i, j, height3);
      }
      else if (height >= .7 || (height2 > .65 && height > .55)) {
        area->SetTile(i, j, 0, TILE_DIRT);
        area->SetBlock(i, j, 0, BuildStoneBlock());
        if (height < .7) area->SetHeightMap(i, j, height2);
        else area->SetHeightMap(i, j, height);
      }
      else if (height < .7) {
        area->SetTile(i, j, 0, TILE_DIRT);
        area->SetBlock(i, j, 0, BuildAirBlock());
        area->SetHeightMap(i, j, height);
      }
    }
}

void WorldGen::GeneratePlants(World* world) {
  srand(time(0));
  int areawidth = world->GetArea(0,0)->width,
      areaheight = world->GetArea(0,0)->height;
  for (int i = 0; i < world->width * areawidth; i++)
    for (int j = 0; j < world->height * areaheight; j++) {
      Area* area = world->GetArea(i / areawidth, j / areaheight);
      Block *block = area->GetBlock(i % areawidth, j % areaheight, 0);
      Tile *tile = area->GetTile(i % areawidth, j % areaheight, 0);
      bool cur_point_adjacent_to_water = MapHelper::PointAdjacentToTileOfType(area, {i % areawidth, j % areaheight}, 0, "water");
      bool cur_point_adjacent_to_stone = MapHelper::PointAdjacentToBlockOfType(area, {i % areawidth, j % areaheight}, 0, "stone");
      bool tile_is_dirt = (tile->name == "dirt");
      bool block_is_air = (block->name == "air");
      if (rand() % 10 < 6 && tile_is_dirt && block_is_air) {
        if (cur_point_adjacent_to_water) {
          if (rand() % 10 < 1)
            area->SetBlock(i % areawidth, j % areaheight, 0, BuildTree());
          else
            area->SetBlock(i % areawidth, j % areaheight, 0, BuildGrassBlock());
        }
        else if (cur_point_adjacent_to_stone) {
          // build highland grass block
          area->SetBlock(i % areawidth, j % areaheight, 0, BuildHlGrassBlock());
        }
      }
    }
  for (int g = 0; g < 30; g++)
    UpdatePlants(world);
}

void WorldGen::GenerateWorld(Game *game, int size, int slot) {
// pick random seed from clock
  chrono::high_resolution_clock::time_point d = chrono::high_resolution_clock::now();
  unsigned seed2 = d.time_since_epoch().count();
  default_random_engine generator(seed2);
  binomial_distribution<int> distribution(INT_MAX,.5);
  int dice_roll = distribution(generator);
// initialize the world with the seed
  game->CleanupResources();
  game->world = new World(size,size,156,156,slot);
  game->world->seed = dice_roll;
// generate terrain and determine temperature
  vector<int> walkable_positions;
  for (int i = 0; i < game->world->width; i++)
    for (int j = 0; j < game->world->height; j++) {
    // generate height map
      GeneratePerlinMap(game->world->GetArea(i, j), i, j, 0.002, 16, game->world->seed);
    // determine area terrain type
      BiomeGen::DetermineAreaTerrainType(game->world->GetArea(i, j));
    // determine temperature
      BiomeGen::DetermineAreaTemperature(game->world->GetArea(i, j), j, game->world->height);
    // collect areas where we want to place the player
      TerrainType tt = game->world->GetArea(i, j)->terrain_type;
      switch (tt) {
        case TerrainType::Plain: 
        case TerrainType::Hill:
          walkable_positions.push_back(i * game->world->width + j);
          break;
      }
    }
  BiomeGen::DetermineHumidityMap(game->world); // puts humidity for every world tile
  BiomeGen::DetermineBiomes(game->world);      // assigns biomes to every world tile
  DungeonGen::PlaceDungeons(game->world);      // puts all dungeons on the world
  GeneratePlants(game->world);                 // generates plants
  srand(time(0));
  int player_pos = walkable_positions[0+rand()%walkable_positions.size()];
  PlaceEntities(game->world, player_pos);
}

void WorldGen::PlaceEntities(World* world, int player_wpos) {
  uint16_t world_x = static_cast<uint16_t>(player_wpos / world->width), 
           world_y = static_cast<uint16_t>(player_wpos % world->width);
  vector<int> walkable_positions;
  for (int i = 0; i < world->GetArea(0,0)->width; i++)
    for (int j = 0; j < world->GetArea(0,0)->height; j++) {
      if (world->GetArea(world_x, world_y)->GetTile(i,j,0)->walkable)
        walkable_positions.push_back(i * world->GetArea(0,0)->width + j);
    }
  srand(time(0));
  int player_pos = walkable_positions[rand()%walkable_positions.size()];
  uint16_t x_pos = static_cast<uint16_t>(player_pos / world->GetArea(0,0)->width), 
           y_pos = static_cast<uint16_t>(player_pos % world->GetArea(0,0)->width);
// add player
  world->AddEntity(Entity({"@", "yellow", "black" }, "player", { x_pos, y_pos, 0, world_x, world_y }, 28));
}
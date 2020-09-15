#include "world_gen.h"

#include "./map_generators/perlin_generator.h"

#include "game.h"
#include "world.h"
#include "map.h"
#include "entity.h"

#include <chrono>
#include <random>
#include <stdlib.h>
#include <algorithm>

void WorldGen::GeneratePerlinMap(Map *map, int wx, int wy, float freq, int depth, int seed)
{
  PerlinGenerator perlinGenerator = PerlinGenerator(seed);

  int map_w = map->width, map_h = map->height;
  for (int i = 0; i < map_w; i++)
    for (int j = 0; j < map_h; j++) {
    // TODO(Tristan): add sand to the map generator
      float height = perlinGenerator.Perlin2d(wx * map_w + i, wy * map_h + j, freq, depth);
      float height2 = perlinGenerator.Perlin2d(wx * map_w + i, wy * map_h + j, freq*6, depth/2);
      float height3 = perlinGenerator.Perlin2d(wx * map_w + i, wy * map_h + j, freq/3, depth);
      if (height < .45) {
        map->SetTile(i, j, {{"~", "blue", "black"},false,false,false});
        map->SetHeightMap(i, j, height);
      }
      else if (height <.75 && height3 < .4) {
        map->SetTile(i, j, {{"~", "blue", "black"},false,false,false});
        map->SetHeightMap(i, j, height3);
      }
      else if (height >= .7 || height2 > .65) {
        map->SetTile(i, j, {{"#", "gray", "black"},false,true,false});
        map->SetHeightMap(i, j, height2);
      }
      else if (height < .7) {
        map->SetTile(i, j, {{".", "165,42,42", "black"},true,false,false});
        map->SetHeightMap(i, j, height);
      }
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
// generate terrain and determine temperature
  vector<int> walkable_positions;
  for (int i = 0; i < game->world->width; i++)
    for (int j = 0; j < game->world->height; j++) {
    // generate height map
      GeneratePerlinMap(game->world->GetMap(i, j), i, j, 0.002, 16, game->world->seed);
    // determine area terrain type
      DetermineMapTerrainType(game->world->GetMap(i, j));
    // determine temperature
      DetermineMapTemperature(game->world->GetMap(i, j), j, game->world->height);
    // determine areas where we want to place the player
      TerrainType tt = game->world->GetMap(i, j)->terrain_type;
      switch (tt) {
        case TerrainType::Plain: 
        case TerrainType::Hill:
          walkable_positions.push_back(i * game->world->width + j);
          break;
      }
    }
  // generate humidity map
  DetermineHumidityMap(game->world);
  // determine biomes
  DetermineBiomes(game->world);
  // seed plants/ trees
  // SeedPlants(game->world);
  // for (int i = 0; i < plant_generations; ++i)
  // game->world->GrowPlants();
  // 
  srand(time(0));
  int position = walkable_positions[0+rand()%walkable_positions.size()];
// add some entities
  game->world->AddEntity(Entity({"@", "yellow", "black" }, "player", { 10, 10, static_cast<uint16_t>(position / game->world->width), static_cast<uint16_t>(position % game->world->width) }, 28));
  game->world->AddEntity(Entity({"g", "green", "black" }, "goblin", { 20, 10, 0, 0 }, 5));
}

void WorldGen::DetermineMapTerrainType(Map* map)
{
  int dirt_num=0, water_num=0, mountain_num=0, beach_num=0;
  for (int i = 0; i < map->width; i++)
    for (int j = 0; j < map->height; j++) {
      if (map->GetTile(i,j)->gset.ch == ".")
        dirt_num++;
      else if (map->GetTile(i,j)->gset.ch == "~")
        water_num++;
      else if (map->GetTile(i,j)->gset.ch == "#")
        mountain_num++;
    }
    if (dirt_num >= water_num && dirt_num >= mountain_num) {
      if (mountain_num > 10)
        map->terrain_type = TerrainType::Hill;
      else 
        map->terrain_type = TerrainType::Plain;
    }
    else if (water_num >= dirt_num && water_num >= mountain_num)
      map->terrain_type = TerrainType::Ocean;
    else if (mountain_num >= water_num && mountain_num >= dirt_num)
      map->terrain_type = TerrainType::Mountain;
}

void WorldGen::DetermineMapTemperature(Map* map, int wy, int world_size) 
{
  double closeness_to_equator = (double)((world_size/2) - abs(world_size/2 - wy)) / (double)(world_size/2);
  double average_height = 0;
  for (int i = 0; i < map->width; ++i)
    for (int j = 0; j < map->height; ++j)
      average_height += map->GetHeightMap(i, j);
  average_height /= map->height * map->width;
  double max_temperature = 50;
  map->temperature = (max_temperature)*closeness_to_equator - std::max(0, (int)(20.0*(average_height-.2)));
}

void WorldGen::DetermineHumidityMap(World* world)
{
  PerlinGenerator perlinGenerator = PerlinGenerator(world->seed);
  int areas_since_ocean = 4;
  int areas_since_mountain = 4;
  for (int j = 0; j < world->height; ++j) {
    areas_since_ocean = 5;
    areas_since_mountain = 5;
    for (int i = 0; i < world->width; ++i) {
      Map* map = world->GetMap(i, j);
      float height = perlinGenerator.Perlin2d(i, j, .4, 8);
      map->humidity = .6 * height;
      if (areas_since_ocean < 3)
        map->humidity += .1;
      if (areas_since_mountain < 4)
        map->humidity -= .2;
      map->humidity = std::max(0.0f, map->humidity);
      if (map->terrain_type == TerrainType::Ocean)
        areas_since_ocean = 0;
      else
        ++areas_since_ocean;
      if (map->terrain_type == TerrainType::Mountain)
        areas_since_mountain = 0;
      else 
        ++areas_since_mountain;
    }
  }
}

void WorldGen::DetermineBiomes(World* world)
{
  for (int j = 0; j < world->height; ++j)
    for (int i = 0; i < world->width; ++i) {
      Map *map = world->GetMap(i, j);
      switch (map->terrain_type) {
        case TerrainType::Mountain:
          if (map->temperature > 18)
            map->biome_type = BiomeType::Barren;
          else
            map->biome_type = BiomeType::SnowyMountain;
          break;
        case TerrainType::Ocean:
          map->biome_type = BiomeType::Barren;
          break;
        case TerrainType::Hill:
        case TerrainType::Plain:
          if (map->temperature > 32) {
            if (map->humidity > .4)
              map->biome_type = BiomeType::TropicalForest;
            else if (map->humidity > .05)
              map->biome_type = BiomeType::GrassyPlain;
            else
              map->biome_type = BiomeType::Desert;
          }
          else if (map->temperature > 16) {
            if (map->humidity > .2)
              map->biome_type = BiomeType::BorealForest;
            else if (map->humidity > .05)
              map->biome_type = BiomeType::GrassyPlain;
            else
              map->biome_type = BiomeType::Desert;
          }
          else if (map->temperature > 7){
            if (map->humidity > .2)
              map->biome_type = BiomeType::Taiga;
            else if (map->humidity > .05)
              map->biome_type = BiomeType::SnowyPlain;
            else
              map->biome_type = BiomeType::Tundra;
          }
          else if (map->temperature > 0) {
            if (map->humidity > .2)
              map->biome_type = BiomeType::Taiga;
            else if (map->humidity > .05)
              map->biome_type = BiomeType::Tundra;
            else
              map->biome_type = BiomeType::Barren;
          }
          else {
            map->biome_type = BiomeType::Tundra;
          }
          break;
      }
    }
}
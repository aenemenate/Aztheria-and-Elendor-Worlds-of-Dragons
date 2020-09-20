#include "world_gen.h"

#include "map_generators/perlin_generator.h"

#include "game.h"
#include "world.h"
#include "map/area.h"
#include "entity.h"

#include <chrono>
#include <random>
#include <stdlib.h>
#include <algorithm>

void WorldGen::GeneratePerlinMap(Area *area, int wx, int wy, float freq, int depth, int seed)
{
  PerlinGenerator perlinGenerator = PerlinGenerator(seed);

  int map_w = area->width, map_h = area->height;
  for (int i = 0; i < map_w; i++)
    for (int j = 0; j < map_h; j++) {
    // TODO(Tristan): add sand to the map generator
      float height = perlinGenerator.Perlin2d(wx * map_w + i, wy * map_h + j, freq, depth);
      float height2 = perlinGenerator.Perlin2d(wx * map_w + i, wy * map_h + j, freq*6, depth/2);
      float height3 = perlinGenerator.Perlin2d(wx * map_w + i, wy * map_h + j, freq/3, depth);
      if (height < .45) {
        area->SetTile(i, j, {{"~", "blue", "black"},false,false,false});
        area->SetHeightMap(i, j, height);
      }
      else if (height <.77 && height3 < .4) {
        area->SetTile(i, j, {{"~", "blue", "black"},false,false,false});
        area->SetHeightMap(i, j, height3);
      }
      else if (height >= .7 || (height2 > .65 && height > .5)) {
        area->SetTile(i, j, {{"#", "gray", "black"},false,true,false});
        if (height < .7)
          area->SetHeightMap(i, j, height2);
        else
          area->SetHeightMap(i, j, height);
      }
      else if (height < .7) {
        area->SetTile(i, j, {{".", "165,42,42", "black"},true,false,false});
        area->SetHeightMap(i, j, height);
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
      GeneratePerlinMap(game->world->GetArea(i, j), i, j, 0.002, 16, game->world->seed);
    // determine area terrain type
      DetermineAreaTerrainType(game->world->GetArea(i, j));
    // determine temperature
      DetermineAreaTemperature(game->world->GetArea(i, j), j, game->world->height);
    // determine areas where we want to place the player
      TerrainType tt = game->world->GetArea(i, j)->terrain_type;
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
  PlaceDungeons(game->world);
  // seed plants/ trees
  // SeedPlants(game->world);
  // for (int i = 0; i < plant_generations; ++i)
  // game->world->GrowPlants();
  srand(time(0));
  int player_pos = walkable_positions[0+rand()%walkable_positions.size()];
  PlaceEntities(game->world, player_pos);
}

void WorldGen::DetermineAreaTerrainType(Area* area)
{
  int dirt_num=0, water_num=0, mountain_num=0, beach_num=0;
  for (int i = 0; i < area->width; i++)
    for (int j = 0; j < area->height; j++) {
      if (area->GetTile(i,j,0)->gset.ch == ".")
        dirt_num++;
      else if (area->GetTile(i,j,0)->gset.ch == "~")
        water_num++;
      else if (area->GetTile(i,j,0)->gset.ch == "#")
        mountain_num++;
    }
    if (dirt_num >= water_num && dirt_num >= mountain_num) {
      if (mountain_num > 10)
        area->terrain_type = TerrainType::Hill;
      else 
        area->terrain_type = TerrainType::Plain;
    }
    else if (water_num >= dirt_num && water_num >= mountain_num)
      area->terrain_type = TerrainType::Ocean;
    else if (mountain_num >= water_num && mountain_num >= dirt_num)
      area->terrain_type = TerrainType::Mountain;
}

void WorldGen::DetermineAreaTemperature(Area* area, int wy, int world_size) 
{
  double closeness_to_equator = (double)((world_size/2) - abs(world_size/2 - wy)) / (double)(world_size/2);
  double average_height = 0;
  for (int i = 0; i < area->width; ++i)
    for (int j = 0; j < area->height; ++j)
      average_height += area->GetHeightMap(i, j);
  average_height /= area->height * area->width;
  double max_temperature = 50;
  area->temperature = (max_temperature)*closeness_to_equator - std::max(0, (int)(20.0*(average_height-.2)));
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
      Area* area = world->GetArea(i, j);
      float height = perlinGenerator.Perlin2d(i, j, .6, 8);
      area->humidity = .55 * height;
      if (areas_since_ocean < 3)
        area->humidity += .1;
      if (areas_since_mountain < 4)
        area->humidity -= .2;
      area->humidity = std::max(0.0f, area->humidity);
      if (area->terrain_type == TerrainType::Ocean)
        areas_since_ocean = 0;
      else
        ++areas_since_ocean;
      if (area->terrain_type == TerrainType::Mountain)
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
      Area *area = world->GetArea(i, j);
      switch (area->terrain_type) {
        case TerrainType::Mountain:
          if (area->temperature > 18)
            area->biome_type = BiomeType::Barren;
          else
            area->biome_type = BiomeType::SnowyMountain;
          break;
        case TerrainType::Ocean:
          area->biome_type = BiomeType::Barren;
          break;
        case TerrainType::Hill:
        case TerrainType::Plain:
          if (area->temperature > 32) {
            if (area->humidity > .37)
              area->biome_type = BiomeType::TropicalForest;
            else if (area->humidity > .05)
              area->biome_type = BiomeType::GrassyPlain;
            else
              area->biome_type = BiomeType::Desert;
          }
          else if (area->temperature > 16) {
            if (area->humidity > .3)
              area->biome_type = BiomeType::BorealForest;
            else if (area->humidity > .05)
              area->biome_type = BiomeType::GrassyPlain;
            else
              area->biome_type = BiomeType::Desert;
          }
          else if (area->temperature > 7){
            if (area->humidity > .3)
              area->biome_type = BiomeType::Taiga;
            else if (area->humidity > .05)
              area->biome_type = BiomeType::SnowyPlain;
            else
              area->biome_type = BiomeType::Tundra;
          }
          else if (area->temperature > -3) {
            if (area->humidity > .2)
              area->biome_type = BiomeType::Taiga;
            else if (area->humidity > .05)
              area->biome_type = BiomeType::Tundra;
            else
              area->biome_type = BiomeType::Barren;
          }
          else {
            area->biome_type = BiomeType::Tundra;
          }
          break;
      }
    }
}

void WorldGen::PlaceDungeons(World* world)
{
  int num_of_dungeons = world->width / 2;
  vector<Point> potential_dungeons;
  for (int i = 0; i < world->width; ++i)
    for (int j = 0; j < world->height; ++j) {
      Area *area = world->GetArea(i, j);
      if (area->terrain_type == TerrainType::Hill)
        potential_dungeons.push_back({i, j});
    }
  srand(time(0));
  for (int i = 0; i < num_of_dungeons && potential_dungeons.size() > 0; ++i) {
    int vec_ind = rand()%potential_dungeons.size();
    Point area_pos = potential_dungeons[vec_ind];
    Area *area = world->GetArea(area_pos.x, area_pos.y);
    area->GetDungeonFloors()->push_back(Dungeon(area->width, area->height));
    potential_dungeons.erase(potential_dungeons.begin() + vec_ind);
  }
}

void WorldGen::PlaceEntities(World* world, int player_wpos)
{
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
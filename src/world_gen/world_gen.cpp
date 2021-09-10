#include "world_gen.h"

#include "perlin_generator.h"

#include "../game.h"
#include "../world/world.h"
#include "../map/area.h"
#include "../map/map_helper.h"
#include "../ecs/entity.h"
#include "../map_objects/block_builders.h"
#include "../map_objects/block_systems.h"

#include "biome_gen.h"
#include "dung_gen.h"

#include "../xml/xml_parser.h"

#include <string>
#include <chrono>
#include <random>
#include <stdlib.h>
#include <algorithm>

#include <iostream>

std::vector<Point> GetWalkablePoints(Map *map) {
  std::vector<Point> walkable_points;
  for (int i = 0; i < map->width; i++)
    for (int j = 0; j < map->height; j++) {
      if (map->GetTile(i,j)->walkable && map->GetEntity(i,j) == nullptr)
        walkable_points.push_back({i, j});
    }
  return walkable_points;
}

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
  std::vector<Block> plants;
  plants = XmlParser::GetBlocksFromXml("./data/plants.xml");
  for (int i = 0; i < world->width; ++i) {
    for (int j = 0; j < world->height; ++j) {
      Area* area = world->GetArea(i, j);
    /* set the plant list for this biome */
      std::vector<int> plants_of_biome;
      for (int t = 0; t < plants.size(); ++t) 
        if (plants[t].HasComponent(PLANT_ID)) {
	  std::shared_ptr<Plant> plant_c = dynamic_pointer_cast<Plant>(plants[t].GetComponent(PLANT_ID));
          if (plant_c->biome == area->biome_type)
            plants_of_biome.push_back(t);
        }
    /* iterate every plant in list, then every point and check rand at each point vs the spawn chance */
      for (int p = 0; p < plants_of_biome.size(); ++p)
        for (int ix = 0; ix < area->width; ++ix)
	  for (int jx = 0; jx < area->height; ++jx) {
          // if rand() % 100 is less then spawn chance and this is the correct biome
	    std::shared_ptr<Plant> plant_c = dynamic_pointer_cast<Plant>(plants[plants_of_biome[p]].GetComponent(PLANT_ID));
            Block *block = area->GetBlock(ix, jx, 0);
            Tile *tile = area->GetTile(ix, jx, 0);
            bool tile_is_seed_tile = (tile->name == plant_c->seed_tile);
            bool block_is_air = (block->name == "air");
            if (rand() % 100 < plant_c->spawn_chance && tile_is_seed_tile && block_is_air)
	      area->SetBlock(ix, jx, 0, plants[plants_of_biome[p]].GetCopy());
          }
    }
  }
  for (int g = 0; g < 30; g++)
    UpdatePlants(world);
}

void WorldGen::PlaceEntities(World* world) {
  vector<Point> walkable_areas;
  for (int i = 0; i < world->width; i++)
    for (int j = 0; j < world->height; j++) {
      TerrainType tt = world->GetArea(i, j)->terrain_type;
      switch (tt) {
        case TerrainType::Plain: 
        case TerrainType::Hill:
          walkable_areas.push_back({i, j});
          break;
      }
    }
  if (walkable_areas.size() == 0)
      return;
  int wp_index = rand()%walkable_areas.size();
  uint16_t world_x = walkable_areas[wp_index].x, world_y = walkable_areas[wp_index].y;
  std::vector<Point> walkable_positions = GetWalkablePoints(world->GetArea(world_x, world_y));
  srand(time(0));
  Point pos = walkable_positions[rand()%walkable_positions.size()];
  world->entities[0].AddComponent(std::make_shared<EntPosition>(EntPosition({ (uint16_t)(pos.x), (uint16_t)(pos.y), 0, world_x, world_y })));
// add animals from xml
  std::vector<Entity> entities;
  entities = XmlParser::GetEntitiesFromXml("./data/animals.xml");
  for (int e = 0; e < world->width * world->height * 5; ++e) {
    wp_index = rand()%walkable_areas.size();
    world_x = walkable_areas[wp_index].x;
    world_y = walkable_areas[wp_index].y;
    std::vector<int> ents_of_biome;
    for (int i = 0; i < entities.size(); ++i) 
      if (entities[i].HasComponent(EC_ANIMALAI_ID)) {
	std::shared_ptr<AnimalAi> animalAi = dynamic_pointer_cast<AnimalAi>(entities[i].GetComponent(EC_ANIMALAI_ID));
        if (animalAi->biome == world->GetArea(world_x, world_y)->biome_type)
          ents_of_biome.push_back(i);
      }
    if (ents_of_biome.size() > 0) {
      walkable_positions = GetWalkablePoints(world->GetArea(world_x, world_y));
      Entity *ent_orig = &(entities[ents_of_biome[rand()%ents_of_biome.size()]]);
      Entity ent;
      for (auto component : ent_orig->components)
        ent.AddComponent(component->GetCopy());
      Point ent_pos = walkable_positions[rand()%walkable_positions.size()];
      ent.AddComponent(std::make_shared<EntPosition>(EntPosition({ (uint16_t)(ent_pos.x), (uint16_t)(ent_pos.y), 0, world_x, world_y })));
      ent.AddComponent(std::make_shared<ActionTime>(ActionTime(Time(world->time))));
      world->AddEntity(ent);
    }
  }
}

void WorldGen::GenerateWorld(Game *game, int size, int slot) {
// pick random seed from clock
  chrono::high_resolution_clock::time_point d = chrono::high_resolution_clock::now();
  unsigned seed2 = d.time_since_epoch().count();
  default_random_engine generator(seed2);
  binomial_distribution<int> distribution(INT_MAX - 1,.5);
  int dice_roll = distribution(generator);
// initialize the world with the seed
  Entity player = game->world->entities[0];
  game->CleanupResources();
  game->world = new World(size,size,156,156,slot);
  game->world->AddEntity(player);
  game->world->seed = dice_roll;
// generate terrain and determine temperature
  for (int i = 0; i < game->world->width; i++)
    for (int j = 0; j < game->world->height; j++) {
    // generate height map
      GeneratePerlinMap(game->world->GetArea(i, j), i, j, 0.002, 16, game->world->seed);
    // determine area terrain type
      BiomeGen::DetermineAreaTerrainType(game->world->GetArea(i, j));
    // determine temperature
      BiomeGen::DetermineAreaTemperature(game->world->GetArea(i, j), j, game->world->height);
    }
  BiomeGen::DetermineHumidityMap(game->world); // puts humidity for every world tile
  BiomeGen::DetermineBiomes(game->world);      // assigns biomes to every world tile
  GeneratePlants(game->world);                 // generates plants
  PlaceEntities(game->world);
  DungeonGen::PlaceDungeons(game->world);      // puts all dungeons on the world
}

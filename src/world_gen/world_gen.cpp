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
  int wp_index = rand()%walkable_areas.size();
  uint16_t world_x = walkable_areas[wp_index].x, world_y = walkable_areas[wp_index].y;
  std::vector<Point> walkable_positions = MapHelper::GetWalkablePoints(world->GetArea(world_x, world_y));
  srand(time(0));
  Point pos = walkable_positions[rand()%walkable_positions.size()];
// add player
  Entity player;
  player.AddComponent(std::make_shared<Renderable>(Renderable({"@", "yellow", "black"})));
  player.AddComponent(std::make_shared<EntPosition>(EntPosition({ pos.x, pos.y, 0, world_x, world_y })));
  player.AddComponent(std::make_shared<Name>(Name("player")));
  player.AddComponent(std::make_shared<Fov>(Fov(28)));
  player.AddComponent(std::make_shared<Player>(Player(true)));
  world->AddEntity(player);
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
      walkable_positions = MapHelper::GetWalkablePoints(world->GetArea(world_x, world_y));
      Entity *ent_orig = &(entities[ents_of_biome[rand()%ents_of_biome.size()]]);
      Entity ent;
      for (auto component : ent_orig->components)
        ent.AddComponent(component->GetCopy());
      Point ent_pos = walkable_positions[rand()%walkable_positions.size()];
      ent.AddComponent(std::shared_ptr<EntityComponent>(new EntPosition({ ent_pos.x, ent_pos.y, 0, world_x, world_y })));
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
  game->CleanupResources();
  game->world = new World(size,size,156,156,slot);
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
  DungeonGen::PlaceDungeons(game->world);      // puts all dungeons on the world
  GeneratePlants(game->world);                 // generates plants
  PlaceEntities(game->world);
}

#include "biome_gen.h"

#include "perlin_generator.h"
#include "../world/world.h"
#include "../map/area.h"
#include "../ecs/entity.h"

#include "../map_objects/block_builders.h"

void BiomeGen::DetermineAreaTerrainType(Area* area) {
  int dirt_num=0, water_num=0, mountain_num=0, beach_num=0;
  for (int i = 0; i < area->width; i++)
    for (int j = 0; j < area->height; j++) {
      if (area->GetBlock(i,j,0)->name == "stone")
        mountain_num++;
      else if (area->GetTile(i,j,0)->name == "dirt")
        dirt_num++;
      else if (area->GetTile(i,j,0)->name == "water")
        water_num++;
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

void BiomeGen::DetermineAreaTemperature(Area* area, int wy, int world_size) {
  double closeness_to_equator = (double)((world_size/2) - abs(world_size/2 - wy)) / (double)(world_size/2);
  double average_height = 0;
  for (int i = 0; i < area->width; ++i)
    for (int j = 0; j < area->height; ++j)
      average_height += area->GetHeightMap(i, j);
  average_height /= area->height * area->width;
  double max_temperature = 50;
  area->temperature = (max_temperature)*closeness_to_equator - std::max(0, (int)(20.0*(average_height-.2)));
}

void BiomeGen::DetermineHumidityMap(World* world) {
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

void convertToSandBiome(Area* area) {
  for (int i = 0; i < area->width; i++)
    for (int j = 0; j < area->height; j++) {
      if (area->GetTile(i, j, 0)->name == "dirt")
	area->SetTile(i, j, 0, TILE_SAND);
      if (area->GetBlock(i, j, 0)->name == "stone")
	area->SetBlock(i, j, 0, BuildSandstoneBlock());
    }
}

void convertToSnowBiome(Area* area) {
  for (int i = 0; i < area->width; i++)
    for (int j = 0; j < area->height; j++)
      if (rand()%100 < 75 && area->GetTile(i,j,0)->name != "water")
        area->GetTile(i, j, 0)->isSnowy = true;
}

void BiomeGen::DetermineBiomes(World* world) {
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
              area->biome_type = BiomeType::DeciduousForest;
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
      if (area->biome_type == BiomeType::Desert)
	convertToSandBiome(area);
      if (area->biome_type == BiomeType::Tundra
      ||  area->biome_type == BiomeType::Taiga
      ||  area->biome_type == BiomeType::SnowyPlain
      ||  area->biome_type == BiomeType::SnowyMountain)
	convertToSnowBiome(area);
    }
}

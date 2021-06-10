#include "dung_gen.h"

#include "../world/world.h"
#include "../map/area.h"
#include "../ecs/entity.h"
#include "../map_objects/block_builders.h"

#include <chrono>
#include <random>

int getNumAdjacentOpenPositions(Area* area, Point pos, int z_level) {
  int num = 0;
  for (int i = -1; i < 1; i++)
    for (int j = -1; j < 1; j++) {
      Point cur_pos = {pos.x + i, pos.y + j};
      if (!(cur_pos.x == 0 && cur_pos.y == 0) 
      && area->PointWithinBounds(cur_pos.x, cur_pos.y)
      && area->GetBlock(cur_pos.x, cur_pos.y, z_level)->solid == false
      && area->GetTile(cur_pos.x, cur_pos.y, z_level)->walkable == true) {
        num++;
      }
    }
  return num;
}

std::vector<Point> getPotentialStairPoints(Area *area, int z_level) {
  std::vector<Point> p_spots;
  for (int i = 0; i < area->width; i++)
    for (int j = 0; j < area->height; j++) {
      if (area->GetBlock(i, j, z_level)->solid == true && 3 <= getNumAdjacentOpenPositions(area, {i,j}, z_level)) {
        p_spots.push_back({i,j});
      }
    }
  return p_spots;
}

std::vector<Point> GetDownStairPoints(Map *map) {
  std::vector<Point> walkable_points;
  for (int i = 0; i < map->width; i++)
    for (int j = 0; j < map->height; j++) {
      if (map->GetTile(i,j)->walkable && map->GetEntity(i,j) == nullptr)
        walkable_points.push_back({i, j});
    }
  return walkable_points;
}

void generateDungeonFloor(Area* area, Point downstair_pos, int levels = 1) {
  area->GetDungeonFloors()->push_back(Dungeon(area->width, area->height));
  Dungeon *dungeon = &(area->GetDungeonFloors()->back());
  int z_level = area->GetDungeonFloors()->size();
  for (int i = 0; i < area->width; i++)
    for (int j = 0; j < area->height; j++) {
      if (i == 0 || j == 0 || i == area->width-1 || j == area->height-1) {
        area->SetBlock(i, j, z_level, BuildStoneBlock());
        area->SetTile(i,j, z_level, TILE_DIRT);
      }
      else {
        area->SetBlock(i, j, z_level, BuildAirBlock());
        area->SetTile(i,j, z_level, TILE_DIRT);
      }
    }
  area->SetBlock(downstair_pos.x, downstair_pos.y, z_level, BuildStoneUpStair());
  // place down stairs
  if (levels > z_level) {
    std::vector<Point> walkable_points = GetDownStairPoints(dungeon);   
    Point stair_point = walkable_points[rand()%walkable_points.size()];
    dungeon->SetBlock(stair_point.x, stair_point.y, BuildStoneDownStair());
    generateDungeonFloor(area, stair_point, levels);
  }
}

void DungeonGen::PlaceDungeons(World* world) {
  int num_of_dungeons = world->width / 3;
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
    int levels = rand()%26;
// put stairs
    std::vector<Point> stair_points = getPotentialStairPoints(area, 0);
    if (stair_points.size() == 0)
    { i--; continue; }
    Point stair_point = stair_points[rand()%stair_points.size()];
    area->SetBlock(stair_point.x, stair_point.y, 0, BuildStoneDownStair());
    generateDungeonFloor(area, stair_point, levels);
    potential_dungeons.erase(potential_dungeons.begin() + vec_ind);
  }
}
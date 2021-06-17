#include "dung_gen.h"

#include "../world/world.h"
#include "../map/area.h"
#include "../ecs/entity.h"
#include "../map_objects/block_builders.h"
#include "../pathfinder.h"
#include "../xml/xml_parser.h"

#include <cmath>
#include <chrono>
#include <random>

int getNumAdjacentOpenPositions(Area* area, Point pos, int z_level) {
  int num = 0;
  for (int i = -1; i <= 1; i++)
    for (int j = -1; j <= 1; j++) {
      Point cur_pos = {pos.x + i, pos.y + j};
      if (!(i == 0 && j == 0) 
      && area->PointWithinBounds(cur_pos.x, cur_pos.y)
      && area->GetBlock(cur_pos.x, cur_pos.y, z_level)->solid == false
      && area->GetTile(cur_pos.x, cur_pos.y, z_level)->walkable == true) {
        num++;
      }
    }
  return num;
}

int getNumAdjacentWalls(Dungeon *dungeon, Point pos) {
  int num = 0;
  for (int i = -1; i <= 1; i++)
    for (int j = -1; j <= 1; j++) {
      Point cur_pos = {pos.x + i, pos.y + j};
      if (dungeon->GetBlock(cur_pos.x, cur_pos.y)->solid) {
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

void applyCellularAutomata(Dungeon *dungeon) {
  for (int i = 1; i < dungeon->width - 1; i++)
    for (int j = 1; j < dungeon->height - 1; j++) {
      if (dungeon->GetBlock(i, j)->solid) {
        if (getNumAdjacentWalls(dungeon, {i, j}) >= 4)
          ;
	else
	  dungeon->SetBlock(i, j, BuildAirBlock());
      }
      else if (getNumAdjacentWalls(dungeon, {i, j}) >= 5)
        dungeon->SetBlock(i, j, BuildStoneBlock());\
    }
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
	if (rand()%5>1 || (abs(downstair_pos.x - i) +
				abs(downstair_pos.y - j) < 15)) {
          area->SetBlock(i, j, z_level, BuildAirBlock());
          area->SetTile(i,j, z_level, TILE_DIRT);
	}
	else {
          area->SetBlock(i, j, z_level, BuildStoneBlock());
          area->SetTile(i,j, z_level, TILE_DIRT);
	}
      }
    }
  for (int i = 0; i < 2; ++i)
    applyCellularAutomata(dungeon);
  area->SetBlock(downstair_pos.x, downstair_pos.y, z_level, BuildStoneUpStair());
  // place down stairs
  if (levels > z_level) {
    std::vector<Point> walkable_points = GetDownStairPoints(dungeon);
    Point stair_point = walkable_points[rand()%walkable_points.size()];
    while (Pathfinder::GetPath(area, z_level, stair_point.x, stair_point.y, 
                                        downstair_pos.x, downstair_pos.y).size() <= 1)
      stair_point = walkable_points[rand()%walkable_points.size()];
    dungeon->SetBlock(stair_point.x, stair_point.y, BuildStoneDownStair());
    generateDungeonFloor(area, stair_point, levels);
  }
}

void placeEntities(int world_x, int world_y, World *world) {
  Area *area = world->GetArea(world_x, world_y);
  std::vector<Entity> entities;
  entities = XmlParser::GetEntitiesFromXml("./data/monsters.xml");
  for (int i = 0; i < area->GetDungeonFloors()->size(); ++i) {
    Dungeon *dungeon = &(area->GetDungeonFloors()[0][i]);
    std::vector<Point> walkable_points = GetDownStairPoints(dungeon);
    for (int j = 0; j < walkable_points.size() / 250; ++j) {
      Entity *ent_orig = &(entities[rand()%entities.size()]);
      Entity ent;
      for (auto component : ent_orig->components)
        ent.AddComponent(component->GetCopy());
      Point ent_pos = walkable_points[rand()%walkable_points.size()];
      ent.AddComponent(std::make_shared<EntPosition>(EntPosition({ (uint16_t)(ent_pos.x), (uint16_t)(ent_pos.y), 
							(uint16_t)(i+1), (uint16_t)(world_x), (uint16_t)(world_y) })));
      ent.AddComponent(std::make_shared<ActionTime>(ActionTime(Time(world->time))));
      world->AddEntity(ent);
    }
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
    placeEntities(area_pos.x, area_pos.y, world);
    potential_dungeons.erase(potential_dungeons.begin() + vec_ind);
  }
}
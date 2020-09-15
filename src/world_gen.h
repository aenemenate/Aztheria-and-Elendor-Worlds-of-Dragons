#pragma once

class Game;
class Map;
class World;

namespace WorldGen
{
  void GeneratePerlinMap(Map*,int wx,int wy,float freq,int depth,int seed);
  void DetermineMapTerrainType(Map*);
  void DetermineMapTemperature(Map*,int wy,int world_size);

  void DetermineHumidityMap(World*);
  void DetermineBiomes(World*);

  void GenerateWorld(Game*, int size, int slot);
}
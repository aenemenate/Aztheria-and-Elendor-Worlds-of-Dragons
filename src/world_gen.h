#pragma once

class Game;
class Area;
class World;

namespace WorldGen
{
  void GeneratePerlinMap(Area*,int wx,int wy,float freq,int depth,int seed);
  void DetermineAreaTerrainType(Area*);
  void DetermineAreaTemperature(Area*,int wy,int world_size);

  void DetermineHumidityMap(World*);
  void DetermineBiomes(World*);
  void PlaceDungeons(World*);

  void PlaceEntities(World*, int player_wpos);

  void GenerateWorld(Game*, int size, int slot);
}
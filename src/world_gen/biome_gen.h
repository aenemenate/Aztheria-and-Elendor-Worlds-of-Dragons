#pragma once

class Area;
class World;

namespace BiomeGen {
  void DetermineAreaTerrainType(Area*);
  void DetermineAreaTemperature(Area*,int wy,int world_size);
  void DetermineHumidityMap(World*);
  void DetermineBiomes(World*);
}
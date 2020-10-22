#pragma once

class Game;
class World;
class Area;

namespace WorldGen {
  void GeneratePerlinMap(Area*,int wx,int wy,float freq,int depth,int seed);
  void GeneratePlants(World*);
  void PlaceEntities(World*);
  void GenerateWorld(Game*, int size, int slot);
}
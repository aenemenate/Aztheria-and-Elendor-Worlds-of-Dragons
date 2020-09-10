#pragma once

class Game;
class Map;

class WorldGen
{
  public:
    static void GeneratePerlinMap(Map*,int,int,float,int,int);
    static void GenerateWorld(Game*, int size, int slot);
};
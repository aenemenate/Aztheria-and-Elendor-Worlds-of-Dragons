#include "map.h"

Map::Map() 
{ 
  width = 0; 
  height = 0;
  tiles.resize(width*height);
  ent_map.resize(width*height, nullptr);
}

Map::Map(uint16_t width, uint16_t height)
{
  this->width = width;
  this->height = height;
  tiles.resize(width*height);
  ent_map.resize(width*height, nullptr);
}

bool Map::PointWithinBounds(int x, int y)
{
  bool ret_val = false;
  if (x >= 0 && y >= 0 && x < width && y < height)
    ret_val = true;
  return ret_val;
}

Tile *Map::GetTile(int x, int y)
{
  Tile *tile = nullptr;
  if (PointWithinBounds(x, y))
    tile = &(tiles[x*width+y]);
  return tile;
}

void Map::SetTile(int x, int y, Tile tile)
{
  if (PointWithinBounds(x, y))
    tiles[x*width+y] = tile;
}

void Map::DetermineGraphic()
{
  int dirt_num=0, water_num=0, mountain_num=0;
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++)
    {
      if (tiles[i*width+j].gset.ch == '.')
        dirt_num++;
      else if (tiles[i*width+j].gset.ch == '~')
        water_num++;
      else if (tiles[i*width+j].gset.ch == '#')
        mountain_num++;
    }
    if (dirt_num >= water_num && dirt_num >= mountain_num)
      gset = Graphic{'.', "165,42,42", "black"};
    else if (water_num >= dirt_num && water_num >= mountain_num)
      gset = Graphic{'~', "blue", "black"};
    else if (mountain_num >= water_num && mountain_num >= dirt_num)
      gset = Graphic{'^', "gray", "black"};
}
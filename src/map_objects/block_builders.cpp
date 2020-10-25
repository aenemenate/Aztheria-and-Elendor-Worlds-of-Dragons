#include "block_builders.h"

Block BuildAirBlock() {
  Block block;
  block.gr = {" ", "black", ""};
  block.explored = false;
  block.opaque = false;
  block.solid = false;
  block.enterable = false;
  block.name = "air";
  return block;
}

Block BuildStoneBlock() {
  Block block;
  block.gr = {"#", "gray", ""};
  block.explored = false;
  block.opaque = true;
  block.solid = true;
  block.enterable = false;
  block.name = "stone";
  return block;
}

Block BuildStoneDownStair() {
  Block block;
  block.gr = {">", "gray", ""};
  block.explored = false;
  block.opaque = true;
  block.solid = false;
  block.enterable = true;
  block.name = "stone down stairs";
  block.blockcomponents.push_back(std::shared_ptr<BlockComponent>(new Stair(ZDirection::down)));
  return block;
}

Block BuildStoneUpStair() {
  Block block;
  block.gr = {"<", "gray", ""};
  block.explored = false;
  block.opaque = true;
  block.solid = false;
  block.enterable = true;
  block.name = "stone up stairs";
  block.blockcomponents.push_back(std::shared_ptr<BlockComponent>(new Stair(ZDirection::up)));
  return block;
}

Block BuildSandstoneBlock() {
  Block block;
  block.gr = {"#", "brown", ""};
  block.explored = false;
  block.opaque = true;
  block.solid = true;
  block.enterable = false;
  block.name = "sandstone";
  return block;
}

Block BuildGrassBlock() {
  Block block;
  block.gr = {",", "96,128,56", ""};
  block.explored = false;
  block.opaque = false;
  block.solid = false;
  block.enterable = false;
  block.name = "grass";
  block.blockcomponents.push_back(std::shared_ptr<BlockComponent>(new Plant(3,4,25,0,{',','\'','\"'})));
  return block;
}

Block BuildHlGrassBlock() {
  Block block;
  block.gr = {",", "156,146,122", ""};
  block.explored = false;
  block.opaque = false;
  block.solid = false;
  block.enterable = false;
  block.name = "highland grass";
  block.blockcomponents.push_back(std::shared_ptr<BlockComponent>(new Plant(3,4,25,0,{',','\'','`'})));
  return block;
}

Block BuildTree() {
  Block block;
  block.gr = {",", "176,146,122", ""};
  block.explored = false;
  block.opaque = true;
  block.solid = true;
  block.enterable = false;
  block.name = "tree";
  block.blockcomponents.push_back(std::shared_ptr<BlockComponent>(new Plant(6,4,50,2,{'.',',',':',';','o','O'})));
  return block;
}
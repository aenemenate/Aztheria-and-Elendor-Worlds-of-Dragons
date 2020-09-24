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
  block.blockcomponents.push_back(new Stair(ZDirection::down));
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
  block.blockcomponents.push_back(new Stair(ZDirection::up));
  return block;
}
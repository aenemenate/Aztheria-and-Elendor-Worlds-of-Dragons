#pragma once
#include "../ecs/entity.h"
#include "../map_objects/block.h"
#include "../util/file_helper.h"

#include <rapidxml.hpp>
#include <string>
#include <vector>


using namespace rapidxml;

namespace XmlParser {
  std::vector<Block> GetBlocksFromXml(std::string filepath);
  std::vector<Entity> GetEntitiesFromXml(std::string filepath);
}

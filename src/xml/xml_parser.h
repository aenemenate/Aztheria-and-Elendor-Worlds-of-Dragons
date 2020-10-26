#pragma once
#include "../ecs/entity.h"
#include "../util/file_helper.h"

#include <rapidxml.hpp>
#include <string>
#include <vector>


using namespace rapidxml;

namespace XmlParser {
  std::vector<Entity> GetEntitiesFromXml(std::string filepath);
}

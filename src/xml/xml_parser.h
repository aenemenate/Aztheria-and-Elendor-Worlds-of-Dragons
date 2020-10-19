#pragma once

#include <rapidxml.hpp>
#include <vector>
#include "../ecs/entity.h"

std::vector<Entity> GetEntitiesFromFile(std::string path);
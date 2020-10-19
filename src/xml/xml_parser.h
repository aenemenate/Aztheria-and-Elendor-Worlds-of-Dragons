#pragma once

#include <rapidxml.hpp>
#include <string>
#include <vector>
#include <memory>
#include "../ecs/entity.h"


using namespace rapidxml;

namespace XmlParser {
  struct XmlDoc {
    xml_document<> doc;
    std::string text;
  };
/* convert file contents to xml document */
  std::shared_ptr<XmlDoc> GetXmlFromFile(std::string filepath);
/* convert xml document to a list of entities */
  std::vector<Entity> GetEntitiesFromXml(std::shared_ptr<XmlDoc> xmldoc);
}
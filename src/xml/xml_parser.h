#pragma once

#include <rapidxml.hpp>
#include <string>
#include <vector>
#include <memory>
#include "../ecs/entity.h"


using namespace rapidxml;

namespace XmlParser {
/* rapidxml uses a specific method for speed which requires that the source
 * text always exists, this is why we must specify a wrapper struct which also
 * hold the text
 */
  struct XmlDoc {
    xml_document<> doc;
    std::string text;
  };
/* convert file contents to xml document */
  std::shared_ptr<XmlDoc> GetXmlFromFile(std::string filepath);
/* convert xml document to a list of entities */
  std::vector<Entity> GetEntitiesFromXml(std::shared_ptr<XmlDoc> xmldoc);
}
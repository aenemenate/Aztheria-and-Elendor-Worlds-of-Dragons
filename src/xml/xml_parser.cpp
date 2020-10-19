#include "xml_parser.h"
#include "../util/file_helper.h"

using namespace XmlParser;

std::shared_ptr<XmlDoc> XmlParser::GetXmlFromFile(std::string filepath) {
  std::shared_ptr<XmlDoc> xmldoc;
  if (FileHelper::file_exists(filepath)) {
    xmldoc->text = FileHelper::get_file_contents(filepath);
    xmldoc->doc.parse<0>(const_cast<char*>(xmldoc->text.c_str()));
  }
  return xmldoc;
}

std::vector<Entity> XmlParser::GetEntitiesFromXml(std::shared_ptr<XmlDoc> xmldoc) {
  std::vector<Entity> entities;
  // cycle through entities
  xml_node<> *node = xmldoc->doc.first_node();
  if (node->first_node()) {
    for (xml_node<> *node = node->first_node();
       node; node = node->next_sibling()) {
      
    }
  }
  return entities;
}
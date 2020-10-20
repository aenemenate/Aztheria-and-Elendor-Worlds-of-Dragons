#include "xml_parser.h"
#include <memory>

#include <bits/stdc++.h>
#include <iostream>

#include <rapidxml_utils.hpp>
#include <rapidxml_print.hpp>

std::shared_ptr<Renderable> renderableFromNode(xml_node<> *node) {
  xml_attribute<> *attr = node->first_attribute();
  std::string ch = std::string(attr->value());
  attr = attr->next_attribute();
  std::string fgcolor = std::string(attr->value());
  attr = attr->next_attribute();
  std::string bgcolor = std::string(attr->value());
  return std::shared_ptr<Renderable>(new Renderable({ch, fgcolor, bgcolor}));
}

std::shared_ptr<Name> nameFromNode(xml_node<> *node) {
  return std::shared_ptr<Name>(new Name(std::string(node->first_attribute()->value())));
}

std::shared_ptr<Fov> fovFromNode(xml_node<> *node) {
  return std::shared_ptr<Fov>(new Fov(std::stoi(std::string(node->first_attribute()->value()))));
}

std::shared_ptr<EntityComponent> componentFromNode(xml_node<> *node) {
  if (std::string(node->name()) == "renderable") {
    return renderableFromNode(node);
  }
  if (std::string(node->name()) == "name") {
    return nameFromNode(node);
  }
  if (std::string(node->name()) == "fov") {
    return fovFromNode(node);
  }
  return nullptr;
}

Entity entityFromNode(xml_node<> *node) {
  Entity entity;
  for (xml_node<> *t_node = node->first_node();
       t_node; t_node = t_node->next_sibling()) {
    entity.components.push_back(componentFromNode(t_node));
  }
  return entity;
}

std::vector<Entity> XmlParser::GetEntitiesFromXml(std::string filepath) {
  char *text = new char[4096];
  strcpy(text, FileHelper::get_file_contents(filepath).c_str());
  xml_document<> doc;
  doc.parse<0>(text);
  std::vector<Entity> entities;
  // cycle through entities
  xml_node<> *node = doc.first_node();
  if (node->first_node()) {
    for (xml_node<> *t_node = node->first_node();
         t_node; t_node = t_node->next_sibling()) {
      Entity entity = entityFromNode(t_node);
      entities.push_back(entity);
      std::cout << entity.components.back()->ID << std::endl;
    }
  }
  return entities;
}

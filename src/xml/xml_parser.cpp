#include "xml_parser.h"

#include <memory>
#include <iostream>
#include <rapidxml_print.hpp>

std::shared_ptr<Renderable> renderableFromNode(xml_node<> *node) {
  xml_attribute<> *attr = node->first_attribute();
  std::string ch = std::string(attr->value());
  attr = attr->next_attribute();
  std::string fgcolor = std::string(attr->value());
  attr = attr->next_attribute();
  std::string bgcolor = std::string(attr->value());
  return std::make_shared<Renderable>(Renderable({ch, fgcolor, bgcolor}));
}

std::shared_ptr<Name> nameFromNode(xml_node<> *node) {
  return std::make_shared<Name>(Name(std::string(node->first_attribute()->value())));
}

std::shared_ptr<Fov> fovFromNode(xml_node<> *node) {
  return std::make_shared<Fov>(Fov(std::stoi(std::string(node->first_attribute("viewradius")->value()))));
}

std::shared_ptr<AnimalAi> animalAiFromNode(xml_node<> *node) {
  return std::make_shared<AnimalAi>(AnimalAi((BiomeType)std::stoi(std::string(node->first_attribute("biome")->value()))));
}

std::shared_ptr<MonsterAi> monsterAiFromNode(xml_node<> *node) {
  return std::make_shared<MonsterAi>(MonsterAi(std::string(node->first_attribute("type")->value())));
}

std::shared_ptr<Stats> bodyFromNode(xml_node<> *node) {
  int atk, def, spd, hp;
  atk = std::stoi(std::string(node->first_attribute("atk")->value()));
  def = std::stoi(std::string(node->first_attribute("def")->value()));
  spd = std::stoi(std::string(node->first_attribute("spd")->value()));
  hp = std::stoi(std::string(node->first_attribute("hp")->value()));
  return std::make_shared<Stats>(Stats(atk, def, spd, hp));
}

std::shared_ptr<EntityComponent> entityComponentFromNode(xml_node<> *node) {
  if (std::string(node->name()) == "renderable") {
    return renderableFromNode(node);
  }
  if (std::string(node->name()) == "name") {
    return nameFromNode(node);
  }
  if (std::string(node->name()) == "fov") {
    return fovFromNode(node);
  }
  if (std::string(node->name()) == "animalai") {
    return animalAiFromNode(node);
  }
  if (std::string(node->name()) == "monsterai") {
    return monsterAiFromNode(node);
  }
  if (std::string(node->name()) == "stats") {
    return bodyFromNode(node);
  }
  return nullptr;
}

Entity entityFromNode(xml_node<> *node) {
  Entity entity;
  for (xml_node<> *t_node = node->first_node();
       t_node; t_node = t_node->next_sibling()) {
    entity.components.push_back(entityComponentFromNode(t_node));
  }
  return entity;
}

std::vector<Entity> XmlParser::GetEntitiesFromXml(std::string filepath) {
  char *text = new char[4096*2];
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
    }
  }
  delete text;
  return entities;
}

std::shared_ptr<Plant> plantFromNode(xml_node<> *node) {
  int spawn_chance =	std::stoi(std::string(node->first_node("spawn_chance")->first_attribute()->value()));
  int biome =		std::stoi(std::string(node->first_node("biome")->first_attribute()->value()));
  std::string seed_tile=std::string(node->first_node("seed_tile")->first_attribute()->value());
  int tickspergrowth =	std::stoi(std::string(node->first_node("tickspergrowth")->first_attribute()->value()));
  int seedradius =	std::stoi(std::string(node->first_node("seedradius")->first_attribute()->value()));
  int required_space =	std::stoi(std::string(node->first_node("required_space")->first_attribute()->value()));
  std::vector<char> stages;
  for (xml_node<> *t_node = node->first_node("stages")->first_node();
       t_node; t_node = t_node->next_sibling()) {
    std::string value = std::string(t_node->first_attribute()->value());
    stages.push_back(value[0]);
  }
/* Plant( int spawn_chance, int biome, std::string seed_tile, int tickstostage, int seedradius, int required_space, std::vector<char> stages ) -> Plant */
  return std::make_shared<Plant>(Plant(spawn_chance, biome, seed_tile, tickspergrowth, seedradius, required_space, stages));
}

std::shared_ptr<BlockComponent> blockComponentFromNode(xml_node<> *node) {
  if (std::string(node->name()) == "plantComponent")
    return plantFromNode(node);
  return nullptr;
}

Block blockFromNode(xml_node<> *node) {
  Block block;
  block.explored = false;
/* base: 
 * get ch, fg and bk color
 * get opacity / solidity / enterable
 * get name
 */
  block.gr.ch 		= 	    std::string(node->first_attribute(	"ch"		)->value());
  block.gr.fgcolor 	= 	    std::string(node->first_attribute(	"fgcolor"	)->value());
  block.gr.bgcolor 	= 	    std::string(node->first_attribute(	"bkcolor"	)->value());
  block.opaque 		= std::stoi(std::string(node->first_attribute(	"opaque"	)->value()));
  block.solid 		= std::stoi(std::string(node->first_attribute(	"solid"		)->value()));
  block.enterable	= std::stoi(std::string(node->first_attribute(	"enterable"	)->value()));
  block.name 		= 	    std::string(node->first_attribute(	"name"		)->value());
/* get blockcomponents: */
  for (xml_node<> *t_node = node->first_node();
       t_node; t_node = t_node->next_sibling()) {
    block.blockcomponents.push_back(blockComponentFromNode(t_node));
  }
  return block;
}

std::vector<Block> XmlParser::GetBlocksFromXml(std::string filepath) {
  char *text = new char[4096*2];
  strcpy(text, FileHelper::get_file_contents(filepath).c_str());
  xml_document<> doc;
  doc.parse<0>(text);
  std::vector<Block> blocks;
  xml_node<> *node = doc.first_node();
  if (node->first_node())
    for (xml_node<> *t_node = node->first_node();
         t_node; t_node = t_node->next_sibling()) {
      Block block = blockFromNode(t_node);
      blocks.push_back(block);
    }
  delete text;
  return blocks;
}

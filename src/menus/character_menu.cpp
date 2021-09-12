#include "character_menu.h"
#include "../game.h"
#include "../draw_funcs.h"
#include "../input_funcs.h"
#include "../world/world.h"
#include "../map/area.h"
#include "../ecs/entity.h"

int currentlySelectedEntity = 0;

void CharacterMenu::CustomDraw(Game* game) {
  std::shared_ptr<Stats> selStats = dynamic_pointer_cast<Stats>(game->world->entities[currentlySelectedEntity].GetComponent(EC_STATS_ID));
  std::shared_ptr<Class> selClass = dynamic_pointer_cast<Class>(game->world->entities[currentlySelectedEntity].GetComponent(EC_CLASS_ID));
  PrintGraphic(xpos, ypos+1,{"ATTRIBUTES:", "white", "black"});
  for (int i = 0; i < selStats->attributes.size(); ++i) {
    PrintGraphic(xpos, ypos + 2 + i, {NameFromAttribute((Attribute)i), "220,220,220", "black"});
    PrintGraphic(xpos+width-to_string(selStats->attributes[(Attribute)i]).length(), ypos + 2 + i, 
		{to_string(selStats->attributes[(Attribute)i]), "220,220,220", "black"});
  }
  PrintGraphic(xpos, ypos+2+selStats->attributes.size(),{"SKILLS:", "white", "black"});
  for (int i = 0; i < selStats->skills.size(); ++i) {
    std::string bg_color = "black";
    if (std::count(selClass->majorSkills.begin(), selClass->majorSkills.end(), (Skill)i) > 0)
      bg_color = "blue";
    if (std::count(selClass->minorSkills.begin(), selClass->minorSkills.end(), (Skill)i) > 0)
      bg_color = "dark yellow";
    PrintGraphic(xpos, ypos + 2 + selStats->attributes.size() + 1 + i, {NameFromSkill((Skill)i), "220,220,220", bg_color});
    PrintGraphic(xpos+width-to_string(selStats->skills[(Skill)i]).length(), ypos + 2 + selStats->attributes.size() + 1 + i, 
		{to_string(selStats->skills[(Skill)i]), "220,220,220", "black"});
  }
}

void CharacterMenu::CustomUpdate(Game* game) {
}

void CharacterMenu::SetSelectedEntity(int selectedEntity) {
  currentlySelectedEntity = selectedEntity;
}
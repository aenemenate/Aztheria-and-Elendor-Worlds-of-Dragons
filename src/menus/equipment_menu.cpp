#include "equipment_menu.h"
#include "../game.h"
#include "../draw_funcs.h"
#include "../input_funcs.h"
#include "../world/world.h"
#include "../map/area.h"
#include "../ecs/entity.h"

int selectedEquipmentIndex = -1;

void EquipmentMenu::CustomDraw(Game* game) {
// draw items
  std::shared_ptr<Equipment> equipment = dynamic_pointer_cast<Equipment>(game->world->entities[0].GetComponent(EC_EQUIPMENT_ID));
  selectedEquipmentIndex = -1;
  for (int i = 0; i < equipment->bodyParts.size(); ++i) {
    std::string fg_color = "white";
    if (TerminalGetMouseX() >= xpos && TerminalGetMouseX() < xpos + width
    && (TerminalGetMouseY() == ypos + 1 + i * 2 || TerminalGetMouseY() == ypos + 1 + i * 2 + 1)) {
        fg_color = "green";
        selectedEquipmentIndex = i;
    }
    BodyPart bodyPart = equipment->bodyParts[i];
    std::string equipmentName;
    if (bodyPart.equippedEntity == nullptr)
      equipmentName = "empty";
    else {
      equipmentName = dynamic_pointer_cast<Name>(bodyPart.equippedEntity->GetComponent(EC_NAME_ID))->name;
    }
    PrintGraphic(xpos, ypos + 1 + i * 2, {equipment->bodyParts[i].name + ":", fg_color, "black"});
    PrintGraphic(xpos, ypos + 1 + i * 2 + 1, {equipmentName, fg_color, "black"});
  }
}

void EquipmentMenu::CustomUpdate(Game* game) {
  if (TerminalGetKey() == MTK_MOUSE_LEFT)
    if (selectedEquipmentIndex != -1)
      game->world->entities[0].actions.push_back(std::shared_ptr<EntityAction>(new Unequip(selectedEquipmentIndex))); 
}
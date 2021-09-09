#include "inventory_menu.h"
#include "../game.h"
#include "../draw_funcs.h"
#include "../input_funcs.h"
#include "../world/world.h"
#include "../map/area.h"
#include "../ecs/entity.h"

int selectedIndex = -1;

void InventoryMenu::CustomDraw(Game* game) {
// draw items
  std::shared_ptr<Inventory> inventory = dynamic_pointer_cast<Inventory>(game->world->entities[0].GetComponent(EC_INVENTORY_ID));
  selectedIndex = -1;
  for (int i = 0; i < inventory->inventory.size(); ++i) {
    std::string fg_color = "white";
    std::shared_ptr<Name> item_name = dynamic_pointer_cast<Name>(inventory->inventory[i]->GetComponent(EC_NAME_ID));
    if (TerminalGetMouseX() >= xpos && TerminalGetMouseX() < xpos + width && TerminalGetMouseY() == ypos + 1 + i) {
        fg_color = "green";
	selectedIndex = i;
    }
    PrintGraphic(xpos, ypos + 1 + i, {item_name->name, fg_color, "black"});
  }
}

void InventoryMenu::CustomUpdate(Game* game) {
  std::shared_ptr<Inventory> inventory = dynamic_pointer_cast<Inventory>(game->world->entities[0].GetComponent(EC_INVENTORY_ID));
  if (TerminalGetKey() == MTK_MOUSE_LEFT)
    if (selectedIndex != -1)
      game->world->entities[0].actions.push_back(std::shared_ptr<EntityAction>(new UseItem(selectedIndex)));      
}
#include "loot_menu.h"
#include "../game.h"
#include "../draw_funcs.h"
#include "../input_funcs.h"
#include "../world/world.h"
#include "../map/area.h"
#include "../ecs/entity.h"

LootMenu LootMenu::lootMenu;
int currentlySelEntity = -1;
int selIndex = -1;

void LootMenu::CustomDraw(Game* game) {
// draw items
  if (currentlySelEntity == -1)
    return;
  std::shared_ptr<Inventory> inventory = dynamic_pointer_cast<Inventory>(game->world->entities[currentlySelEntity].GetComponent(EC_INVENTORY_ID));
  selIndex = -1;
  for (int i = 0; i < inventory->inventory.size(); ++i) {
    std::string fg_color = "white";
    std::shared_ptr<Name> item_name = dynamic_pointer_cast<Name>(game->world->entities[inventory->inventory[i]].GetComponent(EC_NAME_ID));
    if (TerminalGetMouseX() >= xpos && TerminalGetMouseX() < xpos + width && TerminalGetMouseY() == ypos + 1 + i) {
        fg_color = "green";
	selIndex = i;
    }
    PrintGraphic(xpos, ypos + 1 + i, {item_name->name, fg_color, "black"});
  }
}

void LootMenu::CustomUpdate(Game* game) {
  if (TerminalGetKey() == MTK_MOUSE_LEFT)
    if (selIndex != -1)
      game->world->entities[0].actions.push_back(std::shared_ptr<EntityAction>(
						  new GetFromBody(selIndex, &(game->world->entities[currentlySelEntity])))
						);     
}

void LootMenu::SetSelectedEntity(int selectedEntity) {
  currentlySelEntity = selectedEntity;
}
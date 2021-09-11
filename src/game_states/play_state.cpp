#include "play_state.h"

#include "../draw_funcs.h"
#include "../base.h"
#include "../input_funcs.h"
#include "../ecs/entity.h"
#include "../world/world.h"
#include "../map/area.h"
#include "../game_fio.h"
#include "../pathfinder.h"

#include "../menus/map_menu.h"
#include "../menus/inventory_menu.h"
#include "../menus/equipment_menu.h"

#include <algorithm>

MapMenu map_menu;
InventoryMenu inventory_menu;
EquipmentMenu equipment_menu;
StatusPanel status_panel;
vector<Button> pmenu_buttons;
int menu_caret = 0;
bool paused = false;
std::vector<Point> player_path;

PlayState PlayState::playState;

void StopPlaying(Game *game) { game->PopState(); game->CleanupResources(); }

void SaveGame(Game *game) { GameFIO::SaveWorld(game); StopPlaying(game); }

void PlayState::Init(Game *game) {
  int term_width  = GetTermWidth(), 
      term_height = GetTermHeight();
  status_panel = StatusPanel(20);
  pmenu_buttons.resize(0);
  pmenu_buttons.push_back(Button(term_width/2-4,term_height/2, "save & quit", SaveGame));
  pmenu_buttons.push_back(Button(term_width/2-3,term_height/2+2, "just quit", StopPlaying));
  menu_caret = 0;
  map_menu = MapMenu(max(0, term_width/2 - game->world->width/2),
			max(0, term_height/2 - (game->world->height/2+1)), 
			game->world->width, game->world->height);
  map_menu.SetShow(false);
  inventory_menu = InventoryMenu(max(0, term_width/2 - 20/2),
			max(0, term_height/2 - (26/2+1)), 
			20, 26);
  inventory_menu.SetShow(false);
  equipment_menu = EquipmentMenu(max(0, term_width/2 - 20/2),
			max(0, term_height/2 - (12/2+1)), 
			20, 12);
  equipment_menu.SetShow(false);
}

void PlayState::Cleanup() {
  menu_caret = 0;
  paused = false;
  pmenu_buttons.clear();
}

void PlayState::Resize(Game *game) {
  Init(game);
}

void PlayState::HandleEvents(Game *game) {
  int xsign = 0, ysign = 0;
  if (TerminalWasResized())
    this->Init(game);
  // if none of menus are showing
  if (!paused && !map_menu.GetShow() && !inventory_menu.GetShow() && !equipment_menu.GetShow()) {
    switch (TerminalGetKey()) {
      case MTK_M:
        map_menu.SetShow(true);
        break;
      case MTK_I:
        inventory_menu.SetShow(true);
        break;
      case MTK_E:
        equipment_menu.SetShow(true);
        break;
      case MTK_ESCAPE:
        paused = true;
        break;
    }
// handle mouse input for moving the player, done here because it is dependent on gui elements
    Entity *plyr = &(game->world->entities[0]);
    Position plyr_pos = (dynamic_pointer_cast<EntPosition>(plyr->GetComponent(EC_POSITION_ID)))->position;
    int term_width = GetTermWidth(), map_term_width = status_panel.start_x(term_width), term_height = GetTermHeight();
    int startx = min(max(0,game->world->GetArea(0,0)->width-map_term_width),max(0, plyr_pos.x - map_term_width/2));
    int starty = min(max(0,game->world->GetArea(0,0)->height-term_height), max(0, plyr_pos.y - term_height/2));  
    if (TerminalGetKey() == (MTK_MOUSE_RIGHT|MTK_KEY_RELEASED) && TerminalGetMouseX() < map_term_width) {
      player_path = Pathfinder::GetPath(game->world, plyr_pos.wx, plyr_pos.wy, plyr_pos.z, plyr_pos.x, plyr_pos.y, 
                                        TerminalGetMouseX()+startx, TerminalGetMouseY()+starty);
      TerminalSetInputBlockMode(false);
    }
    if (TerminalGetKey() == (MTK_MOUSE_LEFT|MTK_KEY_RELEASED)) {
      if (TerminalGetMouseX()+startx == plyr_pos.x && TerminalGetMouseY()+starty == plyr_pos.y) {
        if (game->world->GetArea(plyr_pos.wx, plyr_pos.wy)->GetBlock(plyr_pos.x, plyr_pos.y, plyr_pos.z)->enterable)
          game->world->GetArea(plyr_pos.wx, plyr_pos.wy)->GetBlock(plyr_pos.x, plyr_pos.y, plyr_pos.z)->Activate(plyr, game->world);
        else {
          if (plyr_pos.x == 0)
            game->world->entities[0].actions.push_back(std::shared_ptr<EntityAction>(new Move(-1,0,0)));
          else if (plyr_pos.y == 0)
            game->world->entities[0].actions.push_back(std::shared_ptr<EntityAction>(new Move(0,-1,0)));
          if (plyr_pos.x == game->world->GetArea(0,0)->width - 1)
            game->world->entities[0].actions.push_back(std::shared_ptr<EntityAction>(new Move(1,0,0)));
          else if (plyr_pos.y == game->world->GetArea(0,0)->height - 1)
            game->world->entities[0].actions.push_back(std::shared_ptr<EntityAction>(new Move(0,1,0)));
        }
      }
      else if (TerminalGetMouseX()+startx == plyr_pos.x + 1 || TerminalGetMouseX()+startx == plyr_pos.x - 1
              || TerminalGetMouseY()+starty == plyr_pos.y + 1 || TerminalGetMouseY()+starty == plyr_pos.y - 1) {
	if (game->world->GetArea(plyr_pos.wx, plyr_pos.wy)->GetEntity(
		TerminalGetMouseX()+startx, TerminalGetMouseY()+starty, plyr_pos.z) != -1) {
          game->world->entities[0].actions.push_back(std::shared_ptr<EntityAction>(new Move(TerminalGetMouseX()+startx - plyr_pos.x,
											    TerminalGetMouseY()+starty - plyr_pos.y,
											    0)));
	}
      }
    }
  }
  else if (paused)
    switch (TerminalGetKey()) {
      case MTK_KP_8:
      case MTK_UP:
        menu_caret = (menu_caret > 0) ? --menu_caret : menu_caret;
        break;
      case MTK_KP_2:
      case MTK_DOWN:
        menu_caret = (menu_caret < pmenu_buttons.size() - 1) ? ++menu_caret : menu_caret;
        break;
      case MTK_KP_ENTER:
      case MTK_ENTER:
        pmenu_buttons[menu_caret].Activate(game);
        TerminalClearKey();
        break;
      case MTK_ESCAPE:
        paused = false;
        break;
    }
  else if (map_menu.GetShow() || inventory_menu.GetShow() || equipment_menu.GetShow()) {
    switch (TerminalGetKey()) {
      case MTK_ESCAPE:
        inventory_menu.SetShow(false);
        map_menu.SetShow(false);
        equipment_menu.SetShow(false);
        break;
      case MTK_M:
        map_menu.SetShow(!map_menu.GetShow());
	break;
      case MTK_I:
        inventory_menu.SetShow(!inventory_menu.GetShow());
	break;
      case MTK_E:
        equipment_menu.SetShow(!equipment_menu.GetShow());
	break;
    }
  }
}

void PlayState::Update(Game *game) {
  status_panel.Update(game);
  if (paused)
    for (int b=0;b<pmenu_buttons.size();b++)
      pmenu_buttons[b].Update(game);
  else {
    Entity *plyr = &(game->world->entities[0]);
    Position plyr_pos = (dynamic_pointer_cast<EntPosition>(plyr->GetComponent(EC_POSITION_ID)))->position;
    if (player_path.size() > 0) {
      plyr->actions.clear();
      plyr->actions.push_back(std::shared_ptr<EntityAction>(new Move(player_path.back().x - plyr_pos.x,
                                                                     player_path.back().y - plyr_pos.y,0)));
      player_path.pop_back();
      if (player_path.size() == 0)
        TerminalSetInputBlockMode(true);
    }
    game->world->Update(game);
  }
  map_menu.Update(game);
  inventory_menu.Update(game);
  equipment_menu.Update(game);
}

void PlayState::Draw(Game *game) {
  Entity *plyr = &(game->world->entities[0]);
  Position plyr_pos = (dynamic_pointer_cast<EntPosition>(plyr->GetComponent(EC_POSITION_ID)))->position;
// set values
  int curwx = plyr_pos.wx,
      curwy = plyr_pos.wy;
  Area *area = game->world->GetArea(curwx, curwy);
  int term_width = GetTermWidth(),
      map_term_width = status_panel.start_x(term_width),
      term_height = GetTermHeight();
  int startx = min(max(0,area->width-map_term_width),max(0, plyr_pos.x - map_term_width/2));
  int starty = min(max(0,area->height-term_height), max(0, plyr_pos.y - term_height/2));
// draw status panel, msg console
  status_panel.Draw(game, startx, starty);
// draw map
  for (int i = startx; i < area->width && i-startx < map_term_width; i++)
    for (int j = starty; j < area->height && j-starty < term_height; j++) {
      Tile *tile = area->GetTile(i, j, plyr_pos.z);
      Block *block = area->GetBlock(i, j, plyr_pos.z);
      if (tile->explored) {
        if (block->gr.ch != " ")
          PrintGraphic(i - startx, j - starty, {block->gr.ch,"darker gray", "black"});
        else
          PrintGraphic(i - startx, j - starty, {tile->gr.ch,"darker gray", "black"});
      }
    }
// draw visible points
  std::shared_ptr<Fov> fov_c = dynamic_pointer_cast<Fov>(plyr->GetComponent(EC_FOV_ID));
  for (int vp = 0; vp < fov_c->visiblepoints.size(); vp++) {
    Position point = fov_c->visiblepoints[vp];
    if (point.x - startx < map_term_width
    &&  point.y - starty < term_height
    &&  point.x - startx >= 0
    &&  point.y - starty >= 0) {
      Tile *tile = area->GetTile(point.x, point.y, point.z);
      Block *block = area->GetBlock(point.x, point.y, point.z);
      Entity* entity = nullptr;
      if (area->GetEntity(point.x, point.y, point.z) != -1)
        entity = &(game->world->entities[area->GetEntity(point.x, point.y, point.z)]);
      if (entity == nullptr) {
        if (block->gr.ch != " ") {
	  std::string fg_color = ((tile->isSnowy) ? "white" : block->gr.fgcolor);
          PrintGraphic(point.x - startx, point.y - starty, { block->gr.ch, fg_color, tile->gr.bgcolor });
	}
        else {
	  std::string fg_color = ((tile->isSnowy) ? "white" : tile->gr.fgcolor);
          PrintGraphic(point.x - startx, point.y - starty, {tile->gr.ch, fg_color, tile->gr.bgcolor});
	}
      }
      else {
        std::shared_ptr<Renderable> rend_c = dynamic_pointer_cast<Renderable>(entity->GetComponent(EC_RENDERABLE_ID));
        PrintGraphic(point.x - startx, point.y - starty, rend_c->graphic);
      }
    }
  }
// draw player
  Point plyr_sc_pos = { plyr_pos.x - startx, plyr_pos.y - starty };
  int plyr_z = plyr_pos.z;
  std::shared_ptr<Renderable> rend_c = dynamic_pointer_cast<Renderable>(plyr->GetComponent(EC_RENDERABLE_ID));
  PrintGraphic(plyr_sc_pos.x, plyr_sc_pos.y, rend_c->graphic);
// draw menu if necessary
  if (paused) {
    ClearTerminalArea(term_width/2-7,term_height/2-2,15,6);
    for (int b=0;b<pmenu_buttons.size();b++)
      pmenu_buttons[b].Render();
    DrawBorder({term_width/2-7,term_width/2+8,term_height/2-2,term_height/2+4}, "white", "black");
    PrintGraphic(pmenu_buttons[menu_caret].GetX()-2, term_height/2 + menu_caret*2, {">", "white", "black"});
  }
  else {
  // draw path if necessary
    if (TerminalRightMouseHeld() && TerminalGetMouseX() < map_term_width
    && TerminalGetMouseX() >= 0 && TerminalGetMouseY() >= 0
    && TerminalGetMouseY() < term_height) {
      std::vector<Point> path;
      path = Pathfinder::GetPath(game->world, plyr_pos.wx, plyr_pos.wy, plyr_pos.z, plyr_pos.x, plyr_pos.y,
                                 TerminalGetMouseX()+startx, TerminalGetMouseY()+starty);
      for (auto point : path) {
        int x = point.x - startx, y = point.y - starty;
        if (x < map_term_width && x >= 0 && y < term_height && y >= 0) {
          PrintGraphic(x, y, {"", "", "blue"});
        }
      }
    }
  // draw ui overlays related to clickable tiles
    if (TerminalGetMouseX() == plyr_sc_pos.x && TerminalGetMouseY() == plyr_sc_pos.y
    && area->GetBlock(plyr_sc_pos.x+startx, plyr_sc_pos.y+starty,plyr_z)->enterable) {
      PrintGraphic(plyr_sc_pos.x, plyr_sc_pos.y, {"", "", "blue"});
    }
    if (((TerminalGetMouseX() == plyr_sc_pos.x - 1 || TerminalGetMouseX() == plyr_sc_pos.x + 1 || TerminalGetMouseX() == plyr_sc_pos.x)
    && (TerminalGetMouseY() == plyr_sc_pos.y - 1 || TerminalGetMouseY() == plyr_sc_pos.y + 1 ||  TerminalGetMouseY() == plyr_sc_pos.y)
    && !(TerminalGetMouseX() == plyr_sc_pos.x && TerminalGetMouseY() == plyr_sc_pos.y))
    && area->GetEntity(TerminalGetMouseX()+startx, TerminalGetMouseY()+starty,plyr_z) != -1) {
      PrintGraphic(TerminalGetMouseX(), TerminalGetMouseY(), {"", "", "blue"});
    }
  }
  map_menu.Draw(game);
  inventory_menu.Draw(game);
  equipment_menu.Draw(game);
  game->world->msgConsole.PrintLines();
}

#include "play_state.h"

#include "../../include/BearLibTerminal.h"
#include "../draw_funcs.h"
#include "../entity.h"
#include "../world.h"
#include "../map/area.h"
#include "../game_fio.h"
#include "../pathfinder.h"

#include "../menus/map_menu.h"

#include <algorithm>

PlayState PlayState::playState;

MapMenu map_menu;

std::vector<Point> player_path;

void StopPlaying(Game *game)
{
  game->PopState();
}

void SaveGame(Game *game)
{
  GameFIO::SaveWorld(game);
  StopPlaying(game);
}

void PlayState::Init(Game *game)
{
  int term_width  = terminal_state(TK_WIDTH), 
      term_height = terminal_state(TK_HEIGHT);
  Instance()->status_panel = StatusPanel(20);
  pmenu_buttons.resize(0);
  pmenu_buttons.push_back(Button(term_width/2-4,term_height/2, "save & quit", SaveGame));
  pmenu_buttons.push_back(Button(term_width/2-3,term_height/2+2, "just quit", StopPlaying));
  menu_caret = 0;

  map_menu = MapMenu(std::max(0, term_width/2 - game->world->width/2), std::max(0, term_height/2 - (game->world->height/2+1)), game->world->width, game->world->height);
  map_menu.SetShow(false);
}

void PlayState::Cleanup()
{
  menu_caret = 0;
  paused = false;
  pmenu_buttons.clear();
}

void PlayState::HandleEvents(Game *game)
{
  int xsign = 0, ysign = 0;
  if (terminal_state(TK_EVENT) == TK_RESIZED)
    this->Init(game);
  if (!paused && !map_menu.GetShow()) {
    switch (game->key)
    {
      case TK_KP_8:
      case TK_UP:
        game->world->entities[0].Move(0,-1, game->world); break;
      case TK_KP_9:
        game->world->entities[0].Move(1,-1, game->world); break;
      case TK_KP_6:
      case TK_RIGHT:
        game->world->entities[0].Move(1,0, game->world); break;
      case TK_KP_3:
        game->world->entities[0].Move(1,1, game->world); break;
      case TK_KP_2:
      case TK_DOWN:
        game->world->entities[0].Move(0,1, game->world); break;
      case TK_KP_1:
        game->world->entities[0].Move(-1,1, game->world); break;
      case TK_KP_4:
      case TK_LEFT:
        game->world->entities[0].Move(-1,0, game->world); break;
      case TK_KP_7:
        game->world->entities[0].Move(-1,-1, game->world); break;
      case TK_M:
        map_menu.SetShow(true);
        break;
      case TK_ESCAPE:
        paused = true;
        break;
    }
    int term_width = terminal_state(TK_WIDTH), map_term_width = status_panel.start_x(term_width), term_height = terminal_state(TK_HEIGHT);
    int startx = min(max(0,game->world->GetArea(0,0)->width-map_term_width),max(0, game->world->entities[0].pos.x - map_term_width/2));
    int starty = min(max(0,game->world->GetArea(0,0)->height-term_height), max(0, game->world->entities[0].pos.y - term_height/2));  
    if (game->key == (TK_MOUSE_RIGHT|TK_KEY_RELEASED) && terminal_state(TK_MOUSE_X) < map_term_width) {
      Entity *player = &(game->world->entities[0]);
      player_path = Pathfinder::GetPath(game->world, player->pos.wx, player->pos.wy, player->pos.z, player->pos.x, player->pos.y, terminal_state(TK_MOUSE_X)+startx, terminal_state(TK_MOUSE_Y)+starty);
      game->SetInputBlockMode(false);
    }
    if (game->key == (TK_MOUSE_LEFT|TK_KEY_RELEASED)) {
      Entity *player = &(game->world->entities[0]);
      if (terminal_state(TK_MOUSE_X)+startx == player->pos.x && terminal_state(TK_MOUSE_Y)+starty == player->pos.y) {
        if (player->pos.x == 0)
          player->Move(-1, 0, game->world);
        else if (player->pos.y == 0)
          player->Move(0, -1, game->world);
        else if (player->pos.x == game->world->GetArea(0,0)->width - 1)
          player->Move(1, 0, game->world);
        else if (player->pos.y == game->world->GetArea(0,0)->height - 1)
          player->Move(0, 1, game->world);
      }
    }
  }
  else if (paused)
    switch (game->key)
    {
      case TK_KP_8:
      case TK_UP:
        menu_caret = (menu_caret > 0) ? --menu_caret : menu_caret; 
        break;
      case TK_KP_2:
      case TK_DOWN:
        menu_caret = (menu_caret < pmenu_buttons.size() - 1) ? ++menu_caret : menu_caret;
        break;
      case TK_KP_ENTER:
      case TK_ENTER:
        pmenu_buttons[menu_caret].Activate(game);
        game->key = 0;
        break;
      case TK_ESCAPE:
        paused = false;
        break;
    }
  else if (map_menu.GetShow())
    switch (game->key)
    {
      case TK_ESCAPE:
      case TK_M:
        map_menu.SetShow(false);
        break;
    }
}

void PlayState::Update(Game *game)
{
  status_panel.Update(game);
  if (paused)
    for (int b=0;b<pmenu_buttons.size();b++)
      pmenu_buttons[b].Update(game);
  else
  {
    if (player_path.size() > 0) {
      game->world->entities[0].Move(player_path.back().x - game->world->entities[0].pos.x, player_path.back().y - game->world->entities[0].pos.y, game->world);
      player_path.pop_back();
      if (player_path.size() == 0)
        game->SetInputBlockMode(true);
    }
    game->world->entities[0].Update(game, true);
    for (int e = game->world->entities.size() - 1; e >= 1; --e)
      game->world->entities[e].Update(game, false);
  }
  map_menu.Update(game);
}

void PlayState::Draw(Game *game)
{
// set values
  int curwx = game->world->entities[0].pos.wx, curwy = game->world->entities[0].pos.wy;
  Area *area = game->world->GetArea(curwx, curwy);
  int term_width = terminal_state(TK_WIDTH), map_term_width = status_panel.start_x(term_width), term_height = terminal_state(TK_HEIGHT);
  int startx = min(max(0,area->width-map_term_width),max(0, game->world->entities[0].pos.x - map_term_width/2));
  int starty = min(max(0,area->height-term_height), max(0, game->world->entities[0].pos.y - term_height/2));
// draw status panel
  status_panel.Draw(game);
// draw map
  for (int i = startx; i < area->width && i-startx < map_term_width; i++)
    for (int j = starty; j < area->height && j-starty < term_height; j++) {
      Tile *tile = area->GetTile(i, j, game->world->entities[0].pos.z);
      if (tile->explored)
        PrintCh(i - startx, j - starty, {tile->gset.ch,"darker gray", "black"});
    }
// draw visible points
  for (int vp = 0; vp < game->world->entities[0].visiblepoints.size(); vp++)
  {
    Position point = game->world->entities[0].visiblepoints[vp];
    if (point.x - startx < map_term_width
    &&  point.y - starty < map_term_width
    &&  point.x - startx >= 0
    &&  point.y - starty >= 0)
    {
      Tile *tile = area->GetTile(point.x, point.y, point.z);
      Entity *entity = area->ent_map[point.x * area->width + point.y];
      if (entity == nullptr)
        PrintCh(point.x - startx, point.y - starty, tile->gset);
      else
        PrintCh(point.x - startx, point.y - starty, entity->gset);
    }
  }
// draw player
  PrintCh(game->world->entities[0].pos.x - startx, game->world->entities[0].pos.y - starty, game->world->entities[0].gset);
// draw path if necessary
  if (terminal_state(TK_MOUSE_RIGHT) && terminal_state(TK_MOUSE_X) < map_term_width 
  && terminal_state(TK_MOUSE_X) >= 0 && terminal_state(TK_MOUSE_Y) >= 0 
  && terminal_state(TK_MOUSE_Y) < term_height) {
    std::vector<Point> path;
    Entity *player = &(game->world->entities[0]);
    path = Pathfinder::GetPath(game->world, player->pos.wx, player->pos.z, player->pos.wy, player->pos.x, player->pos.y, 
                               terminal_state(TK_MOUSE_X)+startx, terminal_state(TK_MOUSE_Y)+starty);
    terminal_bkcolor("blue");
    for (auto point : path) {
      int x = point.x - startx, y = point.y - starty;
      terminal_color(terminal_pick_color(x, y));
      terminal_put(x, y, terminal_pick(x, y));
    }
    terminal_bkcolor("black");
  }
// draw menu if necessary
  if (paused) {
    terminal_clear_area(term_width/2-7,term_height/2-2,15,6);
    for (int b=0;b<pmenu_buttons.size();b++)
      pmenu_buttons[b].Render(game);
    DrawBorder(term_width/2-7,term_width/2+8,term_height/2-2,term_height/2+4, "white", "black");
    PrintCh(pmenu_buttons[menu_caret].GetX()-2, term_height/2 + menu_caret*2, {">", "white", "black"});
  }
  map_menu.Draw(game);
}

#include "play_state.h"

#include "../../include/BearLibTerminal.h"
#include "../draw_funcs.h"
#include "../entity.h"
#include "../world.h"
#include "../map.h"

#include "../menu.h"

#include "../game_fio.h"

#include <algorithm>

PlayState PlayState::playState;

MenuMap map_menu;

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
  pmenu_buttons.push_back(Button((term_width - status_panel.get_width())/2-4,term_height/2, "save & quit", SaveGame));
  pmenu_buttons.push_back(Button((term_width - status_panel.get_width())/2-3,term_height/2+2, "just quit", StopPlaying));
  menu_caret = 0;

  map_menu = MenuMap(term_width/2 - 12, term_height/2 - 12, game->world->width, game->world->height);
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
  if (!paused && !map_menu.GetShow())
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
    for (int e = game->world->entities.size() - 1; e >= 0; --e)
      game->world->entities[e].Update(game, e == 0 ? true : false);
  map_menu.Update(game);
}

void PlayState::Draw(Game *game)
{
// set values
  int curwx = game->world->entities[0].pos.wx, curwy = game->world->entities[0].pos.wy;
  Map *map = game->world->GetMap(curwx, curwy);
  int term_width = terminal_state(TK_WIDTH) - status_panel.get_width(), term_height = terminal_state(TK_HEIGHT);
  int startx = min(max(0,map->width-term_width),max(0, game->world->entities[0].pos.x - term_width/2));
  int starty = min(max(0,map->height-term_height), max(0, game->world->entities[0].pos.y - term_height/2));
// draw status panel
  status_panel.Draw(game);
// draw map
  for (int i = startx; i < map->width && i-startx < term_width; i++)
    for (int j = starty; j < map->height && j-starty < term_height; j++)
    {
      Tile *tile = map->GetTile(i, j);
      if (tile->explored)
        PrintCh(i - startx, j - starty, {tile->gset.ch,"darker gray", "black"});
    }
// draw visible points
  for (int vp = 0; vp < game->world->entities[0].visiblepoints.size(); vp++)
  {
    Position point = game->world->entities[0].visiblepoints[vp];
    if (point.x - startx < term_width 
    &&  point.y - starty < term_height
    &&  point.x - startx >= 0
    &&  point.y - starty >= 0)
    {
      Tile *tile = map->GetTile(point.x, point.y);
      Entity *entity = map->ent_map[point.x * map->width + point.y];
      if (entity == nullptr)
        PrintCh(point.x - startx, point.y - starty, tile->gset);
      else
        PrintCh(point.x - startx, point.y - starty, entity->gset);
    }
  }
// draw entities
  PrintCh(game->world->entities[0].pos.x - startx, game->world->entities[0].pos.y - starty, game->world->entities[0].gset);
// draw menu if necessary
  if (paused) {
    terminal_clear_area(term_width/2-7,term_height/2-2,15,6);
    for (int b=0;b<pmenu_buttons.size();b++)
      pmenu_buttons[b].Render(game);
    DrawBorder(term_width/2-7,term_width/2+8,term_height/2-2,term_height/2+4, "white", "black");
    PrintCh(term_width/2-6, term_height/2 + menu_caret*2, {'>', "white", "black"});
  }
  map_menu.Draw(game);
}

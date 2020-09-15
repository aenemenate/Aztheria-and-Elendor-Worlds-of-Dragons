#include "menu.h"

#include "../game.h"

#include "../world.h"
#include "../map.h"
#include "../entity.h"

#include "../base.h"

#include "../draw_funcs.h"

#include "../../include/BearLibTerminal.h"

void Menu::Draw(Game* game)
{
  if (!show)
    return;
  DrawBorder(xpos-1, xpos+width, ypos, ypos+height+1, "white", "black");
  for (int i = 0; i < width; i++) {
    if (title.length() > i)
      PrintCh(xpos + i, ypos, {title.substr(i,1),"white", "black"});
    else
      terminal_print(xpos + i, ypos, "═");
  }
  PrintCh(xpos + width - 1, ypos, {"x","red", "black"});
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++)
        PrintCh(xpos + i, ypos + 1 + j, {" ","white", "black"});
  CustomDraw(game);
}

void Menu::Update(Game* game)
{
  if (!show)
    return;
  if (!dragging && isclicked(game)) {
    click_offset = terminal_state(TK_MOUSE_X) - xpos;
    if (click_offset == width - 1) {
      show = false;
      return;
    }
  }
  if (isclicked(game) || dragging) {
    int mouse_x = terminal_state(TK_MOUSE_X), mouse_y = terminal_state(TK_MOUSE_Y);
    xpos = mouse_x - click_offset; ypos = mouse_y;
    dragging = true;
  }
  if (game->key == (TK_MOUSE_LEFT|TK_KEY_RELEASED))
    dragging = false;
  CustomUpdate(game);
}

bool Menu::isclicked(Game *game)
{
  bool ret_val = false;
  if (game->key == TK_MOUSE_LEFT)
    if (ishovered())
      ret_val = true;
  return ret_val;
}

bool Menu::ishovered()
{
  bool ret_val = false;
  int mouse_x = terminal_state(TK_MOUSE_X);
  if ((terminal_state(TK_MOUSE_Y) == this->ypos) && (mouse_x >= this->xpos) && (mouse_x < this->xpos + width))
    ret_val = true;
  return ret_val;
}

void MenuMap::CustomDraw(Game* game)
{
// draw world map
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++)
    {
      Map *map = game->world->GetMap(i, j);
        Graphic gset = GetMapGraphic(map->biome_type, map->terrain_type);
        PrintCh(xpos + i, ypos + 1 + j, gset);
    }
    PrintCh(xpos + game->world->entities[0].pos.wx, ypos + 1 + game->world->entities[0].pos.wy, game->world->entities[0].gset);
}

void MenuMap::CustomUpdate(Game* game)
{
}
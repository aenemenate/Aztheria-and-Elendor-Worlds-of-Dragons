#pragma once

#include <string>

class Game;

class Menu
{
  int click_offset;
  std::string title;
  bool dragging, show;

  bool isclicked(Game*);
  bool ishovered();
public:
  int xpos, ypos, width, height;
  Menu() {}
  Menu(int xpos, int ypos, int width, int height, std::string title, bool show)
        : xpos(xpos), ypos(ypos), width(width),  
          height(height), title(title) { dragging = false; }
  
  void Draw(Game*);
  void Update(Game*);

  virtual void CustomDraw(Game*) = 0;
  virtual void CustomUpdate(Game*) = 0;

  void SetShow(bool value) { show = value; }
  bool GetShow() { return show; }
};
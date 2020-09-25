#pragma once

class Game;

class StatusPanel {
  int width;
public:
  StatusPanel() { width = 20; }
  StatusPanel(int _width) : width(_width) {}

  void Update(Game*);
  void Draw(Game*, int map_startx, int map_starty);

  int get_width() { return width; }
  int start_x(int term_width) { return term_width - width; }
};
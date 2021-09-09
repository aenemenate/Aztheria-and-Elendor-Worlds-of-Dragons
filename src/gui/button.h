#pragma once

#include <string>
using namespace std;

class Game;

class Button {
  int x, y;
  string text;
  string bgcolor;
  void (*callback_f)(Game*);
  bool ishovered();
public:
  Button() : x(0), y(0), text(""), callback_f(nullptr) {}
  Button(int x, int y, string text, void(*callback_f)(Game*))
    : x(x), y(y), text(text), callback_f(callback_f) { bgcolor = "black"; }

  void Activate(Game *game);
  void Update(Game *game);
  void Render();

  bool isclicked();

  void SetText(string value) { text = value; }
  string GetText() { return text; }

  void SetBgColor(string value) { bgcolor = value; }
  string GetBgColor() { return bgcolor; }

  void SetX(int value) { x = value; }
  int GetX() { return x; }

  void SetY(int value) { y = value; }
  int GetY() { return y; }
};
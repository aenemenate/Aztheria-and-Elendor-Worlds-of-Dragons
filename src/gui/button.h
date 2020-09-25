#pragma once

#include <string>
using namespace std;

class Game;

class Button {
  int x, y;
  string text;
  void (*callback_f)(Game*);
  bool ishovered();
public:
  Button() : x(0), y(0), text(""), callback_f(nullptr) {}
  Button(int x, int y, string text, void(*callback_f)(Game*))
    : x(x), y(y), text(text), callback_f(callback_f) {  }

  void Activate(Game* game);
  bool isclicked(Game* game);

  void Update(Game* game);
  void Render(Game* game);

  void SetText(string value) { text = value; }
  string GetText() { return text; }

  int GetX() { return x; }
};
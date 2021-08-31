#pragma once

#include <string>

using namespace std;

class Graphic;
class Point;
class Rectangle;

void SetTerminal(std::string);

void ClearTerminalArea(int, int, int, int);
void ClearTerminal();
void RefreshTerminal();
void CloseTerminal();

bool TerminalWasResized();

void ChangeLayer(int);
void PrintGraphic(int,int,Graphic);
void DrawBorder(Rectangle, string, string);

int GetTermWidth();
int GetTermHeight();
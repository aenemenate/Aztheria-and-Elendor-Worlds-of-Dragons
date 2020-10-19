#pragma once

#include <string>

using namespace std;

class Graphic;
class Point;
class Rectangle;

void SetTerminal(std::string);

void ClearTerminal();
void RefreshTerminal();

void PrintGraphic(int,int,Graphic);
void DrawBorder(Rectangle, string, string);

int GetTermWidth();
int GetTermHeight();
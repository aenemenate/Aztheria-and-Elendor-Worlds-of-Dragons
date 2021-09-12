#include "draw_funcs.h"
#include "msg_console.h"
#include "base.h"


void MsgConsole::PushLine(std::string value) {
  lines.push_back(Line(value));
}

void MsgConsole::PrintLines() {
  time_t rawtime;
  time(&rawtime);
  int line = 0;
  int start_line = lines.size() - 3;
  if (start_line < 0) start_line = 0;
  for (int i = start_line; i < lines.size(); ++i) {
    if (rawtime - lines[i].rawtime <= 10) {
      PrintGraphic(0, line, {lines[i].value, "191,255,255,255", ""});
      ++line;
    }
  }
}
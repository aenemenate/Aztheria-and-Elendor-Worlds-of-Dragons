#include "draw_funcs.h"
#include "msg_console.h"
#include "base.h"
#include "game.h"

void MsgConsole::PushLine(std::string value) {
  lines.push_back(Line(value));
}

void MsgConsole::PrintLines(Game *game) {
  time_t rawtime;
  time(&rawtime);
  int line = 0;
  Entity *player = &(game->world->entities[0]);
  std::shared_ptr<EntPosition> plyr_pos_c = std::dynamic_pointer_cast<EntPosition>(player->GetComponent(EC_POSITION_ID));
  int y = 0;
  int term_height = GetTermHeight();
  if (plyr_pos_c->position.y < 3)
    y = term_height - 3;
  int start_line = lines.size() - 3;
  if (start_line < 0) start_line = 0;
  for (int i = start_line; i < lines.size(); ++i) {
    if (rawtime - lines[i].rawtime <= 10) {
      PrintGraphic(0, y + line, {lines[i].value, "191,255,255,255", ""});
      ++line;
    }
  }
}
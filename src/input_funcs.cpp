#include "input_funcs.h"
#include <BearLibTerminal.h>


int key = 0;
bool input_block_mode = true;


// Sets

void TerminalSetInputBlockMode(bool value) {
  input_block_mode = value;
}


// Updates

void TerminalReadKey() {
  if (input_block_mode 
  || terminal_has_input())
    key = terminal_read();
  else { key = 0; }
}

void TerminalClearKey() {
  key = 0;
}

// Gets

bool TerminalWasClosed() {
  return key == TK_CLOSE;
}

bool TerminalRightMouseHeld() {
  return terminal_state(TK_MOUSE_RIGHT);
}

int TerminalGetKey() {
  return key;
}

int TerminalGetMouseX() {
  return terminal_state(TK_MOUSE_X);
}

int TerminalGetMouseY() {
  return terminal_state(TK_MOUSE_Y);
}

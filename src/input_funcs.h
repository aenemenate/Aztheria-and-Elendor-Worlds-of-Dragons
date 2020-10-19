#pragma once

#define MTK_A                0x04
#define MTK_B                0x05
#define MTK_C                0x06
#define MTK_D                0x07
#define MTK_E                0x08
#define MTK_F                0x09
#define MTK_G                0x0A
#define MTK_H                0x0B
#define MTK_I                0x0C
#define MTK_J                0x0D
#define MTK_K                0x0E
#define MTK_L                0x0F
#define MTK_M                0x10
#define MTK_N                0x11
#define MTK_O                0x12
#define MTK_P                0x13
#define MTK_Q                0x14
#define MTK_R                0x15
#define MTK_S                0x16
#define MTK_T                0x17
#define MTK_U                0x18
#define MTK_V                0x19
#define MTK_W                0x1A
#define MTK_X                0x1B
#define MTK_Y                0x1C
#define MTK_Z                0x1D
#define MTK_1                0x1E
#define MTK_2                0x1F
#define MTK_3                0x20
#define MTK_4                0x21
#define MTK_5                0x22
#define MTK_6                0x23
#define MTK_7                0x24
#define MTK_8                0x25
#define MTK_9                0x26
#define MTK_0                0x27
#define MTK_RETURN           0x28
#define MTK_ENTER            0x28
#define MTK_ESCAPE           0x29
#define MTK_BACKSPACE        0x2A
#define MTK_TAB              0x2B
#define MTK_SPACE            0x2C
#define MTK_MINUS            0x2D /*  -  */
#define MTK_EQUALS           0x2E /*  =  */
#define MTK_LBRACKET         0x2F /*  [  */
#define MTK_RBRACKET         0x30 /*  ]  */
#define MTK_BACKSLASH        0x31 /*  \  */
#define MTK_SEMICOLON        0x33 /*  ;  */
#define MTK_APOSTROPHE       0x34 /*  \'  */
#define MTK_GRAVE            0x35 /*  `  */
#define MTK_COMMA            0x36 /*  ,  */
#define MTK_PERIOD           0x37 /*  .  */
#define MTK_SLASH            0x38 /*  /  */
#define MTK_F1               0x3A
#define MTK_F2               0x3B
#define MTK_F3               0x3C
#define MTK_F4               0x3D
#define MTK_F5               0x3E
#define MTK_F6               0x3F
#define MTK_F7               0x40
#define MTK_F8               0x41
#define MTK_F9               0x42
#define MTK_F10              0x43
#define MTK_F11              0x44
#define MTK_F12              0x45
#define MTK_PAUSE            0x48 /* Pause/Break */
#define MTK_INSERT           0x49
#define MTK_HOME             0x4A
#define MTK_PAGEUP           0x4B
#define MTK_DELETE           0x4C
#define MTK_END              0x4D
#define MTK_PAGEDOWN         0x4E
#define MTK_RIGHT            0x4F /* Right arrow */
#define MTK_LEFT             0x50 /* Left arrow */
#define MTK_DOWN             0x51 /* Down arrow */
#define MTK_UP               0x52 /* Up arrow */
#define MTK_KP_DIVIDE        0x54 /* '/' on numpad */
#define MTK_KP_MULTIPLY      0x55 /* '*' on numpad */
#define MTK_KP_MINUS         0x56 /* '-' on numpad */
#define MTK_KP_PLUS          0x57 /* '+' on numpad */
#define MTK_KP_ENTER         0x58
#define MTK_KP_1             0x59
#define MTK_KP_2             0x5A
#define MTK_KP_3             0x5B
#define MTK_KP_4             0x5C
#define MTK_KP_5             0x5D
#define MTK_KP_6             0x5E
#define MTK_KP_7             0x5F
#define MTK_KP_8             0x60
#define MTK_KP_9             0x61
#define MTK_KP_0             0x62
#define MTK_KP_PERIOD        0x63 /* '.' on numpad */
#define MTK_SHIFT            0x70
#define MTK_CONTROL          0x71
#define MTK_ALT              0x72

/*
 * Mouse events/states
 */
#define MTK_MOUSE_LEFT       0x80 /* Buttons */
#define MTK_MOUSE_RIGHT      0x81
#define MTK_MOUSE_MIDDLE     0x82
#define MTK_MOUSE_MOVE       0x85 /* Movement event */
#define MTK_MOUSE_SCROLL     0x86 /* Mouse scroll event */
#define MTK_MOUSE_WHEEL      0x8B /* Scroll direction and amount */
#define MTK_MOUSE_CLICKS     0x8C /* Number of consecutive clicks */

/*
 * If key was released instead of pressed, it's code will be OR'ed with TK_KEY_RELEASED:
 * a) pressed 'A': 0x04
 * b) released 'A': 0x04|VK_KEY_RELEASED = 0x104
 */
#define MTK_KEY_RELEASED     0x100



// Sets

void TerminalSetInputBlockMode(bool value);

// Updates

void TerminalReadKey();
void TerminalClearKey();

// Gets

bool TerminalWasClosed();

bool TerminalRightMouseHeld();

int TerminalGetKey();
int TerminalGetMouseX();
int TerminalGetMouseY();

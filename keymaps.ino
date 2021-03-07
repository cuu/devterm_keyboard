#include "devterm.h"
#include "keyboard.h"
#include "keys.h"

#define EMP 0XFFFF

/*
B1 joystick up
B2 joystick down
B3 joystick left
B4 joystick right

B5 joystick A
B6 joystick B
B7 joystick X
B8 joystick Y

B9 left shift
B10 Fn
B11 left Ctrl
B12 Cmd
B13 left Alt
B14 mouse left
B15 mouse mid
B16 mouse right
*/
#define _PRINT_KEY KEY_PRNT_SCRN
#define _PAUSE_KEY KEY_PAUSE
#define _VOLUME_M KEY_VOLUME_UP
#define _VOLUME_P KEY_VOLUME_DOWN
#define _LEFT_SHIFT_KEY KEY_LEFT_SHIFT 
#define _LEFT_CTRL_KEY  KEY_LEFT_CTRL
#define _CMD_KEY        KEY_RIGHT_GUI 
#define _LEFT_ALT       KEY_LEFT_ALT 

enum SKEYS {
  _SELECT_KEY =0xe8,  //Joystick.button(n)
  _START_KEY,          //Joystick.button(n)
  _JOYSTICK_UP, //B1 //Joystick.Y()
  _JOYSTICK_DOWN,    //Joystick.Y()
  _JOYSTICK_LEFT,    //Joystick.X()
  _JOYSTICK_RIGHT,   //Joystick.X()  
  _JOYSTICK_A,       //Joystick.button(1)
  _JOYSTICK_B,       //Joystick.button(2)
  _JOYSTICK_X,       //Joystick.button(3)
  _JOYSTICK_Y,       //Joystick.button(4)
  _FN_KEY,
  _MOUSE_LEFT,    // Mouse.press(1)
  _MOUSE_MID,     // Mouse.press(2)
  _MOUSE_RIGHT,   // Mouse.press(3)

};

#define DEF_LAYER 0x00
#define SHI_LAYER 0x01
#define FN_LAYER  0x02

const uint16_t keyboard_maps[][MATRIX_ROWS][MATRIX_COLS] = {

  [0] = { _SELECT_KEY,_START_KEY,_VOLUME_M,'`','[',']','-','=', \
    '1','2','3','4','5','6','7','8','9','0', \
  KEY_ESC,'q','w','e','r','t','y','u','i','o','p','\\',KEY_BACKSPACE,\
  KEY_TAB,'a','s','d','f','g','h','j','k','l',';','\'', KEY_RETURN ,\
  'z','x','c','v','b','n','m',',','.','/',  KEY_UP_ARROW ,KEY_RIGHT_SHIFT, \
  ' ',KEY_RIGHT_ALT,KEY_RIGHT_CTRL,KEY_LEFT_ARROW,KEY_DOWN_ARROW,KEY_RIGHT_ARROW },

  [1] = { _SELECT_KEY,_START_KEY,_VOLUME_P,'~','{','}','_','+', \
    '!','@','#','$','%','^','&','*','(',')', \
  KEY_ESC,'Q','W','E','R','T','Y','U','I','O','P','|',KEY_BACKSPACE,\
  KEY_TAB,'A','S','D','F','G','H','J','K','L',':','"', KEY_RETURN ,\
  'Z','X','C','V','B','N','M','<','>','?',  KEY_PAGE_UP ,KEY_RIGHT_SHIFT, \
  ' ',KEY_RIGHT_ALT,KEY_RIGHT_CTRL,KEY_HOME,KEY_PAGE_DOWN,KEY_END },

  [2] = { _PRINT_KEY,_PAUSE_KEY,EMP,EMP,EMP,EMP,KEY_F11,KEY_F12, \
    KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10, \
  EMP,EMP,EMP,EMP,EMP,EMP,EMP,EMP,EMP,EMP,EMP,EMP,KEY_DELETE,\
  KEY_CAPS_LOCK,EMP,EMP,EMP,EMP,EMP,EMP,EMP,EMP,EMP,EMP,EMP, EMP ,\
  EMP,EMP,EMP,EMP,EMP,EMP,EMP,EMP,EMP,EMP,  EMP ,EMP, \
  EMP,EMP,EMP,EMP,EMP,EMP },  

  
  
};

const uint16_t keys_maps[KEYS_NUM] = {_JOYSTICK_UP,_JOYSTICK_DOWN, _JOYSTICK_LEFT, \
                                      _JOYSTICK_RIGHT,_JOYSTICK_A,_JOYSTICK_B, \
                                      _JOYSTICK_X,_JOYSTICK_Y,_LEFT_SHIFT_KEY,_FN_KEY,\
                                      _LEFT_CTRL_KEY,_CMD_KEY , _LEFT_ALT,     \
                                      _MOUSE_LEFT,_MOUSE_MID,_MOUSE_RIGHT};


void keyboard_action(DEVTERM*dv,uint8_t row,uint8_t col,uint8_t mode) {

  uint16_t k;
  
  k = keyboard_maps[dv->Keyboard_state.layer][row][col];

  if(k == EMP){
    return;
  }

  switch(k) {
    case _LEFT_SHIFT_KEY:
    case  KEY_RIGHT_SHIFT:
    if(mode == KEY_PRESSED) {
      dv->Keyboard_state.layer = SHI_LAYER;
      dv->Keyboard->press(k);
    }else if(mode == KEY_RELEASED) {
      dv->Keyboard_state.layer = DEF_LAYER;
      dv->Keyboard->release(k);
    }
    break;
    case _SELECT_KEY:
      dv->Joystick->button(5,mode);
    break;
    case _START_KEY:
      dv->Joystick->button(6,mode);
    break;
    
    
    default:
      if(mode == KEY_PRESSED) {
        dv->Keyboard->press(k);
      }else if(mode == KEY_RELEASED) {
        dv->Keyboard->release(k);
      }
    break;
  }

  
}


void keypad_action(DEVTERM*dv,uint8_t col,uint8_t mode) {

  uint16_t k;
  
  k = keys_maps[col];

  if(k == EMP){
    return;
  }

  switch(k) {
    case _LEFT_SHIFT_KEY:
    case KEY_RIGHT_SHIFT:
      if(mode == KEY_PRESSED) {
        dv->Keyboard_state.layer = SHI_LAYER;
        dv->Keyboard->press(k);
      }else if(mode == KEY_RELEASED) {
        dv->Keyboard_state.layer = DEF_LAYER;
        dv->Keyboard->release(k);
      }
    break;
    case  _FN_KEY:
      if(mode == KEY_PRESSED){
        dv->Keyboard_state.layer = FN_LAYER;
      }else if(mode == KEY_RELEASED ) {
        dv->Keyboard_state.layer = DEF_LAYER;
      }
    break;
    case _JOYSTICK_UP:
      dv->Joystick->Y(0);
    break;
    case _JOYSTICK_DOWN:
      dv->Joystick->Y(1023);
    break;
    case _JOYSTICK_LEFT:
      dv->Joystick->X(0);
    break;
    case _JOYSTICK_RIGHT:
      dv->Joystick->X(1023);
    break;
    case _JOYSTICK_A:
      dv->Joystick->button(1,mode);
    break;
    case _JOYSTICK_B:
      dv->Joystick->button(2,mode);
    break;
    case _JOYSTICK_X:
      dv->Joystick->button(3,mode);
    break;
    case _JOYSTICK_Y:
      dv->Joystick->button(4,mode);
    break;
    case _MOUSE_LEFT:
      if(mode == KEY_PRESSED){
        dv->Mouse->press(1);
      }else if(mode == KEY_RELEASED){
        dv->Mouse->release(1);
      }
    break;
    case _MOUSE_MID:
      if(mode == KEY_PRESSED){
        dv->Mouse->press(2);
      }else if(mode == KEY_RELEASED){
        dv->Mouse->release(2);
      }
    break;

    case _MOUSE_RIGHT:
      if(mode == KEY_PRESSED){
        dv->Mouse->press(3);
      }else if(mode == KEY_RELEASED){
        dv->Mouse->release(3);
      }
    break;
    
    default:break;
    
  }
  
}

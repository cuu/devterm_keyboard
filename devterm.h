#ifndef DEVTERM_H
#define DEVTERM_H


#include <USBComposite.h>

typedef struct keyboard_state{

  uint8_t layer;
  uint8_t shift;
  uint8_t caps_lock;
  
}KEYBOARD_STATE;

class DEVTERM {
  public:
    HIDKeyboard *Keyboard;
    HIDMouse *Mouse; 
    HIDJoystick *Joystick;
    KEYBOARD_STATE Keyboard_state;
  
};


#endif

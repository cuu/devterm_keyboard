#include "keyboard.h"
#include "keys.h"
#include "trackball.h"
#include "devterm.h"

#include <USBComposite.h>

USBHID HID;
DEVTERM dev_term;

void setup() {
  USBComposite.setManufacturerString("ClockworkPI");
  USBComposite.setProductString("DevTerm");

  dev_term.Keyboard = new HIDKeyboard(HID);
  dev_term.Joystick = new HIDJoystick(HID);
  dev_term.Mouse    = new HIDMouse(HID);
  
  dev_term.Keyboard_state.shift = 0;
  dev_term.Keyboard_state.layer = 0;
  
  HID.begin(HID_KEYBOARD_MOUSE_JOYSTICK);

  while(!USBComposite);//wait until usb port been plugged in to PC
  
  Serial.begin(115200);
  keyboard_init();
  keys_init();
  trackball_init();
  
  Serial.println("setup done");
/*

11:43:12.146 -> setup done
11:43:12.146 -> 1 0 M21
11:43:12.146 -> 1 1 M22
11:43:12.146 -> 1 2 M23
11:43:12.146 -> 1 3 M24
11:43:12.146 -> 1 4 M25
11:43:12.146 -> 1 5 M26
11:43:12.146 -> 1 6 M27
11:43:12.146 -> 1 7 M28
after combine with USBComposite , M21-M28 showed up in boot time

 */
  //delay(3000);
}

void loop() {

  trackball_task(&dev_term);
  keys_task(&dev_term); //keys above keyboard
  keyboard_task(&dev_term);
  

}

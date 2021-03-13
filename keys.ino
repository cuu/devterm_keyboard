#include "keys.h"

static bool key_debouncing = false;
static uint16_t key_debouncing_time = 0;

uint8_t keys_io[ KEYS_NUM ]= {KEY1,KEY2,KEY3,KEY4,KEY5,KEY6,KEY7,KEY8,KEY9,KEY10,KEY11,KEY12,KEY13,KEY14,KEY15,KEY16};

/* keys state(1:on, 0:off) */
static uint16_t keys;
static uint16_t keys_debouncing;
static uint16_t keys_prev;


void init_keys(){
  int i;
  for(i=0;i<KEYS_NUM;i++) {
    pinMode( keys_io[i],INPUT_PULLUP);  
  }
}

uint8_t scan_keys(){
  uint16_t data;
  uint8_t s;
  
  data = 0;
  delayMicroseconds(20);
  for(int i = 0;i < KEYS_NUM;i++) {
    
    s = digitalRead(keys_io[i]); //HIGH =0,LOW = 1
    if( s == LOW ){
      data |= 1 << i;
    }else {
      data |= 0 << i;
    }
  }
  
  if ( keys_debouncing != data ) {
      keys_debouncing = data;
      key_debouncing = true;
      key_debouncing_time = millis();
  }

   if (key_debouncing == true  &&  ( (millis() - key_debouncing_time) > KEY_DEBOUNCE )) {
    keys = keys_debouncing;
    debouncing = false;
  }

  return 1;
}


void print_keys(DEVTERM*dv) {
  char buff[128];

  for (int i = 0; i < KEYS_NUM; i++) {
    if( keys & (1<< i) ){
      sprintf(buff,"B%d pressed\n",i+1);
      dv->_Serial->print(buff);
    }
  }
  
  
}

void keys_task(DEVTERM*dv){

  uint16_t _change = 0;

  scan_keys();

  uint16_t _mask =1;
  for(uint8_t c=0;c < KEYS_NUM;c++,_mask <<=1){

    if( (keys_prev & _mask) == 0 && (keys & _mask) > 0){
      keypad_action(dv,c,KEY_PRESSED);
    }
    if( (keys_prev & _mask) > 0 && (keys & _mask) == 0){
      keypad_action(dv,c,KEY_RELEASED);
    }    
    
  }

  keys_prev = keys;
 
}

void keys_init(void){

  init_keys();

   
}

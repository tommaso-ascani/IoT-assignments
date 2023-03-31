#ifndef __BUTTON__
#define __BUTTON__
#include "Arduino.h"


class Button {

  public:
    int Pin;
  
  public:
    Button(int);
    bool isPressed(); 
};

#endif

#include "Button.h"
#include <Arduino.h>

Button::Button(int pin){
  this->Pin = pin;
}

bool Button::isPressed(){
  if (digitalRead(this->Pin) == HIGH){
    return true;
  }
  return false;
}

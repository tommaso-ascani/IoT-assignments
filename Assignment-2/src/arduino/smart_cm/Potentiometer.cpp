#include "Potentiometer.h"
#include <Arduino.h>


Potentiometer::Potentiometer(int pin){
  this->Pin = pin;
}

int Potentiometer::getValue(){
  return (analogRead(this->Pin));            
}

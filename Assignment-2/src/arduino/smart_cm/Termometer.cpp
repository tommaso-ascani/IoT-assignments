#include "Termometer.h"
#include <Arduino.h>

Termometer::Termometer(int pin){
  this->Pin = pin;
}

float Termometer::getTemperature(){
  int valCorrente = analogRead(this->Pin);
  float tempinC = ((valCorrente * 0.00488) - 0.5) / 0.01;     //Formula per conversione dal voltaggio a temperatura in C° del sensore TMP36
  return tempinC;
}

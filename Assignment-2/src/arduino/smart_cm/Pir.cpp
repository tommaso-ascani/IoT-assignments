#include "Pir.h"
#include "Arduino.h"

Pir::Pir(int pin){
  this->Pin = pin;
  this->someDetect = this->somethingDetected();
  pinMode(pin, INPUT);     
} 
  
bool Pir::somethingDetected(){
  if (digitalRead(this->Pin) == HIGH){
    this->someDetect = true;
    return true;
  }
  this->someDetect = false;
  return false;
}

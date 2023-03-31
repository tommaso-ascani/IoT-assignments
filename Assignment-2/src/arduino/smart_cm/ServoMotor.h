#include <ServoTimer2.h>

#ifndef __SERVOMOTOR__
#define __SERVOMOTOR__

#include "ServoTimer2.h"

class ServoMotor: public ServoTimer2 {

  int Pin;

  public:
    ServoMotor(int pin): ServoTimer2{} {
      this->Pin = pin;
      ServoTimer2::attach(this->Pin);
    }
    void moveServoForward();
    void moveServoBackward();
};
#endif

#include "ServoMotor.h"
#include <Arduino.h>

void ServoMotor::moveServoForward(){                  //muove il motore in avanti per 10 sec (avanti = fare il caffe)
  for (int pos = 750; pos <= 2250; pos += 1) {        //La libreria ServoTimer2 (modificata su consiglio trovato su internet)
    ServoTimer2::write(pos);                          // quando si usa la write() ha come inizio il valore 750 = 0° e come fine/max il valore 2250 = 180°
    delay(7);                                         // questo delay permette di effettuare da 0 a 180° in 7 * 1500 = 10500 millisecondi quindi circa 10 come da assignment
  }       
}

void ServoMotor::moveServoBackward(){          //muove il motore indietro nel minor tempo possibile
  ServoTimer2::write(750);
}

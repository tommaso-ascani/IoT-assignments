#ifndef __POTENTIOMETER__
#define __POTENTIOMETER__

class Potentiometer {

  int Pin;

  public:
    Potentiometer(int);
    int getValue();
};
#endif

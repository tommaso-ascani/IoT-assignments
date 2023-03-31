#ifndef __TERMOMETER__
#define __TERMOMETER__

class Termometer{

  int Pin;

  public:
    Termometer(int);
    float getTemperature(); 
};
#endif

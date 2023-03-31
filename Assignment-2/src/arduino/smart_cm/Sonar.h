#ifndef __SONAR__
#define __SONAR__

class Sonar{

  int EchoPin;
  int TrigPin;
  const float VelSuono = 343.5;  //Velocità del suono a 20° in metri al sec

  public:
    Sonar(int, int);
    float getDist();
};
#endif

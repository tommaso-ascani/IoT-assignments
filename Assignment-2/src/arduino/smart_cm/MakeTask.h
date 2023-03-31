#ifndef __MAKETASK__
#define __MAKETASK__

#include "Task.h"
#include "SharedVariables.h"
#include "ServoMotor.h"
#include "Sonar.h"
#include "Lcd.h"
#include "Arduino.h"

class MakeTask: public Task {

  enum State{ MAKING, MADE, INACTIVE} state;
  ServoMotor* srvMtr;
  SharedVariables* sharedVar;
  Sonar* sonar;
  Lcd* lcd;
  bool timeTaken;

  public:
    MakeTask(SharedVariables*, ServoMotor*, Sonar*, Lcd*);
    void init(int);
    void tick();
};
#endif

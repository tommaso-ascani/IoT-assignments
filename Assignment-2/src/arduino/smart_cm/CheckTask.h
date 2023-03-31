#ifndef __CHECKTASK__
#define __CHECKTASK__

#include "Termometer.h"
#include "ServoMotor.h"
#include "SharedVariables.h"
#include "Lcd.h"
#include "Task.h"

class CheckTask: public Task{

  enum State{ INACTIVE, CHECK }state;
  Termometer* term;
  ServoMotor* servo;
  SharedVariables* sharedVar;
  Lcd* lcd;

  public:
    CheckTask(SharedVariables*, ServoMotor*, Termometer*, Lcd*);
    void init(int);
    void tick();
};

#endif

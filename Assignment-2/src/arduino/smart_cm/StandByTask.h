#ifndef __STANDBYTASK__
#define __STANDBYTASK__

#include "Task.h"
#include "Arduino.h"
#include "Pir.h"
#include "SharedVariables.h"
#include "Lcd.h"

class StandByTask: public Task{

  SharedVariables* sharedVar;
  Lcd* lcd;
  enum State{ SLEEPING, AWAKE, INACTIVE} state;

  public:
    StandByTask(SharedVariables*, Lcd*);
    void init(int);
    void tick();
};
#endif

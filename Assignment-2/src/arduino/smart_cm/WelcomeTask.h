#ifndef __WELCOMETASK__
#define __WELCOMETASK__

#include "Task.h"
#include "Arduino.h"
#include "SharedVariables.h"
#include "Lcd.h"


class WelcomeTask: public Task {

  Lcd* lcd;
  SharedVariables* sharedVar;

    public:
      WelcomeTask(Lcd*, SharedVariables*);
      void init(int); 
      void tick();
};
#endif

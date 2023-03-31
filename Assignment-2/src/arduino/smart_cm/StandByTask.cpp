#include "StandByTask.h"
#include <avr/sleep.h>

StandByTask::StandByTask(SharedVariables* sv, Lcd* lcd){
  this->sharedVar = sv;
  this->lcd = lcd;
}

void StandByTask::init(int p){
  Task::init(p);
  this->state = INACTIVE;
}

void StandByTask::tick(){
  switch(this->state) {
    case INACTIVE: {
      if(this->sharedVar->isStandBy()){
        this->state = SLEEPING;
        this->lcd->clear();
        this->lcd->print("Going to sleep in:", 2,1);
        this->lcd->print("3s -- ", 3,3);
        delay(1000);
        this->lcd->print("2s -- ", 9,3);
        delay(1000);
        this->lcd->print("1s", 15,3);
        delay(1000);
        this->lcd->clear();
        this->lcd->print("ZZZZZZZZ", 6,1);
      }
      break;
    }
    case SLEEPING: {
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
      sleep_enable();
      sleep_mode();
      sleep_disable();
      this->state = AWAKE;
      break;
    }
    case AWAKE: {
      this->sharedVar->setIdle();
      this->state = INACTIVE;
      break;
    }
  }
}

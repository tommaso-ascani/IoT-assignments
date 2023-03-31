#include "CheckTask.h"

const int MIN_TEMP = 10;
const int MAX_TEMP = 30;

bool SharedVariables::recoverNeeded = false;

CheckTask::CheckTask(SharedVariables* sv, ServoMotor* sm, Termometer* trm, Lcd* lcd){
  this->sharedVar = sv;
  this->servo = sm;
  this->term = trm;
  this->lcd = lcd;
}

void CheckTask::init(int p){
  Task::init(p);
  this->state = INACTIVE;
}

void CheckTask::tick(){
  switch (this->state) {
    case INACTIVE: {
      if(this->sharedVar->isCheck()){
        this->state = CHECK;
      }
      break;
    }
    case CHECK: {
      this->lcd->clear();
      this->lcd->print("Checking  Mechanical", 0, 1);
      this->servo->moveServoForward();
      this->lcd->clear();
      this->lcd->print("Mechanical check: OK", 0, 1);
      this->servo->moveServoBackward();
      delay(2000);
      this->lcd->clear();
      this->lcd->print("Checking Temperature", 0, 1);
      if( (MIN_TEMP < (this->term->getTemperature())) || ((this->term->getTemperature()) < MAX_TEMP) ){
        this->lcd->clear();
        this->lcd->print("Temperature  Check: ", 0, 1);
        this->lcd->print("FAILED", 7, 3);
        delay(2000);
        this->sharedVar->setMaintenance();
        this->state = INACTIVE;
        SharedVariables::recoverNeeded = true;
        break;
      } else {
        this->lcd->clear();
        this->lcd->print("Temperature Check: ", 0, 1);
        this->lcd->print("SUCCEDED", 6, 3);
        delay(2000);
        this->sharedVar->setIdle();
        Serial.println(this->sharedVar->isIdle());
        this->state = INACTIVE;
        break;
      }
    }
  }
}

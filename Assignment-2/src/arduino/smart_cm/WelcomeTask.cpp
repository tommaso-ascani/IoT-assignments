#include "WelcomeTask.h"
#include <Arduino.h>

WelcomeTask::WelcomeTask(Lcd* lcd, SharedVariables* sv){
  this->lcd = lcd;
  this->sharedVar = sv;
}

void WelcomeTask::init(int period){
  Task::init(period);
}

void WelcomeTask::tick(){
  this->lcd->print("Welcome to", 5,0);
  this->lcd->print("Smart Coffee Machine", 0,2);
  delay(4000);
  this->lcd->clear();
  this->lcd->print("Ready!", 7,1);
  delay(4000);
  this->sharedVar->setIdle();
}

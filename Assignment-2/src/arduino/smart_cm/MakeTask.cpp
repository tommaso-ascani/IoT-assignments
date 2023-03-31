#include "MakeTask.h"

//Dichiaro delle costanti
const long TAKE_TIME = 5000;
const long MAKING_TIME = 10000;

//Dichiaro una variabile utile per il controllo del flusso nel tick
unsigned long  tempinit;

MakeTask::MakeTask(SharedVariables* sv, ServoMotor* smr, Sonar* sn, Lcd* lcd){
  this->sharedVar = sv;
  this->srvMtr = smr;
  this->sonar = sn;
  this->lcd = lcd;
  this->timeTaken = false;
}

void MakeTask::init(int p){
  Task::init(p);
  this->state = INACTIVE;
}

void MakeTask::tick(){
  switch(this->state){
    case INACTIVE:{
      if(this->sharedVar->isMakeProd()){
        this->state = MAKING;
      }
      break;
    }
    case MAKING:{
      this->lcd->clear();
      String str = "Making: " + this->sharedVar->newSelectedProd;
      this->lcd->print(str, 1, 1);
      this->srvMtr->moveServoForward();
      SharedVariables::timerForCheck += MAKING_TIME;
      this->state = MADE;
      break;
    }
    case MADE:{
      SharedVariables::timerForCheck += this->getPeriod();
      if (!this->timeTaken){
        tempinit = millis();
        tempinit = tempinit + TAKE_TIME;
        this->lcd->clear();
        String str = "Made: " + this->sharedVar->newSelectedProd;
        this->lcd->print(str, 1, 1);
        delay(2000);
        this->timeTaken = true;
      }
      if (this->sonar->getDist() >= 0.4 || millis() >= tempinit){
        this->srvMtr->moveServoBackward();;
        this->timeTaken = false;
        this->state = INACTIVE;
        this->sharedVar->setIdle();
      }
      break;
    }
  }
}

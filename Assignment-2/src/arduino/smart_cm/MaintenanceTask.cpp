#include "MaintenanceTask.h"

bool firstRequest = true; 

MaintenanceTask::MaintenanceTask(SharedVariables* sv, Lcd* lcd){
  this->sharedVar = sv;
  this->lcd = lcd;
}

void MaintenanceTask::init(int p){
  Task::init(p);
  this->state = INACTIVE;
}

void MaintenanceTask::tick(){
  switch (this->state){
    case INACTIVE: {
      if(this->sharedVar->isMaintenance()){
        if(SharedVariables::refillNeeded){
          this->state = REFILL;
          break;
        }
        if (SharedVariables::recoverNeeded){
          this->state = RECOVER;
          break;
        }
      }
      break;
    }
    case REFILL: {
      if(firstRequest){
        this->sendMessage("MAINTENANCE: Refill Needed");
        firstRequest = false;
        this->lcd->clear();
        this->lcd->print("Refill Needed", 3, 1);
      }
      String str = this->receiveMessage();
      if (str != NULL){
        this->lcd->clear();
        this->lcd->print("Refill Completed", 2, 1);
        delay(2000);
        this->sharedVar->refillAll();
        this->state = INACTIVE;
        this->sharedVar->setIdle();
        SharedVariables::refillNeeded = false;
        firstRequest = true;
        break;
      }
      break;
    }
    case RECOVER: {
      if(firstRequest){
        this->sendMessage("MAINTENANCE: Recover Needed");
        firstRequest = false;
        this->lcd->clear();
        this->lcd->print("Recover Needed", 3, 1);
      }
      String str = this->receiveMessage();
      if (str != NULL){
        this->lcd->clear();
        this->lcd->print("Recover Completed", 2, 1);
        delay(2000);
        this->state = INACTIVE;
        this->sharedVar->setIdle();
        SharedVariables::recoverNeeded = false;
        firstRequest = true;
        break;
      }
      break;
    }
  }
}

void MaintenanceTask::sendMessage(String str){
  MsgService.sendMsg(str);
}

String MaintenanceTask::receiveMessage(){
  Msg* msg = MsgService.receiveMsg();
  return msg->getContent();
}

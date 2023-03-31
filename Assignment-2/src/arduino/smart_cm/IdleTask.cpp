#include "IdleTask.h"
#include <Arduino.h>
#include "TimerInterrupt.h"

//Dichiaro le costanti
const long GO_CHECK_TIME = 180000;
const long GO_SLEEP_TIME = 60000;
const long GO_BACK_TO_READY_TIME = 5000;

//Inizializzo le variabili statiche in SharedVariables
int SharedVariables::timerForCheck = 0;
int SharedVariables::numOfCheck = 0;
int SharedVariables::NCoffee = INIT_PROD;
int SharedVariables::NTea = INIT_PROD;
int SharedVariables::NChocolate = INIT_PROD;
bool SharedVariables::canPress = true;
bool SharedVariables::refillNeeded = false;
unsigned long SharedVariables::lastMillisPressed = 0;
String SharedVariables::newSelectedProd = "Coffee";
bool oneProdRemain = false;

//Dichiaro e inizializzo delle variabili utili per il controllo del flusso nel tick()
unsigned long timeElapsedForSleep = 0;
bool firstChoosing = true;



IdleTask::IdleTask(Lcd* lcd, SharedVariables* sv, Button* bl, Button* br, Button* bc, Potentiometer* pt, Pir* p){
  this->lcd = lcd;
  this->sharedVar = sv;
  this->bLeft = bl;
  this->bRight = br;
  this->bConfirm = bc;
  this->pot = pt;
  this->pir = p;
  this->backToReady = false;
  this->timeBackToReady = 0;
}

void IdleTask::init(int p){
  Task::init(p);
  this->state = INACTIVE;
  this->currProdSelected = "Coffee";
  this->sugarLevel = this->pot->getValue() * 100;
  this->checkProd = this->currProdSelected;
  this->checkSugar = this->sugarLevel;
}

void IdleTask::tick(){
  switch (state){
    case CHOSEN:{
      this->sharedVar->decNumAvailableProd(this->currProdSelected);
      this->sharedVar->setMakeProd();
      this->state = INACTIVE;
      break;
    }
    case CHOOSING: {
//      oneProdRemain = this->sharedVar->oneProductRemained();
//      if (oneProdRemain){
//        firstChoosing = true;
//      }
      if(!this->backToReady){
        this->timeBackToReady = this->timeBackToReady + this->getPeriod();
      }
      if(this->timeBackToReady >= GO_BACK_TO_READY_TIME){
        this->lcd->clear();
        this->lcd->print("Ready!", 7,1);
        this->timeBackToReady = 0;
        this->backToReady = true;
      }
      if(SharedVariables::timerForCheck >= GO_CHECK_TIME){
        SharedVariables::timerForCheck = 0;
        this->state = INACTIVE;
        this->sharedVar->setCheck();
        SharedVariables::numOfCheck++;
        break;
      }else{
        SharedVariables::timerForCheck = SharedVariables::timerForCheck + this->getPeriod();
      }
      bool checkDetect = this->pir->somethingDetected();                                          //Variabile usata per fare il controllo ad ogni tick() se qualcuno è nelle vacinanze
      if(timeElapsedForSleep >= GO_SLEEP_TIME){
        timeElapsedForSleep = 0;
        this->state = INACTIVE;
        this->sharedVar->setStandBy();
        break;
      }
      if (firstChoosing){
        this->sharedVar->newSelectedProd = this->sharedVar->anAvailableProd();
      }
      this->currProdSelected = this->sharedVar->newSelectedProd;
      this->setSugarLevel(this->pot->getValue());
      this->setSugarLevelString();
      if(this->checkProd != this->currProdSelected || this->checkSugar != this->sugarLevel || firstChoosing) {
        this->lcd->clear();
        this->lcd->print(this->sugarLevelString, 4,3);
        this->lcd->print(this->currProdSelected, 5,1);
        this->checkSugar = this->sugarLevel;
        this->checkProd = this->currProdSelected;
        firstChoosing = false;
        this->backToReady = false;
        this->timeBackToReady = 0;

      }
      if(this->isBConfirmPressed()){
        if(this->backToReady == true){
          firstChoosing = true;
          this->backToReady = false;
        } else{
          this->state = CHOSEN;
          break;
        }
      }
      if(!checkDetect){
        timeElapsedForSleep = timeElapsedForSleep + this->getPeriod();
      } else {
        timeElapsedForSleep = 0;
      }
      break;
    }
    case INACTIVE: {
      if(this->sharedVar->isIdle()){
        if(SharedVariables::NCoffee == 0 && SharedVariables::NTea == 0 && SharedVariables::NChocolate == 0) {
          this->sharedVar->setMaintenance();
          SharedVariables::refillNeeded = true;
          break;
        }
        this->state = CHOOSING;
        firstChoosing = true;
        break;
      }
    }
  }
}

bool IdleTask::isBConfirmPressed(){
  return (this->bConfirm->isPressed());
}

void IdleTask::setSugarLevel(int valPot){
  int lvl = map(valPot, 0, 1023, 0, 6);
  this->sugarLevel = lvl;
}

void IdleTask::setSugarLevelString(){
  this->sugarLevelString = "Zucchero:  " + String(this->sugarLevel);
}

#include <EnableInterrupt.h>
#include "WelcomeTask.h"
#include "IdleTask.h"
#include "MakeTask.h"
#include "StandByTask.h"
#include "Button.h"
#include "Potentiometer.h"
#include "Sonar.h"
#include "Pir.h"
#include "Termometer.h"
#include "Lcd.h"
#include "ServoMotor.h"
#include "SharedVariables.h"
#include "Arduino.h"
#include "Scheduler.h"
#include "CheckTask.h"
#include "MaintenanceTask.h"

#define PINBUTTONLEFT 4
#define PINBUTTONRIGHT 3
#define PINBUTTONCONFIRM 11
#define PINPOTENTIOMETER 0      //Analogico
#define PINTRIGSONAR 8
#define PINECHOSONAR 7
#define PINPIR 2
#define PINTERMOMETER 2         //Analogico
#define PINSERVOMOTOR 5

//Dichiaro una costante
const long DEB_TIME = 300;

//Dichiaro delle variabili per il controllo di flusso negli Interrupt Handler
bool newSelProdModified = false;

//Creo lo scheduler
Scheduler sched;

//Interrupt Handlers
inline void moveRight(){
  if(millis() > SharedVariables::lastMillisPressed + DEB_TIME){
    SharedVariables::canPress = true;
  }
  if(SharedVariables::canPress){
    SharedVariables::lastMillisPressed = millis();
    if(SharedVariables::newSelectedProd == "Coffee"){
      if(newSelProdModified == false && SharedVariables::NTea > 0){
        SharedVariables::newSelectedProd = "Tea";
        newSelProdModified = true;
      }
    } else if(SharedVariables::newSelectedProd == "Chocolate"){
      if(newSelProdModified == false && SharedVariables::NCoffee > 0){
        SharedVariables::newSelectedProd = "Coffee";
        newSelProdModified = true;
      }else if(newSelProdModified == false && SharedVariables::NTea > 0){
        SharedVariables::newSelectedProd = "Tea";
        newSelProdModified = true;
      }
    }
    SharedVariables::canPress = false;
    newSelProdModified = false;
  }
}

inline void moveLeft(){
  if(millis() > SharedVariables::lastMillisPressed + DEB_TIME){
    SharedVariables::canPress = true;
  }
  if(SharedVariables::canPress){
    SharedVariables::lastMillisPressed = millis();  
    if(SharedVariables::newSelectedProd == "Coffee"){
      if(newSelProdModified == false && SharedVariables::NChocolate > 0){
        SharedVariables::newSelectedProd = "Chocolate";
        newSelProdModified = true;
      }
    } else if(SharedVariables::newSelectedProd == "Tea"){
      if(newSelProdModified == false && SharedVariables::NCoffee > 0){
        SharedVariables::newSelectedProd = "Coffee";
        newSelProdModified = true;
      }else if(newSelProdModified == false && SharedVariables::NChocolate > 0){
        SharedVariables::newSelectedProd = "Chocolate";
        newSelProdModified = true;
      }
    }
    SharedVariables::canPress = false;
    newSelProdModified = false;
  }
}

void wakeUp(){
}

void setup() {
  //Setto i PinMode e abilito gli interrupt 
  pinMode(PINBUTTONLEFT,INPUT);
  pinMode(PINBUTTONRIGHT,INPUT);
  pinMode(PINPIR,INPUT);
  enableInterrupt(PINPIR, wakeUp, CHANGE);
  enableInterrupt(PINBUTTONRIGHT, moveRight, HIGH);
  enableInterrupt(PINBUTTONLEFT, moveLeft, HIGH);

  //Inizializzazione dello scheduler
  sched.init(100);
  MsgService.init();

  //Inizializzazione della banda per la connessione seriale
  Serial.begin(9600);

  //Creazione degli oggetti, ognuno dei quali corrisponde ad un sensore/attuatore
  SharedVariables* sharedVar = new SharedVariables();
  Button* BTRight = new Button(PINBUTTONRIGHT);
  Button* BTLeft = new Button(PINBUTTONLEFT);
  Button* BTConfirm = new Button(PINBUTTONCONFIRM);
  Potentiometer* Pot = new Potentiometer(PINPOTENTIOMETER);
  Sonar* Son = new Sonar(PINTRIGSONAR, PINECHOSONAR);
  Pir* PirObject = new Pir(PINPIR);
  Termometer* TermometerObject = new Termometer(PINTERMOMETER);
  Lcd* LcdObject = new Lcd();
  ServoMotor* ServoMotorObject = new ServoMotor(PINSERVOMOTOR);

  //Creazione delle task e aggiunta allo scheduler
  WelcomeTask welcTask(LcdObject, sharedVar);
  welcTask.tick();

  IdleTask* idTask = new IdleTask(LcdObject, sharedVar, BTLeft, BTRight, BTConfirm, Pot, PirObject);
  idTask->init(100);
  sched.addTask(idTask);

  MakeTask* makeTask = new MakeTask(sharedVar, ServoMotorObject, Son, LcdObject);
  makeTask->init(100);
  sched.addTask(makeTask);

  StandByTask* stndbyTask = new StandByTask(sharedVar, LcdObject);
  stndbyTask->init(100);
  sched.addTask(stndbyTask);

  CheckTask* checkTask = new CheckTask(sharedVar, ServoMotorObject, TermometerObject, LcdObject);
  checkTask->init(100);
  sched.addTask(checkTask);

  MaintenanceTask* maintTask = new MaintenanceTask(sharedVar, LcdObject);
  maintTask->init(200);
  sched.addTask(maintTask);
}

void loop() {
  sched.schedule();
}

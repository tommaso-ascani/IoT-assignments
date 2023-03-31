#ifndef __IDLETASK__
#define __IDLETASK__

#include "SharedVariables.h"
#include "Task.h"
#include "Lcd.h"
#include "Button.h"
#include "Potentiometer.h"
#include "Pir.h"

class IdleTask: public Task {

  enum State{ CHOOSING, CHOSEN, INACTIVE} state;
  Lcd* lcd;
  SharedVariables* sharedVar;
  Button* bLeft;
  Button* bRight;
  Button* bConfirm;
  Potentiometer* pot;
  Pir* pir;
  String currProdSelected;
  String sugarLevelString;
  int sugarLevel;
  String checkProd;
  int checkSugar;
  int timeBackToReady;
  bool backToReady;
  

  public:
    IdleTask(Lcd*, SharedVariables*, Button*, Button*, Button*, Potentiometer*, Pir*);
    void init(int);
    void tick(); 
    bool isBConfirmPressed();
    void setSugarLevel(int);
    void setSugarLevelString();
};

#endif

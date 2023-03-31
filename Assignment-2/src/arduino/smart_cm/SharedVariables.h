#ifndef __SHAREDVARIABLES__
#define __SHAREDVARIABLES__

#include "Arduino.h"

const int INIT_PROD = 1;

class SharedVariables {

private:
  bool Welcome;
  bool Idle;
  bool MakeProd;
  bool RemoveProd;
  bool Maintenance;
  bool Check;
  bool StandBy;


public:
  static String newSelectedProd;
  static bool canPress;
  static unsigned long int lastMillisPressed;
  static int NCoffee;
  static int NTea;
  static int NChocolate;
  static int timerForCheck;
  static int numOfCheck;
  static bool refillNeeded;
  static bool recoverNeeded;

public:
  
  SharedVariables(){
    this->Welcome =true;
    this->Idle = false;
    this->MakeProd = false;
    this->Maintenance = false;
    this->Check = false;
    this->StandBy = false;
  }
  
  bool isWelcome() { return this->Welcome; }
  
  bool isIdle() { return this->Idle; }
  
  void setIdle() { 
    this->Welcome = false; 
    this->Maintenance = false; 
    this->StandBy = false; 
    this->MakeProd = false; 
    this->Check = false; 
    this->Idle = true; 
  }

  bool isMakeProd() { return this->MakeProd; }
  
  void setMakeProd() { 
    this->Idle = false; 
    this->MakeProd = true; 
  }

  bool isMaintenance() { return this->Maintenance; } 
   
  void setMaintenance() { 
    this->Check = false; 
    this->Maintenance = true;
  }

  bool isCheck() { return this->Check; }
  
  void setCheck() { 
    this->Idle = false; 
    this->StandBy = false; 
    this->Check = true; 
  }

  bool isStandBy() { return this->StandBy; }
  
  void setStandBy() { 
    this->Idle = false; 
    this->StandBy = true; 
  }

  String anAvailableProd(){
    if (SharedVariables::NCoffee > 0) {
      return "Coffee";
    }
    if (SharedVariables::NTea > 0){
      return "Tea";
    }
    if (SharedVariables::NChocolate > 0) {
      return "Chocolate";
    }
    return "";
  }

  void decNumAvailableProd(String prod) { 
    if(prod == "Coffee"){
      SharedVariables::NCoffee--;
    } else if (prod == "Tea"){
      SharedVariables::NTea--;
    } else if (prod == "Chocolate") {
      SharedVariables::NChocolate--;
    }
  }
  
  void refillAll() { 
    SharedVariables::NChocolate = INIT_PROD; 
    SharedVariables::NTea = INIT_PROD; 
    SharedVariables::NCoffee = INIT_PROD; 
  }

  bool oneProductRemained(){
    if( (SharedVariables::NChocolate == 0 && SharedVariables::NTea == 0 && SharedVariables::NCoffee == 1) ||
        (SharedVariables::NChocolate == 0 && SharedVariables::NTea == 1 && SharedVariables::NCoffee == 0) ||
        (SharedVariables::NChocolate == 1  && SharedVariables::NTea == 0 && SharedVariables::NCoffee == 0) ){
      return true;
    } else {
      return false;
    }
  }
};
#endif

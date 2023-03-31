#ifndef __MAINTENANCETASK__
#define __MAINTENANCETASK__

#include "Task.h"
#include "Lcd.h"
#include "SharedVariables.h"
#include "MsgService.h"

class MaintenanceTask: public Task{

  enum State { INACTIVE, REFILL, RECOVER } state;
  Lcd* lcd;
  SharedVariables* sharedVar;

  public:
    MaintenanceTask(SharedVariables*, Lcd*);
    void init(int);
    void tick();
    void sendMessage(String);
    String receiveMessage();
};
#endif

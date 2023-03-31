#ifndef __TASK__
#define __TASK__

class Task {
  int Period;
  int timeElapsed;
  
public:
  virtual void init(int period){
    this->Period = period;  
    this->timeElapsed = 0;
  }

  virtual void tick() = 0;

  bool updateAndCheckTime(int basePeriod){
    this->timeElapsed += basePeriod;
    if (this->timeElapsed >= Period){
      this->timeElapsed = 0;
      return true;
    }
    return false;
  }

  int getPeriod(){
    return Period;
  }
 
};
#endif

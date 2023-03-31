#ifndef __LCD__
#define __LCD__
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class Lcd{

  public:
    LiquidCrystal_I2C* lcd = new LiquidCrystal_I2C(0x27,20,4);
    String messageToPrint;
    int cursorX;
    int cursorY;

  public:
    Lcd(){
      this->lcd->init();
      this->lcd->backlight();
    }
    void print();
    void print(String, int, int);
    void setCursorCoordinates(int, int);
    void setMessage(String);
    void clear(){ this->lcd->clear(); }
    
};
#endif

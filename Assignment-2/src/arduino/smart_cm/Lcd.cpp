#include "Lcd.h"

void Lcd::print(){
  this->lcd->setCursor(cursorX, cursorY);
  this->lcd->print(this->messageToPrint);
}

void Lcd::print(String msg, int x, int y){
  this->setCursorCoordinates(x, y);
  this->setMessage(msg);
  this->lcd->setCursor(cursorX, cursorY);
  this->lcd->print(this->messageToPrint);  
}

void Lcd::setCursorCoordinates(int newX, int newY){
  this->cursorX = newX;
  this->cursorY = newY;
}

void Lcd::setMessage(String newMessage){
  this->messageToPrint = newMessage;
}

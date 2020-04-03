#ifndef ValueSetup_h
#define ValueSetup_h

#include "Arduino.h"
#include "rgb_lcd.h"

template <typename T>
class ValueSetup
{
public:
  ValueSetup(String valueName, T initial, rgb_lcd lcd);
  T doSetup();
  void setButtonLeft(String buttonLeft);
  void setButtonRight(String buttonRight);
  void SetButtonOK(String buttonOK);

protected:
  T _value;
  virtual void onPressLeft() = 0;
  virtual void onPressRight() = 0;

private:
  rgb_lcd _lcd;
  String _valueName;
  String _buttonLeft = "<";
  String _buttonRight = ">";
  String _buttonOK = "OK";
  void printMessage(String first_part, String second_part);
  void printValueSetToMessage();
  void printSetValueMessage();
};

#endif
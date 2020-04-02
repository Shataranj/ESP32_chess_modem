#ifndef LevelSetup_h
#define LevelSetup_h

#include "Arduino.h"
#include "rgb_lcd.h"

class LevelSetup
{
public:
  LevelSetup(int initial, rgb_lcd lcd);
  int doSetup();

private:
  rgb_lcd _lcd;
  int _level;
  void printMessage(String first_part, String second_part);
  void printLevelSetToMessage();
  void printSetLevelMessage();
  void decreaseLevel();
  void increaseLevel();
  void setLevel();
};

#endif
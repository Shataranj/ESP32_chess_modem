#include "Arduino.h"
#include "rgb_lcd.h"
#include "LevelSetup.h"

LevelSetup::LevelSetup(int initial, rgb_lcd lcd)
{
  level = initial;
  _lcd = lcd;
}

int LevelSetup::doSetup()
{
  printSetLevelMessage();
  while (true)
  {
    if (Serial.available() > 0)
    {
      inputString = "";
      inputString = Serial.readStringUntil('\n');
      inputString.trim();

      if (inputString == ">")
      {
        increaseLevel();
      }
      else if (inputString == "<")
      {
        decreaseLevel();
      }
      else if (inputString == "OK")
      {
        printLevelSetToMessage();
        return _level;
      }

      Serial.flush();
    }
  }
}

void LevelSetup::setLevel(int new_level)
{
  if (new_level < 0 || new_level > 15)
  {
    return;
  }
  _level = new_level;
}

void LevelSetup::increaseLevel()
{
  setLevel(_level + 1);
  printSetLevelMessage();
}

void LevelSetup::decreaseLevel()
{
  setLevel(_level - 1);
  printSetLevelMessage();
}

void LevelSetup::printSetLevelMessage()
{
  printMessage("Set Level:", String(_level));
}

void LevelSetup::printLevelSetToMessage()
{
  printMessage("Level Set to:", "Level:" + String(_level));
}

void LevelSetup::printMessage(String first_part, String second_part)
{
  _lcd.clear();
  _lcd.print(first_part);
  _lcd.setCursor(0, 1);
  _lcd.print(second_part);
}
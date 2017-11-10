/*
  Button.h - Library for handling buttons.
  Created by Matthieu Dubois, September 28, 2017.
  Released into the public domain.
*/
#ifndef Button_h
#define Button_h

#include "Arduino.h"

class Button
{
  public:
    Button(int pin);
    bool isPressed();
  private:
    int _pin;   // arduino pin
    int _state; // current button state
    bool read();
};

#endif

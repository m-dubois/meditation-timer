/*
  LED.h - Library for handling leds.
  Created by Matthieu Dubois, Octobber 1, 2017.
  Released into the public domain.
*/
#ifndef LED_h
#define LED_h

#include "Arduino.h"

class LED
{
  public:
    LED(int pin);
    void on();
    void off();
    void toggle();
    void blink(unsigned long delay);
    void update();
  private:
    int _pin;             // arduino pin
    bool _state;          // LED state (on/off)
    bool _blink;
    unsigned long _delay; // delay between blinks
    unsigned long _timer; // timer to handle blink
};

#endif

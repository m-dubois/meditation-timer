
#include "Button.h"

Button::Button(int pin) {

  _pin = pin;
  _state = digitalRead(_pin);

  pinMode(_pin, INPUT);
}

bool Button::read() {
  _state = digitalRead(_pin) == HIGH;
  return _state;
}

bool Button::isPressed() {
  read();
  return _state;
}

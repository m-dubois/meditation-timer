
#include "LED.h"

LED::LED(int pin) {
  _pin = pin;
  _state = LOW;
  _timer = 0L;
  _blink = false;

  pinMode(_pin, OUTPUT);
}

void LED::on() {
  digitalWrite(_pin, HIGH);
  _state = HIGH;
}

void LED::off() {
  digitalWrite(_pin, LOW);
  _state = LOW;
}

void LED::toggle(void){
	_state == HIGH ? off() : on();
}

void LED::blink(unsigned long delay){
  _delay = delay;
  _blink = true;
}

void LED::update() {
  if (_blink && millis() - _timer > _delay) {
    _timer = millis();
    Serial.print("toggle ");
    Serial.println(_delay);
    toggle();
  }
}

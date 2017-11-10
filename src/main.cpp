/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"
#include <Button.h>
#include <LED.h>
#include <TM1637.h>

// Module connection pins (Digital Pins)
#define CLK 6
#define DIO 7

TM1637 display(CLK, DIO);// // initialize the library for pins 2, 3
int8_t Disp[4]; //Dimension of digits

Button button1 = Button(2, true, true, 25);	//25 ms debounce
Button button2 = Button(3, true, true, 25);	//25 ms debounce
Button button3 = Button(5, true, true, 25);	//25 ms debounce

LED led1 = LED(12);
LED led2 = LED(13);

int brightness = 0;

void setup() {

  Serial.begin(9600);
    Serial.println("starting...");

    display.init(); // Start display
    display.set(brightness); //Setup brightness 0 to 7
}

void loop() {
  button1.read();
  button2.read();
  button3.read();
  /*
  led1.update();
  led2.update();

  if (button1.isPressed()) {
    led1.toggle();
  }

  if (button2.isPressed()) {
    led2.toggle();
  }

  if (button3.isPressed()) {
    led1.toggle();
    led2.toggle();
  }
  */

  if (button1.wasPressed()) {
    if (brightness == 7) {
      brightness = 0;
    } else {
      brightness++;
    }
    display.set(brightness);
  }

  Disp[0] = 1; //Write digits to dimension
  Disp[1] = 3;
  Disp[2] = 3;
  Disp[3] = 7;
  display.display(Disp); //Show dimension(digits) to display
  display.point(true);
}

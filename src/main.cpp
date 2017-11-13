/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"
#include <DebounceButton.h>
#include <LED.h>
#include <TM1637.h>

// Module connection pins (Digital Pins)
#define CLK 6
#define DIO 7

TM1637 display(CLK, DIO);// // initialize the library for pins 2, 3

int8_t* activeDigits;
int8_t digitsToDisplay[4]; //Dimension of digits
uint8_t blankDigits[4] = {0x10, 0x10, 0x10, 0x10};

int minutes = 5;
int seconds = 0;

int brightness = 0;

boolean pointState = false;
boolean displayShow = true;

unsigned long previousMillis = 0;

boolean configureMode = false;

DebounceButton button1 = DebounceButton(2, DBTN_PULLDOWN, 50, 1000, 500);
DebounceButton button2 = DebounceButton(3, DBTN_PULLDOWN, 50, 1000, 500);
DebounceButton button3 = DebounceButton(4, DBTN_PULLDOWN, 50, 1000, 500);

LED led1 = LED(12);
LED led2 = LED(13);

void buildDigitsArray(int minutes, int seconds) {

  if (minutes == 0 && seconds == 0) {
    digitsToDisplay[0] = digitsToDisplay[1] = digitsToDisplay[2] = digitsToDisplay[3] = 0x10;
  } else {
    digitsToDisplay[0] = minutes / 10;
    digitsToDisplay[1] = minutes % 10;
    digitsToDisplay[2] = seconds / 10;
    digitsToDisplay[3] = seconds % 10;
  }
}

void onHoldButton1(DebounceButton* btn) {
  configureMode = true;
}
void onPressButton1(DebounceButton* btn) {
  if (configureMode) {
    configureMode = false;
  };
}

int f(int time, int increment) {
  int t;
  if (configureMode) {
    if (time == 99) {
      t = 0;
    } else {
      t = time + increment;
    }
  }
  return t;
}

void onClickButton2(DebounceButton* btn) {
  minutes = f(minutes, 1);
}
void onHoldButton2(DebounceButton* btn) {
  minutes = f(minutes, 10);
}
void onClickButton3(DebounceButton* btn) {
  seconds = f(seconds, 1);
}
void onHoldButton3(DebounceButton* btn) {
  seconds = f(seconds, 10);
}

void setup() {

  Serial.begin(9600);
  Serial.println("starting...");

  led1.off();
  led2.off();

  display.init(); // Start display
  display.set(brightness); //Setup brightness 0 to 7

  digitsToDisplay[0] = 0; //Write digits to dimension
  digitsToDisplay[1] = 5;
  digitsToDisplay[2] = 0;
  digitsToDisplay[3] = 0;

  //button1.onClick = onClickButton1;
  button1.onHold = onHoldButton1;
  button1.onPress = onPressButton1;
  button2.onClick = onClickButton2;
  button2.onHold = onHoldButton2;
  button3.onClick = onClickButton3;
  button3.onHold = onHoldButton3;
}

void loop() {
  DebounceButton::updateAll();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 500) {
    previousMillis = currentMillis;

    // blink points
    display.point(pointState);
    pointState = !pointState;

    buildDigitsArray(minutes, seconds);
    activeDigits = digitsToDisplay;

    if (configureMode) {
      if (displayShow) {
        activeDigits = blankDigits;
      }
      displayShow = !displayShow;
    }

    if (displayShow) {
      led1.on();
    } else {
      led1.off();
    }

    if (configureMode) {
      led2.on();
    } else {
      led2.off();
    }
  }
  display.display(activeDigits); //Show dimension(digits) to display
}

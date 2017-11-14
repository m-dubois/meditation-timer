/**
 * Meditation timer
 */
#include "Arduino.h"
#include <DebounceButton.h>
#include <LED.h>
#include <TM1637.h>

// Module connection pins (Digital Pins)
#define CLK 6
#define DIO 7

// 4 times 7-segment Display
TM1637 display(CLK, DIO);

// Display's digits
int8_t* activeDigits; // pointer to the digits or blank digits
int8_t digitsToDisplay[4];
uint8_t blankDigits[4] = {0x10, 0x10, 0x10, 0x10};

// display's brightness
int brightness = 0;

// display's points (separator)
boolean pointState = false;
boolean displayShow = true;

// timer - minutes and seconds
unsigned int minutes = 5;
unsigned int seconds = 0;

// variable retaining the previous time tag for points blinking
unsigned long previousMillisPointBlinking = 0;
const unsigned int POINT_BLINK_INTERVAL = 500;

// variable retaining the previous time tag for time blinking
unsigned long previousMillistimeBlinking = 0;
const unsigned int TIME_BLINK_INTERVAL = 250;

// configure mode
boolean configureMode = false;

// buttons
DebounceButton button1 = DebounceButton(2, DBTN_PULLDOWN, 50, 1000, 500);
DebounceButton button2 = DebounceButton(3, DBTN_PULLDOWN, 50, 1000, 500);
DebounceButton button3 = DebounceButton(4, DBTN_PULLDOWN, 50, 1000, 500);

// leds
LED led1 = LED(12);
LED led2 = LED(13);

// buids a 4-int array with minutes and seconds
// (or with nothing at all to build a blank array for blinking)
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

// button 1 events
void onHoldButton1(DebounceButton* btn) {
  configureMode = true;
}
void onPressButton1(DebounceButton* btn) {
  if (configureMode) {
    configureMode = false;
  };
}

// utility method for buttons 2 and 3 events
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

// button 2 events
void onClickButton2(DebounceButton* btn) {
  minutes = f(minutes, 1);
}
void onHoldButton2(DebounceButton* btn) {
  minutes = f(minutes, 10);
}

// button 3 events
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

  if (currentMillis - previousMillisPointBlinking >= POINT_BLINK_INTERVAL) {
    previousMillisPointBlinking = currentMillis;

    // blink points
    display.point(pointState);
    pointState = !pointState;

  }

  if (currentMillis - previousMillistimeBlinking >= TIME_BLINK_INTERVAL) {
    previousMillistimeBlinking = currentMillis;

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
  display.display(activeDigits);
}

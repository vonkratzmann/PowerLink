/** Drives a Powerlink timer box
   Author:    Kirk Kratzmann
   Version:   1.0
   Date:      24/06/2019
*/

/** System Functionality **

** System Harwdare Summary **
   System consists of:

   The main program:-

   Normal operation the diagnostic led fashes on and off every second
*/

#include "PowerLink.h"

uint8_t onBoardLedState = LOW;           //state of led, initially off
unsigned long ledFlashCounter = 0;       //used to count time until flash led

unsigned int mode = DIRECT_MODE;    //mode selected by rotary switch, initial value is arbitary
boolean relayEnergised = false;     //true if relay is energised false if not energised

boolean firstButtonPush = false;          //Used in latch mode to track button pushes
boolean waitingSecondButtonPush = false;  //Used in latch mode to track button pushes
boolean secondButtonPush = false;         //Used in latch mode to track button pushes

int plugInSwitchState;             // the current reading from the plug In Switch
int lastplugInSwitchState = LOW;   // the previous reading from the plug In Switch

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time


unsigned long userRequestedTime = 0;
unsigned long timerStartTime = 0;

/** Setup **/
void setup(void)
{
  /* set up inputs using internal pull up resistors and set up outputs */
  pinMode(onBoardLedPin, OUTPUT);
  digitalWrite(onBoardLedPin, HIGH);       //sets the LED on

  pinMode(userLedPin, OUTPUT);
  digitalWrite(userLedPin, LOW);            //sets the LED to off

  pinMode(relayDriverPin, OUTPUT);
  digitalWrite(relayDriverPin, LOW);        //turn off the relay

  Serial.begin(38400);      //set up serial port for any debug prints
  return;
}//end of setup()


/** Main Loop **/
void loop(void) {

  //flash the led with period of one second, to show something is happening
  if (millis() - ledFlashCounter >= 500) {
    toggleLed();
    ledFlashCounter = millis();
  }

  //check if mode switch position has changed
  unsigned int newMode = getRotarySwitchPosition();
  if (mode != newMode) {
    //switch position has changed, abort everything
    mode = newMode;
    de_energiseRelay();
    //clear flags in a seperate function, so any changes done in the one place
    clearFlags();
  }

  if (mode == LATCH_MODE) {
    boolean buttonPosition = getPlugInButtonPosition();
    //check if first press of the button, if so energise the relay
    if (!firstButtonPush && buttonPosition) {
      firstButtonPush = true;
      energiseRelay();
    }

    //check if button released after first press of the button, if so set flag to say wait second button
    if (firstButtonPush && !buttonPosition) {
      waitingSecondButtonPush = true;
    }

    //check if second press of the button, if so de-energise the relay
    if (waitingSecondButtonPush && buttonPosition) {
      secondButtonPush = true;
      de_energiseRelay();
    }

    //check if button released after second press of the button, if so clear all flags
    if (secondButtonPush && !buttonPosition) {
      clearFlags();
    }

  } else if (mode == TIMEDSECONDS_MODE) {
    //if not timing yet, and switched pressed, start timer
    if (timerStartTime == 0ul && getPlugInButtonPosition()) {
      //get how long user wants the timer to run for
      userRequestedTime = getTimerSetting();
      //get current time
      timerStartTime = millis();
      energiseRelay();
    }
    //check if timer running, and if so if timer has finished
    if (timerStartTime != 0ul && (millis() - timerStartTime >= userRequestedTime)) {
      //time expired
      de_energiseRelay();
      //wait until button is released before finishing, so doesn't start again
      if (!getPlugInButtonPosition())
        clearFlags();
    }

  } else if (mode == TIMEDMINUTES_MODE) {
    //if not timing yet, and switched pressed, start timer
    if (timerStartTime == 0ul && getPlugInButtonPosition()) {
      //get how long user wants the timer to run for
      userRequestedTime = getTimerSetting();
      //as this is the minute timer, convert to minutes
      userRequestedTime *= 60;
      //get current time
      timerStartTime = millis();
      energiseRelay();
    }
    //check if timer running, and if so if timer has finished
    if (timerStartTime != 0ul && (millis() - timerStartTime >= userRequestedTime)) {
      //time expired
      de_energiseRelay();
      //wait until button is released before finishing, so doesn't start again
      if (!getPlugInButtonPosition())
        clearFlags();
    }

  } else if (mode == DIRECT_MODE) {
    //if switch pressed, energise the relay otherwise de-energise the relay
    if (getPlugInButtonPosition())
      energiseRelay();
    else {
      de_energiseRelay();
      clearFlags();
    }
  }
}
//end of loop(void)


/** getRotarySwitchPosition() **/
unsigned int getRotarySwitchPosition() {
  return 0;
}


/** getPlugInButtonPosition() **/
/* only returns the new state */

boolean getPlugInButtonPosition() {

  // read the state of the switch into a local variable:
  int reading = digitalRead(plugInButtonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastplugInSwitchState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != plugInSwitchState) {
      plugInSwitchState = reading;
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastplugInSwitchState = reading;

  return plugInSwitchState;
}


/**  energiseRelay() **/
void energiseRelay() {
  relayEnergised = true;
  digitalWrite(relayDriverPin, 1);
}


/**  de_energiseRelay() **/
void de_energiseRelay() {
  relayEnergised = false;
  digitalWrite(relayDriverPin, 0);
}


/** clearFlags() **/
void clearFlags() {
  firstButtonPush = false;
  waitingSecondButtonPush = false;
  secondButtonPush = false;
  userRequestedTime = 0ul;
  timerStartTime = 0ul;
}

/**  getTimerSetting() **/
int  getTimerSetting() {
  return 0;
}

/** void toggleLed(void) **/
void toggleLed(void) {
  onBoardLedState ? onBoardLedState = LOW : onBoardLedState = HIGH;   //swap led state from high to low or low to high
  digitalWrite(onBoardLedPin, onBoardLedState);                       //update the led
}

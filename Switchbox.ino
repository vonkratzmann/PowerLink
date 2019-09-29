/** Drives a Switchbox
 * Author:    Kirk Kratzmann John McKeown
 * Version:   1.0
 * Date:      24/06/2019
 */

/** System Functionality **/
/*
 *  The main program loop:
 *  Flashes the onboard led on and off every second.
 *  Reads the position of the rotary switch and if valid position calls the appropriate mode processing code.
 *  If no valid position for the rotary switch, it is ignored.
 *  If the rotary switch position is moved, the current mode processing is aborted and the relay de-energised.
 *  If the rotary switch is moved while holding down the plug in switch,
 *  the current mode processing is aborted and a new operation is started immediately.
 *  Note: all inputs are active when input signal is grounded.
 *
 *  There is debug code (defined by macros) used to debug software by printing out selected variables to the serial monitor.
 *  These macros are normally blank, for them to work the token "DEBUG_CODE" has to defined in Switchbox.h
 *
 *  There is a simple hardware diagnostic used to test the digital and analogue input wiring.
 *  The code periodically prints out the state of the digtal and analogue inputs to the serial monitor
 *  This code is only called if the Digital input 12 is grounded.
 */

#include "Switchbox.h"

int onBoardLedState{LOW};           //state of led, initially off
long ledFlashCounter{0};        //used to count time until flash led

int mode{INVALID_MODE};         //mode selected by rotary switch, initial value is invalid so forces a change at startup

boolean firstButtonPush{false};          //Used in latch mode to track button pushes
boolean waitingSecondButtonPush{false};  //Used in latch mode to track button pushes
boolean secondButtonPush{false};         //Used in latch mode to track button pushes

int plugInSwitchPreviousValue{0};        //Previous value of the plug In Switch

long userRequestedTime{0L};      //time requested by the user in timed modes
long timerStartTime{0L};         //time when user timer started in milliseconds in timed modes


/** Setup **/
void setup(void)
{
  /* set up inputs using internal pull up resistors */
  pinMode(rotarySwitchDirectPin, INPUT_PULLUP);
  pinMode(rotarySwitchTimedSecondsPin, INPUT_PULLUP);
  pinMode(rotarySwitchTimedMinutesPin, INPUT_PULLUP);
  pinMode(rotarySwitchLatchPin, INPUT_PULLUP);
  pinMode(plugInButtonPin, INPUT_PULLUP);
  pinMode(runHwDiagnosticsPin, INPUT_PULLUP);

  /* set up outputs */
  pinMode(onBoardLedPin, OUTPUT);
  digitalWrite(onBoardLedPin, HIGH);       //sets the LED on

  pinMode(relayDriverPin, OUTPUT);
  digitalWrite(relayDriverPin, LOW);        //turn off the relay

  //display startup message if debuging is defined
  Serial.begin(38400L);
  DEBUGPRINT_STARTUP;

  return;
}//end of setup()


/** Main Loop **/
void loop(void) {
  //flash the led once a second to show something is happening
  flashLed(1000);

  //only run hardware diagnostics if pin is grounded
  if (!digitalRead(runHwDiagnosticsPin))
    diagnostics();

  //check if mode switch position has changed
  if (mode != getRotarySwitchPosition()) {
    //switch position has changed, abort everything, save the new mode
    mode = getRotarySwitchPosition();
    de_energiseRelay();
    clearFlags();
    //display mode on serial port if debuging is defined
    DEBUGPRINT_MODE(mode);
  }

  // check which mode and process it
  switch (mode) {
    case LATCH_MODE:
      processLatchMode();
      break;

    case DIRECT_MODE:
      processDirectMode();
      break;

    case TIMEDSECONDS_MODE:
      processTimedMode(1);   //function requires a multiplier, so as in seconds use 1
      break;

    case TIMEDMINUTES_MODE:
      processTimedMode(60);   //function requires a multiplier, so as in minutes use 60
      break;

      //ignore invalid values
  }
}
//end of loop(void)


/** flash led **/
/*
 * Period has to be in milliseconds
 */
void flashLed(int period) {
  if (long (millis()) - ledFlashCounter >= long(period) / 2) {  //divide by 2, as on for half the period and off half the period
    toggleLed();
    ledFlashCounter = millis();
  }
}


/** void toggleLed(void) **/
void toggleLed(void) {
  onBoardLedState ? onBoardLedState = LOW : onBoardLedState = HIGH;   //swap led state from high to low or low to high
  digitalWrite(onBoardLedPin, onBoardLedState);                       //update the led
}

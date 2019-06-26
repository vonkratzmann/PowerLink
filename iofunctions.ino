/** Functions for I/O **/

#include "PowerLink.h"

/** getRotarySwitchPosition() **/
/*
   A valid rotary switch position is when the correponding input is 0 or grounded
   returns as soon as it find a stable valid position.
   If no valid position, maybe because the switch is moving, returns invalid mode.
   Does very simple debouncing by doing two reads debounce wait time apart.
*/
int getRotarySwitchPosition() {
  int val{digitalRead(rotarySwitchDirectPin)};
  if (!val) {                                             //! Because input is active when grounded
    delay(DEBOUNCE_WAIT_MILLISEC);
    if (val == digitalRead(rotarySwitchDirectPin))        //check if stable
      return DIRECT_MODE;
  }
  val = digitalRead(rotarySwitchLatchPin);
  if (!val) {                                             //! Because input is active when grounded
    delay(DEBOUNCE_WAIT_MILLISEC);
    if (val == digitalRead(rotarySwitchLatchPin))         //check if stable
      return LATCH_MODE;
  }
  val = digitalRead(rotarySwitchTimedSecondsPin);
  if (!val) {                                              //! Because input is active when grounded
    delay(DEBOUNCE_WAIT_MILLISEC);
    if (val == digitalRead(rotarySwitchTimedSecondsPin))   //check if stable
      return TIMEDSECONDS_MODE;
  }
  val = digitalRead(rotarySwitchTimedMinutesPin);
  if (!val) {                                             //! Because input is active when grounded
    delay(DEBOUNCE_WAIT_MILLISEC);
    if (val == digitalRead(rotarySwitchTimedMinutesPin))   //check if stable
      return TIMEDMINUTES_MODE;
  }

  return INVALID_MODE;
}


/** getPlugInbuttonState() **/
/*
    Returns true if button currently pressed and stable or
    false if button currently not pressed and stable.
    If button not stable returns the previous value.
    Note: when button pressed, input is grounded.
*/

boolean getPlugInbuttonState() {

  // read the state of the switch into a local variable:
  int value{digitalRead(plugInButtonPin)};
  delay (DEBOUNCE_WAIT_MILLISEC);

  //check if stable
  if (value == digitalRead(plugInButtonPin)) {
    //stable, so save value and return
    plugInSwitchPreviousValue = value;
    return value ? false : true;
  }
  else
    return plugInSwitchPreviousValue ? true : false;
}


/**  energiseRelay() **/
void energiseRelay() {
  digitalWrite(relayDriverPin, 1);
  DEBUGPRINT_RELAY(1);
}


/**  de_energiseRelay() **/
void de_energiseRelay() {
  digitalWrite(relayDriverPin, 0);
  DEBUGPRINT_RELAY(0);
}


/** clearFlags() **/
/*
   Use a function so all flags cleared in one place
*/
void clearFlags() {
  firstButtonPush = false;
  waitingSecondButtonPush = false;
  secondButtonPush = false;
  userRequestedTime = 0L;
  timerStartTime = 0L;
}


/**  getTimerSetting() **/
/*
     reads dial and scales value to a range from 1 to MAX_MINUTES_SECONDS
*/
int  getTimerSetting() {
  int value{analogRead(timerPin)};
  int scaledValue{ int (map(value, 0, 1023, 1, MAX_MINUTES_SECONDS))};
  DEBUGPRINT_TIMESELECTED(scaledValue);
  return scaledValue;
}

/** Functions to process selected mode of rotary switch and operation of plug in button **/

#include "Switchbox.h"

/** processDirectMode() **/
/*
   Note: if mode selected will continually energise or de-energise relay
   depending on the plug in button position.
*/
void  processDirectMode() {
  //if button pressed, energise the relay otherwise de-energise the relay
  if (getPlugInbuttonState())
    energiseRelay();
  else {
    de_energiseRelay();
    clearFlags();
  }
}


/** processLatchMode() **/
/* if button pushed for the first time energises the relay and set a flag first push
   then waits for release of the button
   when released sets flag waiting second button push
   when button pressed a second time, denergises the relay, sets flag second button push
   then waits for release of button again
   once button released, clears all the flags
*/
void processLatchMode() {
  boolean buttonState{getPlugInbuttonState()};

  //check if first press of the button, if so energise the relay
  if (!firstButtonPush && buttonState) {
    firstButtonPush = true;
    energiseRelay();
  }

  //check if button released after first press of the button, if so set flag to say wait second button
  if (firstButtonPush && !buttonState) {
    waitingSecondButtonPush = true;
  }

  //check if second press of the button, if so de-energise the relay
  if (waitingSecondButtonPush && buttonState) {
    secondButtonPush = true;
    de_energiseRelay();           //Note will keep de-energising relay until button released
  }

  //check if button released after second press of the button, if so clear all flags
  if (secondButtonPush && !buttonState) {
    clearFlags();
  }
}

/** ProcessTime() **/
/*
   Waits for button to be pressed,
   if pressed gets system time in milliseconds and time selected by user from the ADC,
   saves the the times and users the timerStartTime as a flag to say time running.
   Once time has expired de-energises the relay.
   Waits for the button to be released if not already released
   and only clears timerStartTime when button is released,
   so it doesn't start up again.
*/
void processTimedMode( int multiplier) {

  //if not timing yet, and switched pressed, start timer
  if (timerStartTime == 0L && getPlugInbuttonState()) {
    //get how long user wants the timer to run for and convert to milliseconds
    userRequestedTime = long(getTimerSetting() * long(multiplier) * 1000L);
    //get current time
    timerStartTime = millis();
    energiseRelay();
  }

  //check if timer running, and if so check if time has expired
  if (timerStartTime != 0L) {
    //timer has started, check if time has expired
    long diff{(long) millis() - timerStartTime};
    if (diff >= userRequestedTime) {
      //time expired
      de_energiseRelay();
      //wait until button is released before finishing, so doesn't start again
      if (!getPlugInbuttonState())
        clearFlags();
    }
  }
}

/* PowerLink parameters */

#ifndef PowerLink_h
#define PowerLink_h

#include "arduino.h"

#define MAX_MINUTES 60       //maximum minutes in Timed Minutes mode
#define MAX_SECONDS 60       //maximum seconds in Timed Seconds mode

/* Rotarty Switch Modes */
#define DIRECT_MODE 0
#define TIMEDSECONDS_MODE 1
#define TIMEDMINUTES_MODE 2
#define LATCH_MODE 3 
#define IDLE_MODE 4                                    

/*  Define Inputs */
const int rotarySwitchDirectPin = 0;
const int rotarySwitchTimedSecondsPin = 1;
const int rotarySwitchTimedMinutesPin = 2;
const int rotarySwitchLatchPin = 3;
const int plugInButtonPin = 4;      //true if closed, false if open

/* Define Outputs */
const int userLedPin = 7;           //led visible to the user
const int relayDriverPin = 8;
const int onBoardLedPin = 13;       //the led inside the case not visible to the user

#endif

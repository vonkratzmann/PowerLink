/* Switchbox parameters */

#ifndef Switchbox_h               //if Switchbox.h hasn't been include yet
#define Switchbox_h               //define it and include it, otherwise ignore

#include "arduino.h"

#define MAX_MINUTES_SECONDS 60       //maximum minutes or seconds in Timed mode
#define DEBOUNCE_WAIT_MILLISEC 50    //Debouce wait in milliseconds

/* Rotary Switch Modes */
#define DIRECT_MODE 0
#define LATCH_MODE 1
#define TIMEDSECONDS_MODE 2
#define TIMEDMINUTES_MODE 3
#define INVALID_MODE 4

/*  Define Inputs */
constexpr int rotarySwitchDirectPin = 2;
constexpr int rotarySwitchLatchPin = 3;
constexpr int rotarySwitchTimedSecondsPin = 4;
constexpr int rotarySwitchTimedMinutesPin = 5;
constexpr int plugInButtonPin = 6;
constexpr int runHwDiagnosticsPin = 12;

/* Define Outputs */
constexpr int relayDriverPin = 8;
constexpr int onBoardLedPin = 13;

/* Define Analogut Inputs */
constexpr int timerPin = 0;


/* Debug Code */
/*
   Used to debug software by printing out the value of variables to the serial monitor
   set so if the token: "DEBUG_CODE" is defined, compile the debug code in the macro,
   if the "#define DEBUG_CODE" is commented out, ie not defined,
   the macros are empty and no code is included in the program
   Normally the token: "DEBUG_CODE" is commented out, ie not defined.
*/

/*      **** NOTE ***
  NORMALLY LEAVE "DEBUG_CODE" COMMENTED OUT, 
  only uncomment for software debbuging
*/

//#define DEBUG_CODE              //normally commenetd out

#ifdef   DEBUG_CODE
#define  DEBUGPRINT_STARTUP       Serial.println("Started")
#else
#define  DEBUGPRINT_STARTUP
#endif

#ifdef   DEBUG_CODE
#define  DEBUGPRINT_MODE(mode)    Serial.print("Mode:"); Serial.println(mode)
#else
#define  DEBUGPRINT_MODE(mode)
#endif

#ifdef   DEBUG_CODE
#define  DEBUGPRINT_RELAY(state)    state ? Serial.println("Relay: energised") : Serial.println("Relay: de-energised")
#else
#define  DEBUGPRINT_RELAY(state)
#endif

#ifdef   DEBUG_CODE
#define  DEBUGPRINT_TIMESELECTED(value)    Serial.print("Time Selected:"); Serial.println(value)
#else
#define  DEBUGPRINT_TIMESELECTED(value)
#endif

#ifdef   DEBUG_CODE
#define  DEBUGPRINT_TIMEDIFF(diff)    Serial.print("millis() - timerStartTime: "); Serial.println(diff);
#else
#define  DEBUGPRINT_TIMEDIFF(diff)
#endif

#ifdef   DEBUG_CODE
#define  DEBUGPRINT_LEDON             digitalWrite(onBoardLedPin, 0);
#else
#define  DEBUGPRINT_LEDON
#endif

#ifdef   DEBUG_CODE
#define  DEBUGPRINT_LEDOFF            digitalWrite(onBoardLedPin, 0);
#else
#define  DEBUGPRINT_LEDOFF
#endif

#endif

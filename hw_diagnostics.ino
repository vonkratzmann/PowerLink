/* Hardware Diagnostics */
/*
 * Prints out state of digital & analog inputs used by the program 
 * to check the wiring.
 * Does not test relay output.
 */

#include "Arduino.h"

void diagnostics() {
  Serial.print("  Dir: ");   Serial.print(digitalRead(rotarySwitchDirectPin));
  Serial.print("  Latch: "); Serial.print(digitalRead(rotarySwitchLatchPin));
  Serial.print("  Secs: ");  Serial.print(digitalRead(rotarySwitchTimedSecondsPin));
  Serial.print("  Mins: ");  Serial.print(digitalRead(rotarySwitchTimedMinutesPin));
  Serial.print("  Button: ");  Serial.print(digitalRead(plugInButtonPin));
  Serial.print("  Timer(Raw): ");    Serial.print(analogRead(timerPin));
  Serial.print("  Timer(Scaled): "); Serial.println(getTimerSetting());

  //delay to minimise the output to the screen
  delay(1000);
}

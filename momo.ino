/****************************************************************
 * Split potentiometer Brake / Throttle controller w/ Arduino Leonardo
 * 
 * a WIP by for the D&M Makerspace @ https://github.com/jesssullivan
 * 
 * joystick library (rewrap of pluggableusb) available here:
 * https://github.com/MHeironimus/ArduinoJoystickLibrary
*****************************************************************/


#include <Joystick.h>
bool logs = true;  // use Serial monitor?


// pinout:
int dual = A0;


// generic:
int pinValue = 0;
int pinByte = 0;


// initialize a new instance of Joystick_:
Joystick_ Joystick;


void logger(String axis, int value) {
  if (logs) {
    Serial.print(axis);
    Serial.print(" value = ");
    Serial.print(value);
    Serial.print('\n');
    }
}


void setPin(int pin, int split=127) {
    pinValue = analogRead(pin);
    if (pinValue >= 0) {
      pinByte = pinValue / 4;
        if (pinByte <= split) {  
            // brake range (1 - 254); reverse output
            pinByte = (pinByte - split) >= 0 ? (pinByte - split) : -(pinByte - split);
            Joystick.setYAxis(pinByte * 2);
            logger("Brake", pinByte * 2);
        } else {
            // throttle range (1 - 254); limit to max bit value of 254;
            pinByte = (pinByte - split) > 254 ? (pinByte - split) : 254;
            Joystick.setThrottle(pinByte);
            logger("Throttle", pinByte);
        }
    }
}


// run: //
void setup() {
    Serial.begin(9600);
    Joystick.begin();
}

void loop() {
    delay(10);  // delay value in ms, for read stability
    setPin(dual);
}

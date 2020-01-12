#include <Joystick.h>

// pinout:
int dual = A0;

// generic:
int pinValue = 0;
int pinByte = 0;

// weird name convention here, not sure about usage of class_
Joystick_ Joystick;

void setup() {
    Joystick.begin();
}

void setPin(int pin, int split=127) {
    pinValue = analogRead(pin);
    if (pinValue > 0) {
        pinByte = pinValue / 4;
        Serial.print(pinByte);
        if (pinByte <= split) {  // brake range; reverse output
            pinByte = (pinByte - split) >= 0 ? (pinByte - split) : -(pinByte - split);
            Joystick.setYAxis(pinByte);
        } else {
            Joystick.setThrottle(pinByte);
        }
    } else {
        pinByte = 0;
        Joystick.setThrottle(pinByte);
        Joystick.setYAxis(pinByte);
    }
}

/////////////////////////////////////////////
// RUN:
/////////////////////////////////////////////

void loop() {
    delay(2);  // delay value in ms, for read stability
    setPin(dual);
}







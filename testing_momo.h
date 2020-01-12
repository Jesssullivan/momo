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
int resetLed = ledPin;  // I think this is a builtin type for Arduino
int resetButton = A14; // todo- check default


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


void setLimits(int pin, int resetButton) {
    if (resetButton) {
        for (int x = 0; x < 10; ++x) {
            //  show reset limits is starting w/ 10 blinks over 4 secs
            delay(400);  //  builtin type w/ arduino
            digitalWrite(ledPin, LOW);
            digitalWrite(ledPin, HIGH);
        }

        // gather min value for brake limit:
        #define minValue = analogRead(pin);  // set global

        for (int x = 0; x < 20; ++x) {
            //  show max value for throttle is starting with 20 fast blinks over 4 secs
            delay(200);
            digitalWrite(ledPin, LOW);
            digitalWrite(ledPin, HIGH);
        }

        // gather max value for throttle limit:
        #define maxValue = analogRead(pin);  // set global

        for (int x = 0; x < 2; ++x) {
            //  show complete with two slow blinks
            delay(800);
            digitalWrite(ledPin, LOW);
            digitalWrite(ledPin, HIGH);
        }
        digitalWrite(ledPin, LOW);  // set LOW for off before exiting
    }
}


bool setPin(int pin, rmin=0, rmax=1024) {

    pinVal = analogRead(pin);

    if (pinValue >= 0) {  // no action if not true

        pinValue = (pinVal - rmin) * (1024 / rmax);

        if (pinValue / 4 >= 127) {
            val = (pinValue / 4) - 127;
            pinByte = val * 2 > 254 ? val * 2 : 254;
            Joystick.setThrottle(pinByte);
            logger("Throttle", pinByte);
        }

        if (pinValue / 4 <= 127) {
            val = (pinValue / 4) - 127;
            Joystick.setYAxis(val * 2);
            logger("Brake", val * 2);
        }
    }
    return true;
}


// run: //

void setup() {
    Serial.begin(9600);
    Joystick.begin();
    pinMode(ledPin, OUTPUT);
    pinMode(resetButton, INPUT);

    for (int x = 0; x < 3; ++x) {
        if (setLimits(dual, resetButton) != true) {
            delay(100)
            setLimits(dual, resetButton)
        } else {
            break;
        }
    }
}

void loop() {
    delay(10);  // delay value in ms, for read stability
    setPin(dual, rmin=minValue, rmax=maxValue);
}

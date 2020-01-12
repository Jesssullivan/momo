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
int resetLed = LED_BUILTIN;
int resetSwitch = A1; // todo- check default
int startButton = A2;


// generic:
int pinValue = 0;
int pinByte = 0;
int pin = 0;

// initialize a new instance of Joystick_:
Joystick_ Joystick;


void logger(String axis, int value) {
  if (logs) {
    Serial.print(axis);
    Serial.print(" value = ");
    Serial.print(value);
    Serial.print("\n");
    }
}


void setLimits(int pin, int resetSwitch) {
    if (resetSwitch) {
        delay(200);
        Serial.print("Setting up read limits ... ");
        Serial.print("\n");
        delay(200);
        Serial.print("Setting min value ...");
        Serial.print("\n");
        
        for (int x = 0; x < 10; ++x) {
            //  show reset limits is starting w/ 10 blinks over 4 secs
            delay(400);  //  builtin type w/ arduino
            digitalWrite(resetLed, LOW);
            digitalWrite(resetLed, HIGH);
        }

        // gather min value for brake limit:
        #define minValue analogRead(pin)  // set global
        
        Serial.print("read min value : ");
        Serial.print(minValue);
        Serial.print("\n");
        Serial.print("Setting max value ...");
        
        for (int x = 0; x < 20; ++x) {
            //  show max value for throttle is starting with 20 fast blinks over 4 secs
            delay(200);
            digitalWrite(resetLed, LOW);
            digitalWrite(resetLed, HIGH);
        }

        // gather max value for throttle limit:
        #define maxValue analogRead(pin)  // set global
       
        Serial.print("read max value : ");
        Serial.print(maxValue);
        Serial.print('\n');
        
        for (int x = 0; x < 2; ++x) {
            //  show complete with two slow blinks
            delay(800);
            digitalWrite(resetLed, LOW);
            digitalWrite(resetLed, HIGH);
        }
        digitalWrite(resetLed, LOW);  // set LOW for off before exiting
    }
}


bool setPin(int pin, int rmin, int rmax) {

    int pinVal = analogRead(pin);

    if (pinValue >= 0) {  // no action if not true

        int pinValue = (pinVal - rmin) * (1024 / rmax);

        if (pinValue / 4 >= 127) {
            int val = (pinValue / 4) - 127;
            pinByte = val * 2 > 254 ? val * 2 : 254;
            Joystick.setThrottle(pinByte);
            logger("Throttle", pinByte);
        }

        if (pinValue / 4 <= 127) {
            int val = (pinValue / 4) - 127;
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
    pinMode(resetLed, OUTPUT);
    pinMode(resetSwitch, INPUT);
    delay(100);
    setLimits(dual, resetSwitch);
}


void loop() {
    delay(10);  // delay value in ms, for read stability
    setPin(dual, minValue, maxValue);
}

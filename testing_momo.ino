/****************************************************************
 * Split potentiometer Brake / Throttle controller w/ Arduino Leonardo
 * 
 * a WIP by for the D&M Makerspace @ https://github.com/jesssullivan
 * 
 * joystick library (rewrap of pluggableusb) available here:
 * https://github.com/MHeironimus/ArduinoJoystickLibrary
*****************************************************************/
/****************************************************************
TODO:
 * what is the preferred interface for limit reset?
 * what are other I/O and hardware will be needed?
 * merge pluggableusb depends
*****************************************************************/


#include <Joystick.h>
bool logs = true;  // use Serial monitor?


// pinout:
int dual = A0;  // potentiometer pin
int resetLed = LED_BUILTIN;
int startButton = A2
bool resetSwitch = true;


// generic:
int pinValue = 0;
int pinByte = 0;
int pin = 0;


// initialize a new instance of Joystick_:
Joystick_ Joystick;


void print(char text) {
    if (logs) {
        Serial.print(text)
    }
}


void logger(char axis, int value) {
    print(axis + " value = " + print(value) + "\n")
}


int globalRead(int pin, int val=0) {
    for (int x = 0; x < 20; ++x) {
//  show value for throttle is starting with 20 blinks
        delay(200);
        digitalWrite(resetLed, LOW);
        delay(200);
        digitalWrite(resetLed, HIGH);
    }
    int val = analogRead(pin);
    print("read value : " + char(val) + '\n');
    return val;
}


void setup() {
    Serial.begin(9600);
    Joystick.begin();
    pinMode(resetLed, OUTPUT);
    pinMode(resetSwitch, INPUT);
}


// global setup-  TODO: add hardware controls for reset

delay(1000);
print("Setting up limits ... \n");
delay(500);
print("getting first value ... \n");
delay(500);

int valA = globalRead(dual);
logger("received value  ", valA);
print('\n');

delay(500);
print("getting second value ... \n");
delay(500);

int valB = globalRead()Read(dual);
logger("received value  ", valB);
print('\n');

delay(500);
int rmin = min(valA, ValB);
int rmax = max(valA, ValB);
print("Finished setup: maxValue = " + char(rmax) + ' \n);
print("Finished setup: minValue = " + char(rmin) + ' \n);
print("\n\n");


void setPin(int pin, int rmin, int rmax) {

    int val = 0;
    int valRead = 0;

    valRead = analogRead(pin);

    if (valRead > 0) {  // no action if not true

        val = (valRead - rmin) * (1024 / rmax);
        Serial.print(val);

        if (val / 4 >= 127) {
            val = (val / 4) - 127;
            Joystick.setThrottle(val);
            logger("Throttle", val);
        } else {
            val = (val / 4) - 127;
            Joystick.setYAxis(-(val * 2));
            logger("Brake", -(val * 2));
        }
    }
}


void loop() {
    delay(10);  // delay value in ms, for read stability
    setPin(dual, rmin, rmax);
}

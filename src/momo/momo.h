///////////////////////////////////////////////////////////////
// Split potentiometer Brake / Throttle controller w/ Arduino Leonardo
//
// a WIP by for the D&M Makerspace @ https://github.com/jesssullivan
//
// joystick library (rewrap of pluggableusb) available here:
// https://github.com/MHeironimus/ArduinoJoystickLibrary
//
// compile with:
// arduino --upload momo/momo.ino
///////////////////////////////////////////////////////////////

#include <Joystick.h>
#include <EEPROM.h>  // only used for on the fly limit adjustment

// use Arduino Serial monitor?
bool logs = true;

// use additional reset hardware for on the fly limit adjustment?
bool variableLimits = false;

// sensor rotation in degrees (most potentiometers are 270 degrees)
int sensRotation = 360;

// pinout:
int dual = 0;  // potentiometer pin
int encA = 1;  // encoder first bit pin
int encB = 2;  // encoder alt bit pin
// int resetPin = 1;  // if using on the fly variable adjustment
//int resetLED = 2; // if using on the fly variable adjustment

// "throttle" limit pinout- digitalread(), default is 0
int t1 = 3;
int t1degrees = 15;
int t2 = 4;
int t2degrees = 30;
int t3 = 5;
int t3degrees = 45;

// "brake" limits:
int b1 = 6;
int b1degrees = 15;
int b2 = 7;
int b2degrees = 30;
int b3 = 8;
int b3degrees = 45;

// initialize a new instance of Joystick_:
Joystick_ Joystick;

// init global variables:
float rmin = 0;
float rmax = 0;
float val = 0;
int rminADDR = 1;   // if using on the fly variable adjustment
int rmaxADDR = 2;   // if using on the fly variable adjustment


void lprint(String text) {
    if (logs) {
        Serial.print(text);  // arduino type "String" is only for Serial.print()
    }
}

void indicateBlink() {
    for (int x = 0; x < 20; ++x) {
        delay(400);
        digitalWrite(resetLED, LOW);
        delay(400);
        digitalWrite(resetLED, HIGH);
    }
}

void setPin(float pin, float rmin, float rmax) {
    delay(1);
    float valRead = analogRead(pin);
    if (valRead >= rmax) {
      Joystick.setThrottle(254);
      lprint("Throttle = 254 \n");
        exit(0);
    } else if (valRead <= rmin) {
      Joystick.setYAxis(val);
      lprint("Brake = 254 \n");
      exit(0);
    }
    val = val < 0 ? -val : val;  // gets absolute value
    float val = ((valRead - rmin) / (rmax - rmin)) * 254;
    if (valRead >= (rmax - rmin) / 2) {
        Joystick.setThrottle(val);
        lprint("Throttle = " + String(val) + "\n");
    } else {
        val = ((rmax - rmin) / 2) - val;  // reverses range
        Joystick.setYAxis(val);
        lprint("Brake = " + String(val) + "\n");
    }
}

void setEncoder() {
    // todo- wip!
    int a = digitalRead(encA);
    int b = digitalRead(ncB);
    if (a != pstate) {
        int pstate = a^b ? pos-- : pos++;
       // todo- send pstate to 8 bit serial
    }
}

// FOR VARIABLE LIMITS ONLY:
int globalRead(int pin) {
    val = 0;
    indicateBlink();
    val = analogRead(pin);
    return int(val);
}

// if using a resettable, variable ratio
void resetMaxMin() {

    int valA = 0;
    int valB = 0;
    digitalWrite(resetLED, HIGH);
    delay(400);

    indicateBlink();

    lprint("Setting up limits ... \n");
    lprint("\n");
    delay(200);

    lprint("getting first value ... \n");
    lprint("\n");
    delay(500);

    valA = globalRead(dual);
    lprint("received value  " + String(valA));
    lprint("\n\n");

    indicateBlink();

    delay(500);
    lprint("getting second value ... \n");
    lprint("\n");
    delay(500);

    valB = globalRead(dual);
    lprint("received value  " + String(valB));
    lprint("\n\n");

    rmin = (valA > valB) ? valB : valA;
    rmax = (valA < valB) ? valB : valA;

    lprint("Finished setup: maxValue = " + String(rmax) + " \n");
    lprint("\n");
    lprint("Finished setup: minValue = " + String(rmin) + " \n");
    lprint("\n");
    lprint("Writing to EEPROM ... \n\n  ...  \n\n  ... \n\n");
    EEPROM.write(rminADDR, rmin);
    delay(100);
    lprint("  ...  \n\n  ... \n\n");
    EEPROM.write(rmaxADDR, rmax);
    lprint("Reset Complete!  :) \n\n");
    delay(100);
}


void setup() {

    if (variableLimits) {
        pinMode(resetPin, INPUT);
        pinMode(resetLED, INPUT);

        if (EEPROM.read(rmaxADDR) > 100) {
            rmin = EEPROM.read(rminADDR);
            rmax = EEPROM.read(rmaxADDR);
        }
    }

    for (int i = t1; i <= b3; i++) {
        pinMode(i, INPUT);
        int deg10bit = 1024 / sensRotation;
        if (digitalRead(i)) {
            // sets brake direction
            if (i == b1) { rmin = 512 - (deg10bit * b1degrees); }
            if (i == b2) { rmin = 512 - (deg10bit * b2degrees); }
            if (i == b3) { rmin = 512 - (deg10bit * b3degrees); }
            // sets throttle direction
            if (i == t1) { rmax = 512 + (deg10bit * t1degrees); }
            if (i == t2) { rmax = 512 + (deg10bit * t2degrees); }
            if (i == t3) { rmax = 512 + (deg10bit * t2degrees); }
    }
    if (logs) { Serial.begin(9600); }
    Joystick.begin();
    delay(4);
    }
}


void loop() {
    delay(4);  // delay value in ms, for read stability
    if (variableLimits) {
        if (resetPin) {
            resetMaxMin();
        }
    }
    setPin(dual, rmin, rmax);
}

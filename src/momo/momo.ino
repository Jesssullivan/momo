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
#include <EEPROM.h>

// use Arduino Serial monitor?
bool logs = true;

// use additional reset hardware for on the fly limit adjustment?
bool variableLimits = false;

// pinout:
#define dual 0  // potentiometer pin
#define resetPin 1  // if using on the fly variable adjustment
#define resetLED 2 // if using on the fly variable adjustment

// "throttle" limit pinout- digitalread(), default is 0
#define t1 3  // +15
#define t2 4  // +30
#define t3 5  // +45

// "brake" limits:
#define b1 6  // -15
#define b2 7  // -30
#define b3 8  // -45

// initialize a new instance of Joystick_:
Joystick_ Joystick;

// init global variables:
float rmin = 0;
float rmax = 0;
float val = 0;
int rminADDR = 1;
int rmaxADDR = 2;

// HARDWARE LIMITS:
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

int setPin(float pin, float rmin, float rmax) {
    
    delay(2);
    
    float valRead = analogRead(pin);
    
    if (valRead >= rmax) { 
      Joystick.setThrottle(254);
      lprint("Throttle = 254 \n");
      return 1;
    }

    if (valRead <= rmin) { 
      Joystick.setYAxis(val);
      lprint("Brake = 254 \n");
      return 1;
    }
    
    float val = (valRead - rmin) / (rmax - rmin) * 254;
    val = val < 0 ? -val : val; 
    
    if (val >= (rmax - rmin) / 2) {  // todo- setup detent //
        Joystick.setThrottle(val);
        lprint("Throttle = " + String(val) + "\n");
    } else {
        val = ((rmax -rmin) / 2) - val;
        Joystick.setYAxis(val);
        lprint("Brake = " + String(val) + "\n");
    }
    return 1;
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
        if (digitalRead(i)) {
            // following the lines of ``` 512 -/+ (512 / (270 / 45)) ```
            if (i == b1) { rmin = 426; }  // -45 degrees
            if (i == b2) { rmin = 398; }  // -60 degrees
            if (i == b3) { rmin = 369; }  // -75 degrees
            // set throttle:
            if (i == t1) { rmax = 597; }  // +45 degrees
            if (i == t2) { rmax = 625; }  // +60 degrees
            if (i == t3) { rmax = 645; }  // +75 degrees
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

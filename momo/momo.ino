/****************************************************************
 * Split potentiometer Brake / Throttle controller w/ Arduino Leonardo
 *
 * a WIP by for the D&M Makerspace @ https://github.com/jesssullivan
 *
 * joystick library (rewrap of pluggableusb) available here:
 * https://github.com/MHeironimus/ArduinoJoystickLibrary
 *
 * compile with:
 * arduino --upload momo.ino
*****************************************************************/

#include <EEPROM.h>  // using leonardo only for now (attiny, etc later)
#include <Joystick.h>

bool logs = true;  // use Serial monitor?
bool variableLimits = false;  // use reset hardware for on the fly limit adjustment?

// pinout:
int dual = 0;  // potentiometer pin
int resetPIN = 1;  // only used if using variable limits
int multiLED = 2;  // only used if using variable limits

// "throttle" limits:
int t1 = 3;  // +45
int t2 = 4;  // +60
int t3 = 5;  // +75

// "brake" limits:
int b1 = 6;  // -45
int b2 = 7;  // -60
int b3 = 8;  // -75

// generic max / min:
int rmin = 0;
int rmax = 0;
int rminADDR = 0;
int rmaxADDR = 1;


// initialize a new instance of Joystick_:
Joystick_ Joystick;


void lprint(String text) {
    if (logs) {
        Serial.print(String(text));
    }
}


void logger(String axis, int value) {
    lprint(axis + " value = " + String(value));
    lprint("\n");
}


void indicateBlink() {
    for (int x = 0; x < 20; ++x) {
        delay(400);
        digitalWrite(multiLED, LOW);
        delay(400);
        digitalWrite(multiLED, HIGH);
    }
}


int globalRead(int pin) {
    int val = 0;
    indicateBlink();
    val = analogRead(pin);
    return int(val);
}

int getLimit(int pinMin, int pinMax) {
    for (int i = pinMin; i <= pinMax; i++) {
        if (digitalRead(i)) {
            return i;
        }
    }   // TODO: error handling-
}       // if not detected- fault light?  what is the preferred behavior?


void setPin(int pin, int rmin, int rmax) {

    int val = 0;
    int valRead = analogRead(pin);

    if (valRead > 0) {
        // translate valRead to limits
        val = (valRead - rmin) * (1024 / rmax);

        if (val / 4 >= 127) {
            val = ((val / 4) - 127) * 2;
            val = (val > 254) ? 254 : val;
            Joystick.setThrottle(val);
            logger("Throttle", val);
        } else {
            val = ((val / 4) - 127) * 2;
            val = (val > 254) ? 254 : val;
            Joystick.setYAxis(-(val));
            logger("Brake", -(val));
        }
    } else {
        Joystick.setThrottle(0);
        Joystick.setYAxis(-(val));
    }
}

// if using a resettable, variable ratio:
void resetMaxMin() {

    int valA = 0;
    int valB = 0;
    digitalWrite(multiLED, HIGH);
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

// RUN:

void setup() {

    if (variableLimits) {
        int resetPIN = 1;
        int multiLED = 2;
    } else {
        // set conductive hardware limit inputs:
        for (int i = t1; i <= b3; i++) {
            pinMode(i, INPUT);
        }
    }

        int blimit = getLimit(b1, b3);
        int tlimit = getLimit(t1, t3);

        for (int lim = blimit; lim <= tlimit; lim++) {
            // set brake:
            // following the lines of ``` 512 -/+ (512 / (270 / 45)) ```
            if (lim == b1) { rmin = 426; }  // -45 degrees
            if (lim == b2) { rmin = 398; }  // -60 degrees
            if (lim == b3) { rmin = 369; }  // -75 degrees
            // set throttle:
            if (lim == b1) { rmax = 597; }  // +45 degrees
            if (lim == b2) { rmax = 625; }  // +60 degrees
            if (lim == b3) { rmax = 645; }  // +75 degrees

        EEPROM.write(rminADDR, rmin);
        EEPROM.write(rmaxADDR, rmax);
        delay(4);
    }

    if (logs) { Serial.begin(9600); }

    Joystick.begin();
}

void loop() {

    delay(2);  // delay value in ms, for read stability

    if (variableLimits) {  // if using variable limits, checks if reset button is pressed:
        if (resetPIN) {
            resetMaxMin();
        }
    } else {  // default behavior
        delay(2);  // delay value in ms, for read stability
        setPin(dual, rmin, rmax);
    }
}

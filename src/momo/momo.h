///////////////////////////////////////////////////////////////
// Adaptive brake / throttle controller w/ Arduino Leonardo
//
// a WIP by for the D&M Makerspace @ https://github.com/jesssullivan
//
// joystick library (rewrap of pluggableusb) available here:
// https://github.com/MHeironimus/ArduinoJoystickLibrary
//
// compile with:
// arduino --upload momo/momo.ino
///////////////////////////////////////////////////////////////

#include <Joystick.h>  // https://github.com/MHeironimus/ArduinoJoystickLibrary
#include <Encoder.h> // @ pjrc: http://www.pjrc.com/teensy/td_libs_Encoder.html

// sensor type - encoder or potentiometer?
bool encoder = true;

// use Arduino Serial monitor?
bool logs = true;

// use additional reset hardware for on the fly limit adjustment?
bool variableLimits = false;

// potentiometer rotation in degrees (most potentiometers are 270 degrees)
int potRotation = 360;

// encoder- phase cycles per rotation:
int encRes = 600;

// pinout:
int dual = 0;  // potentiometer pin
int encA = 9;  // encoder first phase pin
int encB = 10;  // encoder alt phase pin
Encoder enc(encA, encB);

int resetPin = -999;  // if using on the fly variable adjustment
int resetLED = -999; // if using on the fly variable adjustment

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
long pos  = -999;
long restingValue;
long tvalue = 0;
unsigned long time = millis();

void lprint(String text) {
    if (millis() - time >= 1200) {
        if (logs) {
            Serial.print(text);  // arduino type "String" is only for Serial.print()
            Serial.print("\n");
        }
        time = millis();
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
    delay(1);
    float valRead = analogRead(pin);
    if (valRead >= rmax) {
        Joystick.setThrottle(254);
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
    tvalue = enc.read() / 4;
    if (tvalue != restingValue) { restingValue = tvalue; }
    if (tvalue <= 0) {
        tvalue = tvalue < 0 ? -tvalue : tvalue;  // gets absolute value
        Joystick.setThrottle(tvalue);
        lprint("Throttle = " + String(tvalue));
    } else {
        tvalue = tvalue < 0 ? -tvalue : tvalue;  // gets absolute value
        Joystick.setYAxis(tvalue);
        lprint("Brake = " + String(tvalue));
    }
}

int globalRead(int pin) {  // for variable limits
    val = 0;
    indicateBlink();
    val = analogRead(pin);
    return int(val);
}

// if using a resettable, variable ratio:
void resetMaxMin() {
    int valA = 0;
    int valB = 0;
    digitalWrite(resetLED, HIGH);
    delay(400);
    // start communicating with user:
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
    // set brake / throttle based on value:
    rmin = (valA > valB) ? valB : valA;
    rmax = (valA < valB) ? valB : valA;
    lprint("Finished setup: maxValue = " + String(rmax) + " \n");
    lprint(" ... \n");
    lprint("Finished setup: minValue = " + String(rmin) + " \n");
    lprint(" ... \n");
    delay(100);
    lprint("Reset Complete!  :) \n\n");
    delay(100);
}

void setup() {
    if (variableLimits) {
        pinMode(resetPin, INPUT);
        pinMode(resetLED, INPUT);
        delay(10);
        resetMaxMin();  // set reset right away without need for EEPROM i/o
        delay(10);
    } else {
        for (int i = t1; i <= b3; i++) {
            pinMode(i, INPUT);
        }
    }
    if (logs) { Serial.begin(9600); }
    if (! encoder) {
        Joystick.begin();
        delay(4);
    }
    int deg10bit = 1024 / potRotation;
    for (int i = t1; i <= b3; i++) {
        if (digitalRead(i)) {
            // sets brake direction
            if (i == b1) { if (! encoder) { rmin = 512 - (deg10bit * b1degrees); } else { rmin = b1degrees; }}
            if (i == b2) { if (! encoder) { rmin = 512 - (deg10bit * b2degrees); } else { rmin = b2degrees; }}
            if (i == b3) { if (! encoder) { rmin = 512 - (deg10bit * b2degrees); } else { rmin = b3degrees; }}
            // sets throttle direction
            if (i == t1) { if (! encoder) { rmin = 512 + (deg10bit * b1degrees); } else { rmax = t1degrees; }}
            if (i == t2) { if (! encoder) { rmin = 512 + (deg10bit * b2degrees); } else { rmax = t2degrees; }}
            if (i == t3) { if (! encoder) { rmin = 512 + (deg10bit * b3degrees); } else { rmax = t3degrees; }}
        }
    }
}


void loop() {
    if (variableLimits) {
        if (resetPin) {
            resetMaxMin();
        }
    }
    if (encoder) {
        setEncoder();
    } else {
        setPin(dual, rmin, rmax);
    }
}

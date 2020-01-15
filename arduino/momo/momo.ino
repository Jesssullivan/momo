/****************************************************************
 * Split potentiometer Brake / Throttle controller w/ Arduino Leonardo
 * 
 * a WIP by for the D&M Makerspace @ https://github.com/jesssullivan
 * 
 * joystick library (rewrap of pluggableusb) available here:
 * https://github.com/MHeironimus/ArduinoJoystickLibrary

 * additional hardware includes:
 * potentiometer to split (P1)
    * hardwired power LED + resistor (L1, R1)
    * multiLED signals reset status (L2)
    * resetPIN- on press: (S1)
           *  blink for min ...
           *  blink for max ...
           *  show store to eeprom ...
           *  restart serial output

 * compile with:
 * arduino --upload arduino/momo/momo.ino

*****************************************************************/

#include <EEPROM.h>  // using leonardo only for now (attiny, etc later)
#include <Joystick.h>

int logs = 1;  // use Serial monitor?

// pinout:
int dual = 0;  // potentiometer pin
int resetPIN = 1;
int multiLED = 2;

// generic:
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


void setPin(int pin, int rmin, int rmax) {

    int val = 0;
    int valRead = analogRead(pin);

    if (valRead > 0) {  // no action if not true

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
    }
}


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
    Serial.begin(9600);
    Joystick.begin();
    pinMode(multiLED, OUTPUT);
    pinMode(resetPIN, INPUT);
}


void loop() {
    delay(5);  // delay value in ms, for read stability
    if (resetPIN) {
        resetMaxMin();
    }
    delay(5);  // delay value in ms, for read stability
    setPin(dual, rmin, rmax);
}

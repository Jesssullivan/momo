//////////////////////////////////////////////////////////////////////////
// Adaptive brake / throttle controller w/ Arduino Leonardo
// a WIP by for the D&M Makerspace @ https://github.com/jesssullivan
//////////////////////////////////////////////////////////////////////////
// joystick library (rewrap of pluggableusb) available here:
// https://github.com/MHeironimus/ArduinoJoystickLibrary
//
// pjrc's excellent encoder library available here:
// http://www.pjrc.com/teensy/td_libs_Encoder.html
//
// compile with:
// arduino --upload utb.ino
//////////////////////////////////////////////////////////////////////////

#include <Joystick.h>
#include <Encoder.h>

// pinout:
int encA = 8;  // encoder's phase 1
int encB = 9;  // encoder's phase 2
int buttonLR = 10;  // momentary switch toggles lever orientation, reverses throttle / brake
int ledL = 11;  // lever direction is Left LED
int ledR = 12;  // lever direction is Right LED

// use Serial monitor?
bool logs = true;

// set default lever direction- if false, lever starts oriented facing right (CCW == throttle, CW == brake)
bool isLeft = false;

// resting position variance in encoder steps:
int variance = 22;

// multiplier for encoder value- larger value == more sensitive
float mx = 1.6;

// initialize a new instance of Joystick_:
Joystick_ Joystick;

// initialize encoder pins:
Encoder enc(encA, encB);

// serial logging:

unsigned long time_a = millis();

void lprint(String text) {
    if (logs & time_a - millis() >= 500) {  // 500 ms log interval
        Serial.print(text + '\n');
        time_a = millis();
    }
}

// read encoder, reverse if needed:
float readLR(float switchLR) {
    if (digitalRead(buttonLR)) {
        isLeft = !isLeft;
        lprint('switchLR pressed! \n' + 'isLeft value = ' + String(isLeft))
        return switchLR < 0 ? -switchLR : switchLR;
    } else {
        return switchLR;
    }
}

// actions when in resting position:
void resting() {
    Joystick.setThrottle(0);
    Joystick.setYAxis(0);
    lprint("Resting... \n");
}

// actions for throttle:
void throttle(auto cuRead) {
    auto cuRead_mx = (cuRead * mx) >= 256 ? 256 : (cuRead * mx); // gets abs. value
    lprint("Throttle = " + String(cuRead_mx) + "\n");
    Joystick.setThrottle(cuRead_mx);
}

// actions for brake:
void brake(auto cuRead) {
    auto cuRead_mx = (cuRead * mx) >= 256 ? 256 : (cuRead * mx); // gets abs. value
    lprint("Brake = " + String(cuRead_mx) + "\n");
    Joystick.setYAxis(cuRead_mx);
}

// initialize encoder step counter:
// type is float to avoid previous rounding errors- todo use smaller type

float encRead;

void encMain() {
    encRead = readLR(enc.read());
    if ((encRead < 0 ? -encRead : encRead) >= variance) {
        // positive value means lever is moved CCW / "Left"
        if (encRead > 0) {
            throttle(encRead);
        } else {
            brake(encRead);
        }
    } else {
        resting();
    }
}

void setup() {
    // setup pins:
    pinMode(encA, INPUT); // not using built in pullup resistors
    pinMode(encB, INPUT); // not using built in pullup resistors
    pinMode(buttonLR, INPUT);  // switchs lever facing left or right to driver
    digitalWrite(ledL, LOW);  // default lever facing right
    digitalWrite(ledR, HIGH); // default lever facing right
    Joystick.begin();
    Serial.begin(9600);
}

void loop() {
    encMain();
}

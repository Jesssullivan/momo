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

// lever value:
float encRead;

// absolute lever value:
float absRead;

// set default lever direction- if false, lever starts oriented facing right (CCW == throttle, CW == brake)
bool isLeft;

// initialize a new instance of Joystick_:
Joystick_ Joystick;

// initialize encoder pins:
Encoder enc(encA, encB);

// serial logging:
void lprint(String text) {
    if (logs) {
        Serial.print(text + '\n');
    }
    else {
        return;
    }
}

// read encoder, reverse if needed:
bool readLR() {
    if (digitalRead(buttonLR)) {
        lprint('switchLR pressed! \n' + 'isLeft value = ' + String(isLeft));
        // reassign leds.  todo drink coffee --> this should be ternary!
        if (digitalRead(ledL) == LOW) {
            digitalWrite(ledL, HIGH);
            digitalWrite(ledR, LOW);
        } else {
            digitalWrite(ledL, LOW);
            digitalWrite(ledR, HIGH);
        }
        // new isLeft value is to be assigned to itself... todo is this way too obtuse
        return !isLeft;
    }
}

// initialize encoder step counter:
// type is float to avoid previous rounding errors- todo use smaller type

void setup() {
    // setup pins:
    pinMode(encA, INPUT_PULLUP);
    pinMode(encB, INPUT_PULLUP);
    pinMode(buttonLR, INPUT);  // switchs lever facing left or right to driver
    pinMode(ledL, OUTPUT);
    pinMode(ledR, OUTPUT);
    digitalWrite(ledL, LOW);  // default lever facing right
    digitalWrite(ledR, HIGH); // default lever facing right
    // starting L/R lever value:
    isLeft = true;
    if (logs) {
        Serial.begin(9600);
    }
    else {
        Joystick.begin();
    }
}

//Joystick_ Joystick;
long cuRead  = -999;

void loop() {

    // read encoder:
    long encRead;
    encRead = enc.read();

    // get absolute encoder value:
    long absRead = encRead < 0 ? -encRead : encRead;

    if (absRead != cuRead) {
        //lprint("val = " + String(absRead) + "\n");
        if (absRead >= 22) {
            if (encRead >= 0) {
                if (absRead * mx <= 256) {
                    lprint("Throttle = " + String(absRead));
                    Joystick.setThrottle(absRead);
                }
                else if (absRead * mx > 256) {
                    lprint("Throttle = 256 \n");
                    Joystick.setThrottle(256);
                }
            }
            else if (encRead < 0) {
                if (absRead * mx <= 256) {
                    lprint("Brake = " + String(absRead));
                    Joystick.setYAxis(absRead);
                }
                else if (absRead * mx > 256) {
                    lprint("Brake = 256");
                    Joystick.setYAxis(256);
                }
            }
        }
        cuRead = absRead;
    }
}


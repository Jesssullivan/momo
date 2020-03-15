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

// timers:
long time_a = millis();
long time_btn = millis();

// serial logging:
void lprint(String text) {
    if (logs & time_a - millis() >= T_lprint) {
        Serial.print(text + '\n');
        time_a = millis();
    }
}

// read encoder, reverse if needed:
bool readLR() {
    if (time_btn - millis() >= 5) {
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
            // update button timer:
            time_btn = millis();
            // new isLeft value is to be assigned to itself... todo is this way too obtuse
            return !isLeft;
        }
    }
}

// actions when in resting position:
void resting() {
    lprint("Resting... \n");
    Joystick.setThrottle(0);
    Joystick.setYAxis(0);
}

// actions for throttle:
void throttle(float absRead) {
    if (!(absRead * mx) >= 256) {
        lprint("Throttle = " + String(encRead) + "\n");
        Joystick.setThrottle(encRead);
        return;
    }
    else {
        lprint("Throttle = 256 \n");
        Joystick.setThrottle(256);
    }
}

// actions for brake:
void brake(float absRead) {
    if (!(absRead * mx) >= 256) {
        lprint("Brake = " + String(encRead) + "\n");
        Joystick.setYAxis(encRead);
        return;
    }
    else {
        lprint("Brake = 256 \n");
        Joystick.setYAxis(256);
    }
}

// initialize encoder step counter:
// type is float to avoid previous rounding errors- todo use smaller type

void setup() {
    // setup pins:
    pinMode(encA, INPUT); // not using built in pullup resistors
    pinMode(encB, INPUT); // not using built in pullup resistors
    pinMode(buttonLR, INPUT);  // switchs lever facing left or right to driver
    pinMode(ledL, OUTPUT);
    pinMode(ledR, OUTPUT);
    digitalWrite(ledL, LOW);  // default lever facing right
    digitalWrite(ledR, HIGH); // default lever facing right
    // starting L/R lever value:
    isLeft = true;
    Joystick.begin();
    Serial.begin(9600);
}

void loop() {
    // read encoder:
    encRead = enc.read();

    // get absolute encoder value:
    absRead = encRead < 0 ? -encRead : encRead;

    // check if L/R has to be switched:
    isLeft = readLR();
    if (isLeft) {
        encRead = -encRead;
    }
    if (!absRead <= variance) {
        if (encRead >= 0) {
            throttle(absRead);
        } else {
            brake(absRead);
        }
    } else {
        resting();
    }
}

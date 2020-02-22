//////////////////////////////////////////////////////////////////////////
// Adaptive brake / throttle controller w/ Arduino Leonardo
// a WIP by for the D&M Makerspace @ https://github.com/jesssullivan
//////////////////////////////////////////////////////////////////////////

// initialize a new instance of Joystick_:
Joystick_ Joystick;

// init global variables:
float rmin;
float rmax;
float val;
float valRead;
int deg10bit;
long encRead;
unsigned long time = millis();

// serial logging:
void lprint(String text) {
    if (logs) {
        if (millis() - time >= logWait) {
            Serial.print(text);
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

// potentiometer- reads and sets brake / throttle
void setPot(float pin, float rmin, float rmax) {
    delay(1);
    valRead = analogRead(pin);
    if (valRead >= rmax) {
        Joystick.setThrottle(254);
        lprint("Throttle = 254 \n");
        exit(0);
    } else if (valRead <= rmin) {
        Joystick.setYAxis(254);
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


void stepperReturn(long enc) {
    digitalWrite(S_dir, DIR);  // initial stepper value sets direction
    if (encRead <= 0) {

        for (int i = 1; i <= 4; i++) {
        if (millis() - S_time >= 4) {
            digitalWrite(S_Pin, (i % 2 == 0 ? LOW : HIGH));
        }
    }
}


// encoder- reads and sets brake / throttle
void setEncoder() {
    encRead = enc.read() / 4;  // dividing by 4 may be specific to our (4x oversampled) encoder, YMMV
    if (encRead >= rmax) {
        Joystick.setThrottle(254);
        stepperHold();
        lprint("Throttle = 254 \n");
        exit(0);
    } else if (valRead <= rmin) {
        Joystick.setYAxis(254);
        stepperHold();
        lprint("Brake = 254 \n");
        exit(0);
    }
    if (encRead <= 0) {
        encRead = encRead < 0 ? -encRead : encRead;  // gets absolute value
        Joystick.setThrottle(encRead);
        lprint("Throttle = " + String(encRead));
    } else {
        encRead = encRead < 0 ? -encRead : encRead;  // gets absolute value
        Joystick.setYAxis(encRead);
        lprint("Brake = " + String(encRead));
    }
}

// encoder- set max / min values from hardware constraints:
void encMaxMin() {
    for (int i = t1; i <= b3; i++) {
        pinMode(i, INPUT);  // set each limit point as input
        if (digitalRead(i)) {
            // check, set brake direction:
            if (i == b1) {
                if (! encoder) {
                    rmin = 512 - (deg10bit * b1degrees);
                } else {
                    rmin = b1degrees;
                }
            }
            if (i == b2) {
                if (! encoder) {
                    rmin = 512 - (deg10bit * b2degrees);
                } else {
                    rmin = b2degrees;
                }
            }
            if (i == b3) {
                if (! encoder) {
                    rmin = 512 - (deg10bit * b2degrees);
                } else {
                    rmin = b3degrees;
                }
            }
            // check, set throttle direction
            if (i == t1) {
                if (! encoder) {
                    rmin = 512 + (deg10bit * b1degrees);
                } else {
                    rmax = t1degrees;
                }
            }
            if (i == t2) {
                if (! encoder) {
                    rmin = 512 + (deg10bit * b2degrees);
                } else {
                    rmax = t2degrees;
                }
            }
            if (i == t3) {
                if (! encoder) {
                    rmin = 512 + (deg10bit * b3degrees);
                } else { rmax = t3degrees;
                }
            }
        }
    }
}

// potentiometer- if using a resettable, variable ratio:
int globalRead(int pin) {
    val = 0;
    indicateBlink();
    val = analogRead(pin);
    return int(val);
}

// potentiometer- preforms a variable ratio reset:
void potMaxMin() {
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

    //  STEPPER INIT:
    unsigned long S_time = millis();

    // setup outputs
    pinMode(S_dir, OUTPUT);
    pinMode(X_step, OUTPUT);

    // microstepping pins
    pinMode(S_M0, OUTPUT);
    pinMode(S_M1, OUTPUT);
    pinMode(S_M2, OUTPUT);

    // EXPLICITLY DISABLING microstepping:
    digitalWrite(S_M1, LOW);
    digitalWrite(S_M1, LOW);
    digitalWrite(S_M2, LOW);

    if (! variableLimits) {
        encMaxMin();
    } else {
        pinMode(resetPin, INPUT);
        pinMode(resetLED, INPUT);
        delay(10);
        potMaxMin();  // resets right away to avoid EEPROM i/o
        delay(10);
    }
    if (logs) {
        Serial.begin(9600);
    }
    int deg10bit = 1024 / potRotation;  // arduino a/d is usually 10 bit
    Joystick.begin();
}

void loop() {
    if (variableLimits) {
        if (resetPin) {
            potMaxMin();
        }
    }
    if (encoder) {
        setEncoder();
    } else {
        setPot(dual, rmin, rmax);
    }
}

//////////////////////////////////////////////////////////////////////////
// Adaptive brake / throttle controller w/ Arduino Leonardo
// a WIP by for the D&M Makerspace @ https://github.com/jesssullivan
//////////////////////////////////////////////////////////////////////////

// microstepping arduino pin connections:
int S_M0 9
#define S_M1 10
#define S_M2 11

// std. NEMA 17 motor definition
#define rev 200;

void setup() {
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
}

// timers
unsigned long X_time = millis();
unsigned long Y_time = millis();

void loop()
{
    // motor directions
    digitalWrite(X_dir, LOW);
    digitalWrite(Y_dir, dir);
    // pulse motors
    {
        digitalWrite(X_step, LOW);
        if (millis() - X_time > rate)
        {
            digitalWrite(X_step, HIGH);
            X_time = millis();
            HaveWound++;  // counter
        }
        digitalWrite(Y_step, LOW);
        if (millis() - Y_time > rate)
        {
            digitalWrite(Y_step, HIGH);
            Y_time = millis();
        }
        if (HaveWound >= OneWind) {
            HaveWound = 0;  // reset counter
            if (dir == LOW)
            {
                dir = HIGH;
            } else {
                dir = LOW;
            }
        }
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
                    rmin = 512 + (deg10bit * 25);
                } else {
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


void stepperReturn(long enc) {
    digitalWrite(S_dir, DIR);  // initial stepper value sets direction
    if (encRead <= 0) {

        for (int i = 1; i <= 4; i++) {
            if (millis() - S_time >= 4) {
                digitalWrite(S_Pin, (i % 2 == 0 ? LOW : HIGH));
            }
        }
    }

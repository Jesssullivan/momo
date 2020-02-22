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
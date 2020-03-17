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

// initialize a new instance of Joystick_:
Joystick_ Joystick;

// initialize encoder pins:
Encoder enc(encA, encB);

// serial logging:
unsigned long ltimer = millis();
void lprint(String text) {
    if (logs) {
        if (millis() - ltimer >= T_lprint) {
            Serial.print(text + '\n');
            ltimer = millis();
        }
    }
}

// initialize encoder step counter:
// type is float to avoid previous rounding errors- todo use smaller type

void setup() {
    // setup pins:
    pinMode(encA, INPUT_PULLUP);
    pinMode(encB, INPUT_PULLUP);
    pinMode(buttonLR, INPUT);  // switches lever orientation, left or right
    pinMode(ledL, OUTPUT);
    pinMode(ledR, OUTPUT);
    digitalWrite(ledL, LOW);  // default lever facing right
    digitalWrite(ledR, HIGH); // default lever facing right
    // starting L/R lever value:
    if (logs) {
        Serial.begin(9600);
    }
    else {
        Joystick.begin();
    }
}

// lever direction variable:
int isLeft;

// lever orientation button lockout:
unsigned long btn_timer = millis();

///////////////////////////////////////////////////////////////////////

void loop() {

    // get encoder value:
    long encRead = enc.read();

    // get absolute encoder value:
    long absRead = encRead < 0 ? -encRead : encRead;

    // check button:
    if (digitalRead(buttonLR) == HIGH) {
        if (millis() - btn_timer >= T_btn) {
            if (digitalRead(ledL) == LOW) {
                digitalWrite(ledL, HIGH);
                digitalWrite(ledR, LOW);
                isLeft = 1;
            }
            else if (digitalRead(ledL == HIGH)) {
                digitalWrite(ledL, LOW);
                digitalWrite(ledR, HIGH);
                isLeft = 0;
            }
            btn_timer = millis();
        }
    }

    if (isLeft != 1) {
        encRead = -encRead;
    }

    if (absRead >= variance) {
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
}



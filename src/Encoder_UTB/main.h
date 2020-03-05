
#include <Joystick.h>
#include <Encoder.h>

Encoder enc(9, 10);


// use Serial monitor?
bool logs = true;
int logWait = 1500;  // serial monitor sample interval (in milliseconds)

unsigned long time_a = millis();

void lprint(String txt) {
    if (logs) {
        if (millis() - time_a >= 500) {
            Serial.print(txt);
            Serial.print("\n");
        }
        time_a = millis();
    }
}

void setup() {
    Serial.begin(9600);
}

long oldPosition  = -999;

void loop() {
    long newPosition = enc.read();
    if (newPosition != oldPosition) {
        oldPosition = newPosition;
        lprint(String(newPosition));
    }
}

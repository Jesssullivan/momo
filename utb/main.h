
#include <Joystick.h>
#include <Encoder.h>

Encoder enc(8, 9);


// use Serial monitor?
bool logs = true;

unsigned long time_a = millis();

void setup() {
    Serial.begin(9600);
}

// serial logging:
void lprint(String text) {
    if (logs) {
        if (time_a - millis() >= 500) {
            Serial.print(text);
            Serial.print("\n");
        }
        time_a = millis();
    }
}

float oldPosition;

void loop() {
    float newPosition;
    newPosition = enc.read();
    if (newPosition != oldPosition) {
        lprint(String(newPosition));
        oldPosition = newPosition;
    }
    //Serial.print(newPosition);
    lprint(String(newPosition));
}

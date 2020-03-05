
#include <Joystick.h>
#include <Encoder.h>
Joystick_ Joystick;

Encoder enc(8, 9);

// use Serial monitor?
bool logs = false;

// read detent:
float variance = 22;

// serial logging:
unsigned long time_a = millis();
void lprint(String text) {
    if (logs) {
        if (time_a - millis() >= 500) {
            Serial.print(text);
            Serial.print("\n");
        }
        time_a = millis();
    }
}

float cuRead;

void setEncoder() {
    float encRead = enc.read();
    cuRead = encRead != cuRead ? encRead : cuRead;
        if (cuRead > 0) {
            if (cuRead >= variance) {
                float cuRead_mx = cuRead * 1.5;
                cuRead_mx = cuRead_mx >= 256 ? 256 : cuRead_mx;
                lprint("Throttle = " + String(cuRead_mx) + "\n");
                Joystick.setThrottle(cuRead_mx);
                } else {
                    Joystick.setThrottle(0);
                    Joystick.setYAxis(0);
                    lprint("Resting... \n");
                }
        } else {
            cuRead = cuRead < 0 ? -encRead : encRead;
            if (cuRead >= variance) {
                float cuRead_mx = cuRead * 1.6;
                cuRead_mx = cuRead_mx >= 256 ? 256 : cuRead_mx;
                lprint("Brake = " + String(cuRead_mx) + "\n");
                Joystick.setYAxis(cuRead_mx);
            }  else {
                Joystick.setThrottle(0);
                Joystick.setYAxis(0);
                lprint("Resting... \n");
            }
        }
}

void setup() {
    //Joystick_ Joystick;
    Joystick.begin();
    Serial.begin(9600);
}

void loop() {
    setEncoder();
}

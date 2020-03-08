//////////////////////////////////////////////////////////////////////////
// Adaptive brake / throttle controller w/ Arduino Leonardo
// a WIP by for the D&M Makerspace @ https://github.com/jesssullivan
/////////////////////////////////////////////////////////////////////////

// sensor type - encoder or potentiometer?
bool encoder = true;

// use Serial monitor?
bool logs = true;
int logWait = 1500;  // serial monitor sample interval (in milliseconds)

// use additional reset hardware for on the fly limit adjustment?
bool variableLimits = false;

// potentiometer rotation
// in degrees (most potentiometers are 270 degrees)
int potRotation = 360;

// encoder- phase cycles per rotation:
int encRes = 600;

// pinout:
int dual = 0;  // potentiometer pin
int encA = 9;  // encoder first phase pin
int encB = 10;  // encoder alt phase pin
Encoder enc(encA, encB);  // declare as encoder pins using pjrc's library


Joystick_ Joystick;

// init global variables:
float rmin;
float rmax;
float val;
float valRead;
int deg10bit;
long encRead;
unsigned long time_a = millis();

// serial logging:
void lprint(String text) {
    if (logs) {
        if (millis() - time_a >= logWait) {
            Serial.print(text);
            Serial.print("\n");
        }
        time_a = millis();
    }
}


void setup() {
    float rmin = 512 + (deg10bit * 25);
    float rmax = 25;
    // initialize a new instance of Joystick_:
    Joystick_ Joystick;
    Serial.begin(9600);
    Joystick.begin();
}

void loop() {
    setEncoder();
}

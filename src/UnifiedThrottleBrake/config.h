//////////////////////////////////////////////////////////////////////////
// Adaptive brake / throttle controller w/ Arduino Leonardo
// a WIP by for the D&M Makerspace @ https://github.com/jesssullivan
//
// compile with:
// arduino --upload UnifiedThrottleBrake/UnifiedThrottleBrake.ino
//////////////////////////////////////////////////////////////////////////
//  configure hardware, pinout, etc:
//////////////////////////////////////////////////////////////////////////

// sensor type - encoder or potentiometer?
bool encoder = true;

// use Arduino Serial monitor?
bool logs = false;
int logWait = 1500;  // serlial monitor sample interval (in milliseconds)

// use additional reset hardware for on the fly limit adjustment?
bool variableLimits = false;

// potentiometer rotation in degrees (most potentiometers are 270 degrees)
int potRotation = 360;

// encoder- phase cycles per rotation:
int encRes = 600;

// pinout:
int dual = 0;  // potentiometer pin
int encA = 9;  // encoder first phase pin
int encB = 10;  // encoder alt phase pin

Encoder enc(encA, encB);  // declare as encoder pins using pjrc's library

int resetPin = 1;  // if using on the fly variable adjustment
int resetLED = 2; // if using on the fly variable adjustment

// "throttle" limit pinout- digitalread(), default is 0
int t1 = 3;
int t1degrees = 15;
int t2 = 4;
int t2degrees = 30;
int t3 = 5;
int t3degrees = 45;

// "brake" limits:
int b1 = 6;
int b1degrees = 15;
int b2 = 7;
int b2degrees = 30;
int b3 = 8;
int b3degrees = 45;

// DRV + stepper pinout:
int S_dir = 6
int S_pin = 7
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

// pinout:
int encA = 9;  // encoder's phase 1
int encB = 10;  // encoder's phase 2
int buttonLR = 8;  // momentary switch toggles lever orientation, reverses throttle / brake
int ledL = 11;  // lever direction is Left LED
int ledR = 12;  // lever direction is Right LED

// use Serial monitor?
bool logs = true;

// resting position variance in encoder steps:
float variance = 22;

// multiplier for encoder value- larger value == more sensitive
float mx = 1.6;

// sensitivity via timers:
long T_lprint = 150; // logging frequency in ms
long T_btn = 1000;  // button sensitivity in ms

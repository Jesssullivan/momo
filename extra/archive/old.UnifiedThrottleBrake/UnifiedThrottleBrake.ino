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
// arduino --upload UnifiedThrottleBrake/UnifiedThrottleBrake.ino
//////////////////////////////////////////////////////////////////////////

#include <Joystick.h>  // https://github.com/MHeironimus/ArduinoJoystickLibrary
#include <Encoder.h> // @ pjrc: http://www.pjrc.com/teensy/td_libs_Encoder.html

// configuration file:
#include "config.h"

// main sketch:
#include "main.h"

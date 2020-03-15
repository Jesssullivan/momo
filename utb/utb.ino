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

// https://github.com/MHeironimus/ArduinoJoystickLibrary
#include <Joystick.h>

// http://www.pjrc.com/teensy/td_libs_Encoder.html
#include <Encoder.h>

// pinout, configuration:
#include "config.h"

#include "main.h"

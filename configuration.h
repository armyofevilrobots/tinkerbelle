#include <InkShieldMega.h>
#ifndef ARDUINO
  #define ARDUINO = 0
#elif (ARDUINO >= 100)
  #include "Arduino.h"
#elif (ARDUINO == 0)
  //Perhaps a dummy include?
#else
  #include "WProgram.h"
#endif
#pragma once

#ifndef CONFIGURATION_H
#define CONFIGURATION_H
//Motion output pins
#define X_STEP  8
#define X_DIR   9
#define Y_STEP  10
#define Y_DIR   11
#define Z_STEP  12
#define Z_DIR   13
#define Y_STOP  14
#define X_STOP  15
#define Z_STOP  16
#define STEP_FWD 1
#define STEP_REV 0
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
#define MAX_COMMAND_SIZE 80
#define Y_STEPS_MM (200.0*8.0)/(16.0*5.08)
//Jerks are defined in units/s
#define X_JERK 100
#define Y_JERK 1000
#define Z_JERK 100
//Acceleration is reduction in delay between steps
// Serial config
#define SERIAL_PORT 0
#define SERIAL_RATE 57600

//Inkshield stuffs
//#define INKSHIELD_CLASS InkShieldA0A3
#define INKSHIELD_PIN 2
extern InkShieldA0A3 inkshield;
#endif

extern void setup_serial();
extern void setup_pins();

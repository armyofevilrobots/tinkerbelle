#if (ARDUINO >= 100)
  #include "Arduino.h"
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
// Serial config
#define SERIAL_PORT 0
#define SERIAL_RATE 115200
#endif



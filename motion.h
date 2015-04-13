#include "configuration.h"
#pragma once

//Values!

#ifndef MOTION_H
#define MOTION_H
typedef enum {ABSOLUTE, RELATIVE} MotionMode;
extern long pos[3];// = {0, 0, 0};
extern MotionMode motion_mode;
const unsigned int AXIS_STEP_PIN [3] = {X_STEP, Y_STEP, Z_STEP};
const unsigned int AXIS_DIR_PIN  [3] = {X_DIR, Y_DIR, Z_DIR};
const unsigned int AXIS_STOP     [3] = {X_STOP, Y_STOP, Z_STOP};
const unsigned int AXIS_JERK     [3] = {X_JERK, Y_JERK, Z_JERK};

//FUNCTIONS
extern void step(int, int, int);
extern long move_axis(unsigned, long);
extern void zero_to_back_axis(int, int);
#endif

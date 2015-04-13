#include "configuration.h"
#pragma once

#ifndef GCODE_H
#define GCODE_H

typedef struct {
    char cmdchar;
    int cmdcode;
} Gcode;

extern bool G00(long [], bool []);
extern bool G28(long [], bool []);
extern bool G90(long [], bool []);
extern bool G91(long [], bool []);
extern bool G92(long [], bool []);
extern bool M47(long [], bool []); //AT codes....
extern bool M114(long [], bool []);
extern bool M503(long [], bool []);
#endif

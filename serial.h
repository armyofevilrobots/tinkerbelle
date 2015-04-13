#include "configuration.h"
#pragma once

#ifndef SERIAL_H
#define SERIAL_H
void setup_serial();
void setup_pins();
void send_and_wait(const char[]);
void get_cmd();
char * read_cmd();
#endif


#pragma once
#include "gcodes.h"

#ifndef PARSER_H
#define PARSER_H
//char *OK_NUMERIC="0123456789 .";
//char *OK_GCODE="GM";
//char *OK_PARAMS="XYZE";
char gmcode(char []);
char * skip_gmcode(char *);
char * has_coord(char *, char);
void get_coords(char *, char *, long *, bool *);
Gcode get_cmd(char *, char *, long *, bool *);
#endif


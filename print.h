#include <InkShieldMega.h>
#include "configuration.h"
#include "motion.h"

#ifndef PRINTER_H
#define PRINTER_H
extern void print_line(char *, int);
extern unsigned int parse_char_data(char *);
#endif


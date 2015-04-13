#include <InkShieldMega.h>
#include "configuration.h"
#include "print.h"
#include "motion.h"

void print_line(char * data, int dir){
    char * tmp = data;
    int columns=strlen(data)/3;

    for(int i=columns; i; i--){
        step(X_STEP, X_DIR, STEP_FWD);
        delayMicroseconds(1000000/AXIS_JERK[X_AXIS]);
        Serial.println(parse_char_data(tmp), BIN);
        tmp += 3;//
        if ((columns-i)<25) delayMicroseconds(4000 - ((columns-i)*160));
        if (i<25) delayMicroseconds(4000 - (i*160));
    }
}

unsigned int parse_char_data(char * data){
    //Parse a single column, then return the new column (12 bits) uint_16t.
    char digits[4]="";
    memcpy(digits, data, 3);
    return strtol(digits, NULL, 16);
}


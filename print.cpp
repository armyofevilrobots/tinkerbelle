#include "configuration.h"
#include "print.h"


void print_line(){
    for(int i=0;i<600;i++){
        step(X_STEP, X_DIR, STEP_FWD);
        delay(2);
        if (i<25) delayMicroseconds(4000 - (i*160));
        if (i>575) delayMicroseconds(4000 - ((600-i)*160));
        
    }
    for(int i=0;i<600;i++){
        step(X_STEP, X_DIR, STEP_REV);
        delay(2);
        if (i<25) delayMicroseconds(4000 - (i*160));
        if (i>575) delayMicroseconds(4000 - ((600-i)*160));
    }
}


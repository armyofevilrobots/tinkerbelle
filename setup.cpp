#include "configuration.h"


void setup_pins(){
    pinMode(X_STEP, OUTPUT);
    pinMode(X_DIR, OUTPUT);
    pinMode(Y_STEP, OUTPUT);
    pinMode(Y_DIR, OUTPUT);
    pinMode(Z_STEP, OUTPUT);
    pinMode(Z_DIR, OUTPUT);
    pinMode(Y_STOP, INPUT_PULLUP);
}


#include "configuration.h"
#include "gcodes.h"
#include "motion.h"

void G00(char buf[0]){
}

void G28(char buf[]){
    //For now, only Y axis.
    if(strchr(buf, 'Y')){
        Serial.print("Zeroing Axis: ");
        Serial.println(buf);
        zero_to_back_axis(Y_AXIS, (strchr(buf, '-') ? 1 : 0));
    }
}

void G91(char buf[]){
    motion_mode=RELATIVE;
}

void G90(char buf[]){
    motion_mode=ABSOLUTE;
}

void M503(char buf[]){
    Serial.println("Key:\t\t\tValue:");
    Serial.print("Y STEPS/MM\t\t");
    Serial.println(Y_STEPS_MM);
}


void M114(char buf[]){
    Serial.println("Axis:\t\t\tPosition:");
    Serial.print("X units\t\t");
    Serial.println(pos[X_AXIS]);
    Serial.print("Y units\t\t");
    Serial.println(pos[Y_AXIS]);
    Serial.print("Z units\t\t");
    Serial.println(pos[Z_AXIS]);
}

#include <InkShieldMega.h>
#include "configuration.h"
#include "print.h"
#include "gcodes.h"
#include "motion.h"
#include "serial.h"

bool G00(long coords[], bool valid[]){
    for(unsigned int axis=0;axis<3;axis++)
        if(valid[axis]){
            Serial.print("Moving axis ");
            Serial.print(coords[axis]);
            Serial.println(" units.");
            long moved = move_axis(axis, coords[axis]);
        }
    return true;
}


bool G28(long *coords, bool valid[3]){
    for(int i=0;i<3;i++){
        Serial.print("Zeroing Axis: ");
        if(valid[i]){
            zero_to_back_axis(i, 0);
        }//if valid
    }
    return true;
}

bool G90(long *coords, bool *valid){
    motion_mode=ABSOLUTE;
    return true;
}

bool G91(long *coords, bool *valid){
    motion_mode=RELATIVE;
    return true;
}

bool G92(long coords[], bool valid[]){
    for(int i=0;i<3;i++){
        if(valid[i]){
            pos[i]=coords[i];
        }//valid
    }//loop
}

bool G90(long *coords, bool *valid){
    motion_mode=ABSOLUTE;
    return true;
}

unsigned int parse_char_datax(char * data){
    //Parse a single column, then return the new column (12 bits) uint_16t.
    char digits[4]="";
    memcpy(digits, data, 3);
    return strtol(digits, NULL, 16);
}

void print_linex(char * img_data, int foo){

    char * tmp = img_data;
    int columns=strlen(img_data)/3;
    unsigned int column=0;

    for(int i=columns; i; i--){
        step(X_STEP, X_DIR, STEP_FWD);
        delayMicroseconds(1000000/AXIS_JERK[X_AXIS]);
        column=parse_char_datax(tmp);
        Serial.println(column, BIN);
        inkshield.spray_ink(column);
        tmp += 3;//
        if ((columns-i)<25) delayMicroseconds(4000 - ((columns-i)*160));
        if (i<25) delayMicroseconds(4000 - (i*160));
    }
}


bool M47(long coords[], bool valid[]){
    // Complex!!!!
    // This command will initiate a print job.
    // After the initial command, we go into a receive state.
    // That state expects to see a line of characters as hex
    // ie: ffa for 4090
    // Those triples must be a list of printable strips of 12
    // nozzles, which get spat out the nozzle.
    Serial.println("Waiting on image data.");
    char * img_data = read_cmd();
    Serial.println("IMAGE DATA FOLLOWS:");
    Serial.println(img_data);
    print_linex(img_data, 1);
    return true;
    

}

bool M503(long *coords, bool *valid){
    Serial.println("Key:\t\t\tValue:");
    Serial.print("Y STEPS/MM\t\t");
    Serial.println(Y_STEPS_MM);
    return true;
}


bool M114(long *coords, bool *valid){
    Serial.println("Axis:\t\t\tPosition:");
    Serial.print("X units\t\t");
    Serial.println(pos[X_AXIS]);
    Serial.print("Y units\t\t");
    Serial.println(pos[Y_AXIS]);
    Serial.print("Z units\t\t");
    Serial.println(pos[Z_AXIS]);
    return true;
}

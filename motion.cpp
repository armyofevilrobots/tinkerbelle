#include "configuration.h"
//#include "print.h"
#include "motion.h"


long pos[] = {0,0,0};
MotionMode motion_mode = ABSOLUTE;

void step(int axispin, int dirpin, int direction){
    digitalWrite(dirpin, direction);
    digitalWrite(axispin, 1);
    delayMicroseconds(2);
    digitalWrite(axispin, 0);
    //digitalWrite(dirpin, 0);
}

long accel_sleep(int axis){
    //Delay long enough we don't drop steps. Will be smarter and stateful
    //after we get some more work done later on.
    delayMicroseconds(1000000/AXIS_JERK[axis]);
}

long move_axis(unsigned int axis, long int steps){
    int dir=0;
    if (motion_mode==ABSOLUTE){
        steps = steps - pos[axis];//it's the difference stupid!
    }

    if (steps<0){
        steps=-steps;
        dir=0;
    }else{
        dir=1;
    }
    for(int i=0; i<steps; i++){
        step(AXIS_STEP_PIN[axis], AXIS_DIR_PIN[axis], dir);
        if(dir) pos[axis]++;
        else pos[axis]--;
        accel_sleep(axis);//This will be smarter later.
    }

    
}

void zero_to_back_axis(int axis, int reverse){
 //Zero the Y
 //First we pull away from the endstop...
  int attempts = 0;
  int stepdir = (reverse ? STEP_FWD : STEP_REV);
  Serial.println("Pulling off endstop if needed...");
  while(!digitalRead(AXIS_STOP[axis])){
    step(AXIS_STEP_PIN[axis], AXIS_DIR_PIN[axis], stepdir); 
    delayMicroseconds(1000000/AXIS_JERK[axis]);
    if(attempts++ > 100) break; //Sanity.
  }
  //Then Zero fast...
  Serial.println("Fast move for ballpark zero...");
  while(digitalRead(AXIS_STOP[axis])){
    step(AXIS_STEP_PIN[axis], AXIS_DIR_PIN[axis], !stepdir); 
    delayMicroseconds(1000000/AXIS_JERK[axis]);
  }
  //Then slow to get exact position...
  //Serial.println("Finally, finesse...");
  //while(!digitalRead(AXIS_STOP[axis])){
    //step(AXIS_STEP_PIN[axis], AXIS_DIR_PIN[axis], stepdir);
    //delay(10);
  //}
  
}

inline void OLD_Y_MOVE(){

      
      for(int i=0;i<8;i++){
        //print_line();
        Serial.println("Line Done!\n");
        for(int j=0; j<256;j++){step(Y_STEP, Y_DIR, STEP_FWD); delay(1);}
      }//y_move
}

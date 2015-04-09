#include "configuration.h"
#include "gcodes.h"
#include "motion.h"

void G28(char buf[]){
  //For now, only Y axis.
  if(strchr(buf, 'Y')){
    Serial.print("Zeroing Axis: ");
    Serial.println(buf);
    zero_to_back_axis(Y_AXIS, (strchr(buf, '-') ? 1 : 0));
  }
}

void M503(char buf[]){
  Serial.println("Key:\t\t\tValue:");
  Serial.print("Y STEPS/MM\t\t");
  Serial.println(Y_STEPS_MM);
}

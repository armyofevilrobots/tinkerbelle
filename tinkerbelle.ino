/*
 * This is the minimalist test firmware for the tinkerbelle
 * powder printer.
 */
#include <avr/pgmspace.h>

//TODO: Break this all into separate files when appropriate.
#pragma once

//Motion output pins
#ifndef MOTION_H
#define MOTION_H
#define X_STEP  8
#define X_DIR   9
#define Y_STEP  10
#define Y_DIR   11
#define Z_STEP  12
#define Z_DIR   13
#define Y_STOP  14
#define X_STOP  15
#define Z_STOP  16
#define STEP_FWD 1
#define STEP_REV 0
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
#define MAX_COMMAND_SIZE 80
#define Y_STEPS_MM (200.0*8.0)/(16.0*5.08)
void step(int axispin, int dirpin, int direction);
#endif


// Serial config
#ifndef SERIAL_PORT
#define SERIAL_PORT 0
#define SERIAL_RATE 115200
#endif

#ifndef SERIAL_H
#define SERIAL_H
void setup_serial();
void setup_pins();
void send_and_wait(const char[]);
void get_cmd();
#endif

#ifndef CMD_BUFFER_H
#define CMD_BUFFER_H
#define CMD_BUFFER_SIZE 1024
char cmd_buffer[CMD_BUFFER_SIZE];
int buflen=0;
#endif

long pos[3];
const unsigned int AXIS_STEP_PIN [3] = {X_STEP, Y_STEP, Z_STEP};
const unsigned int AXIS_DIR_PIN  [3] = {X_STEP, Y_STEP, Z_STEP};
const unsigned int AXIS_STOP     [3] = {X_STOP, Y_STOP, Z_STOP};
long move(unsigned axis, int steps){
    int dir=0;
    if (steps<0){
        steps=-steps;
    }else{
        dir=1;
    }
}

void setup_pins(){
    pinMode(X_STEP, OUTPUT);
    pinMode(X_DIR, OUTPUT);
    pinMode(Y_STEP, OUTPUT);
    pinMode(Y_DIR, OUTPUT);
    pinMode(Z_STEP, OUTPUT);
    pinMode(Z_DIR, OUTPUT);
    pinMode(Y_STOP, INPUT_PULLUP);
}

void setup_serial(){
    Serial.begin(SERIAL_RATE);

}

void step(int axispin, int dirpin, int direction){
    digitalWrite(dirpin, direction);
    digitalWrite(axispin, 1);
    delayMicroseconds(2);
    digitalWrite(axispin, 0);
    digitalWrite(dirpin, 0);
}

void setup() {
    // put your setup code here, to run once:
    setup_serial();
    setup_pins();
    Serial.println("Booting Tinkerbelle.");
   
}

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


inline void zero_to_back_axis(int axis, int reverse){
 //Zero the Y
 //First we pull away from the endstop...
  int attempts = 0;
  int stepdir = (reverse ? STEP_FWD : STEP_REV);
  Serial.println("Pulling off endstop if needed...");
  while(!digitalRead(AXIS_STOP[axis])){
    step(AXIS_STEP_PIN[axis], AXIS_DIR_PIN[axis], stepdir); 
    delay(1);
    if(attempts++ > 100) break; //Sanity.
  }
  //Then Zero fast...
  Serial.println("Fast move for ballpark zero...");
  while(digitalRead(AXIS_STOP[axis])){
    step(AXIS_STEP_PIN[axis], AXIS_DIR_PIN[axis], !stepdir); 
    delay(1);
  }
  //Then slow to get exact position...
  Serial.println("Finally, finesse...");
  while(!digitalRead(AXIS_STOP[axis])){
    step(AXIS_STEP_PIN[axis], AXIS_DIR_PIN[axis], stepdir);
    delay(10);
  }
  
}

inline void OLD_Y_MOVE(){

      
      for(int i=0;i<8;i++){
        print_line();
        Serial.println("Line Done!\n");
        for(int j=0; j<256;j++){step(Y_STEP, Y_DIR, STEP_FWD); delay(1);}
      }//y_move
}

inline void G28(char buf[]){
  //For now, only Y axis.
  if(strchr(buf, 'Y')){
    zero_to_back_axis(Y_AXIS, (strchr(buf, '-') ? 1 : 0));
    Serial.print("Zeroing Axis: ");
    Serial.println(buf);
  }
}

inline void M503(char buf[]){
  Serial.println("Key:\t\t\tValue:");
  Serial.print("Y STEPS/MM\t\t");
  Serial.println(Y_STEPS_MM);
}

void loop() {
    // put your main code here, to run repeatedly:
    
    /*
    while(1){
      if(digitalRead(Y_STOP)){
        Serial.println("YSTOP");
      }else{
        Serial.println("!YSTOP");
      }
      delay(512);
    }
    */
    while(1){
      get_cmd();
    }
}

void get_cmd(){
    char serial_char;    
    
    while(Serial.available() > 0  && buflen < CMD_BUFFER_SIZE) {
      serial_char = Serial.read();
      
      if(serial_char == '\n' ||
         serial_char == '\r' ||
         buflen >= (CMD_BUFFER_SIZE - 1) )
      {
        cmd_buffer[buflen++] = 0;
        Serial.print(cmd_buffer);
        Serial.println(" : OK");
        if(!buflen) return; //Don't try to parse blank lines
        
        if(strstr_P(cmd_buffer, PSTR("G28"))){
          Serial.println("Got G28...");
          G28(cmd_buffer);
        }else if(strstr(cmd_buffer, "M503")){
          M503(cmd_buffer);
        }else{
          Serial.print("Unrecognized command: ");
          Serial.println(cmd_buffer);
        }
        
        memset(cmd_buffer, 0, CMD_BUFFER_SIZE);
        buflen = 0;
      }
      //I guess we don't have a full cmd yet.
      cmd_buffer[buflen++] = serial_char;
       
    }
    return;

       
    //}//while
}

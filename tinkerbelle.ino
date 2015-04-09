/*
 * This is the minimalist test firmware for the tinkerbelle
 * powder printer.
 */
#include <avr/pgmspace.h>
#include "configuration.h"
#include "serial.h"
#include "cmd_buffer.h"
#include "motion.h"
#include "gcodes.h"
#include "print.h"



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


void setup() {
    // put your setup code here, to run once:
    setup_serial();
    setup_pins();
    Serial.println("Booting Tinkerbelle.");
   
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

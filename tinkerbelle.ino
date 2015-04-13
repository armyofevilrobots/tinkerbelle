#include <InkShieldMega.h>

/*
 * This is the minimalist test firmware for the tinkerbelle
 * powder printer.
 */
#include <avr/pgmspace.h>
#include "configuration.h"
#include "serial.h"
#include "cmd_buffer.h"
#include "parser.h"
#include "motion.h"
#include "gcodes.h"
#include "print.h"
#ifdef ARDUINO
#include <avr/wdt.h>
#endif

char * read_cmd();
bool dispatch_cmd(char *);
char cmd_buffer[CMD_BUFFER_SIZE];
//extern int buflen;

InkShieldA0A3 inkshield(INKSHIELD_PIN);


/*
   SUPPORTED G CODES FOR TINKERBELLE

   G28     [X|Y|Z] : Home axis(axes) provided
M503            : Dump current config


*/


void setup_serial(){
    Serial.begin(SERIAL_RATE);
}


void setup() {
    // put your setup code here, to run once:
    wdt_disable();
    setup_serial();
    setup_pins();
    //Serial.println("Booting Tinkerbelle.");
}




void loop() {
    // put your main code here, to run repeatedly:

    while(1){
        Serial.println("FOO");
        //delay(200);
        if (dispatch_cmd(read_cmd())){
            Serial.print(cmd_buffer);
            Serial.println(" : OK");
        }
    }
}

bool shutdown(){
    Serial.println("REBOOTING!");
    wdt_enable(WDTO_15MS);
    for(;;){}
    return true;
}


bool dispatch_cmd(char *cmd){
    if(!cmd) return true; //Blank string.
    static char axes[]="XYZ";
    long xyz[]={0,0,0};
    bool xyzvalid[]={false, false, false};
    Gcode gcode = get_cmd(cmd, axes, xyz, xyzvalid);
    if(gcode.cmdchar=='G'){
        switch(gcode.cmdcode){
            case 0:
            case 1:
                return G00(xyz, xyzvalid);
            case 28:
                return G28(xyz, xyzvalid);
            case 90:
                return G90(NULL, NULL);
            case 91:
                return G91(NULL, NULL);
            case 92:
                return G92(xyz, xyzvalid);
            default:
                return false;
        }//SwitchG
    }
    else if(gcode.cmdchar=='M'){
        switch(gcode.cmdcode){
            case 0:
                return shutdown();
            case 47:
                return M47(NULL, NULL);
            case 114:
                return M114(NULL, NULL);
            case 503:
                return M503(xyz, xyzvalid);


            default:
                return false;
        }//switchM
    }
    //ERROR!
    return false;
}

//char * read_cmd(){
    //char serial_char;    
    //buflen = 0;
    //memset(cmd_buffer, 0, CMD_BUFFER_SIZE);
    //while(true)
        //while(Serial.available() > 0  && buflen < CMD_BUFFER_SIZE) {
            //serial_char = Serial.read();
            //if(serial_char == '\n' ||
                    //serial_char == '\r' ||
                    //buflen >= (CMD_BUFFER_SIZE - 1) )
            //{
                //cmd_buffer[buflen++] = 0;
                //if(!buflen) return 0; //Don't try to parse blank lines
                //return cmd_buffer;
            //}else{
                //cmd_buffer[buflen++] = serial_char;
            //}//EOL/CHARREAD
        //}//while serial
//}



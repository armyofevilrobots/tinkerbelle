#include "configuration.h"
#include "cmd_buffer.h"

//int buflen;

char * read_cmd(){
    char serial_char;    
    int buflen = 0;
    memset(cmd_buffer, 0, CMD_BUFFER_SIZE);
    while(true)
        while(Serial.available() > 0  && buflen < CMD_BUFFER_SIZE) {
            serial_char = Serial.read();
            if(serial_char == '\n' ||
                    serial_char == '\r' ||
                    buflen >= (CMD_BUFFER_SIZE - 1) )
            {
                cmd_buffer[buflen++] = 0;
                if(!buflen) return 0; //Don't try to parse blank lines
                return cmd_buffer;
            }else{
                cmd_buffer[buflen++] = serial_char;
                //Serial.write(serial_char);
            }//EOL/CHARREAD
        }//while serial
}

#include "parser.h"

//For testing.
#ifndef ARDUINO
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#endif

char gmcode(char buf[]){
    //Is this a G or M code?
    if(strchr(buf, 'G')) return 'G';
    if(strchr(buf, 'M')) return 'M';
    return 0;
}

char * skip_gmcode(char * buf){
    return strchr(buf, gmcode(buf))+1;
}

char * has_coord(char * buf, char coord){
    char * tmp = strchr(buf, coord);
    if(tmp) return tmp+1; //Since the XYZ is before the num, 
    //it's ALWAYS offset from 0 by one
    else return 0; // No such coord.
}

void get_coords(char *buf, char *axis, long *coords, bool *valid){
    char * tmp;
    for(int i=0; i<strlen(axis);i++){
        tmp=has_coord(buf, axis[i]);
        if(tmp){
            coords[i]=atol(tmp);
            valid[i]=1;
        }else{
            valid[i]=0;
        }
    }
}


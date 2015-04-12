#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"


/*
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
*/


int main(int argc, char *argv[]){
    char *cmd[3];//={,, };
    cmd[0]="G28XYZ"; 
    cmd[1]="G01X25Y13"; 
    cmd[2]="G0X0Y25"; 
    char coords[]="XYZ";
    long xyz[]={0,0,0};
    bool xyzvalid[]={0,0,0};
    char *tmp;
    char *tmp2;
    int gcode=0;



    for(int j=0;j<3;j++){
        tmp=cmd[j];
        printf("Scanning command %s\n", tmp);
        if(gmcode(tmp)=='G'){
            //printf("Found code G in %s\n", tmp);
            gcode=atoi(tmp);
            printf("GCode is: G%d\n", gcode);
            get_coords(tmp, "XYZ", xyz, xyzvalid);
            for(int i=0;i<3;i++){
                if(xyzvalid[i]) printf("%c : %ld\n", coords[i], xyz[i]);
            }

        }


    }


    return 0;
}

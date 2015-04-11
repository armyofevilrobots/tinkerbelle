#pragma once


#ifndef PARSER_H
#define PARSER_H
char gmcode(char []);
char * skip_gmcode(char *);
char * has_coord(char *, char);
void get_coords(char *, char *, long *, bool *);
#endif


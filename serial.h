/*
 * Taken from:
 * https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
 */


#ifndef SERIAL_H
#define SERIAL_H


#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>


int set_interface_attribs(int, int);
void set_mincount(int fd, int mcount);


#endif // SERIAL_H

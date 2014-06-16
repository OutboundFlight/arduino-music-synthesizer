#define _POSIX_C_SOURCE	199309L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <endian.h>
#include <time.h>

#include "arduino-serial/arduino-serial-lib.h"
#include "arduino-serial/arduino-serial-lib.c"

void main(void)
{

    int fd, Error;
    int baud = 9600;
    char serialport[] = "/dev/ttyACM0";

    union uuFreq {
        char fBytes[4];
        float fFloat;
    } uFreq;
    uFreq.fFloat = 50.00;

    union uuTime {
        char tBytes[4];
        unsigned long tLong;
    } uTime;
    uTime.tLong = 5000;

    union uuChannel {
       char cBytes[2]; 
       unsigned int cInt;
    } uChannel;
    uChannel.cInt = 1;

    fd = serialport_init(serialport, baud);
    Error = serialport_flush(fd);


    // Loop through several times then send song over

    for (int i = 0; i < 71; i++)
    {
    if (i == 70) 
    {
        uFreq.fFloat = 0xFFFFFFFF;
        uTime.tLong = 0xFFFF;
        uChannel.cInt = 0xFF;
    }

    // Send Frequency - incremented by 20
    uFreq.fFloat = uFreq.fFloat + 20.0;
    Error = serialport_writebyte(fd, uFreq.fBytes[0]);
    tcdrain(fd);
    Error = serialport_writebyte(fd, uFreq.fBytes[1]);
    tcdrain(fd);
    Error = serialport_writebyte(fd, uFreq.fBytes[2]);
    tcdrain(fd);
    Error = serialport_writebyte(fd, uFreq.fBytes[3]);
    tcdrain(fd);

    // Send Time - incremented by 500 ms
    uTime.tLong = uTime.tLong + 500;
    Error = serialport_writebyte(fd, uTime.tBytes[0]);
    tcdrain(fd);
    Error = serialport_writebyte(fd, uTime.tBytes[1]);
    tcdrain(fd);
    Error = serialport_writebyte(fd, uTime.tBytes[2]);
    tcdrain(fd);
    Error = serialport_writebyte(fd, uTime.tBytes[3]);
    tcdrain(fd);

    // Send Channel
    Error = serialport_writebyte(fd, uChannel.cBytes[0]);
    tcdrain(fd);
    Error = serialport_writebyte(fd, uChannel.cBytes[1]);
    tcdrain(fd);

    // Send \0
    Error = serialport_writebyte(fd, '\0');
    
    usleep(50000);
    tcdrain(fd);
    }


    serialport_close(fd);


//return 0;
}

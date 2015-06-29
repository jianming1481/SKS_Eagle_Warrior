//-------cssl include-------//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "cssl/cssl.h"
#include "cssl/port.h"
#include "cssl/cssl.c"
//#include "cssl/info.h"
#define VERSION "by Lain-Jinn Hwang\n"

//-------cssl variable-------//
char *echo ="\r";
cssl_t *serial=0;



//-------ROS variable-------//
const char *motion_topic_name = "/sks_speed_topic";


//====================//
//   cssl callback    //
//====================//
static void mcssl_callback(int id, uint8_t *buf, int length)
{
    /*int i;

    if (length == 0) return;

    for(i=0;i<length;i++) {
        putchar(buf[i]);
    }

    fflush(stdout);
    */
}

//====================//
//   cssl init        //
//====================//
int mcssl_init()
{
    char *devs="/dev/ttyUSB0";

     cssl_start();

    // modify 19200 to desire value
    serial=cssl_open(devs, mcssl_callback, 0, 115200, 8, 0, 1);

    if (!serial){
        printf("%s\n",cssl_geterrormsg());
        puts("\n--->RS232 OPEN FAIL (cssl_open error) <---");
        fflush(stdout);
        return -1;
    }
    cssl_setflowcontrol(serial, 0, 0);


    return 1;
}

//====================//
//   cssl finish      //
//====================//
void mcssl_finish(){

    cssl_close(serial);
    cssl_stop();

}


void mcssl_send2motor(double motor_left, double motor_right){
    char motor_left_dir  = (motor_left > 0)  ? 0x80 : 0;
    char motor_right_dir = (motor_right > 0) ? 0x80 : 0;


    char motor_left_byte = (char) abs(motor_left);
    char motor_right_byte =(char) abs(motor_right);

    cssl_putchar(serial,0xff);
    cssl_putchar(serial,0xfa);
    cssl_putchar(serial,0x00);
    cssl_putchar(serial,0x00);
    cssl_putchar(serial,motor_right_byte + motor_right_dir);
    cssl_putchar(serial,motor_left_byte + motor_left_dir);
    cssl_putchar(serial,0x00);
}

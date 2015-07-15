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
const char *arm_topic_name = "/cmd_pos";


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
    char *devs="/dev/ttyACM1";

     cssl_start();

    // modify 19200 to desire value
    serial=cssl_open(devs, mcssl_callback, 0, 9600, 8, 0, 1);

    if (!serial){
        printf("%s\n",cssl_geterrormsg());
        puts("\n--->RS232 OPEN FAIL (cssl_open error) <---");
        fflush(stdout);
        return -1;
    }else{
	printf("Connected RS232 Success!");
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

void mcssl_send2arm(char cmd){

    cssl_putchar(serial,cmd);
//    cssl_putchar(serial,0xff);
}

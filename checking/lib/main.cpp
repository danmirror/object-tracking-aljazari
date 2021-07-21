#include "mySerial.h"
#include <iostream>
using namespace std;



int  main(void)
{

    mySerial serial("/dev/ttyACM0",9600);

   //  // One Byte At the time
   //   serial.Send(128);
   //   serial.Send(132);

   //  // An array of byte
   //  unsigned char  dataArray[] = { 142,0};
   //  serial.Send(dataArray,sizeof(dataArray));

    // Or a string
    serial.Send("1,3,3");

    return 0;
}
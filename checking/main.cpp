#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <sys/time.h>

using namespace std;

char serialPortFilename[] = "/dev/ttyACM0";

int main()
{
   struct timeval tv;
   long s_prev = tv.tv_sec; 
   long s_prev_close  = tv.tv_sec; 
   FILE *serPort;

   while(1){

      gettimeofday(&tv,NULL);
      long s = tv.tv_sec; 
      // long ms = tv.tv_sec*1000 + tv.tv_usec/1000;
      

      if( s %2==0 && s != s_prev ){
         s_prev = s;

         serPort = fopen(serialPortFilename, "w");
         if (serPort == NULL)
         {
            printf("ERROR");	
            return 0;
         }
         string arduino="1,90,11";

         for(int i = 0; i<arduino.length();i++){
            char send =arduino[i];
            fwrite(&send, sizeof(send),1, serPort);
         }
         cout<<"send"<<endl;
      }
      if(s == s_prev+1 &&  s_prev_close != s){
         s_prev_close = s;

         fclose(serPort);
         cout<<"close"<<endl; 
      }
      cout<<"--"<<endl;
      // usleep(200000);
      

   }
	return 0;
   
}

// #include <iostream>
// #include <fstream>

// int main(int argc, char* argv[])
// {
//       std::ofstream arduino;
//       arduino.open( "/dev/ACM0");
//    while(1){
// 	//open arduino device file (linux)


//       //write to it
//    arduino << "1";
//       arduino.close();
//    }

// 	return 0;
// // }
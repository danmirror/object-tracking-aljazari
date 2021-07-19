#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
using namespace std;

char serialPortFilename[] = "/dev/ttyACM0";

int main()
{
	FILE *serPort = fopen(serialPortFilename, "w");

	if (serPort == NULL)
	{
		printf("ERROR");	
		return 0;
	}

   string data = "2";
	char writeBuffer[data.size() + 1];

   strcpy(writeBuffer, data.c_str());
   cout<<writeBuffer<<endl;


   string str="danu2";
   int c = str.length();
   cout<<str[1];
   

   for(int i = 0; i<str.length();i++){
      char send =str[i];
	   fwrite(&send, sizeof(send),1, serPort);
   }
	 
    sleep(1);

   fclose(serPort);
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
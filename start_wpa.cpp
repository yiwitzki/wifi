#include <iostream>
#include <unistd.h>
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
using namespace std;

const string wpa_supplicant_path = "/mnt/wifi/";

int main(int argc, char **argv)
{
	string command = "";
        command = wpa_supplicant_path + "wpa_supplicant -iwlan0 -Dwext -c " + wpa_supplicant_path + "wpa_supplicant.conf -B";
        const char* commandstr = command.data();
        char* commandch = new char [strlen(commandstr) + 1];
        strcpy(commandch, commandstr);
        cout<<commandch<<endl;
        system(commandstr);
        system("ifconfig wlan0 down");
        sleep(1);
        system("ifconfig wlan0 up");
        system(commandch);
        free(commandch);
	return 0;
}

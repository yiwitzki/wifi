#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>
#include "iostream"
using namespace std;

#define TERMINAL_MAIN_PATHNAME "/home/hadoop/tp/wpa_supplicant_hostapd-0.8_rtw_r7475.20130812/wpa_supplicant"
#define TERMINAL_MAIN_NAME "wpa_supplicant"
#define TERMINALMAIN_OK 0
#define TERMINALMAIN_ERROR -1

int main(int argc, char **argv)
{
    pid_t fPid = 0;
    int status = 0;
    while (1) 
    {
        //cout<<"in while"<<endl;
        //system("killall wifi_maintain");
        fPid = fork(); 
        if (0 == fPid)
        {
            cout<<"execute"<<endl;
            execl(TERMINAL_MAIN_PATHNAME, TERMINAL_MAIN_NAME,"-iwlan0", "-Dwext", "-c/home/hadoop/tp/wpa_supplicant_hostapd-0.8_rtw_r7475.20130812/wpa_supplicant.conf", (char *)0);
            exit(-1);
        }
        if (0 < fPid) 
        {
            fPid = wait(&status);
        }
    }
    return 0;
}


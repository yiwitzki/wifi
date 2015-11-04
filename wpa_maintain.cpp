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

#define TERMINAL_MAIN_PATHNAME "/tmp/wifi/wpa_supplicant"
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
        //system("killall wpa_supplicant");
        fPid = fork(); 
        if (0 == fPid)
        {
            execl(TERMINAL_MAIN_PATHNAME, TERMINAL_MAIN_NAME,"-iwlan0", "-Dwext", "-c/tmp/wifi/wpa_supplicant.conf", (char *)0);
            exit(-1);
        }
        if (0 < fPid) 
        {
            fPid = wait(&status);
        }
    }
    return 0;
}


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

#define TERMINAL_MAIN_PATHNAME "/mnt/wifi/wifi_main"
#define TERMINAL_MAIN_NAME "wifi_main"
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
           // cout<<"execute"<<endl;
            execl(TERMINAL_MAIN_PATHNAME, TERMINAL_MAIN_NAME, (char *)0);
            exit(-1);
        }
        if (0 < fPid) 
        {
            fPid = wait(&status);
        }
    }
    return 0;
}



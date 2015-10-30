#include <iostream>
#include <unistd.h>
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>
#include "libini.h"
using namespace std;


char AP1_SSID[20] = {0};
char AP2_SSID[20] = {0};
char AP1_password[64] = {0};
char AP2_password[64] = {0};
const char* NO_AP_CONNECTED = "NOT CONNECTED";
const string wpa_supplicant_path = "/mnt/wifi/";
char* ap_conf_path = "/mnt/wifi/ap_conf";
//char* ap_conf_path = "/home/hadoop/tp/libini/ap_conf";

void readconf();
int startwpa();
int scannap();
int connectap(const char *SSID, const char *password);
const char* whichapLinked();
char* string_to_char(string str);
int cgi_cmdExecAndGetLongResult(const char *pCmdStr, char *pRstStr);
int cgi_GetStrBetweenTwoStr(const char *pSrcStr, char *pHeadStr, char *pTailStr, char *pRstStr);

int main(int argc, char **argv)
{
    readconf();
    startwpa();
    while(1)
    {
        scannap();
        sleep(10);
    }
}

void readconf()
{
    
    INI *pini = NULL;  //cfg file prt
    pini = ini_open(ap_conf_path);
    if (NULL == pini)
    {
        cout<<"load config file failed"<<endl;
        return;
    }
    ini_get(pini, "AP1.SSID", AP1_SSID);
    ini_get(pini, "AP2.SSID", AP2_SSID);
    ini_get(pini, "AP1.password", AP1_password);
    ini_get(pini, "AP2.password", AP2_password);
    cout<<AP1_SSID<<endl<<AP2_SSID<<endl<<AP1_password<<endl<<AP2_password<<endl;
    //close ini file
    ini_close(pini);
    return;
}
int startwpa()
{
    string command = "";
    command = wpa_supplicant_path + "wpa_maintain";
    const char* commandstr = command.data();
    char* commandch = new char [strlen(commandstr) + 1];
    strcpy(commandch, commandstr);
    cout<<commandch<<endl;
    pid_t fpid = 0;
    int status = 0;
    fpid = fork();
    if (0 == fpid)
    {
       system(commandstr);
       free(commandch);
       exit(-1);
    }
    else if(0 < fpid)
    {
       free(commandch);
       sleep(3);
    }
    return 0;
}
int scannap()
{
    string command = "";
    command = wpa_supplicant_path + "wpa_cli -p/var/run/wpa_supplicant scan";
    
    system("ifconfig wlan0 up");
    const char* commandstr = command.data();
    char* commandch = new char [strlen(commandstr) + 1];
    strcpy(commandch, commandstr);
    //cout<<commandch<<endl;

    system(commandch);
    free(commandch);

    command = wpa_supplicant_path + "wpa_cli -p/var/run/wpa_supplicant scan_results";
    const char* commandstr1 = command.data();
    cout<<commandstr1<<endl;
    char result[4096]={0};
    cgi_cmdExecAndGetLongResult(commandstr1, result);
    //cout<<result<<endl;
    const char* connected_ap = whichapLinked();
    if (connected_ap == AP1_SSID)        //已经连接到AP1
    {
	system("echo AP1 >> ./a.txt");
        return 0;
    }
    else
    {
        if (strstr(result, AP1_SSID) != NULL)   //扫描列表中存在AP1
        {
            connectap(AP1_SSID, AP1_password);
            system("echo connect AP1 >> ./a.txt");
            cout<<"connecting AP1"<<endl;
        }
        else
        {
            if (strstr(result, AP2_SSID) != NULL)  //扫描列表中不存在AP1，但是有AP2
	    {
		if (strcmp(connected_ap, NO_AP_CONNECTED) == 0)
	    	{
		   cout<<"connecting AP2"<<endl;
		   system("echo connect AP2 >> ./a.txt");
                   connectap(AP2_SSID, AP2_password);
		}
            }
	    else    //删除wifi默认网关
            {
              system("route del default dev wlan0");
              system("echo no ap >> ./a.txt");
            }
        }
    }
    return 0;
}
int connectap(const char *SSID, const char *password)
{
    string command1, command2, command3, command4, command5, command6 = "";
    command1 = wpa_supplicant_path + "wpa_cli -p/var/run/wpa_supplicant remove_network 0";
    command2 = wpa_supplicant_path + "wpa_cli -p/var/run/wpa_supplicant ap_scan 1";
    command3 = wpa_supplicant_path + "wpa_cli -p/var/run/wpa_supplicant add_network";
    command4 = wpa_supplicant_path + "wpa_cli -p/var/run/wpa_supplicant set_network 0 ssid '" + '"' + SSID + '"' + "'";
    command5 = wpa_supplicant_path + "wpa_cli -p/var/run/wpa_supplicant set_network 0 psk '" + '"' + password + '"' + "'";
    command6 = wpa_supplicant_path + "wpa_cli -p/var/run/wpa_supplicant select_network 0";

    char* commandch1 = string_to_char(command1);
    cout<<commandch1<<endl;
    system(commandch1);
    free(commandch1);
    char* commandch2 = string_to_char(command2);
    cout<<commandch2<<endl;
    system(commandch2);
    free(commandch2);
    char* commandch3 = string_to_char(command3);
    cout<<commandch3<<endl;
    system(commandch3);
    free(commandch3);
    char* commandch4 = string_to_char(command4);
    cout<<commandch4<<endl;
    system(commandch4);
    free(commandch4);
    char* commandch5 = string_to_char(command5);
    cout<<commandch5<<endl;
    system(commandch5);
    free(commandch5);
    char* commandch6 = string_to_char(command6);
    cout<<commandch6<<endl;
    system(commandch6);
    free(commandch6);
    sleep(20);
    const char* connected_ap = whichapLinked();
    if (strcmp(connected_ap, NO_AP_CONNECTED) != 0)
    	system("udhcpc -i wlan0 -q");  
    return 0;
}
char* string_to_char(string str)
{
    cout<<"string to char"<<endl;
    const char* commandstr = str.data();
    char* commandch = new char [strlen(commandstr) + 1];
    strcpy(commandch, commandstr);
    return commandch;
}
const char* whichapLinked()
{
    string command = "";
    const char* query_no_ap = "ESSID:off/any";
    const char*query_ap1 = AP1_SSID;
    const char*query_ap2 = AP2_SSID;
    command = wpa_supplicant_path + "iwconfig";
    //command = "iwconfig";
    const char* commandstr = command.data();
    char* commandch = new char [strlen(commandstr) + 1];
    strcpy(commandch, commandstr);
    char result[4096]={0};
    cgi_cmdExecAndGetLongResult(commandch,result);
    free(commandch);
    if (strstr(result, query_no_ap))
    {
        system("echo noapconnected >> ./whichapconnect.txt");
        return NO_AP_CONNECTED;
    }
    else if (strstr(result, query_ap1))
    {
        system("echo ap1connected >> ./whichapconnect.txt");
        return AP1_SSID;
    }
    else if (strstr(result, query_ap2))
    {
        system("echo ap2connected >> ./whichapconnect.txt");
        return AP2_SSID;
    }
}
/****************************************************************************
*  Function:        cgi_cmdExecAndGetLongResult
*  Description:     exec cmd and return result
*  Input:           char *pCmdStr: cmd string
*  Output:          char* pRstStr: result string    (CGI_FILE_BUF_LEN)
*  Return:          CGI_OK, CGI_ERROR
*  Others:
*****************************************************************************/
int cgi_cmdExecAndGetLongResult(const char *pCmdStr, char *pRstStr)
{
    //pipe
    FILE *pP = NULL;
    int nCh = '\0';
    int nStrNum = 0;

    if ((NULL == pCmdStr) || (NULL == pRstStr))
    {
        printf("%s%c%c ", "Content-Type:text/html;charset=gb2312", 13, 10);
        printf("cgi_cmdExecAndGetResult ERROR: invalid input arg!");
        return -1;
    }

    pP = popen(pCmdStr, "r");
    if (NULL == pP)
    {
        printf("%s%c%c ", "Content-Type:text/html;charset=gb2312", 13, 10);
        printf("cgi_cmdExecAndGetResult ERROR: popen failed!");
        return -1;
    }

    while((nCh=getc(pP)) != EOF)
    {
        if (nStrNum == (4096-1))   //out of buffer size
        {
            break;
        }
        pRstStr[nStrNum] = nCh;
        nStrNum++;
    }
    pRstStr[nStrNum] = '\0';

    pclose(pP);
    return 1;
}

/****************************************************************************
*  Function:        cgi_cmdExecAndGetResult
*  Description:     get string between 2 strings, exclude head and tail string
*  Input:           char *pSrcStr: source string
                    char *pHeadStr: head string
                    char *pTailStr: tail string
*  Output:          char* pRstStr: result string (len MUST = CGI_LONG_TEXT_LEN)
*  Return:          CGI_OK, CGI_ERROR
*  Others:
*****************************************************************************/
int cgi_GetStrBetweenTwoStr(char *pSrcStr, char *pHeadStr, char *pTailStr, char *pRstStr)
{
    int nBeginPos = 0;
    int nTailPos = 0;
    int nTmpNum = 0;
    char *pTmpPtr = NULL;
    int CGI_ERROR = -1;
    //input para checking
    //ptr null
    if ((NULL == pSrcStr) || (NULL == pHeadStr) || (NULL == pTailStr) || (NULL == pRstStr))
    {
        return CGI_ERROR;
    }
#if 1
    //str len = 0
    if ((0 == strlen(pSrcStr)) || (0 == strlen(pHeadStr)) || (0 == strlen(pTailStr)))
    {
        return CGI_ERROR;
    }
    //head = tail
    if (0 == strcmp(pHeadStr, pTailStr))
    {
        return CGI_ERROR;
    }
#endif

    //get head begin pos
    pTmpPtr = strstr(pSrcStr, pHeadStr);
    if (NULL == pTmpPtr)
    {
        return CGI_ERROR;
    }
    nBeginPos = pTmpPtr - pSrcStr;

    //get tail begin pso
    pTmpPtr = strstr(pSrcStr, pTailStr);
    if (NULL == pTmpPtr)
    {
        return CGI_ERROR;
    }
    nTailPos = pTmpPtr - pSrcStr;

    //pos valid check
    if (nTailPos <= nBeginPos)
    {
        return CGI_ERROR;
    }

    //get target pos
    nBeginPos = nBeginPos + strlen(pHeadStr);
    nTailPos = nTailPos - 1;

    //check pos valid
    if (nTailPos < nBeginPos)
    {
        return CGI_ERROR;
    }

    //begin = tail, rst = '\0'
    if (nTailPos == nBeginPos)
    {
        pRstStr[0] = '\0';
        return 1;
    }

    //get target string
    for (nTmpNum=0; nTmpNum<=(nTailPos - nBeginPos); nTmpNum++)
    {
        pRstStr[nTmpNum] = pSrcStr[nBeginPos + nTmpNum];
        //over len
        if ((4096 - 2) == nTmpNum)
        {
            break;
        }
    }
    pRstStr[nTmpNum+1] = '\0';

    return 1;
}

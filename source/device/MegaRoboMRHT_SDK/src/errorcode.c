#include "errorcode.h"
#include "storage.h"

int mrgErrorCodeConfigUpload(ViSession vi, int code, int *type,  int *response, int *diagnose, int *enable)
{
    char error[RECV_BUF_LEN] = "";
    int len = RECV_BUF_LEN;
    char args[SEND_BUF_LEN];
    int retlen = 0;
    char values[32][64] = {""};

    snprintf(args, SEND_BUF_LEN, ":ERRCode:UPLoad? %d\n", code);
    if ((retlen = busQuery(vi, args, strlen(args), error, len)) == 0) {
        return -1;
    }

    splitString(error, ",", values);

    //! type
    if(STRCASECMP(values[0], "F") == 0){
        *type = 1;
    }
    else if(STRCASECMP(values[0], "W") == 0){
        *type = 2;
    }
    else if(STRCASECMP(values[0], "I") == 0){
        *type = 3;
    }
    else{
        return -2;
    }

    //! response
    if(STRCASECMP(values[1], "A") == 0){
        *response = 1;
    }
    else if(STRCASECMP(values[1], "B") == 0){
        *response = 2;
    }
    else if(STRCASECMP(values[1], "C") == 0){
        *response = 3;
    }
    else if(STRCASECMP(values[1], "D") == 0){
        *response = 4;
    }
    else if(STRCASECMP(values[1], "E") == 0){
        *response = 5;
    }
    else if(STRCASECMP(values[1], "F") == 0){
        *response = 6;
    }
    else if(STRCASECMP(values[1], "G") == 0){
        *response = 7;
    }
    else if(STRCASECMP(values[1], "NONE") == 0){
        *response = -1;
    }
    else{
        return -3;
    }

    //! diagnose
    if(STRCASECMP(values[2], "ON") == 0){
        *diagnose = 1;
    }
    else if(STRCASECMP(values[2], "OFF") == 0){
        *diagnose = 0;
    }
    else{
        return -4;
    }


    //! enable
    if(STRCASECMP(values[3], "Y") == 0){
        *enable = 1;
    }
    else if(STRCASECMP(values[3], "N") == 0){
        *enable = 0;
    }
    else{
        return -5;
    }

    return 0;
}

int mrgErrorCodeConfigDownload(ViSession vi, int code, int type, int response, int diagnose, int enable)
{
    char args[SEND_BUF_LEN];
    int retlen = 0;

    char ps8Type[10] = "";
    char ps8Diagnose[10] = "";
    char ps8Response[10] = "";
    char ps8Enable[10] = "";

    if (type == 1){
        strcpy(ps8Type, "F");
    }
    else if (type == 2){
        strcpy(ps8Type , "W");
    }
    else if (type == 3){
        strcpy(ps8Type, "I");
    }
    else{
        return -1;
    }

    if( STRCASECMP(ps8Type,"F") == 0 )
    {
        if (response == 1){
            strcpy(ps8Response, "A");
        }
        else if (response == 2){
            strcpy(ps8Response, "B");
        }
        else if (response == 3){
            strcpy(ps8Response, "C");
        }
        else if (response == 4){
            strcpy(ps8Response, "D");
        }
        else if (response == 5){
            strcpy(ps8Response, "E");
        }
        else if (response == 6){
            strcpy(ps8Response, "F");
        }
        else if (response == 7){
            strcpy(ps8Response, "G");
        }
        else{
            return -2;
        }
    }
    else
    {
        strcpy(ps8Response, "NONE"); //不是错误类型，没有响应选项
    }

    if (diagnose == 1){
        strcpy(ps8Diagnose, "ON");
    }
    else if (diagnose == 0){
        strcpy(ps8Diagnose, "OFF");
    }
    else{
        return -3;
    }

    if (enable == 0){
        strcpy(ps8Enable, "N");
    }
    else if (enable == 1){
        strcpy(ps8Enable, "Y");
    }
    else{
        return -4;
    }

    snprintf(args, SEND_BUF_LEN, ":ERRCode:DOWNLoad %d,%s,%s,%s,%s\n", code, ps8Type, ps8Response, ps8Diagnose, ps8Enable);
    if ((retlen = busWrite(vi, args, strlen(args))) <= 0) {
        return -5;
    }
    return 0;
}

int mrgErrorLogUpload(ViSession vi, char* errorLog)
{
    return mrgStorageReadFile(vi, 0, "/home/megarobo/MRH-T/diagnose", "Diagnose.log", (unsigned char *)errorLog);
}

int mrgErrorLogClear(ViSession vi)
{
    char args[SEND_BUF_LEN];
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, ":ERRLOG:CLEAR\n");
    if ((retlen = busWrite(vi, args, strlen(args))) == 0) {
        return -1;
    }
    return 0;
}


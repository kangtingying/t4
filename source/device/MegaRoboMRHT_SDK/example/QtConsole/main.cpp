
#include "MegaGateway.h"
#include <stdio.h>

int main()
{
    int i;
    char buff[4096] = "";
    char mrhtList[20][64] = {""};
    char mrht[64] = "";

    //! 查找网关
    mrgFindGateWay(0, buff, sizeof(buff), 1);

    char *p, *pNext;
    int count = 0;
    p = STRTOK_S(buff, ",", &pNext);
    while (p){
        strcpy(mrhtList[count++], p);
        p = STRTOK_S(NULL, ",", &pNext);
    }

    for(i=0; i<count; i++){
        printf("Find MRH-T[%d]: %s\n", i, mrhtList[i]);
    }
    if(count <= 0){
        printf("Search MRH-T is empty!!!\n");
        return -1;
    }

    strcpy(mrht, mrhtList[0]);
    int visa =  (int)mrgOpenGateWay(mrht, 800);
    printf("device open: %d\n", visa);

    //! 获取网关识别码
    char IDN[64] = "";
    mrgGateWayIDNQuery(visa, IDN);
    printf("device IDN: %s\n", IDN);

    //! 获取机器人
    int robotNames[64] = {0};
    count = mrgGetRobotName(visa, robotNames);
    if(count <= 0){
        printf("Search Robot is empty!!!\n");
        return -2;
    }
    for(i=0; i<count; i++){
        int robotType = mrgGetRobotType(visa, robotNames[i]);
        printf("Find Robot[%d]: %d, type: %d\n", i, robotNames[i], robotType);
    }
    int robotName = robotNames[0];
    printf("robot name: %d\n", robotName);

    //! 获取设备
    int deviceList[32];
    count = mrgGetRobotDevice(visa, robotName, deviceList);
    if(count <= 0){
        printf("Search MRQ is empty!!!\n");
        return -3;
    }
    for(i=0; i<count; i++){
        printf("Find MRQ Device[%d]: %d\n", i, deviceList[i]);
    }
    int deviceName = deviceList[0];
    printf("deviceName name: %d\n", deviceName);

    char deviceType[32] = "";
    mrgGetDeviceType(visa, deviceName, deviceType);
    printf( "device type: %s\n", deviceType );

    int channelCount = mrgGetDeviceChannelCount(visa, deviceName);
    printf( "device channel count: %d\n", channelCount );

    //! 回零
    float posx,posy,posz;
    mrgRobotGoHome(visa, robotName, 0);
    mrgGetRobotCurrentPosition(visa, robotName, &posx, &posy, &posz);
    printf("Current Pos:\t%f,\t%f,\t%f\n", posx, posy, posz);

    //! 关闭网关
    int ret =  mrgCloseGateWay(visa);
    printf("device close: %d %d\n", visa, ret);

    printf("Press AnyKey Continue...\n");
    getchar();
    return 0;
}

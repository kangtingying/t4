
#include "assist.h"
#define msSleep(n) localSleep(n)
#define _msSleep(n) Sleep(n)

#include "./MegaRoboMRHT_SDK/src/common.c"
#include "./MegaRoboMRHT_SDK/src/bus.c"
#include "./MegaRoboMRHT_SDK/src/device.c"
#include "./MegaRoboMRHT_SDK/src/errorcode.c"
#include "./MegaRoboMRHT_SDK/src/megarobot.c"
#include "./MegaRoboMRHT_SDK/src/mrqdevice.c"
#include "./MegaRoboMRHT_SDK/src/project.c"
#include "./MegaRoboMRHT_SDK/src/storage.c"
#include "./MegaRoboMRHT_SDK/src/system.c"

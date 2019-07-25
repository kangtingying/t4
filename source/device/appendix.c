
#include "appendix.h"

EXPORT_API int CALL mrgRobotGetState(ViSession vi, int name, int wavetable, char state[100] )
{
    char args[SEND_BUF];
	
    int time = 0, retlen = 0;
	
	state[0] = 0;

    if (wavetable >= WAVETABLE_MIN && wavetable <= WAVETABLE_MAX)
    {
        snprintf(args, SEND_BUF, "ROBOT:STATe? %d,%d\n", name, wavetable);
    }
    else
    {
        snprintf(args, SEND_BUF, "ROBOT:STATe? %d\n", name);
    }

    {
        if ((retlen = busQuery(vi, args, strlen(args), state, 100)) == 0)
        {
            return -1;
        }
        state[retlen - 1] = '\0';//去掉回车符
        
    }
    return 0;
}

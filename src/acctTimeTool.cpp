#include "../include/acctTimeTool.hpp"

int32 acct_time::getCurTimeusec()
{
    struct timeval timev;
    int ret = gettimeofday(&timev, NULL);
    if (ret < 0)
    {
        return ret;
    }

    return (timev.tv_sec * 1000000 + timev.tv_usec);
}


int32 acct_time::getCurTimeMs()
{
    struct timeval timev;
    int ret = gettimeofday(&timev, NULL);
    if (ret < 0)
    {
        return ret;
    }

    return (timev.tv_sec * 1000000 + timev.tv_usec) / 1000;
}

int32 acct_time::sleepMs(int32 ms)
{
    return usleep(ms * 1000);
}

int32 acct_time::getSysTimeMs()  // not usually used
{
    struct timeb t;
    ftime(&t);
    return 1000 * t.time + t.millitm;
}


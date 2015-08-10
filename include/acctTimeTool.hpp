#ifndef __ACCTTIMETOOL_H__
#define __ACCTTIMETOOL_H__

#include "baseHeader.h"
namespace acct_time
{
int32 getCurTimeusec();
int32 getCurTimeMs();
int32 sleepMs(int32 ms);
int32 getSysTimeMs();  // not usually used
}

#endif 

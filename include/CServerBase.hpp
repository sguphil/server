#ifndef __CSERVERBASE_H__
#define __CSERVERBASE_H__
#include "baseHeader.h"

class CServerBase
{
public:
    virtual int32 getIoEpollfd() = 0;
    virtual int32 getServerId() = 0;
};
#endif 

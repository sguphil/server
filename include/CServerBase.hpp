#ifndef __CSERVERBASE_H__
#define __CSERVERBASE_H__
#include "baseHeader.h"

enum eSERVERTYPE
{
    ACCSvr = 1,
    GWSvr = 2,
    GSvr = 3,
    DBSvr = 4,
};

class CServerBase
{
public:
    virtual int32 getIoEpollfd() = 0;
    virtual int32 getServerID() = 0;
    virtual eSERVERTYPE getServerType() = 0;
};
#endif 

#ifndef __CSERVERBASE_H__
#define __CSERVERBASE_H__
#include "baseHeader.h"

enum eSERVERTYPE
{
    eACCSvr = 1,
    eGWSvr = 2,
    eGSvr = 3,
    eDBSvr = 4,
};

class CServerBase
{
public:
    virtual ~CServerBase() { }
    virtual int32 getIoEpollfd() = 0;
    virtual int32 getSendEpollfd() = 0;
    virtual int32 getServerID() = 0;
    virtual int32 getIoThreadNum() = 0;
    virtual eSERVERTYPE getServerType() = 0;
};
#endif 

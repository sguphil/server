#ifndef __CHANDLERFUNC_H__
#define __CHANDLERFUNC_H__

#include "../include/baseHeader.h"
#include "../network/include/Session.h"
#include "../protocol/testMsg.pb.h" // add all related protocol header here

class CHandlerFunc
{
public:
    static int32 testfunc(CSession *session, char *pMsg, int32 msglen);
};
#endif

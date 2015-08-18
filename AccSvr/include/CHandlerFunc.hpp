#ifndef __CHANDLERFUNC_H__
#define __CHANDLERFUNC_H__

#include "../../include/baseHeader.h"

class CSession;

class CHandlerFunc
{
public:
    static int32 testfunc(CSession *session, char *pMsg, int32 msglen);
};
#endif

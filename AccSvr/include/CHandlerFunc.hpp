#ifndef __CHANDLERFUNC_H__
#define __CHANDLERFUNC_H__

#include "../../include/baseHeader.h"
#include "../../include/PackageHandler.hpp"


typedef int32 (*accFuncHandler)(CSession *session, char *pMsg, int32 msglen);

struct accFuncStruct : public PKGFUNCBASE
{
    accFuncHandler handler;
};

class CAccHandlerMgr : public CPackageMgr<accFuncStruct>
{

public:
    void addAllHandle();
    void registerFunc(int16 sysid, int16 msgid, accFuncStruct *funcStruct);
    accFuncStruct *findFuncStruct(int32 key);
    void registerFunc(int16 sysid, int16 msgid, accFuncHandler func);
};


class CHandlerFunc
{
public:
    static int32 testfunc(CSession *session, char *pMsg, int32 msglen);
};

#endif

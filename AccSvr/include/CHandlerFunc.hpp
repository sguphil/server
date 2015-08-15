#ifndef __CHANDLERFUNC_H__
#define __CHANDLERFUNC_H__

#include "../../include/baseHeader.h"
#include "../../include/PackageHandler.hpp"

class CSession;

class CHandlerFunc
{
public:
    static int32 testfunc(CSession *session, char *pMsg, int32 msglen);
};

typedef int32 (*accFuncHandler)(CSession *session, char *pMsg, int32 msglen);

class accFuncStruct : public pkgFuncBase//PKGFUNCBASE
{
public:
    accFuncHandler handler;
};

template<typename HDLFuncT>
class CAccHandlerMgr : public CPackageMgr<accFuncStruct>
{

public:
    void addAllHandle()
    {
        registerFunc(1, 4, &CHandlerFunc::testfunc);

    }

    void registerFunc(int16 sysid, int16 msgid, HDLFuncT *funcStruct)
    {
        m_functionMap.insert(std::make_pair<int32, accFuncStruct *>(funcStruct->funckey, funcStruct));
    }

    HDLFuncT *findFuncStruct(int32 key)
    {
        if (m_functionMap.empty())
        {
            return NULL;
        }

        std::map<int32, accFuncStruct *>::iterator it = m_functionMap.find(key);
        if (it != m_functionMap.end())
        {
            return it->second;
        }
        else
        {
            return NULL;
        }
    }

    void registerFunc(int16 sysid, int16 msgid, accFuncHandler func)
    {
        accFuncStruct *accFunc = new accFuncStruct;
        accFunc->handler = func;
        accFunc->funckey = pkgFuncBase::makeKey(sysid, msgid);
        registerFunc(sysid, msgid, accFunc);
    }
    
};


#endif

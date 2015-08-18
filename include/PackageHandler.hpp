#ifndef __PACKAGEHANDLER_H__
#define __PACKAGEHANDLER_H__
#include "../include/baseHeader.h"
#include "packHeader.hpp"
#include "../network/include/Session.h"
#include "../network/include/NetWorkObject.h"
#include "../AccSvr/include/CHandlerFunc.hpp"

enum sysid
{
    SYS_SESSION_REGISTER = 1,

};

enum msgid
{
    C_S_SISSION_REGISTER = 1,
};

void handlePackage(CSession *pSession, PkgHeader *header, char *msgbuf, int32 msgsize);
void handlePackage(CSession *pSession, PkgHeader *header, MsgHeader *msgHead, char *msgbuf, int32 msgsize);
void decodeMsgHead(MsgHeader &msghead, char *buf, int32 buffsize);

void encodepkg(char *buf, PkgHeader *head, MsgHeader *msgHead, char *msgbuf, int32 msgbufsize);

class pkgFuncBase
{
public:
    uint32 funckey;
    pkgFuncBase() : funckey(0)
    {

    }

    static int32 makeKey(int16 sysid, int16 systype)
    {
        uint32 key = sysid << 16 | systype;
        return key;
    }
};

typedef struct pkgFuncBase PKGFUNCBASE;

//========define type of handle function type here======DO NOT MODIFY!
template<typename FUNCTYPE>
class CPackageMgr
{
    //typedef std::map<int32, FUNCTYPE *> FuncMap;
public:
    virtual ~CPackageMgr() {}
    virtual void addAllHandle()
    {
    }
    virtual void registerFunc(int16 sysid, int16 msgid, FUNCTYPE *funcStruct)
    {
    }
    virtual FUNCTYPE* findFuncStruct(int32 key)
    {
        return NULL;
    }
    std::map<int32, FUNCTYPE *> m_functionMap;
};

//==========AccHandler=============
typedef int32 (*accFuncHandler)(CSession *session, char *pMsg, int32 msglen);

class accFuncStruct : public pkgFuncBase//PKGFUNCBASE
{
public:
    accFuncHandler handler;
};

class CAccHandlerMgr : public CPackageMgr<accFuncStruct>
{

public:
    void addAllHandle()
    {
        registerFunc(1, 4, &CHandlerFunc::testfunc);

    }

    accFuncStruct *findFuncStruct(int32 key)
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

private:
    void registerFunc(int16 sysid, int16 msgid, accFuncStruct *funcStruct)
    {
        CPackageMgr<accFuncStruct>::m_functionMap.insert(std::make_pair<int32, accFuncStruct *>(funcStruct->funckey, funcStruct));
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

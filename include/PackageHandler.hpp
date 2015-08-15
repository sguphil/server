#ifndef __PACKAGEHANDLER_H__
#define __PACKAGEHANDLER_H__
#include "../include/baseHeader.h"
#include "packHeader.hpp"
#include "../network/include/Session.h"
#include "../network/include/NetWorkObject.h"

//class CSession;

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

//========define type of handle function type here======


template<typename FUNCTYPE>
class CPackageMgr
{
    typedef std::map<int32, FUNCTYPE *> FuncMap;
public:
    CPackageMgr(){}
    virtual ~CPackageMgr(){}
    virtual void addAllHandle() = 0;
    virtual void registerFunc(int16 sysid, int16 msgid, FUNCTYPE *funcStruct) = 0;
    virtual FUNCTYPE* findFuncStruct(int32 key) = 0;

protected:
    FuncMap m_functionMap;
};

#endif

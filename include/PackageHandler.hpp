#ifndef __PACKAGEHANDLER_H__
#define __PACKAGEHANDLER_H__
#include "../include/baseHeader.h"
#include "packHeader.hpp"
#include "../network/include/Session.h"
#include "../network/include/NetWorkObject.h"
//#include "../Handler/CHandlerFunc.hpp"
//#include "ServerInclude.hpp"
/*
enum sysid
{
    SYS_SESSION_REGISTER = 1,

};

enum msgid
{
    C_S_SISSION_REGISTER = 1,
};
*/

enum eServerMessageClass //server message type
{
    eRegister_Message = 1,
    eServerMessage_Client = 2,
    eServerMessage_AccSvr = 3,
    eServerMessage_GateWay = 4,
    eServerMessage_Logic = 5,
    eServerMessage_DBServer = 6,
};

enum eRegister_Message_MsgType
{
    eRegister_Message_MsgType_begin,
    C_S_SISSION_REGISTER,
    eRegister_Message_MsgType_end,
};

enum eServerMessage_Client_MsgType // msg from client 2
{
    erverMessage_Client_MsgType_begin,
    CLI_ACCS_TESTBINPKG,  // JUST TEST
    CLI_ACCS_TESTPROBUFPKG, // JUST TEST
    CLI_ACCS_CHECKLOGINUSER,
    erverMessage_Client_MsgType_end,
};

enum eServerMessage_AccSvr_MsgType // msg from accSvr 3
{
    eServerMessage_AccSvr_MsgType_begin,
    ACCS_DBS_CHECKLOGINUSER,
    ACCS_DBS_TEST, //-----JUST TEST HANDLER

    ACCS_CLI_CHECKLOGINUSER_RET,
    eServerMessage_AccSvr_MsgType_end,
};

enum eServerMessage_GateWay_MsgType // msg from gateway 4
{
    eServerMessage_GateWay_MsgType_begin,
    eServerMessage_GateWay_MsgType_end,
};

enum eServerMessage_Logic_MsgType // msg from logic svr 5
{
    eServerMessage_Logic_MsgType_begin,
    eServerMessage_Logic_MsgType_end,
};

enum eServerMessage_DBServer_MsgType // msg from dbserver 6
{
    eServerMessage_DBServer_MsgType_begin,
    DBS_ACCS_CHECKLOGINUSER_RET = 2,
    eServerMessage_DBServer_MsgType_end,
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

#endif

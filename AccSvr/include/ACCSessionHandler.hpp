#ifndef __ACCSESSIONHANDLER_H__
#define __ACCSESSIONHANDLER_H__
#include "../../include/PackageHandler.hpp"
#include "MessageDef.hpp"
#include "./CACCSHandlerFunc.hpp"

class CAccHandlerMgr : public CPackageMgr<accFuncStruct>
{

public:
    void addAllHandle()
    {
        registerFunc(eServerMessage_Client, CLI_ACCS_TESTBINPKG, &CACCSHandlerFunc::testfunc);
        registerFunc(eServerMessage_Client, CLI_ACCS_TESTPROBUFPKG, &CACCSHandlerFunc::testfunc);
        registerFunc(eServerMessage_Client, CLI_ACCS_CHECKLOGINUSER, &CACCSHandlerFunc::checkuser);
        registerFunc(eServerMessage_DBServer, DBS_ACCS_CHECKLOGINUSER_RET, &CACCSHandlerFunc::dbcheckuserret);

    }
public:
    ~CAccHandlerMgr()
    {
        
    }

    accFuncStruct* findFuncStruct(int32 key)
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


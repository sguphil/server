#ifndef __LOGICSESSIONHANDLER_H__
#define __LOGICSESSIONHANDLER_H__
#include "../../include/PackageHandler.hpp"
#include "CLogicSHandlerFunc.hpp"

class CLogicHandlerMgr : public CPackageMgr<accFuncStruct>
{

public:
    //register all functions in this interface if needed
    void addAllHandle()
    {
        registerFunc(eServerMessage_AccSvr, ACCS_DBS_CHECKLOGINUSER, &CLogicSHandlerFunc::checkuser);
        registerFunc(eServerMessage_AccSvr, ACCS_DBS_TEST, &CLogicSHandlerFunc::testfunc);

    }

public:
    ~CLogicHandlerMgr()
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


#include "../include/CHandlerFunc.hpp"

int32 CHandlerFunc::testfunc(CSession *session, char *pMsg, int32 msglen)
{
    printf("into CHandlerFunc testfunc!!!!!!\n");
    return 0;
}


//========add all the handler functions in this function,
//and it will trigger by session prasePackage()
void CAccHandlerMgr::addAllHandle()
{
    registerFunc(1, 4, &CHandlerFunc::testfunc);

}

void CAccHandlerMgr::registerFunc(int16 sysid, int16 msgid, accFuncHandler func)
{
    accFuncStruct *accFunc = new accFuncStruct;
    accFunc->handler = funcStruct;
    accFunc->funckey = pkgFuncBase::makeKey(sysid, msgid);
    registerFunc(sysid, msgid, accFunc);
}

void CAccHandlerMgr::registerFunc(int16 sysid, int16 msgid, accFuncStruct *funcStruct)
{
    m_functionMap.insert(make_pair<int32, accFuncStruct *>(funcStruct->funckey, funcStruct));
}

accFuncStruct* CAccHandlerMgr::findFuncStruct(int32 key)
{
    if (m_functionMap.empty())
    {
        return NULL;
    }

    map<int32, accFuncStruct *>::iterator it = m_functionMap.find(key);
    if (it != m_functionMap.end())
    {
        return *it;
    }
    else
    {
         return NULL;
    }
}

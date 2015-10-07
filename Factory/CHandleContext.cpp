#include "CHandleContext.hpp"
/*
CHandleContext::CHandleContext()
{

}

CHandleContext::~CHandleContext()
{

}

int32 CHandleContext::init()
{
    return 0;
}

CPackageMgr<accFuncStruct>* CHandleContext::GetHandlerInst(SESSION_TYPE type)
{
    std::map<SESSION_TYPE, CPackageMgr<accFuncStruct> *>::iterator it;
    it = m_FlyWeightHandlerMap.find(type);
    if (it != m_FlyWeightHandlerMap.end())
    {
        return it->second;
    }
    CPackageMgr<accFuncStruct> *inst = NULL;
    switch ((uint32)type)
    {
    case eClient:
        break;
    case eGateWay:
        break;
    case eOtherSvr:
        break;
    case eGameServer:
        break;
    case eDBServer:
        inst = new CDBHandlerMgr;
        break;
    case eStrictClient:
        break;
    case eAccountSvr:
        inst = new CAccHandlerMgr;
        break;
    case eUndefineSessionType:
        break;
    default:
        break;
    }
    
    if (NULL != inst)
    {
        inst->addAllHandle();
        m_FlyWeightHandlerMap.insert(std::pair<SESSION_TYPE, CPackageMgr<accFuncStruct> *>(eDBServer, inst));
        return inst;
    }

    return NULL;
}*/

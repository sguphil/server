#ifndef __LOGICSVR_H__
#define __LOGICSVR_H__

#include "LogicSvrConfig.hpp"
#include "../database/CDBInstFactory.hpp"
#include "../include/EpollServer.hpp"
#include "../include/CSendThread.hpp"
#include "../include/CIoThread.hpp"
#include "../include/acctTimeTool.hpp"
#include "../include/Singleton.h"

#include "./session/ClientSession.h"
#include "./session/AccsvrSession.h"
#include "./session/DBSession.h"
#include "./session/LogicSession.h"
#include "./session/GatewaySession.h"
#include "./session/StrictClient.h"

extern int32 MAXPKGLEN;
extern int32 SESSIONBUFLEN;

class LogicSvr: public EpollServer, public base::Singleton<LogicSvr>
{
public:
    LogicSvr();
    virtual ~LogicSvr();
    void start();
    void update();

    void DestructNetWorkObj(NetWorkObject *netobj);
    NetWorkObject* CreateNetWorkObj(SESSION_TYPE type);

    CDBInstFactory* getDBInstFactory()
    {
        return &m_dbInstFactory;
    }

private:
    CLogicSvrConfig m_Config;
    CDBInstFactory m_dbInstFactory;
};


#endif 


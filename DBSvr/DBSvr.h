#ifndef __DBSVR_H__
#define __DBSVR_H__
#include "../include/baseHeader.h"
#include "../include/Singleton.h"
#include "../network/include/Acceptor.h"
#include "../network/include/CommonList.h"
#include "../network/include/Session.h"
#include "../include/EpollServer.hpp"
#include "../network/include/Connector.h"
#include "../include/CIoThread.hpp"
#include "../include/CSendThread.hpp"
#include "../include/acctTimeTool.hpp"
#include "DBSvrConfig.hpp"
#include "../database/CDBInstFactory.hpp"
#include "../common/SIDGenerator.hpp"

#include "./session/ClientSession.h"
#include "./session/AccsvrSession.h"
#include "./session/DBSession.h"
#include "./session/LogicSession.h"
#include "./session/GatewaySession.h"
#include "./session/StrictClient.h"


extern int32 MAXPKGLEN;
extern int32 SESSIONBUFLEN;

class DBSvr: public EpollServer, public base::Singleton<DBSvr>
{
public:
    DBSvr();
    virtual ~DBSvr();
    void start();
    void update();

    void DestructNetWorkObj(NetWorkObject *netobj);
    NetWorkObject* CreateNetWorkObj(SESSION_TYPE type);

    CDBInstFactory* getDBInstFactory()
    {
        return &m_dbInstFactory;
    }

private:
    CDBSvrConfig m_Config;
    CDBInstFactory m_dbInstFactory;
};


#endif 

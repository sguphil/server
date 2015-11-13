#ifndef __GATEWAYSVR_H__
#define __GATEWAYSVR_H__
#include "../include/baseHeader.h"
#include "../include/Singleton.h"
#include "../network/include/Acceptor.h"
#include "../network/include/CommonList.h"
#include "../network/include/Session.h"
#include "../network/include/Connector.h"
#include "../include/CIoThread.hpp"
#include "../include/CSendThread.hpp"
#include "../include/acctTimeTool.hpp"
#include "GatewaySvrConfig.hpp"
#include "../include/packHeader.hpp"
#include "../common/SIDGenerator.hpp"
#include "../include/EpollServer.hpp"

#include "./session/ClientSession.h"
#include "./session/AccsvrSession.h"
#include "./session/DBSession.h"
#include "./session/LogicSession.h"
#include "./session/GatewaySession.h"
#include "./session/StrictClient.h"

extern int32 MAXPKGLEN;
extern int32 SESSIONBUFLEN;
using std::multimap;

class GatewaySvr: public EpollServer, public base::Singleton<GatewaySvr>
{
public:
    GatewaySvr();
    virtual ~GatewaySvr();
    void start();
    void update(); 
    void DestructNetWorkObj(NetWorkObject *netobj);
    NetWorkObject* CreateNetWorkObj(SESSION_TYPE type);

private:
    CGatewaySvrConfig m_Config;
};

#endif // GatewaySvr_H
